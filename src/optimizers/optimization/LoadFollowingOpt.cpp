#include "optimizers/optimization/LoadFollowingOpt.hpp"

#include "RIMParser.hpp"
#include "administrative/BalancingAuthority.hpp"

#include "simulation/Clock.hpp"
#include "simulation/SDInterface.hpp"
#include "simulation/Scheduler.hpp"

using std::ofstream;
using std::vector;
using std::endl;
using std::cout;
using std::cerr;

namespace model {

LoadFollowingOpt::~LoadFollowingOpt()
{
}

void LoadFollowingOpt::initializeOptimizer(const SimulationInfo &simulationInfo)
{
	// load the shared lfData object
	initBAInfo(simulationInfo);
	initZoneInfo();
	initGenerators(simulationInfo);
}

void
LoadFollowingOpt::initBAInfo(const SimulationInfo &simulationInfo)
{
	interpreter::RIMParser * parser = interpreter::RIMParser::getRIMParser();

	baParams                        = *parser->getBalanceAuthParams(baID);
	lfData.rampSchedule             = interface->getBARampSchedule(baID);
	lfData.nConventionalGenerators  = baParams.nConventional;
	lfData.nStorageGenerators       = baParams.nStorage;
	lfData.nHydroGenerators         = baParams.nHydro;
	lfData.nGenerators              = baParams.nGenerators;
	lfData.nZones                   = baParams.nZones;
	lfData.lfLength                 = simulationInfo.lfLength;
	lfData.rampLength               = HOURS_IN_DAY;  // TIME PROBLEM
	lfData.adjustedZoneLoad         = new FLOAT*[simulationInfo.nZones];
	// get these from the BA-Zone vectors
	lfData.zoneNumberOfGenerators   = new INTEGER[simulationInfo.nZones];
	lfData.zoneGeneratorOffset      = new INTEGER[simulationInfo.nZones];

	for (size_t zone = 0; zone < simulationInfo.nZones; zone++) {
		lfData.zoneNumberOfGenerators[zone] = parser->getNZoneGenerators(zone);
	}

	targetPowerData               = new FLOAT[lfData.lfLength * lfData.nConventionalGenerators];
	targetPower                   = new FLOAT*[lfData.nConventionalGenerators];
	generatorOnData               = new LOGICAL[lfData.lfLength * lfData.nConventionalGenerators];
	generatorOn                   = new LOGICAL*[lfData.nConventionalGenerators];
	switchOnData                  = new LOGICAL[lfData.lfLength * lfData.nConventionalGenerators];
	switchOn                      = new LOGICAL*[lfData.nConventionalGenerators];
	switchOffData                 = new LOGICAL[lfData.lfLength * lfData.nConventionalGenerators];
	switchOff                     = new LOGICAL*[lfData.nConventionalGenerators];

	INTEGER                  step           = 0;

	for (INTEGER gen = 0; gen < lfData.nConventionalGenerators; gen++) {
		targetPower[gen]     = &targetPowerData[step];
		generatorOn[gen]     = &generatorOnData[step];
		switchOn[gen]        = &switchOnData[step];
		switchOff[gen]       = &switchOffData[step];
		step += lfData.lfLength;
	}

}

void
LoadFollowingOpt::initZoneInfo()
{
	simulator::Scheduler   * simulator = simulator::Scheduler::getScheduler();
	lfData.adjustedZoneLoad  = new FLOAT * [baParams.nZones];
	lfData.adjustedBALoad    = new FLOAT[lfData.lfLength];
	for (size_t zone = 0; zone < baParams.nZones; zone++) {
		lfData.adjustedZoneLoad[zone] = simulator->getUCAdjSeries(zone);
	}
}

void
LoadFollowingOpt::initGenerators(const SimulationInfo &simulationInfo)
{

	lfData.costCurve            = new PiecewiseLinear[baParams.nGenerators];
	lfData.genRampDownRate      = new FLOAT[baParams.nGenerators];
	lfData.genRampUpRate        = new FLOAT[baParams.nGenerators];

	lfData.noxRate              = new FLOAT[baParams.nGenerators];
	lfData.soxRate              = new FLOAT[baParams.nGenerators];
	lfData.co2Rate              = new FLOAT[baParams.nGenerators];


	zonePTDFData              = new FLOAT[baParams.nZones*baParams.nZones];
	zoneMaxLinePowerData      = new FLOAT[baParams.nZones*baParams.nZones];
	lfData.zonePTDF             = new FLOAT*[baParams.nZones];
	lfData.zoneMaxLinePower     = new FLOAT*[baParams.nZones];

	for (size_t row = 0; row < baParams.nZones; ++row) {
		lfData.zonePTDF[row]          = &zonePTDFData[row*baParams.nZones];
		lfData.zoneMaxLinePower[row]  = &zoneMaxLinePowerData[row*baParams.nZones];
	}

//	lfData.actualTimeInState    = new INTEGER[baParams.nGenerators];;
//	lfData.status               = new GENERATOR_STATUS[baParams.nGenerators];

	if (baParams.nConventional > 0) {
		initConventional(simulationInfo);
	}

}

void
LoadFollowingOpt::initConventional(const SimulationInfo &simulationInfo)
{
	interpreter::RIMParser * parser          = interpreter::RIMParser::getRIMParser();

	ConventionalParams     * convParams      = NULL;
	const vector<size_t> &   baGenIndex      = parser->getBAGenSet(baID);

	for (INTEGER gen = 0; gen < baParams.nConventional; gen++) {
		convParams  = parser->getConventionalParams(baGenIndex[gen]);

		lfData.costCurve[gen]            = convParams->genParams.costCurve;
		lfData.genPowerMinLimit[gen]     = convParams->genParams.genPowerLimits[LOWER_BOUND];
		lfData.genPowerMaxLimit[gen]     = convParams->genParams.genPowerLimits[UPPER_BOUND];
		// NOTE: all rates are converted to rates in baseTimeStep units. When converting
		//       these rates to correspond to the optimization time step, the normalized
		//       rate is multiplied by the baseStepPer<opimization time step>
		lfData.genRampDownRate[gen]      = convParams->genParams.genRampRates[DOWN]*simulationInfo.baseTimeStep;
		lfData.genRampUpRate[gen]        = convParams->genParams.genRampRates[UP]*simulationInfo.baseTimeStep;

		lfData.noxRate[gen]              = convParams->genParams.noxRate*simulationInfo.baseTimeStep;
		lfData.soxRate[gen]              = convParams->genParams.soxRate*simulationInfo.baseTimeStep;
		lfData.co2Rate[gen]              = convParams->genParams.co2Rate*simulationInfo.baseTimeStep;

//		lfData.actualTimeInState[gen]    = 0;
//		lfData.status[gen]               = GENERATOR_INIT;
	}
//	FLOAT  ** ptdf  = parser->getZoneIntraBAPTDFMatrix(baID);
//	FLOAT  ** connection  = parser->getZoneIntraBAConnectMatrix(baID);

	for (size_t row = 0; row < baParams.nZones; row++) {
		for (size_t col = 0; col < baParams.nZones; col++) {
			vector<vector<Connection> >       connection = parser->getZoneIntraBAConnectMatrix(baID);
			lfData.zonePTDF[row][col]         = connection[row][col].ptdf;
			lfData.zoneMaxLinePower[row][col] = connection[row][col].lineCapacity;
		}
	}
}
/*
 * In the Optimization::setEnvironment method, the environment, model and
 * objective function are generated. After that happens, the UnitCommitOptimizer::loadOptimization()
 * method is called. This updates the model's state stored in the optimizer
 * reloads the CPLEX variables, loads the constraints used to solve the problem
 */
void
LoadFollowingOpt::loadOptimization()
{
	updateDataState();
	loadOptVariables();
	loadConstraint(POWER_BALANCE_CONSTRAINT_LF);
	loadConstraint(RAMP_CONSTRAINT_LF);
	loadConstraint(PIECEWISE_CONSTRAINT_LF);
	/*
	loadConstraint(INTRAZONELINE_CONSTRAINT_LF);
	loadConstraint(INTRAZONEPTDF_CONSTRAINT_LF);
	loadConstraint(ZONE_POWER_BALANCE_LF);
	*/
}

void
LoadFollowingOpt::loadInitialOptimization()
{
	currentTime = simulator::Clock::getStep();
	updateDataState();

	// specify the constraints in the optimization
	loadConstraint(POWER_BALANCE_CONSTRAINT_LF);
	loadConstraint(RAMP_CONSTRAINT_LF);
	loadConstraint(PIECEWISE_CONSTRAINT_LF);
	/*
	loadConstraint(INTRAZONELINE_CONSTRAINT_LF, INTRAZONELINE_CONSTRAINT_LF);
	loadConstraint(INTRAZONEPTDF_CONSTRAINT_LF, INTRAZONEPTDF_CONSTRAINT_LF);
	loadConstraint(ZONE_POWER_BALANCE_LF, ZONE_POWER_BALANCE_LF);
	*/


#if defined CHECK_OPT
//	dumpRampSchedule(); this does not belong here, move it to the init and solver
#endif
}

void
LoadFollowingOpt::updateDataState()
{
	/*
	INTEGER                  currentTime    = simulator::Clock::getCurrentRegStep();
	INTEGER                  prevTime       = simulator::Clock::getPreviousRegStep();
	INTEGER                  nextTime       = simulator::Clock::getNextRegStep();
	*/
	// The step lfData accesses generator lfData based on the SDI step, not on the clock time
	INTEGER                  currentStep    = simulator::Clock::getSDICurrentIndex();
	INTEGER                  prevStep       = simulator::Clock::getSDIPreviousIndex();
	INTEGER                  nextStep       = simulator::Clock::getSDINextIndex();

	lfData.convGenPrev       = interface->getConventionalData(prevStep, genStartIndex);
	lfData.convGenCurrent    = interface->getConventionalData(currentStep, genStartIndex);
	lfData.convGenNext       = interface->getConventionalData(nextStep, genStartIndex);

	// update time series
	for (INTEGER step = 0; step < lfData.lfLength; step++) {
		lfData.adjustedBALoad[step] = 0.0;
	}

	// get the adj load for a BA by summing the zone loads
	for (size_t zone = 0; zone < baParams.nZones; zone++) {
		for (INTEGER step = 0; step < lfData.lfLength; step++) {
			lfData.adjustedBALoad[step] += lfData.adjustedZoneLoad[zone][step];
		}
	}
}

void
LoadFollowingOpt::setupTrigger()
{
	currentTime = simulator::Clock::getStep();
	setInitialEnvironment(); // setup the optimizer lfData in the UCOptData structure
#if defined CHECK_OPT
	char                     fileName[256];
	INTEGER                  day            = simulator::Clock::getCurrentDayInSim();
	sprintf(fileName, "results/ramp-%d.csv", day);
	ofstream                 output(fileName);
	dumpRampSchedule(output, HOURS_IN_DAY);
	output.close();
#endif
}

void
LoadFollowingOpt::dailyTrigger()
{
	currentTime = simulator::Clock::getStep();

	setEnvironment(); // setup the optimizer lfData in the UCOptData structure

#if defined CHECK_OPT
	char                     fileName[256];
	INTEGER                  day            = simulator::Clock::getCurrentDayInSim();
	sprintf(fileName, "results/ramp-%d.csv", day);
	ofstream                 output(fileName);
	dumpRampSchedule(output, HOURS_IN_DAY);
	output.close();
#endif
}

void
LoadFollowingOpt::lfTrigger()
{
	ConventionalData          * currGenData        = interface->getConventionalData(simulator::Clock::getSDICurrentIndex());

	INTEGER                  step           = simulator::Clock::getCurrentHourInDay();
	for (INTEGER gen = 0; gen < lfData.nGenerators; ++gen) {
		currGenData[gen].generatorData.currentPower = ucData.rampSchedule[gen][step].targetPower;
		currGenData[gen].generatorData.status       = lfData.rampSchedule[gen][step].state;
	}
}

void
LoadFollowingOpt::setSchedules()
{
}

void
LoadFollowingOpt::updateOptState()
{
}

// ############################################################################
// ############################################################################
// ############################################################################
// ############################################################################

void
LoadFollowingOpt::dumpRampSchedule()
{
	INTEGER                  day            = simulator::Clock::getCurrentDayInSim();
	char                     fileName[256];

	sprintf(fileName, "results/ramp-%d.csv", day);
	ofstream                 output(fileName);
	dumpRampSchedule(output, HOURS_IN_DAY);

	output.close();

}

void
LoadFollowingOpt::dumpRampSchedule(std::ostream & output, INTEGER timeLength)
{
	output << ",Supporting Generator Power";
	for (INTEGER gen = 0; gen < lfData.nGenerators; gen++) {
		output << ",";
	}
	output << "Load";

	output << ",Current Power";
	for (INTEGER gen = 0; gen < lfData.nGenerators; gen++) {
		output << ",";
	}
	output << "Status";
	for (INTEGER gen = 0; gen < lfData.nGenerators; gen++) {
		output << ",";
	}

	output << "Switch";
	for (INTEGER gen = 0; gen < lfData.nGenerators; gen++){
		output << ",";
	}
	output << endl;

	output << ",";
	for (INTEGER gen = 0; gen < lfData.nGenerators; gen++) {
		output << gen << ",";
	}
	output << ",";
	for (INTEGER gen = 0; gen < lfData.nGenerators; gen++) {
		output << gen << ",";
	}
	for (INTEGER gen = 0; gen < lfData.nGenerators; gen++) {
		output << gen << ",";
	}
	for (INTEGER gen = 0; gen < lfData.nGenerators; gen++) {
		output << gen << ",";
	}

	output << endl;


	for (INTEGER step = 0; step < HOURS_IN_DAY; step++) {
		output << step << ",";

		cout << targetPower[0][step];
		if (lfData.rampSchedule[0][step].state == GENERATOR_SUPPORTING) {
			if (lfData.rampSchedule[0][step].targetPower > 0.000001) {
				output <<  lfData.rampSchedule[0][step].targetPower;
			} else {
				output << "0";
			}
		} else {
			output << "0";
		}

		for (INTEGER gen = 1; gen < lfData.nGenerators; gen++){
			if (lfData.rampSchedule[gen][step].state == GENERATOR_SUPPORTING) {
				if (lfData.rampSchedule[gen][step].targetPower > 0.000001) {
					output << "," <<  lfData.rampSchedule[gen][step].targetPower;
				} else {
					output << ",0";
				}
			} else {
				output << ",0";
			}
		}

		output << "," <<  lfData.adjustedBALoad[step];

		for (INTEGER gen = 0; gen < lfData.nGenerators; gen++){
			if (lfData.rampSchedule[gen][step].targetPower > 0.000001) {
				output << "," <<  lfData.rampSchedule[gen][step].targetPower;
			} else {
				output << ",0";
			}
		}
		for (INTEGER gen = 0; gen < lfData.nGenerators; gen++){
			output << "," << getGeneratorStatus(lfData.rampSchedule[gen][step].state);
		}
		for (INTEGER gen = 0; gen < lfData.nGenerators; gen++){
			output << "," << getSwitchStatus(lfData.rampSchedule[gen][step].genSwitch);
		}
		output << endl;
	}

}


// ############################################################################
// ############################################################################
// ############################################################################
// ############################################################################
void
LoadFollowingOpt::cplexExceptionResponse(std::ostream & out)
{
	cerr << "CPLEX failed to find a solution in LOAD FOLLOWING. The state of \n";
	cerr << "the system when the exception was thrown is:" << endl;

	cerr << "#################################################################\n";
	cerr << "#################################################################\n";
	cerr << "###                                                           ###\n";
	cerr << "###  Model state                                              ###\n";
	cerr << "###                                                           ###\n";
	cerr << "#################################################################\n";
	cerr << "#################################################################\n";
	printState(cerr);
	cerr << "#################################################################\n";
	cerr << "#################################################################\n";
	cerr << "###                                                           ###\n";
	cerr << "###  Optimizer parameters                                     ###\n";
	cerr << "###                                                           ###\n";
	cerr << "#################################################################\n";
	cerr << "#################################################################\n";
	printParameters(cerr);
	cerr << "#################################################################\n";
	cerr << "#################################################################\n";
	cerr << "###                                                           ###\n";
	cerr << "###  Optimizer variables state                                ###\n";
	cerr << "###                                                           ###\n";
	cerr << "#################################################################\n";
	cerr << "#################################################################\n";
	printOptimzationVariables(cerr);
	cerr << "#################################################################\n";
	cerr << "#################################################################\n";

}
void
LoadFollowingOpt::printState(std::ostream & out)
{
}

void
LoadFollowingOpt::printParameters(std::ostream & out)
{
}

} /* END OF NAMESPACE model */
