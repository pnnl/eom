/* ***************************************************************************
 * ScheduleData.cpp
 *
 *      Author: Kevin Glass
 *  Created on: May 28, 2010
 *
 * Inheritance
 *
 * Class members
 *
 * Class variables
 *
 * Purpose
 *
 ************************************************************************** */
#include "models/timeSeries/ScheduleData.hpp"
#include "simulation/Clock.hpp"
#include "generation/ConventionalGenerator.hpp"

namespace model {

ScheduleData * ScheduleData::scheduleSet = NULL;

ScheduleData::ScheduleData(const SimulationInfo & simulationInfo) :
	lfPowerTargetData(NULL),
	genPowerTargetData(NULL),
	nBalancingAuths(0),
	nZones(0),
	nGenerators(0),
	nConventionalGenerators(0),
	ucTimeStepsPerDay(0),
	lfLength(0)
{
	nBalancingAuths         = simulationInfo.nBalancingAuths;
	nZones                  = simulationInfo.nZones;
	nConventionalGenerators = simulationInfo.nConventionalGenerators;

	nGenerators             = simulationInfo.nGenerators;
	ucTimeStepsPerDay       = simulationInfo.nUCTimeStepsInDay;
	lfLength                = simulationInfo.lfLength;

	setSchuduleData();
}

ScheduleData::~ScheduleData()
{
	delete [] lfPowerTargetData;
	delete [] genPowerTargetData;
	ScheduleData::scheduleSet    = NULL;
}

void
ScheduleData::setSchuduleData()
{
	lfPowerTargetData        = new FLOAT[lfLength * nGenerators];
	genPowerTargetData       = new FLOAT*[nGenerators];

	INTEGER                  lfOffset       = 0;
	INTEGER                  ucOffset       = 0;
	for (INTEGER gen = 0; gen < nGenerators; gen++) {
		genPowerTargetData[gen]        = &lfPowerTargetData[lfOffset];
		lfOffset        += lfLength;
		ucOffset        += ucTimeStepsPerDay;
	}
	setupPowerTargetSchedule();
	setupRampingSchedule();
}

void
ScheduleData::setupRampingSchedule()
{
	for (int32_t gen = 0; gen < nGenerators; gen++){
		for (int32_t step = 0; step < ucTimeStepsPerDay; step++) {
//			genRampScheduleData[gen][step].targetPower   = 0.0;
//			genRampScheduleData[gen][step].state         = GENERATOR_INIT;
//			genRampScheduleData[gen][step].errorState    = false;
//			genRampScheduleData[gen][step].timeInState   = 0;
//			genRampScheduleData[gen][step].genSwitch     = NO_SWITCH;
		}
	}
}

void
ScheduleData::setupPowerTargetSchedule()
{
	for (int32_t gen = 0; gen < nGenerators; gen++){
		for (int32_t step = 0; step < lfLength; step++) {
			genPowerTargetData[gen][step]   = 0.0;
		}
	}
}

} /* END OF NAMESPACE model */
