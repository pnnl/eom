// CPLEX-based optimization

#include "float.h"
#include "Optimization.hpp"

#include "optimization_structs.hpp"
#include "optimizer_structs.hpp"
#include "cplex_data.hpp"

#include "optimizers/optimization/UnitCommitOptimizer.hpp"

#include "objectives/MinimizeCostUC.hpp"
#include "constraints/unitCommit/PowerBalanceConstraintUC.hpp"
#include "constraints/unitCommit/PowerLimitConstraintUC.hpp"
#include "constraints/unitCommit/GenStartStopConstraintUC.hpp"
#include "constraints/unitCommit/MinUpDownConstraintUC.hpp"
#include "constraints/unitCommit/RampConstraintUC.hpp"
#include "constraints/unitCommit/ReserveConstraintUC.hpp"
#include "constraints/unitCommit/InterzoneMaxLinePowerUC.hpp"
#include "constraints/unitCommit/InterzonePowerTransferDistributionFactorUC.hpp"
#include "constraints/unitCommit/InterzoneTransportConstraintUC.hpp"
#include "constraints/unitCommit/ZonePowerBalanceConstraintUC.hpp"
#include "constraints/unitCommit/DeratingConstraintUC.hpp"
#include "universe.hpp"

using std::cout;
using std::max;
using std::endl;
using std::cerr;

