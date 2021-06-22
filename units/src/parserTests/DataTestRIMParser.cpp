/*
 * DataTestRIMParser.cpp
 *
 *  Created on: Jan 19, 2012
 *      Author: kglass
 */

#include "parserTests/DataTestRIMParser.hpp"
#include "ModelParser.hpp"
#include "parserTests/BATest.hpp"
#include "parserTests/SimTest.hpp"
#include "parserTests/ZoneTest.hpp"
#include "parserTests/GeneratorTest.hpp"
#include "parserTests/LoadTest.hpp"
#include "parserTests/WindTest.hpp"
#include "parserTests/SolarTest.hpp"
#include <string>

namespace testing {
CPPUNIT_TEST_SUITE_REGISTRATION (DataTestRIMParser);

void
DataTestRIMParser::setUp(void)
{

	rimParser                  = new interpreter::RIMParser("units/data/rimParserTests/basic/sources.txt");
	parameters                 = rimParser->parameterSpace;
	offset                     = rimParser->parameterOffset;
	info                       = (SimulationInfo *)parameters;
}

void
DataTestRIMParser::tearDown(void)
{

	delete(rimParser);
	rimParser = NULL;
	parameters = NULL;
	offset = NULL;
	info  = NULL;
}
/*
 * Verify the data spaces are allocated and initialized
 */
void
DataTestRIMParser::rimSimulationLoadTests(void)
{
	SimulationInfo         * info        = (SimulationInfo *)&parameters[offset[SIMULATION]];
	int                 nSims     = 1;

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
				2,                             // nBalancingAuths
				0,                             // nSchedulers
				5,                             // nZones
				20,                            // nConventionalGenerators
				0,                             // nStorageGenerators
				0,                             // nHydroGenerators
				0,                             // nLines
				0,                             // nDR
				0,                             // nReplicates
				5,                             // nLoads
				20,                            // nWindFarms
				20,                            // nSolarFarms
				0,                             // nInterchanges
				20,                            // nGenerators
				73                             // nModels
			};

	SimTest test(&expectedParams, info, nSims);
}

void
DataTestRIMParser::rimBALoadTests(void)
{
	BalanceAuthParams      * baParams        = (BalanceAuthParams *)&parameters[offset[BALANCE_AUTHS]];
	int                 nBAs         = 2;

	BalanceAuthParams   expectedParams[] = {
			{"BA 0", 100.0, 101.0, 102.0, 103, 104.0, 3, 9, 9, 0, 0, 1, 9, 9, 0.0, 0.0, 0.0, 0.0},
			{"BA 1", 1000.0, 1001.0, 1002.0, 1003.0, 1004.0, 2, 11, 11, 0, 0, 1, 11, 11, 0.0, 0.0, 0.0, 0.0}
		};

	BATest test(expectedParams, baParams, nBAs);
}

void
DataTestRIMParser::rimZoneLoadTests(void)
{
	ZoneParams        * zoneParams     = (ZoneParams *)&parameters[offset[ZONE]];
	int                 nZones         = 5;

	/*
	 * NOTE:
	 * zoneBAId is a complex computation and will be tested in later tests
	 * (ConfigTestRIMParser). For now, set it to zero
	 *
	 * generatorOffset is a complex computation and will be tested in later
	 * tests (ConfigTestRIMParser). For now, set it to zero
	 *
	 */
	ZoneParams   expectedParams[] = {
			{"Zone 00", 100.0,     101.0,     102.0,     0, 0, 2, 2, 0, 0, 1, 2, 2, "data/test/time_series/interchange.data"},
			{"Zone 01", 1000.0,    1001.0,    1002.0,    0, 0, 3, 3, 0, 0, 1, 3, 3, "data/test/time_series/interchange.data"},
			{"Zone 02", 10000.0,   10001.0,   10002.0,   0, 0, 4, 4, 0, 0, 1, 4, 4, "data/test/time_series/interchange.data"},
			{"Zone 10", 100000.0,  100001.0,  100002.0,  1, 0, 5, 5, 0, 0, 1, 5, 5, "data/test/time_series/interchange.data"},
			{"Zone 11", 1000000.0, 1000001.0, 1000002.0, 1, 0, 6, 6, 0, 0, 1, 6, 6, "data/test/time_series/interchange.data"},
		};

	ZoneTest test(expectedParams, zoneParams, nZones);
}

