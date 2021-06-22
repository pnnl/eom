/*
 * TestBAParser.cpp
 *
 *  Created on: Jan 19, 2012
 *      Author: kglass
 */

#include "parserTests/TestBAParser.hpp"
#include "parserTests/BATest.hpp"
#include <string>

namespace testing {
CPPUNIT_TEST_SUITE_REGISTRATION (TestBAParser);

void
TestBAParser::setUp(void)
{
	baParser             = new interpreter::BAParser("units/data/rimParserTests/basic/ba.csv");

	paramSpace      = new char[2*sizeof(BalanceAuthParams)];
	interfaceSpace  = new char[2*sizeof(BalanceAuthData)];
	SimulationInfo  * info  = new SimulationInfo;
	info->nBalancingAuths = 2;

	baParser->setModels(paramSpace, interfaceSpace, info);
}

void
TestBAParser::tearDown(void)
{
	delete(baParser);
}

void
TestBAParser::inputTest(void)
{
	CPPUNIT_ASSERT (baParser != NULL);

	BalanceAuthParams * baParams     = (BalanceAuthParams *)paramSpace;
	BalanceAuthParams   expectedParams[] = {
			{"BA 0", 100, 101, 102, 103, 104, 0, 0, 0, 0, 0.0, 0.0, 0.0, 0.0},
			{"BA 1", 1000, 1001, 1002, 1003, 1004, 0, 0, 0, 0, 0.0, 0.0, 0.0, 0.0}
			};

	BATest test(expectedParams, baParams, 2);
}

} /* namespace testing */
