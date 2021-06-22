/*
 * TestWindParser.cpp
 *
 *  Created on: Jan 19, 2012
 *      Author: kglass
 */

#include "parserTests/TestWindParser.hpp"
#include "parserTests/WindTest.hpp"
#include <string>

namespace testing {
CPPUNIT_TEST_SUITE_REGISTRATION (TestWindParser);

void
TestWindParser::setUp(void)
{
	windParser             = new interpreter::WindParser("units/data/rimParserTests/basic/wind.csv");

	paramSpace      = new char[20*sizeof(WindParams)];
	interfaceSpace  = new char[20*sizeof(WindData)];
	SimulationInfo  * info  = new SimulationInfo;
	info->nWindFarms = 20;

	windParser->setModels(paramSpace, interfaceSpace, info);
}

void
TestWindParser::tearDown(void)
{
	delete(windParser);
}

void
TestWindParser::inputTest(void)
{
	CPPUNIT_ASSERT (windParser != NULL);
	WindParams  * actualValues  = (WindParams *)paramSpace;
	WindParams   expectedParams[] =
	{
		/*
			{"Wind 000"," fileName", ba, zone, currentWind, outputWind}
		*/
			{"Wind 000", "testing/data/validSrc/time_series/testWind", 0, 0, -1.0, -1.0},
			{"Wind 001", "testing/data/validSrc/time_series/testWind", 0, 0, -1.0, -1.0},
			{"Wind 010", "testing/data/validSrc/time_series/testWind", 0, 1, -1.0, -1.0},
			{"Wind 011", "testing/data/validSrc/time_series/testWind", 0, 1, -1.0, -1.0},
			{"Wind 012", "testing/data/validSrc/time_series/testWind", 0, 1, -1.0, -1.0},
			{"Wind 020", "testing/data/validSrc/time_series/testWind", 0, 2, -1.0, -1.0},
			{"Wind 021", "testing/data/validSrc/time_series/testWind", 0, 2, -1.0, -1.0},
			{"Wind 022", "testing/data/validSrc/time_series/testWind", 0, 2, -1.0, -1.0},
			{"Wind 023", "testing/data/validSrc/time_series/testWind", 0, 2, -1.0, -1.0},
			{"Wind 100", "testing/data/validSrc/time_series/testWind", 1, 3, -1.0, -1.0},
			{"Wind 101", "testing/data/validSrc/time_series/testWind", 1, 3, -1.0, -1.0},
			{"Wind 102", "testing/data/validSrc/time_series/testWind", 1, 3, -1.0, -1.0},
			{"Wind 103", "testing/data/validSrc/time_series/testWind", 1, 3, -1.0, -1.0},
			{"Wind 104", "testing/data/validSrc/time_series/testWind", 1, 3, -1.0, -1.0},
			{"Wind 110", "testing/data/validSrc/time_series/testWind", 1, 4, -1.0, -1.0},
			{"Wind 111", "testing/data/validSrc/time_series/testWind", 1, 4, -1.0, -1.0},
			{"Wind 112", "testing/data/validSrc/time_series/testWind", 1, 4, -1.0, -1.0},
			{"Wind 113", "testing/data/validSrc/time_series/testWind", 1, 4, -1.0, -1.0},
			{"Wind 114", "testing/data/validSrc/time_series/testWind", 1, 4, -1.0, -1.0},
			{"Wind 115", "testing/data/validSrc/time_series/testWind", 1, 4, -1.0, -1.0}
	};

	WindTest test(expectedParams, actualValues, 20);
}

} /* namespace testing */
