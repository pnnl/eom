/*
 * TestZoneParser.cpp
 *
 *  Created on: Jan 19, 2012
 *      Author: kglass
 */

#include "parserTests/TestZoneParser.hpp"
#include <string>

namespace testing {
CPPUNIT_TEST_SUITE_REGISTRATION (TestZoneParser);

void
TestZoneParser::setUp(void)
{
	zoneParser             = new interpreter::ZoneParser("units/data/rimParserTests/basic/zone.csv");

	paramSpace      = new char[5*sizeof(ZoneParams)];
	interfaceSpace  = new char[5*sizeof(ZoneData)];
	SimulationInfo  * info  = new SimulationInfo;
	info->nZones = 5;

	zoneParser->setModels(paramSpace, interfaceSpace, info);
}

void
TestZoneParser::tearDown(void)
{
	delete(zoneParser);
}

void
TestZoneParser::inputTest(void)
{
	CPPUNIT_ASSERT (zoneParser != NULL);
	ZoneParams  * zoneParams  = (ZoneParams *)paramSpace;

	test0(&zoneParams[0]);
	test1(&zoneParams[1]);
	test2(&zoneParams[2]);
	test3(&zoneParams[3]);
	test4(&zoneParams[4]);
}

void
TestZoneParser::test0(ZoneParams * zoneParams)
{
	string            value = "Zone 00";
	string            name  = zoneParams->name;

	CPPUNIT_ASSERT_MESSAGE  (" input data: Zone 00 name", name == value);

	CPPUNIT_ASSERT_MESSAGE  (" input data: Zone 00 noxCost", zoneParams->noxCost == 100);
	CPPUNIT_ASSERT_MESSAGE  (" input data: Zone 00 soxCost", zoneParams->soxCost == 101);
	CPPUNIT_ASSERT_MESSAGE  (" input data: Zone 00 soxCost", zoneParams->co2Cost == 102);
	CPPUNIT_ASSERT_MESSAGE  (" input data: Zone 00 balancingAuthIndex", zoneParams->balancingAuthIndex == 0);
	CPPUNIT_ASSERT_MESSAGE  (" input data: Zone 00 zoneBAId", zoneParams->zoneBAId == 0);
	CPPUNIT_ASSERT_MESSAGE  (" input data: Zone 00 nGenerators", zoneParams->nGenerators == 0);
	CPPUNIT_ASSERT_MESSAGE  (" input data: Zone 00 nConventional", zoneParams->nConventional == 0);
	CPPUNIT_ASSERT_MESSAGE  (" input data: Zone 00 nStorage", zoneParams->nStorage == 0);
	CPPUNIT_ASSERT_MESSAGE  (" input data: Zone 00 nHydro", zoneParams->nHydro == 0);
	CPPUNIT_ASSERT_MESSAGE  (" input data: Zone 00 nLoads", zoneParams->nLoads == 0);
	CPPUNIT_ASSERT_MESSAGE  (" input data: Zone 00 nWindFarms", zoneParams->nWindFarms == 0);
	CPPUNIT_ASSERT_MESSAGE  (" input data: Zone 00 nSolarFarms", zoneParams->nSolarFarms == 0);

	value = "data/test/time_series/interchange.data";
	CPPUNIT_ASSERT_MESSAGE  (" input data: Zone 00 interchange file", zoneParams->interchangeFile == value);

}

void
TestZoneParser::test1(ZoneParams * zoneParams)
{
	string            value = "Zone 01";
	string            name  = zoneParams->name;

	CPPUNIT_ASSERT_MESSAGE  (" input data: Zone 01 name", name == value);

	CPPUNIT_ASSERT_MESSAGE  (" input data: Zone 01 noxCost", zoneParams->noxCost == 1000);
	CPPUNIT_ASSERT_MESSAGE  (" input data: Zone 01 soxCost", zoneParams->soxCost == 1001);
	CPPUNIT_ASSERT_MESSAGE  (" input data: Zone 01 soxCost", zoneParams->co2Cost == 1002);
	CPPUNIT_ASSERT_MESSAGE  (" input data: Zone 01 balancingAuthIndex", zoneParams->balancingAuthIndex == 0);
	CPPUNIT_ASSERT_MESSAGE  (" input data: Zone 01 zoneBAId", zoneParams->zoneBAId == 0);
	CPPUNIT_ASSERT_MESSAGE  (" input data: Zone 01 nGenerators", zoneParams->nGenerators == 0);
	CPPUNIT_ASSERT_MESSAGE  (" input data: Zone 01 nConventional", zoneParams->nConventional == 0);
	CPPUNIT_ASSERT_MESSAGE  (" input data: Zone 01 nStorage", zoneParams->nStorage == 0);
	CPPUNIT_ASSERT_MESSAGE  (" input data: Zone 01 nHydro", zoneParams->nHydro == 0);
	CPPUNIT_ASSERT_MESSAGE  (" input data: Zone 01 nLoads", zoneParams->nLoads == 0);
	CPPUNIT_ASSERT_MESSAGE  (" input data: Zone 01 nWindFarms", zoneParams->nWindFarms == 0);
	CPPUNIT_ASSERT_MESSAGE  (" input data: Zone 01 nSolarFarms", zoneParams->nSolarFarms == 0);

	value = "data/test/time_series/interchange.data";
	CPPUNIT_ASSERT_MESSAGE  (" input data: Zone 01 interchange file", zoneParams->interchangeFile == value);
}

void
TestZoneParser::test2(ZoneParams * zoneParams)
{
	string            value = "Zone 02";
	string            name  = zoneParams->name;

	CPPUNIT_ASSERT_MESSAGE  (" input data: Zone 02 name", name == value);

	CPPUNIT_ASSERT_MESSAGE  (" input data: Zone 02 noxCost", zoneParams->noxCost == 10000);
	CPPUNIT_ASSERT_MESSAGE  (" input data: Zone 02 soxCost", zoneParams->soxCost == 10001);
	CPPUNIT_ASSERT_MESSAGE  (" input data: Zone 02 soxCost", zoneParams->co2Cost == 10002);
	CPPUNIT_ASSERT_MESSAGE  (" input data: Zone 02 balancingAuthIndex", zoneParams->balancingAuthIndex == 0);
	CPPUNIT_ASSERT_MESSAGE  (" input data: Zone 02 zoneBAId", zoneParams->zoneBAId == 0);
	CPPUNIT_ASSERT_MESSAGE  (" input data: Zone 02 nGenerators", zoneParams->nGenerators == 0);
	CPPUNIT_ASSERT_MESSAGE  (" input data: Zone 02 nConventional", zoneParams->nConventional == 0);
	CPPUNIT_ASSERT_MESSAGE  (" input data: Zone 02 nStorage", zoneParams->nStorage == 0);
	CPPUNIT_ASSERT_MESSAGE  (" input data: Zone 02 nHydro", zoneParams->nHydro == 0);
	CPPUNIT_ASSERT_MESSAGE  (" input data: Zone 02 nLoads", zoneParams->nLoads == 0);
	CPPUNIT_ASSERT_MESSAGE  (" input data: Zone 02 nWindFarms", zoneParams->nWindFarms == 0);
	CPPUNIT_ASSERT_MESSAGE  (" input data: Zone 02 nSolarFarms", zoneParams->nSolarFarms == 0);

	value = "data/test/time_series/interchange.data";
	CPPUNIT_ASSERT_MESSAGE  (" input data: Zone 02 interchange file", zoneParams->interchangeFile == value);
}

void
TestZoneParser::test3(ZoneParams * zoneParams)
{
	string            value = "Zone 10";
	string            name  = zoneParams->name;

	CPPUNIT_ASSERT_MESSAGE  (" input data: Zone 10 name", name == value);

	CPPUNIT_ASSERT_MESSAGE  (" input data: Zone 10 noxCost", zoneParams->noxCost == 100000);
	CPPUNIT_ASSERT_MESSAGE  (" input data: Zone 10 soxCost", zoneParams->soxCost == 100001);
	CPPUNIT_ASSERT_MESSAGE  (" input data: Zone 10 soxCost", zoneParams->co2Cost == 100002);
	CPPUNIT_ASSERT_MESSAGE  (" input data: Zone 10 balancingAuthIndex", zoneParams->balancingAuthIndex == 1);
	CPPUNIT_ASSERT_MESSAGE  (" input data: Zone 10 zoneBAId", zoneParams->zoneBAId == 0);
	CPPUNIT_ASSERT_MESSAGE  (" input data: Zone 10 nGenerators", zoneParams->nGenerators == 0);
	CPPUNIT_ASSERT_MESSAGE  (" input data: Zone 10 nConventional", zoneParams->nConventional == 0);
	CPPUNIT_ASSERT_MESSAGE  (" input data: Zone 10 nStorage", zoneParams->nStorage == 0);
	CPPUNIT_ASSERT_MESSAGE  (" input data: Zone 10 nHydro", zoneParams->nHydro == 0);
	CPPUNIT_ASSERT_MESSAGE  (" input data: Zone 10 nLoads", zoneParams->nLoads == 0);
	CPPUNIT_ASSERT_MESSAGE  (" input data: Zone 10 nWindFarms", zoneParams->nWindFarms == 0);
	CPPUNIT_ASSERT_MESSAGE  (" input data: Zone 10 nSolarFarms", zoneParams->nSolarFarms == 0);

	value = "data/test/time_series/interchange.data";
	CPPUNIT_ASSERT_MESSAGE  (" input data: Zone 10 interchange file", zoneParams->interchangeFile == value);
}

void
TestZoneParser::test4(ZoneParams * zoneParams)
{
	string            value = "Zone 11";
	string            name  = zoneParams->name;

	CPPUNIT_ASSERT_MESSAGE  (" input data: Zone 11 name", name == value);

	CPPUNIT_ASSERT_MESSAGE  (" input data: Zone 11 noxCost", zoneParams->noxCost == 1000000);
	CPPUNIT_ASSERT_MESSAGE  (" input data: Zone 11 soxCost", zoneParams->soxCost == 1000001);
	CPPUNIT_ASSERT_MESSAGE  (" input data: Zone 11 soxCost", zoneParams->co2Cost == 1000002);
	CPPUNIT_ASSERT_MESSAGE  (" input data: Zone 11 balancingAuthIndex", zoneParams->balancingAuthIndex == 1);
	CPPUNIT_ASSERT_MESSAGE  (" input data: Zone 11 zoneBAId", zoneParams->zoneBAId == 0);
	CPPUNIT_ASSERT_MESSAGE  (" input data: Zone 11 nGenerators", zoneParams->nGenerators == 0);
	CPPUNIT_ASSERT_MESSAGE  (" input data: Zone 11 nConventional", zoneParams->nConventional == 0);
	CPPUNIT_ASSERT_MESSAGE  (" input data: Zone 11 nStorage", zoneParams->nStorage == 0);
	CPPUNIT_ASSERT_MESSAGE  (" input data: Zone 11 nHydro", zoneParams->nHydro == 0);
	CPPUNIT_ASSERT_MESSAGE  (" input data: Zone 11 nLoads", zoneParams->nLoads == 0);
	CPPUNIT_ASSERT_MESSAGE  (" input data: Zone 11 nWindFarms", zoneParams->nWindFarms == 0);
	CPPUNIT_ASSERT_MESSAGE  (" input data: Zone 11 nSolarFarms", zoneParams->nSolarFarms == 0);

	value = "data/test/time_series/interchange.data";
	CPPUNIT_ASSERT_MESSAGE  (" input data: Zone 11 interchange file", zoneParams->interchangeFile == value);
}

void
TestZoneParser::derivedTest(void)
{
	CPPUNIT_ASSERT (zoneParser != NULL);
}

} /* namespace testing */
