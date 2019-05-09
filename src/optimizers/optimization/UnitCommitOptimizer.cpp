#include "optimizers/optimization/UnitCommitOptimizer.hpp"
#include "structures.hpp"
#include "universe.hpp"

#include "RIMParser.hpp"
#include "administrative/BalancingAuthority.hpp"

#include "simulation/Clock.hpp"
#include "simulation/SDInterface.hpp"
#include "simulation/Scheduler.hpp"

#include <algorithm>
#include <boost/assign/list_of.hpp>
#include <boost/date_time/posix_time/posix_time_types.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>

using ::std::vector;
using ::std::count_if;
using ::std::ofstream;
using ::std::ostream;
using ::std::endl;
using ::boost::posix_time::ptime;
using ::boost::posix_time::ptime;
using ::boost::gregorian::date;
using ::boost::posix_time::time_duration;

namespace model {

UnitCommitOptimizer::~UnitCommitOptimizer()
{
	delete [] ucData.costCurve;
	delete [] ucData.inputOutputCurve;

	delete [] ucData.fuelCost;
	delete [] ucData.genTurnOnCost;      // start up, shut down states
	delete [] ucData.genTurnOffCost;     // start up, shut down states
	delete [] ucData.genPowerMaxLimit;   // pMin, pMax
	delete [] ucData.genRampUpRate;      // rampDownRate, rampUpRate
	delete [] ucData.genRampDownRate;    // rampDownRate, rampUpRate
	delete [] ucData.soxRate;            // rampDownRate, rampUpRate
	delete [] ucData.co2Rate;            // rampDownRate, rampUpRate
	delete [] ucData.operationsAndMaintenanceVariableCost;
	delete [] ucData.zoneCo2Cost;
	delete [] ucData.minUpTime;
	delete [] ucData.minDownTime;
	delete [] ucData.adjustedZoneLoad;      // ba's set
	delete [] ucData.adjustedBALoad;
	delete [] ucData.genPowerMinLimit;
	delete [] ucData.noxRate;
	delete [] ucData.mustbeon;
	delete [] zonePTDFData;
	delete [] zoneMaxLinePowerData;
	delete [] ucData.zonePTDF;
	delete [] ucData.zoneMaxLinePower;
	delete [] ucData.zoneNumberOfGenerators;
	delete [] ucData.zoneGeneratorOffset;
}

void
UnitCommitOptimizer::initializeOptimizer(void)
{
	interpreter::RIMParser * parser = interpreter::RIMParser::getRIMParser();
	// load the shared ucData object
	initBAInfo(parser);
	initZoneInfo(parser);
	initGenerators(parser);
}

void
UnitCommitOptimizer::initBAInfo(interpreter::RIMParser * parser)
{
	ucData.reserveFactor            = baParams.reserveFactor;
	ucData.rampSchedule             = interface->getBARampSchedule(baID);
	ucData.nConventionalGenerators  = baParams.nConventional;
	ucData.nStorageGenerators       = baParams.nStorage;
	ucData.nHydroGenerators         = baParams.nHydro;
	ucData.nGenerators              = baParams.nGenerators;
	ucData.nZones                   = baParams.nZones;
	ucData.rampLength               = HOURS_IN_DAY;  // TIME PROBLEM
	ucData.zoneCo2Cost              = new FLOAT  [baParams.nZones];
	ucData.zoneNumberOfGenerators   = new INTEGER[baParams.nZones];
	ucData.zoneGeneratorOffset      = new INTEGER[baParams.nZones];

	INTEGER zoneGeneratorOffset = 0;
	for (size_t zone = 0; zone < baParams.nZones; zone++) {
		ucData.zoneNumberOfGenerators[zone] = parser->getNZoneGenerators(zone);
		ucData.zoneGeneratorOffset[zone] = zoneGeneratorOffset;
		zoneGeneratorOffset += ucData.zoneNumberOfGenerators[zone];
		ucData.zoneCo2Cost[zone] = parser->getZoneParams(zone)->co2Cost;
	}
}

void
UnitCommitOptimizer::initZoneInfo(interpreter::RIMParser * parser)
{
	simulator::Scheduler   * simulator = simulator::Scheduler::getScheduler();

	ucData.adjustedZoneLoad  = new FLOAT * [baParams.nZones];
	ucData.adjustedBALoad    = new FLOAT[ucData.ucLength];

	for (size_t zone = 0; zone < baParams.nZones; zone++) {
		ucData.adjustedZoneLoad[zone] = simulator->getUCAdjSeries(zone);
	}
}

void
UnitCommitOptimizer::initGenerators(interpreter::RIMParser * parser)
{

	ucData.costCurve            = new PiecewiseLinear[baParams.nGenerators];
	ucData.inputOutputCurve     = new PiecewiseLinear[baParams.nGenerators];
	ucData.fuelCost             = new FLOAT[baParams.nGenerators];
	ucData.genTurnOffCost       = new FLOAT[baParams.nGenerators];
	ucData.genTurnOnCost        = new FLOAT[baParams.nGenerators];
	ucData.genPowerMinLimit     = new FLOAT[baParams.nGenerators];
	ucData.genPowerMaxLimit     = new FLOAT[baParams.nGenerators];
	ucData.genRampDownRate      = new FLOAT[baParams.nGenerators];
	ucData.genRampUpRate        = new FLOAT[baParams.nGenerators];
	
	ucData.noxRate              = new FLOAT[baParams.nGenerators];
	ucData.soxRate              = new FLOAT[baParams.nGenerators];
	ucData.co2Rate              = new FLOAT[baParams.nGenerators];

	ucData.operationsAndMaintenanceVariableCost = new FLOAT[baParams.nGenerators];
	ucData.mustbeon		    = new INTEGER[baParams.nGenerators];
	ucData.minDownTime          = new INTEGER[baParams.nGenerators];
	ucData.minUpTime            = new INTEGER[baParams.nGenerators];


	zonePTDFData              = new FLOAT[baParams.nZones*baParams.nZones];
	zoneMaxLinePowerData      = new FLOAT[baParams.nZones*baParams.nZones];
	ucData.zonePTDF             = new FLOAT*[baParams.nZones];
	ucData.zoneMaxLinePower     = new FLOAT*[baParams.nZones];

	for (size_t row = 0; row < baParams.nZones; ++row) {
		ucData.zonePTDF[row]          = &zonePTDFData[row*baParams.nZones];
		ucData.zoneMaxLinePower[row]  = &zoneMaxLinePowerData[row*baParams.nZones];
	}

	if (baParams.nConventional > 0) {
		initConventional(parser);
	}

}

void
UnitCommitOptimizer::initConventional(interpreter::RIMParser * parser)
{
	ConventionalParams     * convParams      = NULL;
	const vector<size_t> &   baGenIndex      = parser->getBAGenSet(baID);

	for (INTEGER gen = 0; gen < baParams.nConventional; gen++) {
		convParams  = parser->getConventionalParams(baGenIndex[gen]);

		ucData.costCurve[gen]            = convParams->genParams.costCurve;
		ucData.inputOutputCurve[gen]     = convParams->genParams.heatRateCurve;
		ucData.fuelCost[gen]             = convParams->genParams.fuelCost;
		ucData.genTurnOffCost[gen]       = convParams->genTransitionCost[OFF_STATE];
		ucData.genTurnOnCost[gen]        = convParams->genTransitionCost[ON_STATE];
		ucData.genPowerMinLimit[gen]     = convParams->genParams.genPowerLimits[LOWER_BOUND];
		ucData.genPowerMaxLimit[gen]     = convParams->genParams.genPowerLimits[UPPER_BOUND];
		// NOTE: all rates are converted to rates in baseTimeStep units. When converting
		//       these rates to correspond to the optimization time step, the normalized
		//       rate is multiplied by the baseStepPer<opimization time step>
		// ucLength = uc steps in iteration
		// ucTimeStep = minutes in uc step
		// baseTimeStep = length of simulation loop in minutes
		// genPArams.genRampRates = (n MW / HOUR) / (x minutes / loop) / (60 minutes/hour / x minutes/loop)
		//                        = (n MW / HOUR) / (60 minutes / hour)
		//                        = (n/60 MW/minute)
		int ucStepLengthInMinutes = MINUTES_IN_HOUR; // TODO FIXME this needs to be fixed to allow uc step length other than 1 hour
		ucData.genRampDownRate[gen]      = convParams->genParams.genRampRates[DOWN]*ucStepLengthInMinutes;
		ucData.genRampUpRate[gen]        = convParams->genParams.genRampRates[UP]*ucStepLengthInMinutes;

		ucData.noxRate[gen]              = convParams->genParams.noxRate*ucStepLengthInMinutes;
		ucData.soxRate[gen]              = convParams->genParams.soxRate*ucStepLengthInMinutes;
		ucData.co2Rate[gen]              = convParams->genParams.co2Rate*ucStepLengthInMinutes;

		ucData.operationsAndMaintenanceVariableCost[gen] = convParams->genParams.operationsAndMaintenanceVariableCost;
		if (convParams->genParams.muston){
			ucData.mustbeon[gen]	= 1 ;
		}
		ucData.minDownTime[gen]          = convParams->minTimeInState[OFF_STATE];
		ucData.minUpTime[gen]            = convParams->minTimeInState[ON_STATE];

	}
	for (size_t row = 0; row < baParams.nZones; row++) {
		for (size_t col = 0; col < baParams.nZones; col++) {
			vector<vector<Connection> >       connection = parser->getZoneIntraBAConnectMatrix(baID);
			ucData.zonePTDF[row][col]         = connection[row][col].ptdf;
			ucData.zoneMaxLinePower[row][col] = connection[row][col].lineCapacity;
		}
	}
}

static const vector<UC_CONSTRAINT> unitCommitConstraints = boost::assign::list_of
//	(POWER_BALANCE_CONSTRAINT_UC)
	(POWER_LIMIT_CONSTRAINT_UC  )
	(START_STOP_CONSTRAINT_UC   )
	(RAMP_CONSTRAINT_UC         )
	(RESERVE_CONSTRAINT_UC      )
	(UP_DOWN_CONSTRAINT_UC      )
	(PIECEWISE_CONSTRAINT_UC    )
	(INTRAZONELINE_CONSTRAINT_UC)
//	(INTRAZONEPTDF_CONSTRAINT_UC)
//	(TRANSPORT_CONSTRAINT_UC    )
	(ZONE_POWER_BALANCE_UC      )
	(DERATING_UC                )
;
struct NonZero { bool operator() (const FLOAT & value) {return value < -1e-6 || value > 1e-6;} };
struct AnyNonZeros {
	AnyNonZeros(ptrdiff_t n):n(n){}
	bool operator() (const FLOAT *x) { ptrdiff_t count = count_if(&x[0], &x[n], NonZero()); return 0 < count; }
	private: ptrdiff_t n;
};
bool UnitCommitOptimizer::constraint_enabled(UC_CONSTRAINT c) {
	//if ( c == INTRAZONEPTDF_CONSTRAINT_UC) {
		//ptrdiff_t ptdfCount = count_if(&ucData.zonePTDF[0], ucData.zonePTDF + this->ucData.nZones, AnyNonZeros(this->ucData.nZones));
    //bool hasPtdf = 0 < ptdfCount;
		//if (hasPtdf)
			//return c == INTRAZONEPTDF_CONSTRAINT_UC;
		//return c == TRANSPORT_CONSTRAINT_UC;
	//}
	return c != PIECEWISE_CONSTRAINT_UC;
}
bool UnitCommitOptimizer::constraint_enabled_for_initial_optimization(UC_CONSTRAINT c) {
	//if ( c == INTRAZONEPTDF_CONSTRAINT_UC) {
		//ptrdiff_t ptdfCount = count_if(&ucData.zonePTDF[0], ucData.zonePTDF + this->ucData.nZones, AnyNonZeros(this->ucData.nZones));
    //bool hasPtdf = 0 < ptdfCount;
		//if (hasPtdf)
			//return c == INTRAZONEPTDF_CONSTRAINT_UC;
		//return c == TRANSPORT_CONSTRAINT_UC;
	//}
	return
			RAMP_CONSTRAINT_UC      != c &&
			UP_DOWN_CONSTRAINT_UC   != c &&
			PIECEWISE_CONSTRAINT_UC != c;
}
/*
 * In the Optimization::setEnvironment method, the environment, model and
 * objective function are generated. After that happens, the UnitCommitOptimizer::loadOptimization()
 * method is called. This updates the model's state stored in the optimizer
 * reloads the CPLEX variables, loads the constraints used to solve the problem
 */
void
UnitCommitOptimizer::loadOptimization()
{
	updateDataState();
	loadOptVariables();
	for (vector<UC_CONSTRAINT>::const_iterator it = unitCommitConstraints.begin(); it != unitCommitConstraints.end(); it++) {
		if (constraint_enabled(*it))
			loadConstraint(*it);
	}
}

void
UnitCommitOptimizer::loadInitialOptimization()
{
	currentTime = simulator::Clock::getStep();
	updateDataState();

	for (vector<enum UC_CONSTRAINT>::const_iterator it = unitCommitConstraints.begin(); it != unitCommitConstraints.end(); it++) {
		if (constraint_enabled_for_initial_optimization(*it))
			loadConstraint(*it);
	}
}

void
UnitCommitOptimizer::updateDataState()
{
	// The step ucData accesses generator ucData based on the SDI step, not on the clock time
	INTEGER                  currentStep    = simulator::Clock::getSDICurrentIndex();
	INTEGER                  prevStep       = simulator::Clock::getSDIPreviousIndex();
	INTEGER                  nextStep       = simulator::Clock::getSDINextIndex();

	ucData.convGenPrev       = interface->getConventionalData(prevStep, genStartIndex);
	ucData.convGenCurrent    = interface->getConventionalData(currentStep, genStartIndex);
	ucData.convGenNext       = interface->getConventionalData(nextStep, genStartIndex);

	ptime t0(simulator::Clock::getCurrentTime());
	time_duration ucStepDuration(0,universe.simulationInfo.ucTimeStep,0);
	for (size_t step = 0; step < ucData.ucLength; step++) {
		ucData.time[step] = t0 + ucStepDuration * step;
	}

	// update time series
	for (size_t step = 0; step < ucData.ucLength; step++) {
		ucData.adjustedBALoad[step] = 0.0;
	}

	// get the adj load for a BA by summing the zone loads
	for (size_t zone = 0; zone < baParams.nZones; zone++) {
		for (size_t step = 0; step < ucData.ucLength; step++) {
			ucData.adjustedBALoad[step] += ucData.adjustedZoneLoad[zone][step];
		}
	}
}

void
UnitCommitOptimizer::setupTrigger()
{
	currentTime = simulator::Clock::getStep();
	setInitialEnvironment(); // setup the optimizer ucData in the UCOptData structure
	dumpRampSchedule();
}

void
UnitCommitOptimizer::dailyTrigger()
{
	currentTime = simulator::Clock::getStep();
	setEnvironment(); // setup the optimizer ucData in the UCOptData structure
	dumpRampSchedule();
}

void
UnitCommitOptimizer::ucTrigger()
{
}

void
UnitCommitOptimizer::updateOptState()
{
}

void
dumpRampScheduleCsv(ostream & output, INTEGER const timeLength, INTEGER const nGen, RampControl const* const* ramp, FLOAT const* adjustedBALoad );
void
UnitCommitOptimizer::dumpRampSchedule()
{
#if defined CHECK_OPT
	INTEGER                  day            = simulator::Clock::getCurrentDayInSim();
	char                     fileName[256];
	sprintf(fileName, "results/ramp-%d.csv", day);
	ofstream                 output(fileName);

	INTEGER timeLength = simulator::Clock::getNumberOfUnitCommitStepsInProjection();
	dumpRampScheduleCsv(output, timeLength, ucData.nGenerators, ucData.rampSchedule, ucData.adjustedBALoad);
	output.close();
#endif
}

double round(double precision, double value) {
  return floor(value / precision + .5) * precision;
}
void
dumpRampScheduleCsv(ostream & output, INTEGER const timeLength, INTEGER const nGen, RampControl const* const* ramp, FLOAT const* adjustedBALoad )
{
	output << ",Supporting Generator Power";
	for (INTEGER gen = 0; gen < nGen; gen++) {
		output << ",";
	}
	output << "Load";

	output << ",Current Power";
	for (INTEGER gen = 0; gen < nGen; gen++) {
		output << ",";
	}
	output << "Status";
	for (INTEGER gen = 0; gen < nGen; gen++) {
		output << ",";
	}

	output << "Switch";
	for (INTEGER gen = 0; gen < nGen; gen++){
		output << ",";
	}
	output << endl;

	output << ",";
	for (INTEGER gen = 0; gen < nGen; gen++) {
		output << gen << ",";
	}
	output << ",";
	for (INTEGER gen = 0; gen < nGen; gen++) {
		output << gen << ",";
	}
	for (INTEGER gen = 0; gen < nGen; gen++) {
		output << gen << ",";
	}
	for (INTEGER gen = 0; gen < nGen; gen++) {
		output << gen << ",";
	}

	output << endl;

	const double precision = 1e-6;
	for (INTEGER step = 0; step < timeLength; step++) {
		output << step;
		for (INTEGER gen = 0; gen < nGen; gen++) {
			if (GENERATOR_SUPPORTING == ramp[gen][step].state)
				output << "," <<  round(precision, ramp[gen][step].targetPower);
			else
				output << ",0";
		}
		output << "," <<  adjustedBALoad[step];
		for (INTEGER gen = 0; gen < nGen; gen++){
				output << "," <<  round(precision, ramp[gen][step].targetPower);
		}
		for (INTEGER gen = 0; gen < nGen; gen++){
			output << "," << getGeneratorStatus(ramp[gen][step].state);
		}
		for (INTEGER gen = 0; gen < nGen; gen++){
			output << "," << getSwitchStatus(ramp[gen][step].genSwitch);
		}
		output << endl;
	}
}

void
UnitCommitOptimizer::cplexExceptionResponse(ostream & out)
{
	out << "CPLEX failed to find a solution in UNIT COMMIT. The state of the\n";
	out << "system when the exception was thrown is:" << endl;

	out << "#################################################################\n";
	out << "#################################################################\n";
	out << "###                                                           ###\n";
	out << "###  Model state                                              ###\n";
	out << "###                                                           ###\n";
	out << "#################################################################\n";
	out << "#################################################################\n";
	printState(out);
	out << "#################################################################\n";
	out << "#################################################################\n";
	out << "###                                                           ###\n";
	out << "###  Optimizer parameters                                     ###\n";
	out << "###                                                           ###\n";
	out << "#################################################################\n";
	out << "#################################################################\n";
	printParameters(out);
	out << "#################################################################\n";
	out << "#################################################################\n";
	out << "###                                                           ###\n";
	out << "###  Optimizer variables state                                ###\n";
	out << "###                                                           ###\n";
	out << "#################################################################\n";
	out << "#################################################################\n";
	printOptimzationVariables(out);
	out << "#################################################################\n";
	out << "#################################################################\n";

}

void
UnitCommitOptimizer::printState(ostream & out)
{
	out << "BA #" << baID << " : " << baParams.name << endl;
	out << "Current time                = " << currentTime << endl;
	out << "Conventional state ucData " << endl;
	for (INTEGER gen = 0; gen < baParams.nConventional; gen++) {
		out << "Generator #" << gen << endl;
		out << "PREVIOUS STEP " << endl;
		out << "\tcurrent power                  : " << ucData.convGenPrev[gen].generatorData.currentPower << endl;
		out << "\toutput time in state           : " << ucData.convGenPrev[gen].generatorData.timeInState << endl;
		out << "\tcommit state                   : " << ucData.convGenPrev[gen].generatorData.status << endl;
		out << "CURRENT STEP " << endl;
		out << "\tcurrent power                  : " << ucData.convGenCurrent[gen].generatorData.currentPower << endl;
		out << "\toutput time in state           : " << ucData.convGenCurrent[gen].generatorData.timeInState << endl;
		out << "\tcommit state                   : " << ucData.convGenCurrent[gen].generatorData.status << endl;
		out << "NEXT STEP " << endl;
		out << "\tcurrent power                  : " << ucData.convGenNext[gen].generatorData.currentPower << endl;
		out << "\toutput time in state           : " << ucData.convGenNext[gen].generatorData.timeInState << endl;
		out << "\tcommit state                   : " << ucData.convGenNext[gen].generatorData.status << endl;
	}
	out << "UC adj load" << endl;
	if (ucData.ucLength > 8) {
		for (size_t step = 0; step < 4; step++) {
			out << ucData.adjustedBALoad[step] << ", ";
		}
		out << " . . . ,";
		for (size_t step = ucData.ucLength-4; step < ucData.ucLength - 1; step++) {
			out << ucData.adjustedBALoad[step] << ", ";
		}
		out << ucData.adjustedBALoad[ucData.ucLength - 1] << endl;
	} else {
		for (size_t step = 0; step < ucData.ucLength; step++) {
			out << ucData.adjustedBALoad[step] << ", ";
		}
	}
}

void
UnitCommitOptimizer::printParameters(ostream & out)
{
	out << "Current time                = " << currentTime;
	out << "\treserve factor               : " << ucData.reserveFactor << endl;
	out << "\tramp schedule                : " << ucData.rampSchedule << endl;
	out << "\tn conventional generators    : " << ucData.nConventionalGenerators << endl;
	out << "\tn storage generators         : " << ucData.nStorageGenerators << endl;
	out << "\tn hydro generators           : " << ucData.nHydroGenerators << endl;
	out << "\tn generators                 : " << ucData.nGenerators << endl;
	out << "\tn zones                      : " << ucData.nZones << endl;
	out << "\tUC projection length         : " << ucData.ucLength << endl;
	out << "\tramp length                  : " << ucData.rampLength << endl;
	for (size_t gen = 0; gen < ucData.nGenerators; gen++) {
		out << "\tGenerator cost curve: "<< endl;
		printPWCurve(ucData.costCurve[gen], out);
		out << "\tstart up cost                : " << ucData.genTurnOffCost[gen] << endl;
		out << "\tshut down cost               : " << ucData.genTurnOnCost[gen] << endl;
		out << "\tminimum power limit          : " << ucData.genPowerMinLimit[gen] << endl;
		out << "\tmaximum power limit          : " << ucData.genPowerMaxLimit[gen] << endl;
		out << "\tramp down rate               : " << ucData.genRampDownRate[gen] << endl;
		out << "\tramp up rate                 : " << ucData.genRampUpRate[gen] << endl;
		out << "\tNOx emission rate            : " << ucData.noxRate[gen] << endl;
		out << "\tSOx emission rate            : " << ucData.soxRate[gen] << endl;
		out << "\tCO2 emission rate            : " << ucData.co2Rate[gen] << endl;
		out << "\tMinimum off time             : " << ucData.minDownTime[gen] << endl;
		out << "\tMinimum on time              : " << ucData.minUpTime[gen] << endl;
	}
}

} /* END OF NAMESPACE model */