void
DataTestRIMParser::rimConventionalTests(void)
{
	ConventionalParams * actualValues    = (ConventionalParams *)&parameters[offset[CONVENTIONAL]];
	int                  nConventional    = 20;

	ConventionalParams   expectedParams[] =
	{
	/*
		{
			{"Generator 000",
			{nSegments,{{{sp0, sp1},{ep0, ep1}, inter, slp},{{sp0, sp1},{ep0, ep1}, inter, slp},{{sp0, sp1},{ep0, ep1}, inter, slp},{{sp0, sp1},{ep0, ep1}, inter, slp},{{sp0, sp1},{ep0, ep1}, inter, slp}}},   // heat rate curve
			{nSegments,{{{sp0, sp1},{ep0, ep1}, inter, slp},{{sp0, sp1},{ep0, ep1}, inter, slp},{{sp0, sp1},{ep0, ep1}, inter, slp},{{sp0, sp1},{ep0, ep1}, inter, slp},{{sp0, sp1},{ep0, ep1}, inter, slp}}},   // heat rate curve
			{power limits}, {ramp rates}, fuelCost, ba,zone, canCommit, canLoadFollow, canRegulate, noxRate,soxRate, co2Rate, fuelType, currentPower, timeInState, outputPower, powerCostOnStep, accumulatedPowerCost,
			noxEmitted, soxEmitted, co2emitted, requiredPower},
			{genTransCost}, {minTIS}, convZoneID, status
		}
	*/
			{  // conventional generator 0
				{"Generator 000",
				{4,{{{201,202},{203,204}, 0.0, 0.0},{{205,206},{207,208}, 0.0, 0.0},{{209,210},{211,212}, 0.0, 0.0},{{213,214},{215,216}, 0.0, 0.0},{{0.0, 0.0},{0.0, 0.0}, 0.0, 0.0}}},   // heat rate curve
				{4,{{{0.0, 0.0},{0.0, 0.0}, 0.0, 0.0},{{0.0, 0.0},{0.0, 0.0}, 0.0, 0.0},{{0.0, 0.0},{0.0, 0.0}, 0.0, 0.0},{{0.0, 0.0},{0.0, 0.0}, 0.0, 0.0},{{0.0, 0.0},{0.0, 0.0}, 0.0, 0.0}}},   // heat rate curve
				{0,0}, {217, 218}, 223, 0,0, true, false, true, 226,227, 228, NUCLEAR, 0.0, 226, -1.0, -1.0, -1.0,
				-1.0, -1.0, -1.0, -1.0},{219,220}, {224,225}, 0, GENERATOR_INIT
			}, // end conventional generator 0
			{  // conventional generator 1
				{"Generator 001",
				{4,{{{2001,2002},{2003,2004}, 0.0, 0.0},{{2005,2006},{2007,2008}, 0.0, 0.0},{{2009,2010},{2011,2012}, 0.0, 0.0},{{2013,2014},{2015,2016}, 0.0, 0.0},{{0.0, 0.0},{0.0, 0.0}, 0.0, 0.0}}},   // heat rate curve
				{4,{{{0.0, 0.0},{0.0, 0.0}, 0.0, 0.0},{{0.0, 0.0},{0.0, 0.0}, 0.0, 0.0},{{0.0, 0.0},{0.0, 0.0}, 0.0, 0.0},{{0.0, 0.0},{0.0, 0.0}, 0.0, 0.0},{{0.0, 0.0},{0.0, 0.0}, 0.0, 0.0}}},   // heat rate curve
				{0,0}, {2017, 2018}, 2023, 0,0, true, false, true, 2026,2027, 2028, NUCLEAR, 0.0, 2026, -1.0, -1.0, -1.0,
				-1.0, -1.0, -1.0, -1.0},{2019,2020}, {2024,2025}, 0, GENERATOR_INIT
			}, // end conventional generator 1
			{  // conventional generator 2
				{"Generator 010",
				{4,{{{20001,20002},{20003,20004}, 0.0, 0.0},{{20005,20006},{20007,20008}, 0.0, 0.0},{{20009,20010},{20011,20012}, 0.0, 0.0},{{20013,20014},{20015,20016}, 0.0, 0.0},{{0.0, 0.0},{0.0, 0.0}, 0.0, 0.0}}},   // heat rate curve
				{4,{{{0.0, 0.0},{0.0, 0.0}, 0.0, 0.0},{{0.0, 0.0},{0.0, 0.0}, 0.0, 0.0},{{0.0, 0.0},{0.0, 0.0}, 0.0, 0.0},{{0.0, 0.0},{0.0, 0.0}, 0.0, 0.0},{{0.0, 0.0},{0.0, 0.0}, 0.0, 0.0}}},   // heat rate curve
				{0,0}, {20017, 20018}, 20023, 0,1, true, false, true, 20026,20027, 20028, NUCLEAR, 0.0, 20026, -1.0, -1.0, -1.0,
				-1.0, -1.0, -1.0, -1.0},{20019,20020}, {20024,20025}, 0, GENERATOR_INIT
			}, // end conventional generator 2
			{  // conventional generator 3
				{"Generator 011",
				{4,{{{201,202},{203,204}, 0.0, 0.0},{{205,206},{207,208}, 0.0, 0.0},{{209,210},{211,212}, 0.0, 0.0},{{213,214},{215,216}, 0.0, 0.0},{{0.0, 0.0},{0.0, 0.0}, 0.0, 0.0}}},   // heat rate curve
				{4,{{{0.0, 0.0},{0.0, 0.0}, 0.0, 0.0},{{0.0, 0.0},{0.0, 0.0}, 0.0, 0.0},{{0.0, 0.0},{0.0, 0.0}, 0.0, 0.0},{{0.0, 0.0},{0.0, 0.0}, 0.0, 0.0},{{0.0, 0.0},{0.0, 0.0}, 0.0, 0.0}}},   // heat rate curve
				{0,0}, {217, 218}, 223, 0,1, true, false, true, 226,227, 228, NUCLEAR, 0.0, 226, -1.0, -1.0, -1.0,
				-1.0, -1.0, -1.0, -1.0},{219,220}, {224,225}, 0, GENERATOR_INIT
			}, // end conventional generator 3
			{  // conventional generator 4
				{"Generator 012",
				{4,{{{2001,2002},{2003,2004}, 0.0, 0.0},{{2005,2006},{2007,2008}, 0.0, 0.0},{{2009,2010},{2011,2012}, 0.0, 0.0},{{2013,2014},{2015,2016}, 0.0, 0.0},{{0.0, 0.0},{0.0, 0.0}, 0.0, 0.0}}},   // heat rate curve
				{4,{{{0.0, 0.0},{0.0, 0.0}, 0.0, 0.0},{{0.0, 0.0},{0.0, 0.0}, 0.0, 0.0},{{0.0, 0.0},{0.0, 0.0}, 0.0, 0.0},{{0.0, 0.0},{0.0, 0.0}, 0.0, 0.0},{{0.0, 0.0},{0.0, 0.0}, 0.0, 0.0}}},   // heat rate curve
				{0,0}, {2017, 2018}, 2023, 0,1, true, false, true, 2026,2027, 2028, NUCLEAR, 0.0, 2026, -1.0, -1.0, -1.0,
				-1.0, -1.0, -1.0, -1.0},{2019,2020}, {2024,2025}, 0, GENERATOR_INIT
			}, // end conventional generator 4
			{  // conventional generator 5
				{"Generator 020",
				{4,{{{20001,20002},{20003,20004}, 0.0, 0.0},{{20005,20006},{20007,20008}, 0.0, 0.0},{{20009,20010},{20011,20012}, 0.0, 0.0},{{20013,20014},{20015,20016}, 0.0, 0.0},{{0.0, 0.0},{0.0, 0.0}, 0.0, 0.0}}},   // heat rate curve
				{4,{{{0.0, 0.0},{0.0, 0.0}, 0.0, 0.0},{{0.0, 0.0},{0.0, 0.0}, 0.0, 0.0},{{0.0, 0.0},{0.0, 0.0}, 0.0, 0.0},{{0.0, 0.0},{0.0, 0.0}, 0.0, 0.0},{{0.0, 0.0},{0.0, 0.0}, 0.0, 0.0}}},   // heat rate curve
				{0,0}, {20017, 20018}, 20023, 0,2, true, false, true, 20026,20027, 20028, NUCLEAR, 0.0, 20026, -1.0, -1.0, -1.0,
				-1.0, -1.0, -1.0, -1.0},{20019,20020}, {20024,20025}, 0, GENERATOR_INIT
			}, // end conventional generator 5
			{  // conventional generator 6
				{"Generator 021",
				{4,{{{201,202},{203,204}, 0.0, 0.0},{{205,206},{207,208}, 0.0, 0.0},{{209,210},{211,212}, 0.0, 0.0},{{213,214},{215,216}, 0.0, 0.0},{{0.0, 0.0},{0.0, 0.0}, 0.0, 0.0}}},   // heat rate curve
				{4,{{{0.0, 0.0},{0.0, 0.0}, 0.0, 0.0},{{0.0, 0.0},{0.0, 0.0}, 0.0, 0.0},{{0.0, 0.0},{0.0, 0.0}, 0.0, 0.0},{{0.0, 0.0},{0.0, 0.0}, 0.0, 0.0},{{0.0, 0.0},{0.0, 0.0}, 0.0, 0.0}}},   // heat rate curve
				{0,0}, {217, 218}, 223, 0,2, true, false, true, 226,227, 228, NUCLEAR, 0.0, 226, -1.0, -1.0, -1.0,
				-1.0, -1.0, -1.0, -1.0},{219,220}, {224,225}, 0, GENERATOR_INIT
			}, // end conventional generator 6
			{  // conventional generator 7
				{"Generator 022",
				{4,{{{2001,2002},{2003,2004}, 0.0, 0.0},{{2005,2006},{2007,2008}, 0.0, 0.0},{{2009,2010},{2011,2012}, 0.0, 0.0},{{2013,2014},{2015,2016}, 0.0, 0.0},{{0.0, 0.0},{0.0, 0.0}, 0.0, 0.0}}},   // heat rate curve
				{4,{{{0.0, 0.0},{0.0, 0.0}, 0.0, 0.0},{{0.0, 0.0},{0.0, 0.0}, 0.0, 0.0},{{0.0, 0.0},{0.0, 0.0}, 0.0, 0.0},{{0.0, 0.0},{0.0, 0.0}, 0.0, 0.0},{{0.0, 0.0},{0.0, 0.0}, 0.0, 0.0}}},   // heat rate curve
				{0,0}, {2017, 2018}, 2023, 0,2, true, false, true, 2026,2027, 2028, NUCLEAR, 0.0, 2026, -1.0, -1.0, -1.0,
				-1.0, -1.0, -1.0, -1.0},{2019,2020}, {2024,2025}, 0, GENERATOR_INIT
			}, // end conventional generator 7
			{  // conventional generator 8
				{"Generator 023",
				{4,{{{20001,20002},{20003,20004}, 0.0, 0.0},{{20005,20006},{20007,20008}, 0.0, 0.0},{{20009,20010},{20011,20012}, 0.0, 0.0},{{20013,20014},{20015,20016}, 0.0, 0.0},{{0.0, 0.0},{0.0, 0.0}, 0.0, 0.0}}},   // heat rate curve
				{4,{{{0.0, 0.0},{0.0, 0.0}, 0.0, 0.0},{{0.0, 0.0},{0.0, 0.0}, 0.0, 0.0},{{0.0, 0.0},{0.0, 0.0}, 0.0, 0.0},{{0.0, 0.0},{0.0, 0.0}, 0.0, 0.0},{{0.0, 0.0},{0.0, 0.0}, 0.0, 0.0}}},   // heat rate curve
				{0,0}, {20017, 20018}, 20023, 0,2, true, false, true, 20026,20027, 20028, NUCLEAR, 0.0, 20026, -1.0, -1.0, -1.0,
				-1.0, -1.0, -1.0, -1.0},{20019,20020}, {20024,20025}, 0, GENERATOR_INIT
			}, // end conventional generator 8
			{  // conventional generator 9
				{"Generator 100",
				{4,{{{201,202},{203,204}, 0.0, 0.0},{{205,206},{207,208}, 0.0, 0.0},{{209,210},{211,212}, 0.0, 0.0},{{213,214},{215,216}, 0.0, 0.0},{{0.0, 0.0},{0.0, 0.0}, 0.0, 0.0}}},   // heat rate curve
				{4,{{{0.0, 0.0},{0.0, 0.0}, 0.0, 0.0},{{0.0, 0.0},{0.0, 0.0}, 0.0, 0.0},{{0.0, 0.0},{0.0, 0.0}, 0.0, 0.0},{{0.0, 0.0},{0.0, 0.0}, 0.0, 0.0},{{0.0, 0.0},{0.0, 0.0}, 0.0, 0.0}}},   // heat rate curve
				{0,0}, {217, 218}, 223, 1,3, true, false, true, 226,227, 228, NUCLEAR, 0.0, 226, -1.0, -1.0, -1.0,
				-1.0, -1.0, -1.0, -1.0},{219,220}, {224,225}, 0, GENERATOR_INIT
			}, // end conventional generator 9
			{  // conventional generator 10
				{"Generator 101",
				{4,{{{2001,2002},{2003,2004}, 0.0, 0.0},{{2005,2006},{2007,2008}, 0.0, 0.0},{{2009,2010},{2011,2012}, 0.0, 0.0},{{2013,2014},{2015,2016}, 0.0, 0.0},{{0.0, 0.0},{0.0, 0.0}, 0.0, 0.0}}},   // heat rate curve
				{4,{{{0.0, 0.0},{0.0, 0.0}, 0.0, 0.0},{{0.0, 0.0},{0.0, 0.0}, 0.0, 0.0},{{0.0, 0.0},{0.0, 0.0}, 0.0, 0.0},{{0.0, 0.0},{0.0, 0.0}, 0.0, 0.0},{{0.0, 0.0},{0.0, 0.0}, 0.0, 0.0}}},   // heat rate curve
				{0,0}, {2017, 2018}, 2023, 1,3, true, false, true, 2026,2027, 2028, NUCLEAR, 0.0, 2026, -1.0, -1.0, -1.0,
				-1.0, -1.0, -1.0, -1.0},{2019,2020}, {2024,2025}, 0, GENERATOR_INIT
			}, // end conventional generator 10
			{  // conventional generator 11
				{"Generator 102",
				{4,{{{20001,20002},{20003,20004}, 0.0, 0.0},{{20005,20006},{20007,20008}, 0.0, 0.0},{{20009,20010},{20011,20012}, 0.0, 0.0},{{20013,20014},{20015,20016}, 0.0, 0.0},{{0.0, 0.0},{0.0, 0.0}, 0.0, 0.0}}},   // heat rate curve
				{4,{{{0.0, 0.0},{0.0, 0.0}, 0.0, 0.0},{{0.0, 0.0},{0.0, 0.0}, 0.0, 0.0},{{0.0, 0.0},{0.0, 0.0}, 0.0, 0.0},{{0.0, 0.0},{0.0, 0.0}, 0.0, 0.0},{{0.0, 0.0},{0.0, 0.0}, 0.0, 0.0}}},   // heat rate curve
				{0,0}, {20017, 20018}, 20023, 1,3, true, false, true, 20026,20027, 20028, NUCLEAR, 0.0, 20026, -1.0, -1.0, -1.0,
				-1.0, -1.0, -1.0, -1.0},{20019,20020}, {20024,20025}, 0, GENERATOR_INIT
			}, // end conventional generator 11
			{  // conventional generator 12
				{"Generator 103",
				{4,{{{201,202},{203,204}, 0.0, 0.0},{{205,206},{207,208}, 0.0, 0.0},{{209,210},{211,212}, 0.0, 0.0},{{213,214},{215,216}, 0.0, 0.0},{{0.0, 0.0},{0.0, 0.0}, 0.0, 0.0}}},   // heat rate curve
				{4,{{{0.0, 0.0},{0.0, 0.0}, 0.0, 0.0},{{0.0, 0.0},{0.0, 0.0}, 0.0, 0.0},{{0.0, 0.0},{0.0, 0.0}, 0.0, 0.0},{{0.0, 0.0},{0.0, 0.0}, 0.0, 0.0},{{0.0, 0.0},{0.0, 0.0}, 0.0, 0.0}}},   // heat rate curve
				{0,0}, {217, 218}, 223, 1,3, true, false, true, 226,227, 228, NUCLEAR, 0.0, 226, -1.0, -1.0, -1.0,
				-1.0, -1.0, -1.0, -1.0},{219,220}, {224,225}, 0, GENERATOR_INIT
			}, // end conventional generator 12
			{  // conventional generator 13
				{"Generator 104",
				{4,{{{2001,2002},{2003,2004}, 0.0, 0.0},{{2005,2006},{2007,2008}, 0.0, 0.0},{{2009,2010},{2011,2012}, 0.0, 0.0},{{2013,2014},{2015,2016}, 0.0, 0.0},{{0.0, 0.0},{0.0, 0.0}, 0.0, 0.0}}},   // heat rate curve
				{4,{{{0.0, 0.0},{0.0, 0.0}, 0.0, 0.0},{{0.0, 0.0},{0.0, 0.0}, 0.0, 0.0},{{0.0, 0.0},{0.0, 0.0}, 0.0, 0.0},{{0.0, 0.0},{0.0, 0.0}, 0.0, 0.0},{{0.0, 0.0},{0.0, 0.0}, 0.0, 0.0}}},   // heat rate curve
				{0,0}, {2017, 2018}, 2023, 1,3, true, false, true, 2026,2027, 2028, NUCLEAR, 0.0, 2026, -1.0, -1.0, -1.0,
				-1.0, -1.0, -1.0, -1.0},{2019,2020}, {2024,2025}, 0, GENERATOR_INIT
			}, // end conventional generator 13
			{  // conventional generator 14
				{"Generator 110",
				{4,{{{20001,20002},{20003,20004}, 0.0, 0.0},{{20005,20006},{20007,20008}, 0.0, 0.0},{{20009,20010},{20011,20012}, 0.0, 0.0},{{20013,20014},{20015,20016}, 0.0, 0.0},{{0.0, 0.0},{0.0, 0.0}, 0.0, 0.0}}},   // heat rate curve
				{4,{{{0.0, 0.0},{0.0, 0.0}, 0.0, 0.0},{{0.0, 0.0},{0.0, 0.0}, 0.0, 0.0},{{0.0, 0.0},{0.0, 0.0}, 0.0, 0.0},{{0.0, 0.0},{0.0, 0.0}, 0.0, 0.0},{{0.0, 0.0},{0.0, 0.0}, 0.0, 0.0}}},   // heat rate curve
				{0,0}, {20017, 20018}, 20023, 1,4, true, false, true, 20026,20027, 20028, NUCLEAR, 0.0, 20026, -1.0, -1.0, -1.0,
				-1.0, -1.0, -1.0, -1.0},{20019,20020}, {20024,20025}, 0, GENERATOR_INIT
			}, // end conventional generator 14
			{  // conventional generator 15
				{"Generator 111",
				{4,{{{201,202},{203,204}, 0.0, 0.0},{{205,206},{207,208}, 0.0, 0.0},{{209,210},{211,212}, 0.0, 0.0},{{213,214},{215,216}, 0.0, 0.0},{{0.0, 0.0},{0.0, 0.0}, 0.0, 0.0}}},   // heat rate curve
				{4,{{{0.0, 0.0},{0.0, 0.0}, 0.0, 0.0},{{0.0, 0.0},{0.0, 0.0}, 0.0, 0.0},{{0.0, 0.0},{0.0, 0.0}, 0.0, 0.0},{{0.0, 0.0},{0.0, 0.0}, 0.0, 0.0},{{0.0, 0.0},{0.0, 0.0}, 0.0, 0.0}}},   // heat rate curve
				{0,0}, {217, 218}, 223, 1,4, true, false, true, 226,227, 228, NUCLEAR, 0.0, 226, -1.0, -1.0, -1.0,
				-1.0, -1.0, -1.0, -1.0},{219,220}, {224,225}, 0, GENERATOR_INIT
			}, // end conventional generator 15
			{  // conventional generator 16
				{"Generator 112",
				{4,{{{2001,2002},{2003,2004}, 0.0, 0.0},{{2005,2006},{2007,2008}, 0.0, 0.0},{{2009,2010},{2011,2012}, 0.0, 0.0},{{2013,2014},{2015,2016}, 0.0, 0.0},{{0.0, 0.0},{0.0, 0.0}, 0.0, 0.0}}},   // heat rate curve
				{4,{{{0.0, 0.0},{0.0, 0.0}, 0.0, 0.0},{{0.0, 0.0},{0.0, 0.0}, 0.0, 0.0},{{0.0, 0.0},{0.0, 0.0}, 0.0, 0.0},{{0.0, 0.0},{0.0, 0.0}, 0.0, 0.0},{{0.0, 0.0},{0.0, 0.0}, 0.0, 0.0}}},   // heat rate curve
				{0,0}, {2017, 2018}, 2023, 1,4, true, false, true, 2026,2027, 2028, NUCLEAR, 0.0, 2026, -1.0, -1.0, -1.0,
				-1.0, -1.0, -1.0, -1.0},{2019,2020}, {2024,2025}, 0, GENERATOR_INIT
			}, // end conventional generator 16
			{  // conventional generator 17
				{"Generator 113",
				{4,{{{20001,20002},{20003,20004}, 0.0, 0.0},{{20005,20006},{20007,20008}, 0.0, 0.0},{{20009,20010},{20011,20012}, 0.0, 0.0},{{20013,20014},{20015,20016}, 0.0, 0.0},{{0.0, 0.0},{0.0, 0.0}, 0.0, 0.0}}},   // heat rate curve
				{4,{{{0.0, 0.0},{0.0, 0.0}, 0.0, 0.0},{{0.0, 0.0},{0.0, 0.0}, 0.0, 0.0},{{0.0, 0.0},{0.0, 0.0}, 0.0, 0.0},{{0.0, 0.0},{0.0, 0.0}, 0.0, 0.0},{{0.0, 0.0},{0.0, 0.0}, 0.0, 0.0}}},   // heat rate curve
				{0,0}, {20017, 20018}, 20023, 1,4, true, false, true, 20026,20027, 20028, NUCLEAR, 0.0, 20026, -1.0, -1.0, -1.0,
				-1.0, -1.0, -1.0, -1.0},{20019,20020}, {20024,20025}, 0, GENERATOR_INIT
			}, // end conventional generator 17
			{  // conventional generator 18
				{"Generator 114",
				{4,{{{201,202},{203,204}, 0.0, 0.0},{{205,206},{207,208}, 0.0, 0.0},{{209,210},{211,212}, 0.0, 0.0},{{213,214},{215,216}, 0.0, 0.0},{{0.0, 0.0},{0.0, 0.0}, 0.0, 0.0}}},   // heat rate curve
				{4,{{{0.0, 0.0},{0.0, 0.0}, 0.0, 0.0},{{0.0, 0.0},{0.0, 0.0}, 0.0, 0.0},{{0.0, 0.0},{0.0, 0.0}, 0.0, 0.0},{{0.0, 0.0},{0.0, 0.0}, 0.0, 0.0},{{0.0, 0.0},{0.0, 0.0}, 0.0, 0.0}}},   // heat rate curve
				{0,0}, {217, 218}, 223, 1,4, true, false, true, 226,227, 228, NUCLEAR, 0.0, 226, -1.0, -1.0, -1.0,
				-1.0, -1.0, -1.0, -1.0},{219,220}, {224,225}, 0, GENERATOR_INIT
			}, // end conventional generator 18
			{  // conventional generator 19
				{"Generator 115",
				{4,{{{2001,2002},{2003,2004}, 0.0, 0.0},{{2005,2006},{2007,2008}, 0.0, 0.0},{{2009,2010},{2011,2012}, 0.0, 0.0},{{2013,2014},{2015,2016}, 0.0, 0.0},{{0.0, 0.0},{0.0, 0.0}, 0.0, 0.0}}},   // heat rate curve
				{4,{{{0.0, 0.0},{0.0, 0.0}, 0.0, 0.0},{{0.0, 0.0},{0.0, 0.0}, 0.0, 0.0},{{0.0, 0.0},{0.0, 0.0}, 0.0, 0.0},{{0.0, 0.0},{0.0, 0.0}, 0.0, 0.0},{{0.0, 0.0},{0.0, 0.0}, 0.0, 0.0}}},   // heat rate curve
				{0,0}, {2017, 2018}, 2023, 1,4, true, false, true, 2026,2027, 2028, NUCLEAR, 0.0, 2026, -1.0, -1.0, -1.0,
				-1.0, -1.0, -1.0, -1.0},{2019,2020}, {2024,2025}, 0, GENERATOR_INIT
			} // end conventional generator 19
	};

	GeneratorTest test(expectedParams, actualValues, nConventional);
}

