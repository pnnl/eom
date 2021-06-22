/*
 * TestModel2.cpp
 *
 *  Created on: Jan 19, 2012
 *      Author: kglass
 */

#include "modelTests/TestModel2.hpp"
#include <string>

namespace testing {
CPPUNIT_TEST_SUITE_REGISTRATION (TestModel2);

void
TestModel2::setUp(void)
{
	const char             * fileName = "units/data/rimParserTests/rimBasic/sources.txt";
	parser = interpreter::RIMParser::getRIMParser(fileName);
	parser->configure();
	scheduler           = simulator::Scheduler::makeScheduler();
	scheduler->setAdjLoads();
}

void
TestModel2::tearDown(void)
{
	delete(scheduler);
	delete(parser);
}

void
TestModel2::schedulerSetupTests(void)
{
	/*
	const char             * fileName = "units/data/rimParserTests/rimBasic/sources.txt";
	parser = interpreter::RIMParser::getRIMParser(fileName);
	parser->configure();
	scheduler           = simulator::Scheduler::makeScheduler();
	scheduler->setAdjLoads();
	*/
	CPPUNIT_ASSERT (scheduler != NULL);
	CPPUNIT_ASSERT (scheduler != NULL);
	CPPUNIT_ASSERT_MESSAGE  ("INCORRECT N BA", scheduler->getNBAs() == 2);
	CPPUNIT_ASSERT_MESSAGE  ("INCORRECT N Zones", scheduler->getNZones() == 5);
	CPPUNIT_ASSERT_MESSAGE  ("INCORRECT N Generators", scheduler->getNGenerators() == 20);
	CPPUNIT_ASSERT_MESSAGE  ("INCORRECT N Conventional Generators", scheduler->getNConventionalGenerators() == 20);
	CPPUNIT_ASSERT_MESSAGE  ("INCORRECT N Storage Generators", scheduler->getNStorageGenerators() == 0);
	CPPUNIT_ASSERT_MESSAGE  ("INCORRECT N Hydro Generators", scheduler->getNHydroGenerators() == 0);

	CPPUNIT_ASSERT_MESSAGE  ("SD Interface not allocated", scheduler->getSDI() != NULL);
	CPPUNIT_ASSERT_MESSAGE  ("schedule data", scheduler->getScheduleData() != NULL);
	/*
	delete(scheduler);
	delete(parser);
	*/
}

void
TestModel2::generatorSetupTests(void)
{
	scheduler->setupModels();
	int              offset = scheduler->getNConventionalGenerators();
	for (int i = 0; i < scheduler->getNConventionalGenerators(); i++) {
		CPPUNIT_ASSERT (i == scheduler->getConvGenSetData(i)->getConvGeneratorID());
		CPPUNIT_ASSERT (i == scheduler->getConvGenSetData(i)->getGeneratorID());
	}

	offset += scheduler->getNStorageGenerators();
	for (int i = 0; i < scheduler->getNStorageGenerators(); i++) {
		CPPUNIT_ASSERT (i == scheduler->getStorageGenSetData(i)->getStorageGeneratorID());
		CPPUNIT_ASSERT (offset + i == scheduler->getStorageGenSetData(i)->getGeneratorID());
	}

	offset += scheduler->getNHydroGenerators();
	for (int i = 0; i < scheduler->getNHydroGenerators(); i++) {
		CPPUNIT_ASSERT (i == scheduler->getHydroGenSetData(i)->getHydroGeneratorID());
		CPPUNIT_ASSERT (offset + i == scheduler->getHydroGenSetData(i)->getGeneratorID());
	}

	for (int i = 0; i < scheduler->getNZones(); i++) {
		CPPUNIT_ASSERT (i == scheduler->getZone(i)->getZoneID());
	}

	for (int i = 0; i < scheduler->getNBAs(); i++) {
		CPPUNIT_ASSERT (i == scheduler->getBA(i)->getID());
	}

	CPPUNIT_ASSERT ( 0 == scheduler->getBAGenertor(0,SIMULATION)->start);
	CPPUNIT_ASSERT (-1 == scheduler->getBAGenertor(0,SIMULATION)->length);
	CPPUNIT_ASSERT ( 0 == scheduler->getBAGenertor(0,BALANCE_AUTHS)->start);
	CPPUNIT_ASSERT (-1 == scheduler->getBAGenertor(0,BALANCE_AUTHS)->length);
	CPPUNIT_ASSERT ( 0 == scheduler->getBAGenertor(0,ZONE)->start);
	CPPUNIT_ASSERT (-1 == scheduler->getBAGenertor(0,ZONE)->length);
	CPPUNIT_ASSERT ( 0 == scheduler->getBAGenertor(0,CONVENTIONAL)->start);
	CPPUNIT_ASSERT ( 9 == scheduler->getBAGenertor(0,CONVENTIONAL)->length);
	CPPUNIT_ASSERT ( 9 == scheduler->getBAGenertor(0,STORAGE)->start);
	CPPUNIT_ASSERT ( 0 == scheduler->getBAGenertor(0,STORAGE)->length);
	CPPUNIT_ASSERT ( 9 == scheduler->getBAGenertor(0,HYDRO)->start);
	CPPUNIT_ASSERT ( 0 == scheduler->getBAGenertor(0,HYDRO)->length);
	CPPUNIT_ASSERT ( 0 == scheduler->getBAGenertor(0,LINE)->start);
	CPPUNIT_ASSERT (-1 == scheduler->getBAGenertor(0,LINE)->length);
	CPPUNIT_ASSERT ( 0 == scheduler->getBAGenertor(0,DEMAND_RESPONSE)->start);
	CPPUNIT_ASSERT (-1 == scheduler->getBAGenertor(0,DEMAND_RESPONSE)->length);
	CPPUNIT_ASSERT ( 0 == scheduler->getBAGenertor(0,REPLICATE)->start);
	CPPUNIT_ASSERT (-1 == scheduler->getBAGenertor(0,REPLICATE)->length);
	CPPUNIT_ASSERT ( 0 == scheduler->getBAGenertor(0,LOAD_TIMESERIES)->start);
	CPPUNIT_ASSERT (-1 == scheduler->getBAGenertor(0,LOAD_TIMESERIES)->length);
	CPPUNIT_ASSERT ( 0 == scheduler->getBAGenertor(0,WIND_TIMESERIES)->start);
	CPPUNIT_ASSERT (-1 == scheduler->getBAGenertor(0,WIND_TIMESERIES)->length);
	CPPUNIT_ASSERT ( 0 == scheduler->getBAGenertor(0,SOLAR_TIMESERIES)->start);
	CPPUNIT_ASSERT (-1 == scheduler->getBAGenertor(0,SOLAR_TIMESERIES)->length);
	CPPUNIT_ASSERT ( 0 == scheduler->getBAGenertor(0,INTERCHANGE_TIMESERIES)->start);
	CPPUNIT_ASSERT (-1 == scheduler->getBAGenertor(0,INTERCHANGE_TIMESERIES)->length);

	CPPUNIT_ASSERT ( 0 == scheduler->getBAGenertor(1,SIMULATION)->start);
	CPPUNIT_ASSERT (-1 == scheduler->getBAGenertor(1,SIMULATION)->length);
	CPPUNIT_ASSERT ( 0 == scheduler->getBAGenertor(1,BALANCE_AUTHS)->start);
	CPPUNIT_ASSERT (-1 == scheduler->getBAGenertor(1,BALANCE_AUTHS)->length);
	CPPUNIT_ASSERT ( 0 == scheduler->getBAGenertor(1,ZONE)->start);
	CPPUNIT_ASSERT (-1 == scheduler->getBAGenertor(1,ZONE)->length);
	CPPUNIT_ASSERT ( 9 == scheduler->getBAGenertor(1,CONVENTIONAL)->start);
	CPPUNIT_ASSERT (11 == scheduler->getBAGenertor(1,CONVENTIONAL)->length);
	CPPUNIT_ASSERT (20 == scheduler->getBAGenertor(1,STORAGE)->start);
	CPPUNIT_ASSERT ( 0 == scheduler->getBAGenertor(1,STORAGE)->length);
	CPPUNIT_ASSERT (20 == scheduler->getBAGenertor(1,HYDRO)->start);
	CPPUNIT_ASSERT ( 0 == scheduler->getBAGenertor(1,HYDRO)->length);
	CPPUNIT_ASSERT ( 0 == scheduler->getBAGenertor(1,LINE)->start);
	CPPUNIT_ASSERT (-1 == scheduler->getBAGenertor(1,LINE)->length);
	CPPUNIT_ASSERT ( 0 == scheduler->getBAGenertor(1,DEMAND_RESPONSE)->start);
	CPPUNIT_ASSERT (-1 == scheduler->getBAGenertor(1,DEMAND_RESPONSE)->length);
	CPPUNIT_ASSERT ( 0 == scheduler->getBAGenertor(1,REPLICATE)->start);
	CPPUNIT_ASSERT (-1 == scheduler->getBAGenertor(1,REPLICATE)->length);
	CPPUNIT_ASSERT ( 0 == scheduler->getBAGenertor(1,LOAD_TIMESERIES)->start);
	CPPUNIT_ASSERT (-1 == scheduler->getBAGenertor(1,LOAD_TIMESERIES)->length);
	CPPUNIT_ASSERT ( 0 == scheduler->getBAGenertor(1,WIND_TIMESERIES)->start);
	CPPUNIT_ASSERT (-1 == scheduler->getBAGenertor(1,WIND_TIMESERIES)->length);
	CPPUNIT_ASSERT ( 0 == scheduler->getBAGenertor(1,SOLAR_TIMESERIES)->start);
	CPPUNIT_ASSERT (-1 == scheduler->getBAGenertor(1,SOLAR_TIMESERIES)->length);
	CPPUNIT_ASSERT ( 0 == scheduler->getBAGenertor(1,INTERCHANGE_TIMESERIES)->start);
	CPPUNIT_ASSERT (-1 == scheduler->getBAGenertor(1,INTERCHANGE_TIMESERIES)->length);

	CPPUNIT_ASSERT ( 0 == scheduler->getZoneGenerator(0,SIMULATION)->start);
	CPPUNIT_ASSERT (-1 == scheduler->getZoneGenerator(0,SIMULATION)->length);
	CPPUNIT_ASSERT ( 0 == scheduler->getZoneGenerator(0,BALANCE_AUTHS)->start);
	CPPUNIT_ASSERT (-1 == scheduler->getZoneGenerator(0,BALANCE_AUTHS)->length);
	CPPUNIT_ASSERT ( 0 == scheduler->getZoneGenerator(0,ZONE)->start);
	CPPUNIT_ASSERT (-1 == scheduler->getZoneGenerator(0,ZONE)->length);
	CPPUNIT_ASSERT ( 0 == scheduler->getZoneGenerator(0,CONVENTIONAL)->start);
	CPPUNIT_ASSERT ( 2 == scheduler->getZoneGenerator(0,CONVENTIONAL)->length);
	CPPUNIT_ASSERT ( 2 == scheduler->getZoneGenerator(0,STORAGE)->start);
	CPPUNIT_ASSERT ( 0 == scheduler->getZoneGenerator(0,STORAGE)->length);
	CPPUNIT_ASSERT ( 2 == scheduler->getZoneGenerator(0,HYDRO)->start);
	CPPUNIT_ASSERT ( 0 == scheduler->getZoneGenerator(0,HYDRO)->length);
	CPPUNIT_ASSERT ( 0 == scheduler->getZoneGenerator(0,LINE)->start);
	CPPUNIT_ASSERT (-1 == scheduler->getZoneGenerator(0,LINE)->length);
	CPPUNIT_ASSERT ( 0 == scheduler->getZoneGenerator(0,DEMAND_RESPONSE)->start);
	CPPUNIT_ASSERT (-1 == scheduler->getZoneGenerator(0,DEMAND_RESPONSE)->length);
	CPPUNIT_ASSERT ( 0 == scheduler->getZoneGenerator(0,REPLICATE)->start);
	CPPUNIT_ASSERT (-1 == scheduler->getZoneGenerator(0,REPLICATE)->length);
	CPPUNIT_ASSERT ( 0 == scheduler->getZoneGenerator(0,LOAD_TIMESERIES)->start);
	CPPUNIT_ASSERT (-1 == scheduler->getZoneGenerator(0,LOAD_TIMESERIES)->length);
	CPPUNIT_ASSERT ( 0 == scheduler->getZoneGenerator(0,WIND_TIMESERIES)->start);
	CPPUNIT_ASSERT (-1 == scheduler->getZoneGenerator(0,WIND_TIMESERIES)->length);
	CPPUNIT_ASSERT ( 0 == scheduler->getZoneGenerator(0,SOLAR_TIMESERIES)->start);
	CPPUNIT_ASSERT (-1 == scheduler->getZoneGenerator(0,SOLAR_TIMESERIES)->length);
	CPPUNIT_ASSERT ( 0 == scheduler->getZoneGenerator(0,INTERCHANGE_TIMESERIES)->start);
	CPPUNIT_ASSERT (-1 == scheduler->getZoneGenerator(0,INTERCHANGE_TIMESERIES)->length);

	CPPUNIT_ASSERT ( 0 == scheduler->getZoneGenerator(1,SIMULATION)->start);
	CPPUNIT_ASSERT (-1 == scheduler->getZoneGenerator(1,SIMULATION)->length);
	CPPUNIT_ASSERT ( 0 == scheduler->getZoneGenerator(1,BALANCE_AUTHS)->start);
	CPPUNIT_ASSERT (-1 == scheduler->getZoneGenerator(1,BALANCE_AUTHS)->length);
	CPPUNIT_ASSERT ( 0 == scheduler->getZoneGenerator(1,ZONE)->start);
	CPPUNIT_ASSERT (-1 == scheduler->getZoneGenerator(1,ZONE)->length);
	CPPUNIT_ASSERT ( 2 == scheduler->getZoneGenerator(1,CONVENTIONAL)->start);
	CPPUNIT_ASSERT ( 3 == scheduler->getZoneGenerator(1,CONVENTIONAL)->length);
	CPPUNIT_ASSERT ( 5 == scheduler->getZoneGenerator(1,STORAGE)->start);
	CPPUNIT_ASSERT ( 0 == scheduler->getZoneGenerator(1,STORAGE)->length);
	CPPUNIT_ASSERT ( 5 == scheduler->getZoneGenerator(1,HYDRO)->start);
	CPPUNIT_ASSERT ( 0 == scheduler->getZoneGenerator(1,HYDRO)->length);
	CPPUNIT_ASSERT ( 0 == scheduler->getZoneGenerator(1,LINE)->start);
	CPPUNIT_ASSERT (-1 == scheduler->getZoneGenerator(1,LINE)->length);
	CPPUNIT_ASSERT ( 0 == scheduler->getZoneGenerator(1,DEMAND_RESPONSE)->start);
	CPPUNIT_ASSERT (-1 == scheduler->getZoneGenerator(1,DEMAND_RESPONSE)->length);
	CPPUNIT_ASSERT ( 0 == scheduler->getZoneGenerator(1,REPLICATE)->start);
	CPPUNIT_ASSERT (-1 == scheduler->getZoneGenerator(1,REPLICATE)->length);
	CPPUNIT_ASSERT ( 0 == scheduler->getZoneGenerator(1,LOAD_TIMESERIES)->start);
	CPPUNIT_ASSERT (-1 == scheduler->getZoneGenerator(1,LOAD_TIMESERIES)->length);
	CPPUNIT_ASSERT ( 0 == scheduler->getZoneGenerator(1,WIND_TIMESERIES)->start);
	CPPUNIT_ASSERT (-1 == scheduler->getZoneGenerator(1,WIND_TIMESERIES)->length);
	CPPUNIT_ASSERT ( 0 == scheduler->getZoneGenerator(1,SOLAR_TIMESERIES)->start);
	CPPUNIT_ASSERT (-1 == scheduler->getZoneGenerator(1,SOLAR_TIMESERIES)->length);
	CPPUNIT_ASSERT ( 0 == scheduler->getZoneGenerator(1,INTERCHANGE_TIMESERIES)->start);
	CPPUNIT_ASSERT (-1 == scheduler->getZoneGenerator(1,INTERCHANGE_TIMESERIES)->length);

	CPPUNIT_ASSERT ( 0 == scheduler->getZoneGenerator(2,SIMULATION)->start);
	CPPUNIT_ASSERT (-1 == scheduler->getZoneGenerator(2,SIMULATION)->length);
	CPPUNIT_ASSERT ( 0 == scheduler->getZoneGenerator(2,BALANCE_AUTHS)->start);
	CPPUNIT_ASSERT (-1 == scheduler->getZoneGenerator(2,BALANCE_AUTHS)->length);
	CPPUNIT_ASSERT ( 0 == scheduler->getZoneGenerator(2,ZONE)->start);
	CPPUNIT_ASSERT (-1 == scheduler->getZoneGenerator(2,ZONE)->length);
	CPPUNIT_ASSERT ( 5 == scheduler->getZoneGenerator(2,CONVENTIONAL)->start);
	CPPUNIT_ASSERT ( 4 == scheduler->getZoneGenerator(2,CONVENTIONAL)->length);
	CPPUNIT_ASSERT ( 9 == scheduler->getZoneGenerator(2,STORAGE)->start);
	CPPUNIT_ASSERT ( 0 == scheduler->getZoneGenerator(2,STORAGE)->length);
	CPPUNIT_ASSERT ( 9 == scheduler->getZoneGenerator(2,HYDRO)->start);
	CPPUNIT_ASSERT ( 0 == scheduler->getZoneGenerator(2,HYDRO)->length);
	CPPUNIT_ASSERT ( 0 == scheduler->getZoneGenerator(2,LINE)->start);
	CPPUNIT_ASSERT (-1 == scheduler->getZoneGenerator(2,LINE)->length);
	CPPUNIT_ASSERT ( 0 == scheduler->getZoneGenerator(2,DEMAND_RESPONSE)->start);
	CPPUNIT_ASSERT (-1 == scheduler->getZoneGenerator(2,DEMAND_RESPONSE)->length);
	CPPUNIT_ASSERT ( 0 == scheduler->getZoneGenerator(2,REPLICATE)->start);
	CPPUNIT_ASSERT (-1 == scheduler->getZoneGenerator(2,REPLICATE)->length);
	CPPUNIT_ASSERT ( 0 == scheduler->getZoneGenerator(2,LOAD_TIMESERIES)->start);
	CPPUNIT_ASSERT (-1 == scheduler->getZoneGenerator(2,LOAD_TIMESERIES)->length);
	CPPUNIT_ASSERT ( 0 == scheduler->getZoneGenerator(2,WIND_TIMESERIES)->start);
	CPPUNIT_ASSERT (-1 == scheduler->getZoneGenerator(2,WIND_TIMESERIES)->length);
	CPPUNIT_ASSERT ( 0 == scheduler->getZoneGenerator(2,SOLAR_TIMESERIES)->start);
	CPPUNIT_ASSERT (-1 == scheduler->getZoneGenerator(2,SOLAR_TIMESERIES)->length);
	CPPUNIT_ASSERT ( 0 == scheduler->getZoneGenerator(2,INTERCHANGE_TIMESERIES)->start);
	CPPUNIT_ASSERT (-1 == scheduler->getZoneGenerator(2,INTERCHANGE_TIMESERIES)->length);

	CPPUNIT_ASSERT ( 0 == scheduler->getZoneGenerator(3,SIMULATION)->start);
	CPPUNIT_ASSERT (-1 == scheduler->getZoneGenerator(3,SIMULATION)->length);
	CPPUNIT_ASSERT ( 0 == scheduler->getZoneGenerator(3,BALANCE_AUTHS)->start);
	CPPUNIT_ASSERT (-1 == scheduler->getZoneGenerator(3,BALANCE_AUTHS)->length);
	CPPUNIT_ASSERT ( 0 == scheduler->getZoneGenerator(3,ZONE)->start);
	CPPUNIT_ASSERT (-1 == scheduler->getZoneGenerator(3,ZONE)->length);
	CPPUNIT_ASSERT ( 9 == scheduler->getZoneGenerator(3,CONVENTIONAL)->start);
	CPPUNIT_ASSERT ( 5 == scheduler->getZoneGenerator(3,CONVENTIONAL)->length);
	CPPUNIT_ASSERT (14 == scheduler->getZoneGenerator(3,STORAGE)->start);
	CPPUNIT_ASSERT ( 0 == scheduler->getZoneGenerator(3,STORAGE)->length);
	CPPUNIT_ASSERT (14 == scheduler->getZoneGenerator(3,HYDRO)->start);
	CPPUNIT_ASSERT ( 0 == scheduler->getZoneGenerator(3,HYDRO)->length);
	CPPUNIT_ASSERT ( 0 == scheduler->getZoneGenerator(3,LINE)->start);
	CPPUNIT_ASSERT (-1 == scheduler->getZoneGenerator(3,LINE)->length);
	CPPUNIT_ASSERT ( 0 == scheduler->getZoneGenerator(3,DEMAND_RESPONSE)->start);
	CPPUNIT_ASSERT (-1 == scheduler->getZoneGenerator(3,DEMAND_RESPONSE)->length);
	CPPUNIT_ASSERT ( 0 == scheduler->getZoneGenerator(3,REPLICATE)->start);
	CPPUNIT_ASSERT (-1 == scheduler->getZoneGenerator(3,REPLICATE)->length);
	CPPUNIT_ASSERT ( 0 == scheduler->getZoneGenerator(3,LOAD_TIMESERIES)->start);
	CPPUNIT_ASSERT (-1 == scheduler->getZoneGenerator(3,LOAD_TIMESERIES)->length);
	CPPUNIT_ASSERT ( 0 == scheduler->getZoneGenerator(3,WIND_TIMESERIES)->start);
	CPPUNIT_ASSERT (-1 == scheduler->getZoneGenerator(3,WIND_TIMESERIES)->length);
	CPPUNIT_ASSERT ( 0 == scheduler->getZoneGenerator(3,SOLAR_TIMESERIES)->start);
	CPPUNIT_ASSERT (-1 == scheduler->getZoneGenerator(3,SOLAR_TIMESERIES)->length);
	CPPUNIT_ASSERT ( 0 == scheduler->getZoneGenerator(3,INTERCHANGE_TIMESERIES)->start);
	CPPUNIT_ASSERT (-1 == scheduler->getZoneGenerator(3,INTERCHANGE_TIMESERIES)->length);

	CPPUNIT_ASSERT ( 0 == scheduler->getZoneGenerator(4,SIMULATION)->start);
	CPPUNIT_ASSERT (-1 == scheduler->getZoneGenerator(4,SIMULATION)->length);
	CPPUNIT_ASSERT ( 0 == scheduler->getZoneGenerator(4,BALANCE_AUTHS)->start);
	CPPUNIT_ASSERT (-1 == scheduler->getZoneGenerator(4,BALANCE_AUTHS)->length);
	CPPUNIT_ASSERT ( 0 == scheduler->getZoneGenerator(4,ZONE)->start);
	CPPUNIT_ASSERT (-1 == scheduler->getZoneGenerator(4,ZONE)->length);
	CPPUNIT_ASSERT (14 == scheduler->getZoneGenerator(4,CONVENTIONAL)->start);
	CPPUNIT_ASSERT ( 6 == scheduler->getZoneGenerator(4,CONVENTIONAL)->length);
	CPPUNIT_ASSERT (20 == scheduler->getZoneGenerator(4,STORAGE)->start);
	CPPUNIT_ASSERT ( 0 == scheduler->getZoneGenerator(4,STORAGE)->length);
	CPPUNIT_ASSERT (20 == scheduler->getZoneGenerator(4,HYDRO)->start);
	CPPUNIT_ASSERT ( 0 == scheduler->getZoneGenerator(4,HYDRO)->length);
	CPPUNIT_ASSERT ( 0 == scheduler->getZoneGenerator(4,LINE)->start);
	CPPUNIT_ASSERT (-1== scheduler->getZoneGenerator(4,LINE)->length);
	CPPUNIT_ASSERT ( 0 == scheduler->getZoneGenerator(4,DEMAND_RESPONSE)->start);
	CPPUNIT_ASSERT (-1 == scheduler->getZoneGenerator(4,DEMAND_RESPONSE)->length);
	CPPUNIT_ASSERT ( 0 == scheduler->getZoneGenerator(4,REPLICATE)->start);
	CPPUNIT_ASSERT (-1 == scheduler->getZoneGenerator(4,REPLICATE)->length);
	CPPUNIT_ASSERT ( 0 == scheduler->getZoneGenerator(4,LOAD_TIMESERIES)->start);
	CPPUNIT_ASSERT (-1 == scheduler->getZoneGenerator(4,LOAD_TIMESERIES)->length);
	CPPUNIT_ASSERT ( 0 == scheduler->getZoneGenerator(4,WIND_TIMESERIES)->start);
	CPPUNIT_ASSERT (-1 == scheduler->getZoneGenerator(4,WIND_TIMESERIES)->length);
	CPPUNIT_ASSERT ( 0 == scheduler->getZoneGenerator(4,SOLAR_TIMESERIES)->start);
	CPPUNIT_ASSERT (-1 == scheduler->getZoneGenerator(4,SOLAR_TIMESERIES)->length);
	CPPUNIT_ASSERT ( 0 == scheduler->getZoneGenerator(4,INTERCHANGE_TIMESERIES)->start);
	CPPUNIT_ASSERT (-1 == scheduler->getZoneGenerator(4,INTERCHANGE_TIMESERIES)->length);
}

void
TestModel2::zoneSetupTests(void)
{
	/*
	const char             * fileName = "units/data/rimParserTests/rimBasic/sources.txt";
	parser = interpreter::RIMParser::getRIMParser(fileName);
	parser->configure();
	scheduler           = simulator::Scheduler::makeScheduler();
	scheduler->setAdjLoads();
	*/
	scheduler->setupModels();

	CPPUNIT_ASSERT (scheduler != NULL);

	/*
	delete(scheduler);
	delete(parser);
	*/
}

void
TestModel2::baSetupTests(void)
{
//	CPPUNIT_ASSERT (scheduler != NULL);
}

} /* namespace testing */
