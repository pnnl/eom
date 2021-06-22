/*
 * TestSolarParser.cpp
 *
 *  Created on: Jan 19, 2012
 *      Author: kglass
 */

#include "parserTests/TestSolarParser.hpp"
#include "parserTests/SolarTest.hpp"
#include <string>

namespace testing {
CPPUNIT_TEST_SUITE_REGISTRATION (TestSolarParser);

void
TestSolarParser::setUp(void)
{
	solarParser             = new interpreter::SolarParser("units/data/rimParserTests/basic/solar.csv");

	paramSpace      = new char[20*sizeof(SolarParams)];
	interfaceSpace  = new char[20*sizeof(SolarData)];
	SimulationInfo  * info  = new SimulationInfo;
	info->nSolarFarms = 20;

	solarParser->setModels(paramSpace, interfaceSpace, info);
}

void
TestSolarParser::tearDown(void)
{
	delete(solarParser);
}

void
TestSolarParser::inputTest(void)
{
	CPPUNIT_ASSERT (solarParser != NULL);
	SolarParams  * actualValues  = (SolarParams *)paramSpace;
	SolarParams   expectedParams[] =
	{
		/*
			{"Solar 000"," fileName", ba, zone, currentSolar, outputSolar}
		*/
			{"Solar 000", "testing/data/validSrc/time_series/testSolar", 0, 0, -1.0, -1.0},
			{"Solar 001", "testing/data/validSrc/time_series/testSolar", 0, 0, -1.0, -1.0},
			{"Solar 010", "testing/data/validSrc/time_series/testSolar", 0, 1, -1.0, -1.0},
			{"Solar 011", "testing/data/validSrc/time_series/testSolar", 0, 1, -1.0, -1.0},
			{"Solar 012", "testing/data/validSrc/time_series/testSolar", 0, 1, -1.0, -1.0},
			{"Solar 020", "testing/data/validSrc/time_series/testSolar", 0, 2, -1.0, -1.0},
			{"Solar 021", "testing/data/validSrc/time_series/testSolar", 0, 2, -1.0, -1.0},
			{"Solar 022", "testing/data/validSrc/time_series/testSolar", 0, 2, -1.0, -1.0},
			{"Solar 023", "testing/data/validSrc/time_series/testSolar", 0, 2, -1.0, -1.0},
			{"Solar 100", "testing/data/validSrc/time_series/testSolar", 1, 3, -1.0, -1.0},
			{"Solar 101", "testing/data/validSrc/time_series/testSolar", 1, 3, -1.0, -1.0},
			{"Solar 102", "testing/data/validSrc/time_series/testSolar", 1, 3, -1.0, -1.0},
			{"Solar 103", "testing/data/validSrc/time_series/testSolar", 1, 3, -1.0, -1.0},
			{"Solar 104", "testing/data/validSrc/time_series/testSolar", 1, 3, -1.0, -1.0},
			{"Solar 110", "testing/data/validSrc/time_series/testSolar", 1, 4, -1.0, -1.0},
			{"Solar 111", "testing/data/validSrc/time_series/testSolar", 1, 4, -1.0, -1.0},
			{"Solar 112", "testing/data/validSrc/time_series/testSolar", 1, 4, -1.0, -1.0},
			{"Solar 113", "testing/data/validSrc/time_series/testSolar", 1, 4, -1.0, -1.0},
			{"Solar 114", "testing/data/validSrc/time_series/testSolar", 1, 4, -1.0, -1.0},
			{"Solar 115", "testing/data/validSrc/time_series/testSolar", 1, 4, -1.0, -1.0}
	};

	SolarTest test(expectedParams, actualValues, 20);
}

} /* namespace testing */
