/*
 * SimulationParserTest.cpp
 *
 *  Created on: Sep 15, 2011
 *      Author: kglass
 */

#include "SimulationParserTest.hpp"

namespace testing {

SimulationParserTest::SimulationParserTest(){}

SimulationParserTest::~SimulationParserTest() {}


void
SimulationParserTest::constructSimulationParserTest(const char * fileName, INTEGER nSimulations)
{
	incrementTest();
	interpreter::SimulationParser   * parser         =
			new interpreter::SimulationParser(fileName);

	char                * parameters        = new char[sizeof(SimulationInfo)*nSimulations];
	char                * interface         = new char[sizeof(SimulationData)*nSimulations];
	SimulationInfo      * info              = (SimulationInfo *)parameters;
	SimulationData      * data              = (SimulationData *)interface;

	info->nSimulations  = nSimulations;
	// load all the data in the selected file
	*validationLog << "\tSimulation Parser Constructor" << endl;
	parser->setModels(parameters, interface, info);
	INTEGER             base                = 100;
	INTEGER             offset              = 0;
	INTEGER             failed              = false;

	for (INTEGER i = 0; i < nSimulations; i++) {
		failed = !checkSim(i, info, data);

		cout << "\tSimulation " << i << endl;
		INTEGER              inc            = 0;
		offset          = base;

		if (info[i].nDumpSteps != (offset + inc)) {
			*validationLog << "\tFAILED: incorrect nDumpSteps " << endl;
			failed      |= true;
		}
		++inc;

		if (info[i].baseTimeStep != (offset + inc)) {
			*validationLog << "\tFAILED: incorrect baseTimeStep " << endl;
			failed      |= true;
		}
		++inc;

		if (info[i].ucTrigger != (offset + inc)) {
			*validationLog << "\tFAILED: incorrect ucTrigger " << endl;
			failed      |= true;
		}
		++inc;

		if (info[i].ucTimeStep != (offset + inc)) {
			*validationLog << "\tFAILED: incorrect ucTimeStep " << endl;
			failed      |= true;
		}
		++inc;

		if (info[i].ucLength != (offset + inc)) {
			*validationLog << "\tFAILED: incorrect ucLength " << endl;
			failed      |= true;
		}
		++inc;

		if (info[i].lfLength != (offset + inc)) {
			*validationLog << "\tFAILED: incorrect lfLength " << endl;
			failed      |= true;
		}
		++inc;

		if (info[i].lfTimeStep != (offset + inc)) {
			*validationLog << "\tFAILED: incorrect lfTimeStep " << endl;
			failed      |= true;
		}
		++inc;

		if (info[i].regTimeStep != (offset + inc)) {
			*validationLog << "\tFAILED: incorrect regTimeStep " << endl;
			failed      |= true;
		}
		++inc;

		if (info[i].hydroTimeStep != (offset + inc)) {
			*validationLog << "\tFAILED: incorrect hydroTimeStep " << endl;
			failed      |= true;
		}
		++inc;

		if (info[i].forecastDays != (offset + inc)) {
			*validationLog << "\tFAILED: incorrect forecastDays " << endl;
			failed      |= true;
		}
		++inc;

		if (info[i].nRepeats != (offset + inc)) {
			*validationLog << "\tFAILED: incorrect nRepeats " << endl;
			failed      |= true;
		}
		++inc;

		base *= 10;
	}

	if(!failed) {
		*validationLog << "\tPASS (constructSimulationParserTest.positive) : test passed" << endl;
	} else {
		incrementFailure();
		*validationLog << "\tFAIL (constructSimulationParserTest.positive) : test failed" << endl;
	}
	// verify the correct load
}

void
SimulationParserTest::deconstructSimulationParserTest(const char * fileName)
{
}

bool
SimulationParserTest::checkSim(INTEGER sim, SimulationInfo * info,
		SimulationData * data)
{
	bool                     pass           = true;

	switch(sim) {
	case 0:
		*validationLog << "\tSim 0 " << endl;
		if (info[sim].start.date.day != 1 ||
				info[sim].start.date.month != 1 ||
				info[sim].start.date.year!= 2010 ||
				info[sim].start.tod.hours != 0 ||
				info[sim].start.tod.minutes != 0 ||
				info[sim].start.tod.seconds != 0) {
			*validationLog << "\tFAILED: incorrect start time " << endl;
			pass = false;
		}
		if (info[sim].stop.date.day != 14 ||
				info[sim].stop.date.month != 1 ||
				info[sim].stop.date.year!= 2010 ||
				info[sim].stop.tod.hours != 23 ||
				info[sim].stop.tod.minutes != 59 ||
				info[sim].stop.tod.seconds != 59) {
			*validationLog << "\tFAILED: incorrect stop time " << endl;
			pass = false;
		}
		if (info[sim].repeatType != NO_REPEAT) {
			*validationLog << "\tFAILED: incorrect repeat time " << endl;
			pass = false;
		}
		if (info[sim].duration != 20159) {
			*validationLog << "\tFAILED: incorrect duration should be 20159 was " << info[sim].duration << endl;
			pass = false;
		}
		break;
	case 1:
		*validationLog << "\tSim 1 " << endl;
		if (info[sim].start.date.day != 1 ||
				info[sim].start.date.month != 12 ||
				info[sim].start.date.year!= 2010 ||
				info[sim].start.tod.hours != 0 ||
				info[sim].start.tod.minutes != 0 ||
				info[sim].start.tod.seconds != 0) {
			*validationLog << "\tFAILED: incorrect start time " << endl;
			pass = false;
		}
		if (info[sim].stop.date.day != 1 ||
				info[sim].stop.date.month != 1 ||
				info[sim].stop.date.year!= 2011 ||
				info[sim].stop.tod.hours != 23 ||
				info[sim].stop.tod.minutes != 59 ||
				info[sim].stop.tod.seconds != 59) {
			*validationLog << "\tFAILED: incorrect stop time " << endl;
			pass = false;
		}
		if (info[sim].repeatType != MONTE_CARLO) {
			*validationLog << "\tFAILED: incorrect repeat time " << endl;
			pass = false;
		}
		if (info[sim].duration != 46079) {
			*validationLog << "\tFAILED: incorrect duration should be 2879 was " << info[sim].duration << endl;
			pass = false;
		}
		break;
	case 2:
		*validationLog << "\tSim 2 " << endl;
		if (info[sim].start.date.day != 1 ||
				info[sim].start.date.month != 1 ||
				info[sim].start.date.year!= 2010 ||
				info[sim].start.tod.hours != 0 ||
				info[sim].start.tod.minutes != 0 ||
				info[sim].start.tod.seconds != 0) {
			*validationLog << "\tFAILED: incorrect start time " << endl;
			pass = false;
		}
		if (info[sim].stop.date.day != 1 ||
				info[sim].stop.date.month != 1 ||
				info[sim].stop.date.year!= 2010 ||
				info[sim].stop.tod.hours != 23 ||
				info[sim].stop.tod.minutes != 59 ||
				info[sim].stop.tod.seconds != 59) {
			*validationLog << "\tFAILED: incorrect stop time " << endl;
			pass = false;
		}
		if (info[sim].repeatType != SENSITIVITY_ANALYSIS) {
			*validationLog << "\tFAILED: incorrect repeat time " << endl;
			pass = false;
		}
		if (info[sim].duration != 1439) {
			*validationLog << "\tFAILED: incorrect duration should be 1439 was " << info[sim].duration << endl;
			pass = false;
		}
		break;
	case 3:
		*validationLog << "\tSim 3 " << endl;
		if (info[sim].start.date.day != 1 ||
				info[sim].start.date.month != 1 ||
				info[sim].start.date.year!= 2010 ||
				info[sim].start.tod.hours != 0 ||
				info[sim].start.tod.minutes != 0 ||
				info[sim].start.tod.seconds != 0) {
			*validationLog << "\tFAILED: incorrect start time " << endl;
			pass = false;
		}
		if (info[sim].stop.date.day != 1 ||
				info[sim].stop.date.month != 1 ||
				info[sim].stop.date.year!= 2010 ||
				info[sim].stop.tod.hours != 23 ||
				info[sim].stop.tod.minutes != 59 ||
				info[sim].stop.tod.seconds != 59) {
			*validationLog << "\tFAILED: incorrect stop time " << endl;
			pass = false;
		}
		if (info[sim].repeatType != NO_REPEAT) {
			*validationLog << "\tFAILED: incorrect repeat time " << endl;
			pass = false;
		}
		if (info[sim].duration != 1439) {
			*validationLog << "\tFAILED: incorrect duration should be 1439 was " << info[sim].duration << endl;
			pass = false;
		}
		break;
	default:
		break;
	}

	return pass;
}

void
SimulationParserTest::setModelsTest(char * parameters, char * interface, SimulationInfo * info)
{
}

void
SimulationParserTest::loadSimulationTwst(INTEGER i, char * parameters, char * interface)
{
}

void
SimulationParserTest::setDerivedDataTest(char * parameters, char * interface, SimulationInfo * info)
{
}

void
SimulationParserTest::getCountTest()
{
}


} /* namespace testing */
