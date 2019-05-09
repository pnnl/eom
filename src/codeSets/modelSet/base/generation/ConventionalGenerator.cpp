#include "generation/ConventionalGenerator.hpp"
#include "models/timeSeries/ScheduleData.hpp"
#include "simulation/SDInterface.hpp"
#include "simulation/Scheduler.hpp"
#include "simulation/Clock.hpp"
#include "exceptions/Exception.hpp"

#include <string.h>

using std::clog;
using std::endl;

namespace model {

ConventionalGenerator::ConventionalGenerator(const SimulationInfo & simulationInfo, size_t id, ConventionalParams & params) :
	conventionalID(id),
	ucRampSchedule(NULL),
	params(&params),
	interface(NULL)
{
	interface                = simulator::SDInterface::getSDInterface();
	// setup time step dependent parameters to match the regulation time step
	// ASSUMPTION: ramp rates are given in hourly time steps
	params.genParams.genRampRates[UP]    /= (MINUTES_IN_HOUR/simulationInfo.baseTimeStep);
	params.genParams.genRampRates[DOWN]  /= (MINUTES_IN_HOUR/simulationInfo.baseTimeStep);
	params.genParams.noxRate             /= (MINUTES_IN_HOUR/simulationInfo.baseTimeStep);
	params.genParams.soxRate             /= (MINUTES_IN_HOUR/simulationInfo.baseTimeStep);
	params.genParams.co2Rate             /= (MINUTES_IN_HOUR/simulationInfo.baseTimeStep);

	// the ramp schedule is setup for all generators (perhaps)
	ucRampSchedule      = interface->getGenRampSchedule(generatorID);

