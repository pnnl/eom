/*
 * TestModel1.cpp
 *
 *  Created on: Jan 19, 2012
 *      Author: kglass
 */

#include "modelTests/TestModel1.hpp"
#include <string>

namespace testing {
CPPUNIT_TEST_SUITE_REGISTRATION (TestModel1);

void
TestModel1::setUp(void)
{
	const char             * fileName = "units/data/rimParserTests/rimBasic/sources.txt";
	parser = interpreter::RIMParser::getRIMParser(fileName);
	parser->configure();
	scheduler           = simulator::Scheduler::makeScheduler();
	scheduler->setAdjLoads();
}

void
TestModel1::tearDown(void)
{

	delete(scheduler);
	delete(parser);
}

void
TestModel1::schedulerSetupTests(void)
{
	const char             * fileName = "units/data/rimParserTests/rimBasic/sources.txt";
	CPPUNIT_ASSERT (scheduler != NULL);
	CPPUNIT_ASSERT_MESSAGE  ("INCORRECT N BA", scheduler->getNBAs() == 2);
	CPPUNIT_ASSERT_MESSAGE  ("INCORRECT N Zones", scheduler->getNZones() == 5);
	CPPUNIT_ASSERT_MESSAGE  ("INCORRECT N Generators", scheduler->getNGenerators() == 20);
	CPPUNIT_ASSERT_MESSAGE  ("INCORRECT N Conventional Generators", scheduler->getNConventionalGenerators() == 20);
	CPPUNIT_ASSERT_MESSAGE  ("INCORRECT N Storage Generators", scheduler->getNStorageGenerators() == 0);
	CPPUNIT_ASSERT_MESSAGE  ("INCORRECT N Hydro Generators", scheduler->getNHydroGenerators() == 0);

	CPPUNIT_ASSERT_MESSAGE  ("SD Interface not allocated", scheduler->getSDI() != NULL);
	CPPUNIT_ASSERT_MESSAGE  ("schedule data", scheduler->getScheduleData() != NULL);
	delete(scheduler);
	delete(parser);
	/*
	*/
}

void
TestModel1::generatorSetupTests(void)
{
	const char             * fileName = "units/data/rimParserTests/rimBasic/sources.txt";
	parser = interpreter::RIMParser::getRIMParser(fileName);
	parser->configure();
	scheduler           = simulator::Scheduler::makeScheduler();
	scheduler->setAdjLoads();
	scheduler->setupModels();
	/*
	*/

	CPPUNIT_ASSERT (scheduler != NULL);

	delete(scheduler);
	delete(parser);
	/*
	*/
}

void
TestModel1::zoneSetupTests(void)
{
//	CPPUNIT_ASSERT (scheduler != NULL);
}

void
TestModel1::baSetupTests(void)
{
//	CPPUNIT_ASSERT (scheduler != NULL);
}

} /* namespace testing */
