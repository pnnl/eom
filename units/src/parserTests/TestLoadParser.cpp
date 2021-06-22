/*
 * TestLoadParser.cpp
 *
 *  Created on: Jan 19, 2012
 *      Author: kglass
 */

#include "parserTests/TestLoadParser.hpp"
#include "parserTests/LoadTest.hpp"
#include <string>

namespace testing {
CPPUNIT_TEST_SUITE_REGISTRATION (TestLoadParser);

void
TestLoadParser::setUp(void)
{
	loadParser             = new interpreter::LoadParser("units/data/rimParserTests/basic/load.csv");

	paramSpace      = new char[5*sizeof(LoadParams)];
	interfaceSpace  = new char[5*sizeof(LoadData)];
	SimulationInfo  * info  = new SimulationInfo;
	info->nLoads = 5;

	loadParser->setModels(paramSpace, interfaceSpace, info);
}

void
TestLoadParser::tearDown(void)
{
	delete(loadParser);
}

void
TestLoadParser::inputTest(void)
{
	CPPUNIT_ASSERT (loadParser != NULL);
	LoadParams  * actualValues  = (LoadParams *)paramSpace;
	LoadParams   expectedParams[] =
	{
		/*
			{{"Load 000"," fileName", ba, zone, currentLoad, outputLoad}}
		*/
			{"Load 000", "testing/data/validSrc/time_series/testLoad", 0, 0, -1.0, -1.0},
			{"Load 010", "testing/data/validSrc/time_series/testLoad", 0, 1, -1.0, -1.0},
			{"Load 020", "testing/data/validSrc/time_series/testLoad", 0, 2, -1.0, -1.0},
			{"Load 100", "testing/data/validSrc/time_series/testLoad", 1, 3, -1.0, -1.0},
			{"Load 110", "testing/data/validSrc/time_series/testLoad", 1, 4, -1.0, -1.0}
	};

	LoadTest test(expectedParams, actualValues, 5);
}

} /* namespace testing */