	implementsLoadFollowing = simulationInfo.implementsLoadFollowing;
	implementsRegulation = simulationInfo.implementsRegulation;
}

ConventionalGenerator::~ConventionalGenerator()
{
}

void
ConventionalGenerator::dailyTrigger()
{

}

void
ConventionalGenerator::setupTrigger()
{
	// each invocation of this comes from the Zone setupTrigger, which loops through the zone's generator set
	GeneratorData &nextStepGenData         = *((GeneratorData*) interface->getConventionalData(simulator::Clock::getSDINextIndex(), generatorID));
	GeneratorData &currentStepGenData      = *((GeneratorData*) interface->getConventionalData(simulator::Clock::getSDICurrentIndex(), generatorID));
	GeneratorData &previousStepGenData     = *((GeneratorData*) interface->getConventionalData(simulator::Clock::getSDIPreviousIndex(), generatorID));
	// make sure we get initialized with defaults, in case the interface is allocating a big char* and giving me a chunk
	nextStepGenData = GeneratorData();
	currentStepGenData = GeneratorData();
	previousStepGenData = GeneratorData();
}

void
ConventionalGenerator::ucTrigger()
{
	INTEGER       step            = simulator::Clock::getCurrentUCStep();
	GeneratorData &previous       = interface->getConventionalData(simulator::Clock::getSDIPreviousIndex(), generatorID)->generatorData;
	GeneratorData &current        = interface->getConventionalData(simulator::Clock::getSDICurrentIndex(), generatorID)->generatorData;

	current.currentPower = ucRampSchedule[step].targetPower;
	current.status       = ucRampSchedule[step].state;
	current.timeInState  = (current.status == previous.status) ? previous.timeInState + 1 : 0;
}

void
ConventionalGenerator::lfTrigger()
{
}
/*
 * The system has some initial data, some from the last step and some from the
 * optimizer this data is used to fill in the blanks of the startData
 */
void
ConventionalGenerator::regTrigger()
{
}


char * ConventionalGenerator::getName(){return params->genParams.name;};
void ConventionalGenerator::printState(std::ostream & out){};
void ConventionalGenerator::printParameters(std::ostream & out){};
bool ConventionalGenerator::getCommit(){return params->genParams.canCommit;};
bool ConventionalGenerator::getRegulate(){return params->genParams.canRegulate;};
bool ConventionalGenerator::getZone(){return params->genParams.zone;};
bool ConventionalGenerator::getBA(){return params->genParams.balancingAuth;};

void
ConventionalGenerator::dumpData()
{
	clog << "\tGenerator " << conventionalID << endl;
	clog << "\tName                 = " << params->genParams.name << endl;
	clog << "\tBalancing Authority  = " << params->genParams.balancingAuth << endl;
	clog << "\tZone                 = " << params->genParams.zone << endl;
	clog << "\tRamp Schedule: " << endl;
	for (INTEGER step = 0; step < HOURS_IN_DAY; step++) {
		clog << "\t    target power     = " << ucRampSchedule[step].targetPower << endl;
		clog << "\t    state            = " << getGeneratorStatus(ucRampSchedule[step].state) << endl;
		clog << "\t    time in state    = " << ucRampSchedule[step].timeInState << endl;
		clog << "\t    state switch     = " << ucRampSchedule[step].genSwitch << endl;
//		clog << "\t    error in state   = " << ucRampSchedule[step].errorState << endl;
	}
	clog << "\tParameters: " << endl;
	clog << "\t\tHeat Rate curve: " << endl;
	clog << "\t\t{" << params->genParams.heatRateCurve.nSegments;
	for (INTEGER seg = 0; seg < N_PW_SEGMENTS; seg++) {
		clog << " (";
		clog << params->genParams.heatRateCurve.segment[seg].startPoint[POWER];
		clog << " ,";
		clog << params->genParams.heatRateCurve.segment[seg].startPoint[COST];
		clog << " ) ";
		clog << " (";
		clog << params->genParams.heatRateCurve.segment[seg].endPoint[POWER];
		clog << " ,";
		clog << params->genParams.heatRateCurve.segment[seg].endPoint[COST];
		clog << " ) ";
		clog << params->genParams.heatRateCurve.segment[seg].intercept;
		clog << " ,";
		clog << params->genParams.heatRateCurve.segment[seg].slope;
	}
	clog << endl;
	clog << "\t\tCost curve: " << endl;
	clog << "\t\t{" << params->genParams.heatRateCurve.nSegments;
	for (INTEGER seg = 0; seg < N_PW_SEGMENTS; seg++) {
		clog << " (";
		clog << params->genParams.costCurve.segment[seg].startPoint[POWER];
		clog << " ,";
		clog << params->genParams.costCurve.segment[seg].startPoint[COST];
		clog << " ) ";
		clog << " (";
		clog << params->genParams.costCurve.segment[seg].endPoint[POWER];
		clog << " ,";
		clog << params->genParams.costCurve.segment[seg].endPoint[COST];
		clog << " ) ";
		clog << params->genParams.costCurve.segment[seg].intercept;
		clog << " ,";
		clog << params->genParams.costCurve.segment[seg].slope;
	}
	clog << "}" << endl;
	clog << "\t    Min Power limit    = " << params->genParams.genPowerLimits[UPPER_BOUND] << endl;
	clog << "\t    Max Power limit    = " << params->genParams.genPowerLimits[LOWER_BOUND] << endl;
	clog << "\t    Ramp down rate     = " << params->genParams.genRampRates[DOWN] << endl;
	clog << "\t    Ramp up rate       = " << params->genParams.genRampRates[UP] << endl;
	clog << "\t    Fuel costs         = " << params->genParams.fuelCost << endl;

	clog << "\t    Can commit         = " << getBooleanValue(params->genParams.canCommit) << endl;
	clog << "\t    Can load following = " << getBooleanValue(params->genParams.canLoadFollow) << endl;
	clog << "\t    Can regulate       = " << getBooleanValue(params->genParams.canRegulate) << endl;

	clog << "\t    NOx rate           = " << params->genParams.noxRate << endl;
	clog << "\t    SOx rate           = " << params->genParams.soxRate << endl;
	clog << "\t    CO2 rate           = " << params->genParams.co2Rate << endl;

	clog << "\t    Fuel type          = " << getFuelType(params->genParams.fuelType) << endl;

	clog << "\t    Shut down cost     = " << params->genTransitionCost[DOWN] << endl;
	clog << "\t    Start up cost      = " << params->genTransitionCost[UP] << endl;
	clog << "\t    Min Off time       = " << params->minTimeInState[DOWN] << endl;
	clog << "\t    Min On time        = " << params->minTimeInState[UP] << endl;

	clog << "\t    Generator status   = " << getGeneratorStatus(params->status) << endl;
}
} /* END OF NAMESPACE model */