namespace model {
/*
 * The set up optimization set the
 */
UnitCommitOptimizer::UnitCommitOptimizer(Universe & universe, UCData & ucData, size_t ba) :
	ucData(ucData),
	universe(universe),
	smallestObjective(FLT_MAX),
	interface(NULL),
	baID(ba),
	currentTime(0),
	genStartIndex(0),
	convStartIndex(0),
	storageStartIndex(0),
	hydroStartIndex(0),
	zonePTDFData(NULL),
	zoneMaxLinePowerData(NULL)
{
	interpreter::RIMParser * parser = interpreter::RIMParser::getRIMParser();
	interface              = simulator::SDInterface::getSDInterface();
	baParams               = *parser->getBalanceAuthParams(baID);
	storageStartIndex      = baParams.nConventional;
	hydroStartIndex        = baParams.nConventional + baParams.nStorage;
	initializeOptimizer();
}

void
UnitCommitOptimizer::loadOptVariables()
{
	optData.control.iloObj           = IloExpr      (optData.control.iloEnv, 0);
	optData.rangeConstraints         = IloRangeArray(optData.control.iloEnv);
	optData.iloSegmentPower          = NumVar3D     (optData.control.iloEnv, ucData.ucLength);
	optData.iloSegmentIsOn           = BoolVarMatrix(optData.control.iloEnv, ucData.ucLength); // Switch to numVar so we can get the duals
	optData.iloGenPower              = NumExprMatrix(optData.control.iloEnv, ucData.ucLength);
	optData.iloGenIsOn               = NumExprMatrix(optData.control.iloEnv, ucData.ucLength);
	optData.iloGenStartSupport       = NumVarMatrix (optData.control.iloEnv, ucData.ucLength);
	//optData.iloGenStopSupport        = NumVarMatrix (optData.control.iloEnv, ucData.ucLength);
	optData.iloStepsToSwitch         = NumVarMatrix (optData.control.iloEnv, ucData.ucLength);

	optData.iloZonePowerTransferOut       = NumVarMatrix (optData.control.iloEnv, ucData.ucLength);
	optData.iloZonePowerTransferIn        = NumExprMatrix(optData.control.iloEnv, ucData.ucLength);
	optData.iloZonePowerTransferInPerZone = NumVar3D     (optData.control.iloEnv, ucData.ucLength);

	optData.iloMMBtuFuelInput     = NumExprMatrix(optData.control.iloEnv, ucData.ucLength);
	optData.iloFuelCost           = NumExprMatrix(optData.control.iloEnv, ucData.ucLength);
	optData.iloPoundsCo2Emissions = NumExprMatrix(optData.control.iloEnv, ucData.ucLength);
	optData.iloCo2EmissionsCost   = NumExprMatrix(optData.control.iloEnv, ucData.ucLength);

	for (size_t step = 0; step < ucData.ucLength; step++) {
		optData.iloSegmentPower[step]           = NumVarMatrix   (optData.control.iloEnv, ucData.nGenerators);
		optData.iloSegmentIsOn[step]            = IloBoolVarArray(optData.control.iloEnv, ucData.nGenerators);
		optData.iloGenPower[step]               = IloNumExprArray(optData.control.iloEnv, ucData.nGenerators);
		optData.iloGenIsOn[step]                = IloNumExprArray(optData.control.iloEnv, ucData.nGenerators);
		optData.iloGenStartSupport[step]        = IloNumVarArray (optData.control.iloEnv, ucData.nGenerators);
		//optData.iloGenStopSupport[step]         = IloNumVarArray (optData.control.iloEnv, ucData.nGenerators);
		optData.iloStepsToSwitch[step]          = IloNumVarArray (optData.control.iloEnv, ucData.nGenerators);
		//optData.iloZCoeff[step]                 = NumVarMatrix(optData.control.iloEnv, data.nGenerators);
		optData.iloZonePowerTransferOut      [step] = IloNumVarArray (optData.control.iloEnv, ucData.nZones);
		optData.iloZonePowerTransferIn       [step] = IloNumExprArray(optData.control.iloEnv, ucData.nZones);
		optData.iloZonePowerTransferInPerZone[step] = NumVarMatrix   (optData.control.iloEnv, ucData.nZones);

		optData.iloMMBtuFuelInput    [step] = IloNumExprArray(optData.control.iloEnv, ucData.nGenerators);
		optData.iloFuelCost          [step] = IloNumExprArray(optData.control.iloEnv, ucData.nGenerators);
		optData.iloPoundsCo2Emissions[step] = IloNumExprArray(optData.control.iloEnv, ucData.nGenerators);
		optData.iloCo2EmissionsCost  [step] = IloNumExprArray(optData.control.iloEnv, ucData.nGenerators);
		for (size_t zone = 0; zone < ucData.nZones; zone++) {
			optData.iloZonePowerTransferOut      [step][zone] = IloNumVar(optData.control.iloEnv, 0, DBL_MAX, IloNumVar::Float);
			optData.iloZonePowerTransferIn       [step][zone] = IloNumExpr(optData.control.iloEnv);
			optData.iloZonePowerTransferInPerZone[step][zone] = IloNumVarArray(optData.control.iloEnv, ucData.nZones);
		}
		// Assumes PTDF's are positive
		for (size_t zoneTo = 0; zoneTo < ucData.nZones; ++zoneTo) {
			for (size_t zoneFrom = 0; zoneFrom < ucData.nZones; ++zoneFrom) {
				optData.iloZonePowerTransferInPerZone[step][zoneTo][zoneFrom] = IloNumVar(optData.control.iloEnv, 0, DBL_MAX, IloNumVar::Float);
			}
		}
		for (size_t zoneTo = 0; zoneTo < ucData.nZones; ++zoneTo) {
			optData.iloZonePowerTransferIn[step][zoneTo] = IloSum(optData.iloZonePowerTransferInPerZone[step][zoneTo]);
		}
		for (size_t gen = 0; gen < ucData.nGenerators; gen++) {
			const size_t nSegments = ucData.costCurve[gen].nSegments;
			optData.iloSegmentPower[step][gen]             = IloNumVarArray (optData.control.iloEnv, nSegments);
			optData.iloSegmentIsOn[step][gen]              = IloBoolVar(optData.control.iloEnv);
			//if (!gettingShadowPrices)
			//	optData.iloSegmentIsOn[step][gen].addIntegerConstraint();
			//else
			//	optData.control.iloModel.add(optData.iloSegmentIsOn[step][gen] = results.segmentIsOn[step][gen]);
			optData.iloGenStartSupport[step][gen]          = IloNumVar(optData.control.iloEnv);
			//optData.iloGenStopSupport[step][gen]           = IloNumVar(optData.control.iloEnv);
			optData.iloStepsToSwitch[step][gen]            = IloNumVar(optData.control.iloEnv, 0, max(ucData.minDownTime[gen], ucData.minUpTime[gen]), IloNumVar::Float);
			for (size_t segment = 0; segment < nSegments; segment++) {
				optData.iloSegmentPower[step][gen][segment] = IloNumVar(optData.control.iloEnv, 0, ucData.costCurve[gen].segment[segment].endPoint[POWER], IloNumVar::Float);
				//optData.iloSegmentIsOn[step][gen][segment]  = IloBoolVar(optData.control.iloEnv);
			}
			optData.iloGenPower[step][gen]                 = IloSum(optData.iloSegmentPower[step][gen]);
			//optData.iloGenIsOn[step][gen]                  = IloSum(optData.iloSegmentIsOn[step][gen]);

			optData.iloMMBtuFuelInput    [step][gen] = IloNumExpr(optData.control.iloEnv);
			optData.iloFuelCost          [step][gen] = IloNumExpr(optData.control.iloEnv);
			optData.iloPoundsCo2Emissions[step][gen] = IloNumExpr(optData.control.iloEnv);
			optData.iloCo2EmissionsCost  [step][gen] = IloNumExpr(optData.control.iloEnv);
		}
	}
}

/*
 * This whole function may be unnecessary. It is used to clean up the optimizer
 * when UnitCommitOptimizer is destructed to free CPLEX variables as needed. So far,
 * this seems to have little impact on the CPLEX related valgrind errors
 */
void
UnitCommitOptimizer::finalOptimizerCleanUp()
{
	optData.rangeConstraints.end();
	for (size_t step = 0; step < ucData.ucLength; step++) {
		for (size_t gen = 0; gen < ucData.nGenerators; gen++) {
			for (size_t segment = 0; segment < ucData.costCurve[gen].nSegments; segment++) {
				optData.iloSegmentPower[step][gen][segment].end();
				//optData.iloSegmentIsOn[step][gen][segment].end();
			}
			optData.iloSegmentPower   [step][gen].endElements();
			optData.iloSegmentPower   [step][gen].end();
			optData.iloGenPower       [step][gen].end();
			//optData.iloSegmentIsOn    [step][gen].endElements();
			optData.iloSegmentIsOn    [step][gen].end();
			//optData.iloGenIsOn        [step][gen].end();
			optData.iloGenStartSupport[step][gen].end();
			//optData.iloGenStopSupport [step][gen].end();
			optData.iloStepsToSwitch  [step][gen].end();
		}
		optData.iloSegmentPower   [step].end();
		optData.iloGenPower       [step].endElements();
		optData.iloGenPower       [step].end();
		optData.iloSegmentIsOn    [step].end();
		//optData.iloGenIsOn        [step].endElements();
		//optData.iloGenIsOn        [step].end();
		optData.iloGenStartSupport[step].endElements();
		optData.iloGenStartSupport[step].end();
		//optData.iloGenStopSupport [step].endElements();
		//optData.iloGenStopSupport [step].end();
		optData.iloStepsToSwitch  [step].endElements();
		optData.iloStepsToSwitch  [step].end();
	}
	optData.iloSegmentPower   .end();
	optData.iloGenPower       .end();
	optData.iloSegmentIsOn    .end();
	//optData.iloGenIsOn        .end();
	optData.iloGenStartSupport.end();
	//optData.iloGenStopSupport .end();
	optData.iloStepsToSwitch  .end();
}

void
UnitCommitOptimizer::initializeModelSet()
{
	typedef InterzonePowerTransferDistributionFactorUC PTDF_UC;
	initConstraint(new PowerBalanceConstraintUC      (ucData, optData), POWER_BALANCE_CONSTRAINT_UC);
	initConstraint(new PowerLimitConstraintUC        (universe, ucData, optData), POWER_LIMIT_CONSTRAINT_UC  );
	initConstraint(new GenStartStopConstraintUC      (ucData, optData), START_STOP_CONSTRAINT_UC   );
	initConstraint(new RampConstraintUC              (ucData, optData), RAMP_CONSTRAINT_UC         );
	initConstraint(new ReserveConstraintUC           (universe, ucData, optData), RESERVE_CONSTRAINT_UC      );
	initConstraint(new MinUpDownConstraintUC         (ucData, optData), UP_DOWN_CONSTRAINT_UC      );
	initConstraint(new InterzoneMaxLinePowerUC       (ucData, optData), INTRAZONELINE_CONSTRAINT_UC);
	initConstraint(new PTDF_UC                       (ucData, optData), INTRAZONEPTDF_CONSTRAINT_UC);
	initConstraint(new InterzoneTransportConstraintUC(ucData, optData), TRANSPORT_CONSTRAINT_UC    );
	initConstraint(new ZonePowerBalanceConstraintUC  (ucData, optData), ZONE_POWER_BALANCE_UC      );
	initConstraint(new DeratingConstraintUC          (universe, ucData, optData), DERATING_UC                );
	initObjective(new MinimizeCostUC(universe, ucData, optData));
}

void dumpDuals(IloCplex &cplex, IloRangeArray &rangeConstraints) {
	IloNumArray duals(cplex.getEnv(), rangeConstraints.getSize());
	for (int i = 0; i < duals.getSize(); i++)
		duals[i] = DBL_MIN;
	int day = simulator::Clock::getCurrentDayInSim();
	char fileName[256];
	sprintf(fileName, "results/duals-%d.csv", day);
	std::ofstream out(fileName);
	cplex.solveFixed(); // treat as LP
	cplex.getDuals(duals, rangeConstraints);
	for (int i = 0; i < duals.getSize(); i++) {
		double dual = duals[i];
		std::string name = rangeConstraints[i].getName();
		out << dual << "," << name << endl;
	}
	cplex.solve(); // revert to MIPS
	out.close();
}
// ############################################################################
// ## CHECK SOLUTION
// ############################################################################
void
UnitCommitOptimizer::handleSolution()
{
	IloCplex &cplex = optData.control.iloCplex;
	dumpDuals(cplex, optData.rangeConstraints);
	FLOAT objectiveValue = cplex.getObjValue();
	cout << " best integer objective so far" << objectiveValue << endl;
	cout << " objValue(0) " << cplex.getObjValue(0) << endl;
	//cplex.writeMIPStart("results/uc.mst");
	if (this->smallestObjective != FLT_MAX)
		cout << " smallest objective: " << this->smallestObjective << endl;
	//double dual = cplex.getDual(optData.rangeConstraints.at(0));
	//cout << "dual = " << dual << endl;
	for (size_t gen = 0; gen < ucData.nGenerators; gen++){
		for (size_t step = 0; step < ucData.ucLength; step++) {
			double power    = cplex.getValue(optData.iloGenPower[step][gen]);
			bool isOn       = 0.9 < cplex.getValue(optData.iloSegmentIsOn[step][gen]); // cplex values for true may be slightly less than one, depending on precision (z.b. 0.9999)
			bool isStarting = 0.9 < cplex.getValue(optData.iloGenStartSupport[step][gen]);
			//bool isStopping = 0.9 < cplex.getValue(optData.iloGenStopSupport[step][gen]);
			ucData.rampSchedule[gen][step].targetPower = power;
			ucData.rampSchedule[gen][step].state = isOn ? GENERATOR_SUPPORTING : GENERATOR_OFF;
			ucData.rampSchedule[gen][step].genSwitch = NO_SWITCH;
			if (isStarting && isOn) ucData.rampSchedule[gen][step].genSwitch = SW_ON_STATE;
			if (isStarting && !isOn) ucData.rampSchedule[gen][step].genSwitch = SW_OFF_STATE;
		}
	}
	int nDumpSteps = 24; // FIXME TODO this # should come from the clock or something
	int dumpIndex = ::simulator::Clock::getCurrentDayInSim(); // FIXME TODO should be current uc step or something, not current day.
	for (INTEGER step = 0; step < nDumpSteps; step++) {
		for (size_t zoneFrom = 0; zoneFrom < ucData.nZones; zoneFrom++) {
			for (size_t zoneTo = 0; zoneTo < ucData.nZones; zoneTo++) {
				if (zoneFrom!=zoneTo)
				{
					double amount = cplex.getValue(optData.iloZonePowerTransferInPerZone[step][zoneTo][zoneFrom]);
					if (amount > POWER_EPS)
						ucData.powerTransfer.push_back(PowerTransfer(dumpIndex * nDumpSteps + step, zoneFrom, zoneTo, amount));
				}
			}
		}
	}
}

void
UnitCommitOptimizer::handleNoSolution()
{
	IloCplex & cplex = this->optData.control.iloCplex;
	IloCplex::CplexStatus cplexStatus = cplex.getCplexStatus();
	IloAlgorithm::Status status = cplex.getStatus();
	cerr << "IloCplex::CplexStatus = " << cplexStatus << endl;
	cerr << "IloCplex::Status = " << status << endl;
	cerr << "No solution for unit commit at day " << ::simulator::Clock::getCurrentDayInSim() << "." << endl;
	printState(cerr);
	printParameters(cerr);
	printOptimzationVariables(cerr);

	// the calling procedure has to deal with this.
	throw OptimizationException(__FILE__, __LINE__ , Exception::UNIT_COMMIT_FAILED);
}


// ############################################################################
// ############################################################################
// ############################################################################
// ############################################################################
void
UnitCommitOptimizer::printOptimzationVariables(std::ostream & out)
{
	cerr << "UNIT COMMIT OPTIMIZATION VARIABLES" << endl;

	for (size_t step = 0; step < ucData.ucLength; step++) {
		for (size_t gen = 0; gen < ucData.nGenerators; gen++) {
			cerr << "steps to switch " << step << " " << gen << " " << optData.iloStepsToSwitch[step][gen] << endl;
			cerr << "power/load balance " << step << " " << gen << " " << optData.iloGenPower[step][gen] << endl;
			cerr << "generator is on " << step  << " " << gen<< " " << optData.iloSegmentIsOn[step][gen] << endl;
			cerr << "generator support " << step << " " << gen << " " << optData.iloGenStartSupport[step][gen] << endl;
			//cerr << "generator stop " << step << " " << gen << " " << optData.iloGenStopSupport[step][gen] << endl;
		}
	}
}

class MyCallback : public IloCplex::MIPInfoCallbackI {
public:
	IloCplex::CallbackI* duplicateCallback() const {
		return (new (getEnv()) MyCallback(*this));
	}
	MyCallback(IloEnv env) : IloCplex::MIPInfoCallbackI(env) {
	}
	void main() {
		FLOAT incumbentObjectiveValue = this->getIncumbentObjValue();
		FLOAT bestObjectiveValue = this->getBestObjValue();
		INTEGER iterations = this->getNiterations(); // returns the total number of iterations executed so far during the current optimization to solve the node relaxations
		FLOAT percentDifference = (incumbentObjectiveValue - bestObjectiveValue) / bestObjectiveValue;
		FLOAT relativeGap = this->getMIPRelativeGap();
		cout << "iteration " << iterations << " %difference " << percentDifference << " relative gap: " << relativeGap << endl;
	}
};


class MyCallback2 : public IloCplex::ContinuousCallbackI {
	float & smallestObjective;
public:
	IloCplex::CallbackI* duplicateCallback() const {
		return (new (getEnv()) MyCallback2(*this));
	}
	MyCallback2(IloEnv env, float & smallestObjective) : IloCplex::ContinuousCallbackI(env), smallestObjective(smallestObjective) {
		smallestObjective = FLT_MAX;
	}
	void main() {
		register float objectiveValue = this->getObjValue();
		register IloBool feasible = this->isFeasible();
		if (feasible)
			cout << "feasible: " << feasible << "\t objective " << objectiveValue << endl;
		if (feasible && smallestObjective > objectiveValue) {
			smallestObjective = objectiveValue;
			cout << "smallest: " << smallestObjective << endl;
		}
	}
};

void
UnitCommitOptimizer::tune(IloCplex &cplex)
{
	//MyCallback * mc = new MyCallback(this->optData.control.iloEnv);
	//IloCplex::Callback cb = IloCplex::Callback(mc);
	//cplex.use(cb);
	//MyCallback2 * mc = new MyCallback2(this->optData.control.iloEnv, this->smallestObjective);
	//IloCplex::Callback cb = IloCplex::Callback(mc);
	//cplex.use(cb);

	//below is added by xinda
	//cplex.setParam(cplex.RepeatPresolve,   3   );//-1 Automatic: let CPLEX choose; default, 3 epresovle with cuts and allow new root cuts;2 Represolve with cuts
	cplex.setParam(cplex.MIPDisplay,   3   ); //2 Display integer feasible solutions plus an entry for every n-th node; default; 5 maximum
	//cplex.setParam(cplex.VarSel   ,   3   );
	//cplex.setParam(cplex.RepeatPresolve,   3   );//-1 Automatic: let CPLEX choose; default, 3 epresovle with cuts and allow new root cuts;2 Represolve with cuts
	//cplex.setParam(cplex.MIPDisplay,   5   ); //2 Display integer feasible solutions plus an entry for every n-th node; default; 5 maximum
	//cplex.setParam(cplex.VarSel   ,   3   );
	//cplex.setParam(cplex.MIPEmphasis, CPX_MIPEMPHASIS_OPTIMALITY);
	//cplex.setParam(cplex.HeurFreq, 10);
	//cplex.setParam(cplex.RINSHeur, 20);
	//cplex.setParam(cplex.RINSHeur, 20);
	//cplex.setParam(cplex.ParallelMode, CPX_PARALLEL_DETERMINISTIC  );
	//cplex.setParam(cplex.TuningDisplay, 3  );
	//cplex.setParam(cplex.TuningRepeat, 4  );
	//cplex.setParam(cplex.TuningTiLim, 40000  );
	//cplex.setParam(cplex.MIPEmphasis, CPX_MIPEMPHASIS_HIDDENFEAS);
	//cplex.setParam(cplex.MIPEmphasis, CPX_MIPEMPHASIS_FEASIBILITY);
	//cplex.setParam(cplex.FPHeur, 2);
	//cplex.setParam(cplex.Probe    ,   2   );
	//cplex.setParam(cplex.DisjCuts	,   2   );
	//cplex.setParam(cplex.EpOpt	,   1e-04   );
	//above is added by xinda for test
	//cplex.setParam(cplex.Threads, 4);
	//cplex.setParam(cplex.MIPEmphasis, 1);
	//cplex.setParam(cplex.MemoryEmphasis, CPX_ON); // default off; conserve memory where possible if on
	//cplex.setParam(cplex.WorkMem,   1   ); // default 128; Specifies an upper limit on the amount of central memory, in megabytes, that CPLEX is permitted to use for working memory before swapping to disk files, compressing memory, or taking other actions.
	//cplex.setParam(cplex.BarEpComp,   1   ); // default 1e-8; Convergence tolerance for LP and QP problems
	//cplex.setParam(cplex.Probe    ,   3   ); // default, CPLEX decides level of probing.  1-3=more, -1=none.
	//cplex.setParam(cplex.ObjDif   , 100.  ); // default 0.0, forces MIP optimization to ignore integer solutions that are not at least this much better
	//cplex.setParam(cplex.RelObjDif,    .01); // default 0.0, forces the mixed integer optimization to ignore integer solutions that are not at least this amount better than the one found so far
	cplex.setParam(cplex.EpGap    ,   this->interface->getEpGap()); // Default 1e-4; Sets a relative tolerance on the gap between the best integer objective and the objective of the best node remaining.
	//cplex.setParam(cplex.EpAGap   ,   1   ); // Default 1e-6; Sets an absolute tolerance on the gap between the best integer objective and the objective of the best node remaining.
	//cplex.setParam(cplex.EpInt    ,   1e-5); // Default 1e-5; Specifies the amount by which an integer variable can be different from an integer and still be considered feasible.
	//cplex.setParam(cplex.EpLin    ,   1e-3); // Default 1e-3; This parameter controls how strict inequalities are managed during linearization (ie x < a => x <= a-eplin)
	//cplex.setParam(cplex.VarSel   ,   0   ); // default 0=automatic, -1=branch on variable with minimum infeasibility, 1=branch on variable with maximum infeasibility, 2=branch based on pseudo costs, 3=strong branching, 4=branch on reduced pseudo costs
	//cplex.setParam(cplex.MIPDisplay,  5   ); // default = 2; 0 = no node log until optimal solution found; 1 = one entry for each integer feasible solution; 2 = for every n-th node; 3 = include number of cuts added; 4 = include entries for LP root relaxation; 5 = include entries for LP subproblems
	//cplex.setParam(cplex.MIPInterval,  0   ); // default = 0 (let CPLEX pick); negative = display new incumbents and frequent log lines starting and fewer over time; positive = display new incumbents; display a log line every n nodes
	//cplex.setParam(cplex.MIPEmphasis, CPX_MIPEMPHASIS_FEASIBILITY); // default = BALANCED; FEASIBILITY, OPTIMALITY, BESTBOUND, HIDDENFEAS
	//cplex.setParam(cplex.LBHeur     , CPX_OFF       ); // default = off; Controls whether CPLEX applies a local branching heuristic to try to improve new incumbents found during a MIP search
	//cplex.setParam(cplex.BrDir      , CPX_BRDIR_AUTO); // branch up or down first, or let CPLEX pick
	//cplex.setParam(cplex.BndStrenInd, -1            ); // Decides whether to apply bound strengthening in mixed integer programs; -1 = let CPLEX choose; 0 = do not apply; 1 = apply
	//cplex.setParam(cplex.Cliques    ,  0            ); // Decides whether or not clique cuts should be generated for the problem; -1 = do not; 0 = let CPLEX choose; 1 = moderately; 2 = aggressively; 3 = very aggressively
	//cplex.setParam(cplex.DiveType   ,  0            ); // Controls the MIP dive strategy; 0 = let CPLEX choose; 1 = traditional; 2 = probing; 3 = guided
	// continue exploring parameters with "MIP kappa computation" at http://pic.dhe.ibm.com/infocenter/cosinfoc/v12r4/index.jsp?topic=%2Filog.odms.cplex.help%2FCPLEX%2FParameters_reference%2Ftopics%2FEpAGap.html
	bool warm = false;
	{
		std::ifstream my_file("results/uc.mst");
		if (my_file.good())
		{
			warm = true;
		}
	}
	if (warm)
		cplex.readMIPStart("results/uc.mst");

}

} /* END OF NAMESPACE model */

