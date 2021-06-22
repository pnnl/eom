/*
 * BasicTestRIMParser.cpp
 *
 *  Created on: Jan 19, 2012
 *      Author: kglass
 */

#include "parserTests/BasicTestRIMParser.hpp"
#include "ModelParser.hpp"
#include <string>

namespace testing {
CPPUNIT_TEST_SUITE_REGISTRATION (BasicTestRIMParser);

void
BasicTestRIMParser::setUp(void)
{
	rimParser             = new interpreter::RIMParser("units/data/rimParserTests/basic/sources.txt");
	rimParser->setupRIMParsing();
	rimParser->loadSourceFiles();
	rimParser->collectModelInformation();
	rimParser->constructParameterSpace();
	char                   * parameterSpace = rimParser->parameterSpace;
	rimParser->modelParser[SIMULATION]->setModels(&parameterSpace[rimParser->parameterOffset[SIMULATION]], NULL, NULL);
	rimParser->constructInterfaceSpace();
	info           = (SimulationInfo *)parameterSpace;

}

void
BasicTestRIMParser::tearDown(void)
{
//	delete(rimParser);
}

void
BasicTestRIMParser::rimLoadSourceFileTests(void)
{
	interpreter::ModelParser  * parser         = NULL;
	string                 name           = rimParser->srcFileName[SIMULATION];
	string                 parserName     = rimParser->modelParser[SIMULATION]->file->fileName;
	CPPUNIT_ASSERT_MESSAGE  (" source data:  SIMULATION file name ", name == "units/data/rimParserTests/basic/simulation.csv");
	CPPUNIT_ASSERT_MESSAGE  (" source data:  SIMULATION parser incorrect", parserName == name);
	CPPUNIT_ASSERT_MESSAGE  (" source data:  SIMULATION interface struct size", rimParser->interfaceSize[SIMULATION] == sizeof(SimulationData));
	CPPUNIT_ASSERT_MESSAGE  (" source data:  SIMULATION parameter struct size", rimParser->parameterSize[SIMULATION] == sizeof(SimulationInfo));

	name           = rimParser->srcFileName[BALANCE_AUTHS];
	parserName     = rimParser->modelParser[BALANCE_AUTHS]->file->fileName;
	CPPUNIT_ASSERT_MESSAGE  (" source data:  BALANCE_AUTHS file name ", name == "units/data/rimParserTests/basic/ba.csv");
	CPPUNIT_ASSERT_MESSAGE  (" source data:  BALANCE_AUTHS parser incorrect", parserName == name);
	CPPUNIT_ASSERT_MESSAGE  (" source data:  BALANCE_AUTHS interface struct size", rimParser->interfaceSize[BALANCE_AUTHS] == sizeof(BalanceAuthData));
	CPPUNIT_ASSERT_MESSAGE  (" source data:  BALANCE_AUTHS parameter struct size", rimParser->parameterSize[BALANCE_AUTHS] == sizeof(BalanceAuthParams));

	name           = rimParser->srcFileName[ZONE];
	parserName     = rimParser->modelParser[ZONE]->file->fileName;
	CPPUNIT_ASSERT_MESSAGE  (" source data:  ZONE file name ", name == "units/data/rimParserTests/basic/zone.csv");
	CPPUNIT_ASSERT_MESSAGE  (" source data:  ZONE parser incorrect", parserName == name);
	CPPUNIT_ASSERT_MESSAGE  (" source data:  ZONE interface struct size", rimParser->interfaceSize[ZONE] == sizeof(ZoneData));
	CPPUNIT_ASSERT_MESSAGE  (" source data:  ZONE parameter struct size", rimParser->parameterSize[ZONE] == sizeof(ZoneParams));

	name           = rimParser->srcFileName[CONVENTIONAL];
	parserName     = rimParser->modelParser[CONVENTIONAL]->file->fileName;
	CPPUNIT_ASSERT_MESSAGE  (" source data:  CONVENTIONAL file name ", name == "units/data/rimParserTests/basic/generators.csv");
	CPPUNIT_ASSERT_MESSAGE  (" source data:  CONVENTIONAL parser incorrect", parserName == name);
	CPPUNIT_ASSERT_MESSAGE  (" source data:  CONVENTIONAL interface struct size", rimParser->interfaceSize[CONVENTIONAL] == sizeof(ConventionalData));
	CPPUNIT_ASSERT_MESSAGE  (" source data:  CONVENTIONAL parameter struct size", rimParser->parameterSize[CONVENTIONAL] == sizeof(ConventionalParams));

	name           = rimParser->srcFileName[LOAD_TIMESERIES];
	parserName     = rimParser->modelParser[LOAD_TIMESERIES]->file->fileName;
	CPPUNIT_ASSERT_MESSAGE  (" source data:  LOAD_TIMESERIES file name ", name == "units/data/rimParserTests/basic/load.csv");
	CPPUNIT_ASSERT_MESSAGE  (" source data:  LOAD_TIMESERIES parser incorrect", parserName == name);
	CPPUNIT_ASSERT_MESSAGE  (" source data:  LOAD_TIMESERIES interface struct size", rimParser->interfaceSize[LOAD_TIMESERIES] == sizeof(LoadData));
	CPPUNIT_ASSERT_MESSAGE  (" source data:  LOAD_TIMESERIES parameter struct size", rimParser->parameterSize[LOAD_TIMESERIES] == sizeof(LoadParams));

	name           = rimParser->srcFileName[WIND_TIMESERIES];
	parserName     = rimParser->modelParser[WIND_TIMESERIES]->file->fileName;
	CPPUNIT_ASSERT_MESSAGE  (" source data:  WIND_TIMESERIES file name ", name == "units/data/rimParserTests/basic/wind.csv");
	CPPUNIT_ASSERT_MESSAGE  (" source data:  WIND_TIMESERIES parser incorrect", parserName == name);
	CPPUNIT_ASSERT_MESSAGE  (" source data:  WIND_TIMESERIES interface struct size", rimParser->interfaceSize[WIND_TIMESERIES] == sizeof(WindData));
	CPPUNIT_ASSERT_MESSAGE  (" source data:  WIND_TIMESERIES parameter struct size", rimParser->parameterSize[WIND_TIMESERIES] == sizeof(WindParams));

	name           = rimParser->srcFileName[SOLAR_TIMESERIES];
	parserName     = rimParser->modelParser[SOLAR_TIMESERIES]->file->fileName;
	CPPUNIT_ASSERT_MESSAGE  (" source data:  SOLAR_TIMESERIES file name ", name == "units/data/rimParserTests/basic/solar.csv");
	CPPUNIT_ASSERT_MESSAGE  (" source data:  SOLAR_TIMESERIES parser incorrect", parserName == name);
	CPPUNIT_ASSERT_MESSAGE  (" source data:  SOLAR_TIMESERIES interface struct size", rimParser->interfaceSize[SOLAR_TIMESERIES] == sizeof(SolarData));
	CPPUNIT_ASSERT_MESSAGE  (" source data:  SOLAR_TIMESERIES parameter struct size", rimParser->parameterSize[SOLAR_TIMESERIES] == sizeof(SolarParams));

}

void
BasicTestRIMParser::rimCollectModelInformationTests(void)
{
	int              count      = 0;
	int              offsetI    = 0;
	int              offsetP    = 0;

	CPPUNIT_ASSERT_MESSAGE  (" source data:  SIMULATION interface offset", rimParser->interfaceOffset[SIMULATION] == offsetI);
	CPPUNIT_ASSERT_MESSAGE  (" source data:  SIMULATION parameter offset", rimParser->parameterOffset[SIMULATION] == offsetP);
	CPPUNIT_ASSERT_MESSAGE  (" source data:  SIMULATION model count", rimParser->modelCount[SIMULATION] == 1);
	offsetI += sizeof(SimulationData);
	offsetP += sizeof(SimulationInfo);

	CPPUNIT_ASSERT_MESSAGE  (" source data:  BALANCE_AUTHS interface offset", rimParser->interfaceOffset[BALANCE_AUTHS] == offsetI);
	CPPUNIT_ASSERT_MESSAGE  (" source data:  BALANCE_AUTHS parameter offset", rimParser->parameterOffset[BALANCE_AUTHS] == offsetP);
	CPPUNIT_ASSERT_MESSAGE  (" source data:  BALANCE_AUTHS model count", rimParser->modelCount[BALANCE_AUTHS] == 2);
	offsetI += 2*sizeof(BalanceAuthData);
	offsetP += 2*sizeof(BalanceAuthParams);

	CPPUNIT_ASSERT_MESSAGE  (" source data:  ZONE interface offset", rimParser->interfaceOffset[ZONE] == offsetI);
	CPPUNIT_ASSERT_MESSAGE  (" source data:  ZONE parameter offset", rimParser->parameterOffset[ZONE] == offsetP);
	CPPUNIT_ASSERT_MESSAGE  (" source data:  ZONE model count", rimParser->modelCount[ZONE] == 5);
	offsetI += 5*sizeof(ZoneData);
	offsetP += 5*sizeof(ZoneParams);

	CPPUNIT_ASSERT_MESSAGE  (" source data:  CONVENTIONAL interface offset", rimParser->interfaceOffset[CONVENTIONAL] == offsetI);
	CPPUNIT_ASSERT_MESSAGE  (" source data:  CONVENTIONAL parameter offset", rimParser->parameterOffset[CONVENTIONAL] == offsetP);
	CPPUNIT_ASSERT_MESSAGE  (" source data:  CONVENTIONAL model count", rimParser->modelCount[CONVENTIONAL] == 20);
	offsetI += 20*sizeof(ConventionalData);
	offsetP += 20*sizeof(ConventionalParams);

	CPPUNIT_ASSERT_MESSAGE  (" source data:  LOAD_TIMESERIES interface offset", rimParser->interfaceOffset[LOAD_TIMESERIES] == offsetI);
	CPPUNIT_ASSERT_MESSAGE  (" source data:  LOAD_TIMESERIES parameter offset", rimParser->parameterOffset[LOAD_TIMESERIES] == offsetP);
	CPPUNIT_ASSERT_MESSAGE  (" source data:  LOAD_TIMESERIES model count", rimParser->modelCount[LOAD_TIMESERIES] == 5);
	offsetI += 5*sizeof(LoadData);
	offsetP += 5*sizeof(LoadParams);

	CPPUNIT_ASSERT_MESSAGE  (" source data:  WIND_TIMESERIES interface offset", rimParser->interfaceOffset[WIND_TIMESERIES] == offsetI);
	CPPUNIT_ASSERT_MESSAGE  (" source data:  WIND_TIMESERIES parameter offset", rimParser->parameterOffset[WIND_TIMESERIES] == offsetP);
	CPPUNIT_ASSERT_MESSAGE  (" source data:  WIND_TIMESERIES model count", rimParser->modelCount[WIND_TIMESERIES] == 20);
	offsetI += 20*sizeof(WindData);
	offsetP += 20*sizeof(WindParams);

	CPPUNIT_ASSERT_MESSAGE  (" source data:  SOLAR_TIMESERIES interface offset", rimParser->interfaceOffset[SOLAR_TIMESERIES] == offsetI);
	CPPUNIT_ASSERT_MESSAGE  (" source data:  SOLAR_TIMESERIES parameter offset", rimParser->parameterOffset[SOLAR_TIMESERIES] == offsetP);
	CPPUNIT_ASSERT_MESSAGE  (" source data:  SOLAR_TIMESERIES model count", rimParser->modelCount[SOLAR_TIMESERIES] == 20);
	offsetI += 20*sizeof(SolarData);
	offsetP += 20*sizeof(SolarParams);
}

void
BasicTestRIMParser::rimConstructParameterSpaceTests(void)
{
	char                   * parameterSpace = rimParser->parameterSpace;
	SimulationInfo         * info           = (SimulationInfo *)parameterSpace;

	CPPUNIT_ASSERT_MESSAGE  (" source data: info n simulations ", info->nSimulations == 1);
	CPPUNIT_ASSERT_MESSAGE  (" source data: info n bas ", info->nBalancingAuths == 2);
	CPPUNIT_ASSERT_MESSAGE  (" source data: info n zones", info->nZones == 5);
	CPPUNIT_ASSERT_MESSAGE  (" source data: info n conventional", info->nConventionalGenerators == 20);
	CPPUNIT_ASSERT_MESSAGE  (" source data: info n storage", info->nStorageGenerators == 0);
	CPPUNIT_ASSERT_MESSAGE  (" source data: info n hydro", info->nHydroGenerators == 0);
	CPPUNIT_ASSERT_MESSAGE  (" source data: info n lines ", info->nLines == 0);
	CPPUNIT_ASSERT_MESSAGE  (" source data: info n demand repsonse", info->nDR == 0);
	CPPUNIT_ASSERT_MESSAGE  (" source data: info n replicates", info->nReplicates == 0);
	CPPUNIT_ASSERT_MESSAGE  (" source data: info n load TS", info->nLoads == 5);
	CPPUNIT_ASSERT_MESSAGE  (" source data: info n wind TS", info->nWindFarms == 20);
	CPPUNIT_ASSERT_MESSAGE  (" source data: info n solar TS ", info->nSolarFarms == 20);
	CPPUNIT_ASSERT_MESSAGE  (" source data: info n interchange TS ", info->nInterchanges == 0);
	CPPUNIT_ASSERT_MESSAGE  (" source data: info n generators ", info->nGenerators == 20);
}

void
BasicTestRIMParser::rimConstructInterfaceSpaceTests(void)
{
	int                      timeOffset     = 0;
	int                      dataOffset     = sizeof(SimulationData) +
			                                  2 * sizeof(BalanceAuthData) +
			                                  5 * sizeof(ZoneData) +
			                                  20 * sizeof(ConventionalData) +
			                                  5 * sizeof(LoadData) +
			                                  20 * sizeof(WindData) +
			                                  20 * sizeof(SolarData);

	CPPUNIT_ASSERT_MESSAGE  (" source data: info n models ", info->nInterfaceSteps == 16);
	for (INTEGER i = 0; i < info->nInterfaceSteps; i++) {
		CPPUNIT_ASSERT_MESSAGE  (" source data: info n models ", rimParser->interfaceStepOffset[i] == timeOffset);
		timeOffset += dataOffset;
	}

}

} /* namespace testing */
