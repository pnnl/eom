/*
 * ConfigTestRIMParser.cpp
 *
 *  Created on: Jan 19, 2012
 *      Author: kglass
 */

#include "parserTests/ConfigTestRIMParser.hpp"
#include "ModelParser.hpp"
#include "parserTests/BATest.hpp"
#include "parserTests/SimTest.hpp"
#include "parserTests/ZoneTest.hpp"
#include <string>

namespace testing {
CPPUNIT_TEST_SUITE_REGISTRATION (ConfigTestRIMParser);

void
ConfigTestRIMParser::setUp(void)
{

	rimParser                  = new interpreter::RIMParser("units/data/rimParserTests/basic/sources.txt");
	parameters                 = rimParser->parameterSpace;
	offset                     = rimParser->parameterOffset;
	info                       = (SimulationInfo *)parameters;
}

void
ConfigTestRIMParser::tearDown(void)
{
	delete(rimParser);
}
/*
 * Verify the data spaces are allocated and initialized
 */
void
ConfigTestRIMParser::baZoneSpaceTests(void)
{
	int         expectedZonePopulation[] = {3,2};
	int         expectedZoneOffset[] = {0,3,5};
	int         expectedZoneReordering[] = {0,1,2,3,4};

	BalanceAuthParams      * baParams        = (BalanceAuthParams *)&parameters[offset[BALANCE_AUTHS]];

	for (int i = 0; i < 2; i++) {
		CPPUNIT_ASSERT_MESSAGE  (" source data:  zone population correct", baParams[i].nZones == expectedZonePopulation[i]);
	}
}

void
ConfigTestRIMParser::baGeneratorSpaceTests(void)
{
	int         expectedBAPopulation[] = {9,11};
	int         expectedBAOffset[] = {0,9,20};
	int         expectedBAReordering[] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19};
	bool        baPopulationCorrect   = true;
	bool        baOffsetCorrect       = true;
	bool        baReorderingCorrect   = true;
	BalanceAuthParams      * baParams        = (BalanceAuthParams *)&parameters[offset[BALANCE_AUTHS]];

	for (int i = 0; i < 2; i++) {
		baPopulationCorrect &= baParams[i].nGenerators == expectedBAPopulation[i];
	}
	CPPUNIT_ASSERT_MESSAGE  (" source data:  zone population correct", baPopulationCorrect);
}

void
ConfigTestRIMParser::zoneGeneratorSpaceTests(void)
{
	int         expectedZonePopulation[] = {2,3,4,5,6};
	int         expectedZoneOffset[] = {0,2,5,9,14,20};
	int         expectedZoneReordering[] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19};
	bool        zonePopulationCorrect   = true;
	bool        zoneOffsetCorrect       = true;
	bool        zoneReorderingCorrect   = true;
	ZoneParams        * zoneParams     = (ZoneParams *)&parameters[offset[ZONE]];


	for (int i = 0; i < 2; i++) {
		zonePopulationCorrect &= zoneParams[i].nGenerators == expectedZonePopulation[i];
	}
	CPPUNIT_ASSERT_MESSAGE  (" source data:  zone population correct", zonePopulationCorrect);
}


void
ConfigTestRIMParser::zoneWindSpaceTests(void)
{
	int         expectedZonePopulation[] = {2,3,4,5,6};
	int         expectedZoneOffset[] = {0,2,5,9,14,20};
	int         expectedZoneReordering[] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19};
	bool        zonePopulationCorrect   = true;
	bool        zoneOffsetCorrect       = true;
	bool        zoneReorderingCorrect   = true;
	ZoneParams        * zoneParams     = (ZoneParams *)&parameters[offset[ZONE]];


	for (int i = 0; i < 2; i++) {
		zonePopulationCorrect &= zoneParams[i].nWindFarms == expectedZonePopulation[i];
	}
	CPPUNIT_ASSERT_MESSAGE  (" source data:  zone population correct", zonePopulationCorrect);
}

void
ConfigTestRIMParser::zoneSolarSpaceTests(void)
{
	int         expectedZonePopulation[] = {2,3,4,5,6};
	int         expectedZoneOffset[] = {0,2,5,9,14,20};
	int         expectedZoneReordering[] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19};
	bool        zonePopulationCorrect   = true;
	bool        zoneOffsetCorrect       = true;
	bool        zoneReorderingCorrect   = true;
	ZoneParams        * zoneParams     = (ZoneParams *)&parameters[offset[ZONE]];


	for (int i = 0; i < 2; i++) {
		zonePopulationCorrect &= zoneParams[i].nSolarFarms == expectedZonePopulation[i];
	}
	CPPUNIT_ASSERT_MESSAGE  (" source data:  zone population correct", zonePopulationCorrect);
}

} /* namespace testing */