void
DataTestRIMParser::rimLoadTests(void)
{
	LoadParams  * actualValues  = (LoadParams *)&parameters[offset[LOAD_TIMESERIES]];
	LoadParams   expectedParams[] =
	{
		/*
			{{"Load 000"," fileName", ba, zone, currentLoad, outputLoad}}
		*/
			{"Load 000", "testing/data/validSrc/time_series/testLoad", 0, 0, -1.0, -1.0}, // end load 0
			{"Load 010", "testing/data/validSrc/time_series/testLoad", 0, 1, -1.0, -1.0}, // end load 1
			{"Load 020", "testing/data/validSrc/time_series/testLoad", 0, 2, -1.0, -1.0}, // end load 2
			{"Load 100", "testing/data/validSrc/time_series/testLoad", 1, 3, -1.0, -1.0}, // end load 3
			{"Load 110", "testing/data/validSrc/time_series/testLoad", 1, 4, -1.0, -1.0}  // end load 4
	};

	LoadTest test(expectedParams, actualValues, 5);
}

void
DataTestRIMParser::rimWindTests(void)
{
	WindParams  * actualValues  = (WindParams *)&parameters[offset[WIND_TIMESERIES]];
	WindParams   expectedParams[] =
	{
		/*
			{"Wind 000"," fileName", ba, zone, currentWind, outputWind}
		*/
			{"Wind 000", "testing/data/validSrc/time_series/testWind", 0, 0, -1.0, -1.0}, // end load 0
			{"Wind 001", "testing/data/validSrc/time_series/testWind", 0, 0, -1.0, -1.0}, // end load 0
			{"Wind 010", "testing/data/validSrc/time_series/testWind", 0, 1, -1.0, -1.0}, // end load 1
			{"Wind 011", "testing/data/validSrc/time_series/testWind", 0, 1, -1.0, -1.0}, // end load 1
			{"Wind 012", "testing/data/validSrc/time_series/testWind", 0, 1, -1.0, -1.0}, // end load 1
			{"Wind 020", "testing/data/validSrc/time_series/testWind", 0, 2, -1.0, -1.0}, // end load 2
			{"Wind 021", "testing/data/validSrc/time_series/testWind", 0, 2, -1.0, -1.0}, // end load 2
			{"Wind 022", "testing/data/validSrc/time_series/testWind", 0, 2, -1.0, -1.0}, // end load 2
			{"Wind 023", "testing/data/validSrc/time_series/testWind", 0, 2, -1.0, -1.0}, // end load 2
			{"Wind 100", "testing/data/validSrc/time_series/testWind", 1, 3, -1.0, -1.0}, // end load 3
			{"Wind 101", "testing/data/validSrc/time_series/testWind", 1, 3, -1.0, -1.0}, // end load 3
			{"Wind 102", "testing/data/validSrc/time_series/testWind", 1, 3, -1.0, -1.0}, // end load 3
			{"Wind 103", "testing/data/validSrc/time_series/testWind", 1, 3, -1.0, -1.0}, // end load 3
			{"Wind 104", "testing/data/validSrc/time_series/testWind", 1, 3, -1.0, -1.0}, // end load 3
			{"Wind 110", "testing/data/validSrc/time_series/testWind", 1, 4, -1.0, -1.0},  // end load 4
			{"Wind 111", "testing/data/validSrc/time_series/testWind", 1, 4, -1.0, -1.0},  // end load 4
			{"Wind 112", "testing/data/validSrc/time_series/testWind", 1, 4, -1.0, -1.0},  // end load 4
			{"Wind 113", "testing/data/validSrc/time_series/testWind", 1, 4, -1.0, -1.0},  // end load 4
			{"Wind 114", "testing/data/validSrc/time_series/testWind", 1, 4, -1.0, -1.0},  // end load 4
			{"Wind 115", "testing/data/validSrc/time_series/testWind", 1, 4, -1.0, -1.0}  // end load 4
	};

	WindTest test(expectedParams, actualValues, 20);
}

