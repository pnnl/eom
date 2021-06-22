/*
 * TestSimParser.cpp
 *
 *  Created on: Jan 19, 2012
 *      Author: kglass
 */

#include "parserTests/TestSimParser.hpp"
#include "parserTests/SimTest.hpp"
#include <string>

namespace testing {
CPPUNIT_TEST_SUITE_REGISTRATION (TestSimParser);

void
TestSimParser::setUp(void)
{
	simParser            = new interpreter::SimulationParser("units/data/rimParserTests/basic/simulation.csv");

	paramSpace      = new char[sizeof(SimulationInfo)];
	interfaceSpace  = new char[sizeof(SimulationData)];
	SimulationInfo  * info  = (SimulationInfo *)paramSpace;
	info->nSimulations = 1;

	info->nZones = 0;
	info->nConventionalGenerators = 0;
	info->nStorageGenerators = 0;
	info->nHydroGenerators = 0;
	info->nLines = 0;
	info->nDR = 0;
	info->nReplicates = 0;

	simParser->setModels(paramSpace, interfaceSpace, info);
}

void
TestSimParser::tearDown(void)
{
	delete(simParser);
}

void
TestSimParser::inputTest(void)
{
	CPPUNIT_ASSERT (simParser != NULL);

	SimulationInfo * simInfo     = (SimulationInfo *)paramSpace;

	/*
	 * NOTES:
	 * start and stop minutesPastZero must be computed, which is pointless
	 * because duration gives a reasonable test of the correctness of
	 * minutesPastZero: set minutesPastZero = 0 in both actual start and
	 * actual stop
	 *
	 * dump size relies on the size of a single time step, which must be
	 * computed: do not compare the dump size
	 *
	 * circular buffer size relies on the size of a single time step, which
	 * must be computed: do not compare the dump size
	 */
	SimulationInfo   expectedParams = {
			"Test 1",                      // scenario name
			{{2001,12,31},{0,0,0},0},      // start time,
			{{2002,1,1},{23,59,59},0},     // stop time
			1,                             // base time step
			8,                             // n dump steps
			0,                             // uc trigger
			4,                             // uc time step
			4,                             // uc step length
			2,                             // lf time step
			4,                             // lf step length
			1,                             // reg time step
			0,                             // hydro time step
			4,                             // forecastDays
			NO_REPEAT,                     // repeatType
			0,                             // nRepeats
			1,                             // nInterfaceSteps
			2,                             // ucTimeStepsPerDay
			4,                             // lfTimeStepsPerDay
			8,                             // regTimeStepsPerDay
			1,                             // ucTimeStepsPerHour
			2,                             // lfTimeStepsPerHour
			4,                             // regTimeStepsPerHour
			2,                             // lfStepsPerUCStep
			4,                             // regStepsPerUCStep
			2,                             // regStepsPerLFStep
			2879,                          // duration
			0,                             // dumpSize
			0,                             // circularBuffer
			1,                             // nSimulations
			0,                             // nBalancingAuths
			0,                             // nSchedulers
			0,                             // nZones
			0,                            // nConventionalGenerators
			0,                             // nStorageGenerators
			0,                             // nHydroGenerators
			0,                             // nLines
			0,                             // nDR
			0,                             // nReplicates
			0,                            // nLoads
			0,                            // nWindFarms
			0,                            // nSolarFarms
			0,                             // nInterchanges
			0,                            // nGenerators
			0                             // nModels
			};

	simInfo->start.minutesPastZero = 0;
	simInfo->stop.minutesPastZero = 0;
	simInfo->circularBuffer= 0;
	SimTest test(&expectedParams, simInfo, 1);

}

} /* namespace testing */
