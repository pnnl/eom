/* ***************************************************************************
 * Author      : Kevin Glass
 * Date        : Feb 11, 2010
 * File        : Zone.cpp
 * Project     : RIM
 *
 *
 * Contents    :
 *
 * Assumptions :
 *
 * ---------------------------------------------------------------------------
 */
#include "administrative/Zone.hpp"
#include "simulation/Scheduler.hpp"
#include "exceptions/Exception.hpp"

#include <string.h>

namespace model {


INTEGER model::Zone::ZoneID = 0;

Zone::Zone(Generator ** generatorSet, const ZoneParams & zoneParameters) :
	zoneID(ZoneID++),
	generatorSet(generatorSet),
	nGenerators(zoneParameters.nGenerators)
{
}

Zone::~Zone()
{
	--ZoneID;
}

void
Zone::dailyTrigger()
{
	for (size_t i = 0; i < nGenerators; i++) {
		generatorSet[i]->dailyTrigger();
	}
};

void
Zone::setupTrigger()
{
	for (size_t i = 0; i < nGenerators; i++) {
		generatorSet[i]->setupTrigger();
	}
}

void
Zone::ucTrigger()
{
	for (size_t gen = 0; gen < nGenerators; gen++) {
		generatorSet[gen]->ucTrigger();
	}
};

void
Zone::lfTrigger()
{
	for (size_t i = 0; i < nGenerators; i++) {
		generatorSet[i]->lfTrigger();
	}
}

void
Zone::regTrigger()
{
	for (size_t i = 0; i < nGenerators; i++) {
		generatorSet[i]->regTrigger();
	}
}

#if defined RIM_DEBUG || defined SIMULATION_TEST

void
Zone::printState(ostream & out)
{
}

void
Zone::printParameters(ostream & out)
{
}
#endif
} /* END OF NAMESPACE model */