void
DataTestRIMParser::rimSolarTests(void)
{
	SolarParams  * actualValues  = (SolarParams *)&parameters[offset[SOLAR_TIMESERIES]];
	SolarParams   expectedParams[] =
	{
		/*
			{"Solar 000"," fileName", ba, zone, currentSolar, outputSolar}
		*/
			{"Solar 000", "testing/data/validSrc/time_series/testSolar", 0, 0, -1.0, -1.0}, // end load 0
			{"Solar 001", "testing/data/validSrc/time_series/testSolar", 0, 0, -1.0, -1.0}, // end load 0
			{"Solar 010", "testing/data/validSrc/time_series/testSolar", 0, 1, -1.0, -1.0}, // end load 1
			{"Solar 011", "testing/data/validSrc/time_series/testSolar", 0, 1, -1.0, -1.0}, // end load 1
			{"Solar 012", "testing/data/validSrc/time_series/testSolar", 0, 1, -1.0, -1.0}, // end load 1
			{"Solar 020", "testing/data/validSrc/time_series/testSolar", 0, 2, -1.0, -1.0}, // end load 2
			{"Solar 021", "testing/data/validSrc/time_series/testSolar", 0, 2, -1.0, -1.0}, // end load 2
			{"Solar 022", "testing/data/validSrc/time_series/testSolar", 0, 2, -1.0, -1.0}, // end load 2
			{"Solar 023", "testing/data/validSrc/time_series/testSolar", 0, 2, -1.0, -1.0}, // end load 2
			{"Solar 100", "testing/data/validSrc/time_series/testSolar", 1, 3, -1.0, -1.0}, // end load 3
			{"Solar 101", "testing/data/validSrc/time_series/testSolar", 1, 3, -1.0, -1.0}, // end load 3
			{"Solar 102", "testing/data/validSrc/time_series/testSolar", 1, 3, -1.0, -1.0}, // end load 3
			{"Solar 103", "testing/data/validSrc/time_series/testSolar", 1, 3, -1.0, -1.0}, // end load 3
			{"Solar 104", "testing/data/validSrc/time_series/testSolar", 1, 3, -1.0, -1.0}, // end load 3
			{"Solar 110", "testing/data/validSrc/time_series/testSolar", 1, 4, -1.0, -1.0},  // end load 4
			{"Solar 111", "testing/data/validSrc/time_series/testSolar", 1, 4, -1.0, -1.0},  // end load 4
			{"Solar 112", "testing/data/validSrc/time_series/testSolar", 1, 4, -1.0, -1.0},  // end load 4
			{"Solar 113", "testing/data/validSrc/time_series/testSolar", 1, 4, -1.0, -1.0},  // end load 4
			{"Solar 114", "testing/data/validSrc/time_series/testSolar", 1, 4, -1.0, -1.0},  // end load 4
			{"Solar 115", "testing/data/validSrc/time_series/testSolar", 1, 4, -1.0, -1.0}  // end load 4
	};

	SolarTest test(expectedParams, actualValues, 20);
}

} /* namespace testing */
