/*
 * LFOptConfigure.cpp
 *
 *  Created on: Nov 28, 2011
 *      Author: kglass
 *
 *  NOTE: this contains the CPLEX specific code for LoadFollowingOpt
 */


#include "Optimization.hpp"

#include "optimization_structs.hpp"
#include "optimizer_structs.hpp"
#include "cplex_data.hpp"

#include "optimizers/optimization/LoadFollowingOpt.hpp"

#include "objectives/MinimizeCostLF.hpp"
#include "constraints/loadFollowing/PowerBalanceConstraintLF.hpp"
#include "constraints/loadFollowing/RampConstraintLF.hpp"
#include "constraints/loadFollowing/IntraZoneLineConstraintLF.hpp"
#include "constraints/loadFollowing/IntraZonePowerInConstraintLF.hpp"
#include "constraints/loadFollowing/ZonePowerBalanceConstraintLF.hpp"

using std::endl;
using std::cerr;

namespace model {

LoadFollowingOpt::LoadFollowingOpt(UCData & ucData, LFData & lfData, size_t baID) :
	ucData(ucData),
	lfData(lfData),
	baID(baID),
	currentTime(0),
	genStartIndex(0),
	storageStartIndex(0),
	hydroStartIndex(0),
	zonePTDFData(NULL),
	zoneMaxLinePowerData(NULL),
	targetPowerData(NULL),
	targetPower(NULL),
	generatorOnData(NULL),
	generatorOn(NULL),
	switchOnData(NULL),
	switchOn(NULL),
	switchOffData(NULL),
	switchOff(NULL)
{
	interpreter::RIMParser * parser = interpreter::RIMParser::getRIMParser();
	interface              = simulator::SDInterface::getSDInterface();
	baParams               = *parser->getBalanceAuthParams(baID);
	genStartIndex          = 0;
	convStartIndex         = 0;
	storageStartIndex      = baParams.nConventional;
	hydroStartIndex        = baParams.nConventional + baParams.nStorage;
}

void
LoadFollowingOpt::loadOptVariables()
{
	optData.control.iloObj           = IloExpr(optData.control.iloEnv);
	optData.iloGenPower              = SemiContVarMatrix(optData.control.iloEnv, lfData.lfLength);

	optData.iloRampUpConstraint      = IfThenMatrix(optData.control.iloEnv, lfData.lfLength);
	optData.iloRampDownConstraint    = IfThenMatrix(optData.control.iloEnv, lfData.lfLength);

	optData.iloZonePowerConstraint      = NumExprArgMatrix(optData.control.iloEnv, lfData.lfLength);
	optData.iloZoneInConstraint         = NumExprArgMatrix(optData.control.iloEnv, lfData.lfLength);
	optData.iloZoneIntraLineConstraint  = NumVar3D        (optData.control.iloEnv, lfData.lfLength);
	optData.iloZonePowerTransferOut     = NumVarMatrix    (optData.control.iloEnv, lfData.lfLength);
	optData.iloZonePowerTransferIn      = NumVarMatrix    (optData.control.iloEnv, lfData.lfLength);
	optData.iloZonePowerInPerConnection = NumExprMatrix   (optData.control.iloEnv, lfData.lfLength);
	optData.iloZonePowerOutNonnegativeConstraint = BoolVarMatrix(optData.control.iloEnv, lfData.lfLength);

	for (INTEGER step = 0; step < lfData.lfLength; step++) {
		optData.iloGenPower[step]               = IloSemiContVarArray(optData.control.iloEnv, lfData.nGenerators);

		optData.iloRampUpConstraint[step]       = IfThenArray(optData.control.iloEnv, lfData.nGenerators);
		optData.iloRampDownConstraint[step]     = IfThenArray(optData.control.iloEnv, lfData.nGenerators);

		optData.iloZonePowerConstraint      [step] = NumExprArgArray(optData.control.iloEnv, lfData.nZones);
		optData.iloZoneInConstraint         [step] = NumExprArgArray(optData.control.iloEnv, lfData.nZones);
		optData.iloZoneIntraLineConstraint  [step] = NumVarMatrix   (optData.control.iloEnv, lfData.nZones);
		optData.iloZonePowerTransferOut     [step] = IloNumVarArray (optData.control.iloEnv, lfData.nZones);
		optData.iloZonePowerTransferIn      [step] = IloNumVarArray (optData.control.iloEnv, lfData.nZones);
		optData.iloZonePowerInPerConnection [step] = IloNumExprArray(optData.control.iloEnv, lfData.nZones);
		optData.iloZonePowerOutNonnegativeConstraint[step] = IloBoolVarArray(optData.control.iloEnv, lfData.nZones);

		for (int zone = 0; zone < lfData.nZones; zone++) {
			optData.iloZoneIntraLineConstraint[step][zone] = IloNumVarArray(optData.control.iloEnv, lfData.nZones);
			optData.iloZonePowerTransferOut   [step][zone] = IloNumVar(optData.control.iloEnv);
			optData.iloZonePowerTransferIn    [step][zone] = IloNumVar(optData.control.iloEnv);
		}
		for (INTEGER gen = 0; gen < lfData.nGenerators; gen++) {
			optData.iloGenPower[step][gen]                 = IloSemiContVar(optData.control.iloEnv, lfData.genPowerMinLimit[gen], lfData.genPowerMaxLimit[gen]);
		}
	}
}

/*
 * This whole function may be unnecessary. It is used to clean up the optimizer
 * when UnitCommitOptimizer is destructed to free CPLEX variables as needed. So far,
 * this seems to have little impact on the CPLEX related valgrind errors
 */
void
LoadFollowingOpt::finalOptimizerCleanUp()
{
}

void
LoadFollowingOpt::initializeModelSet()
{
	initConstraint(new PowerBalanceConstraintLF(ucData, lfData, optData), POWER_BALANCE_CONSTRAINT_LF);
	initConstraint(new RampConstraintLF(ucData, lfData, optData), RAMP_CONSTRAINT_LF);
	initConstraint(new IntraZoneLineConstraintLF(ucData, lfData, optData), INTRAZONELINE_CONSTRAINT_LF);
	initConstraint(new IntraZonePowerInConstraintLF(ucData, lfData, optData), INTRAZONEPTDF_CONSTRAINT_LF);
	initConstraint(new ZonePowerBalanceConstraintLF(ucData, lfData, optData), ZONE_POWER_BALANCE_LF);
	initObjective(new MinimizeCostLF(ucData, lfData, optData));
}



// ############################################################################
// ## CHECK SOLUTION
// ############################################################################
void
LoadFollowingOpt::handleSolution()
{

	for (INTEGER gen = 0; gen < lfData.nGenerators; gen++){
		for (INTEGER step = 0; step < lfData.lfLength; step++) {
			targetPower[gen][step]    = (FLOAT)optData.control.iloCplex.getValue(optData.iloGenPower[step][gen]);
			generatorOn[gen][step]    = true;
			switchOn[gen][step]       = true;
			switchOff[gen][step]      = true;
		}
	}

	// convert the optimizer results to the initial ramp rate schedule, this
	// schedule will be completed in the setSchedule() method
	for (INTEGER gen = 0; gen < lfData.nGenerators; gen++){
		for (INTEGER step = 0; step < lfData.lfLength; step++) {
			lfData.rampSchedule[gen][step].targetPower = targetPower[gen][step];

			/*
			 *  an on generator is presumed to be supporting the load. When the
			 *  generator is off, it may be off OR it may be ramping up or down.
			 *  The computation to determine ramping status is made by the set
			 *  schedule using the optimized switch on/off states
			 */
			if (generatorOn[gen][step]) {
				lfData.rampSchedule[gen][step].state      = GENERATOR_SUPPORTING;
			} else {
				lfData.rampSchedule[gen][step].state      = GENERATOR_OFF;
			}

			/*
			 * Store the generator switch state. There are three conditions:
			 * the generator just switched on (generator is supporting the
			 * load); the generator just switched off (generator
			 * is starting to ramp down) or there is no change in the
			 * switch state. The no change condition must be investigated to
			 * determine when the ramp up starts based on the time the generator
			 * switches on and when the generator is off based on the generator
			 * ramp down time
			 */

			if (switchOn[gen][step]) {
				lfData.rampSchedule[gen][step].genSwitch    = SW_ON_STATE;
			} else if (switchOff[gen][step]) {
				lfData.rampSchedule[gen][step].genSwitch    = SW_OFF_STATE;
			} else {
				lfData.rampSchedule[gen][step].genSwitch    = NO_SWITCH;
			}
		}
	}
}

void
LoadFollowingOpt::handleNoSolution()
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
LoadFollowingOpt::printOptimzationVariables(std::ostream & out)
{
	cerr << "LOAD FOLLOWING OPTIMIZATION VARIABLES" << endl;
	/*
	for (INTEGER step = 0; step < lfData.lfLength; step++) {
		cerr << optData.iloPowerLoadBalance[step] << endl;
		cerr << optData.iloAvailablePower[step] << endl;
		cerr << optData.iloReserve[step] << endl;
		for (INTEGER gen = 0; gen < lfData.nGenerators; gen++) {
			cerr << optData.iloGenPower[step][gen] << endl;

			cerr << optData.iloGenIsOn[step][gen] << endl;

			cerr << optData.iloGenStartSupport[step][gen] << endl;
			cerr << optData.iloGenStopSupport[step][gen] << endl;

			cerr << optData.iloPowerLimit[step][gen] << endl;
			cerr << optData.iloPowerOffConstraint[step][gen] << endl;


			cerr << optData.iloOnOffError[step][gen] << endl;
			cerr << optData.iloStartStopConstraint[step][gen] << endl;

			cerr << optData.iloRampUpConstraint[step][gen] << endl;
			cerr << optData.iloRampDownConstraint[step][gen] << endl;

			cerr << optData.iloRampStatus[step][gen] << endl;

			cerr << optData.iloInitialMinUpTime << endl;
			cerr << optData.iloInitialMinDownTime << endl;
			cerr << optData.iloMinDownTimeConstraint[step][gen] << endl;
			cerr << optData.iloMinUpTimeConstraint[step][gen] << endl;
			cerr << optData.iloGeneratorMustBeOn[step][gen] << endl;
			cerr << optData.iloGeneratorMustBeOff[step][gen] << endl;

			cerr << optData.iloZCoeffConstraint[step][gen] << endl;
			cerr << optData.iloZValueConstraint[step][gen] << endl;
			for (INTEGER bp = 0; bp < x; bp++) {
				cerr << optData.iloZCoeff[step][gen][bp] << endl;
			}

		}
	}
	*/
}


} /* END OF NAMESPACE model */

