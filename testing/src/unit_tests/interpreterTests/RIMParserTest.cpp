/*
 * RIMParserTest.cpp
 *
 *  Created on: Aug 25, 2011
 *      Author: kglass
 */

#include "unit_tests/interpreterTests/RIMParserTest.hpp"

namespace testing {

bool
RIMParserTest::constructRIMParserTest(const char * fileName)
{
	bool                     pass           = true;
	incrementTest();
	constructParser(fileName);
	 // Every test depends on the success of the previous tests, any failure means
	 // the remaining tests cannot be performed
	cout << "SETUP TEST" << endl;
	pass = setupRIMParsingTest();
	if (!pass) return false;
	cout << "Load Source Files Test" << endl;
	pass = loadSourceFilesTest();
	if (!pass) return false;
	cout << "Collect model info test" << endl;
	pass = collectModelInformationTest();
	if (!pass) return false;
	cout << "Construct parameters test" << endl;
	pass = constructParameterSpaceTest();
	if (!pass) return false;
	// set model is tested separately by other test classes

	parser->modelParser[SIMULATION]->setModels(&parser->parameterSpace[parser->parameterOffset[SIMULATION]], NULL, (SimulationInfo *)&parser->parameterSpace[parser->parameterOffset[SIMULATION]]);
	cout << "Construct interface test" << endl;
	pass = constructInterfaceSpaceTest();
	if (!pass) return false;
	for (INTEGER i = 1; i < NUM_MODELS; i++) {
		if (parser->modelParser[i] != NULL) {
			parser->modelParser[i]->setModels(&parser->parameterSpace[parser->parameterOffset[i]],
					&parser->interfaceSpace[parser->interfaceOffset[i]],
					(SimulationInfo *)&parser->parameterSpace[parser->parameterOffset[SIMULATION]]);
			parser->modelCount[i] = parser->modelParser[i]->getNLines();
		} else {
			parser->modelCount[i] = 0;
		}
	}

	cout << "resolve dependencies test" << endl;
	pass = resolveDependenciesTest();
	if (!pass) return false;

	return true;
}

bool
RIMParserTest::constructParser(const char * name)
{
	bool                     pass           = true;

	parser              = new interpreter::RIMParser(name);

	return pass;
}

bool
RIMParserTest::destructParser(const char * name)
{
	bool                     pass           = true;
	return pass;

}

bool
RIMParserTest::setupRIMParsingTest()
{
	bool                     pass           = true;

	parser->setupRIMParsing();
	// verify values set by setupRIMParsing()
	for (INTEGER i = 0; i < NUM_MODELS; i++) {
		if (parser->srcFileName[i] != NULL) pass = false;
		if (parser->modelParser[i] != NULL) pass = false;
		if (parser->modelCount[i] != 0) pass = false;
		if (parser->parameterOffset[i] != -1) pass = false;
		if (parser->interfaceSize[i] != 0) pass = false;
		if (parser->parameterSize[i] != 0) pass = false;
	}

	if (!pass) {
		*validationLog << "\tFAILED: parser structures were not correctly configured (setupRIMParsing) " << endl;
	}
	return pass;
}

bool
RIMParserTest::loadSourceFilesTest()
{
	bool                     pass           = true;

	parser->loadSourceFiles();  // this calls configure for parsing
	// verify values set by setupRIMParsing()
	for (INTEGER i = 0; i < NUM_MODELS; i++) {
		string modelType  = getModelType((INFORMATION_TYPE)i) ;
		if (parser->modelParser[i] != NULL) {
			if (!(i != SIMULATION || i != BALANCE_AUTHS || i != ZONE ||
					i != CONVENTIONAL || i != LOAD_TIMESERIES ||
					i != WIND_TIMESERIES || i != SOLAR_TIMESERIES)) {
				*validationLog << "\tFAILED: parser " << modelType << " not set to NULL" << endl;
				pass = false;
			}
		}
		if (i == SIMULATION) {
			if (parser->interfaceSize[i] != sizeof(SimulationData)) {
				*validationLog << "\tFAILED: " << modelType << " model has incorrect interface size" << endl;
				pass = false;
			}
			if (parser->parameterSize[i] != sizeof(SimulationInfo)) {
				*validationLog << "\tFAILED: " << modelType << " model has incorrect parameter size" << endl;
				pass = false;
			}
		} else if (i == BALANCE_AUTHS) {
			if (parser->interfaceSize[i] != sizeof(BalanceAuthData)) {
				*validationLog << "\tFAILED: " << modelType << " model has incorrect interface size" << endl;
				pass = false;
			}
			if (parser->parameterSize[i] != sizeof(BalanceAuthParams)) {
				*validationLog << "\tFAILED: " << modelType << " model has incorrect parameter size" << endl;
				pass = false;
			}
		} else if (i == ZONE) {
			if (parser->interfaceSize[i] != sizeof(ZoneData)) {
				*validationLog << "\tFAILED: " << modelType << " model has incorrect interface size" << endl;
				pass = false;
			}
			if (parser->parameterSize[i] != sizeof(ZoneParams)) {
				*validationLog << "\tFAILED: " << modelType << " model has incorrect parameter size" << endl;
				pass = false;
			}
		} else if (i == CONVENTIONAL) {
			if (parser->interfaceSize[i] != sizeof(ConventionalData)) {
				*validationLog << "\tFAILED: " << modelType << " model has incorrect interface size" << endl;
				pass = false;
			}
			if (parser->parameterSize[i] != sizeof(ConventionalParams)) {
				*validationLog << "\tFAILED: " << modelType << " model has incorrect parameter size" << endl;
				pass = false;
			}
		} else if (i == LOAD_TIMESERIES) {
			if (parser->interfaceSize[i] != sizeof(LoadData)) {
				*validationLog << "\tFAILED: " << modelType << " model has incorrect interface size" << endl;
				pass = false;
			}
			if (parser->parameterSize[i] != sizeof(LoadParams)) {
				*validationLog << "\tFAILED: " << modelType << " model has incorrect parameter size" << endl;
				pass = false;
			}
		} else if (i == WIND_TIMESERIES) {
			if (parser->interfaceSize[i] != sizeof(WindData)) {
				*validationLog << "\tFAILED: " << modelType << " model has incorrect interface size" << endl;
				pass = false;
			}
			if (parser->parameterSize[i] != sizeof(WindParams)) {
				*validationLog << "\tFAILED: " << modelType << " model has incorrect parameter size" << endl;
				pass = false;
			}
		} else if (i == SOLAR_TIMESERIES) {
			if (parser->interfaceSize[i] != sizeof(SolarData)) {
				*validationLog << "\tFAILED: " << modelType << " model has incorrect interface size" << endl;
				pass = false;
			}
			if (parser->parameterSize[i] != sizeof(SolarParams)) {
				*validationLog << "\tFAILED: " << modelType << " model has incorrect parameter size" << endl;
				pass = false;
			}
		} else {

		}
	}

	if (!pass) {
		*validationLog << "\tFAILED: parser structures were not correctly configured (setupRIMParsing) " << endl;
	}
	// verify values

	return pass;
}

bool
RIMParserTest::collectModelInformationTest()
{
	bool                     pass           = true;

	parser->collectModelInformation();
	for (INTEGER i = 0; i < NUM_MODELS; i++) {
        if (i == SIMULATION) {
            if (parser->modelCount[i] != 4) {
                *validationLog << "\tFAILED: simulation count was wrong (" << parser->modelCount[i] << ")" << endl;
                pass = false;
            }
        } else if (i == BALANCE_AUTHS) {
            if (parser->modelCount[i] != 2) {
                *validationLog << "\tFAILED: ba count was wrong (" << parser->modelCount[i] << ")" << endl;
                pass = false;
            }
        } else if (i == ZONE) {
            if (parser->modelCount[i] != 5) {
                *validationLog << "\tFAILED: zone count was wrong (" << parser->modelCount[i] << ")" << endl;
                pass = false;
            }
        } else if (i == CONVENTIONAL) {
            if (parser->modelCount[i] != 20) {
                *validationLog << "\tFAILED: conventional count was wrong (" << parser->modelCount[i] << ")" << endl;
                pass = false;
            }
        } else if (i == LOAD_TIMESERIES) {
            if (parser->modelCount[i] != 20) {
                *validationLog << "\tFAILED: load count was wrong (" << parser->modelCount[i] << ")" << endl;
                pass = false;
            }
        } else if (i == WIND_TIMESERIES) {
            if (parser->modelCount[i] != 20) {
                *validationLog << "\tFAILED: wind count was wrong (" << parser->modelCount[i] << ")" << endl;
                pass = false;
            }
        } else if (i == SOLAR_TIMESERIES) {
            if (parser->modelCount[i] != 20) {
                *validationLog << "\tFAILED: solar count was wrong (" << parser->modelCount[i] << ")" << endl;
                pass = false;
            }
        } else {

        }
	}

	if (!pass) {
		*validationLog << "\tFAILED: parser structures were not correctly configured (setupRIMParsing) " << endl;
	}
	// verify values

	return pass;
}

bool
RIMParserTest::constructParameterSpaceTest()
{
	bool                     pass           = true;
	parser->constructParameterSpace();
	return pass;
}

bool
RIMParserTest::constructInterfaceSpaceTest()
{
	bool                     pass           = true;
	parser->constructInterfaceSpace();
	return pass;
}

bool
RIMParserTest::resolveDependenciesTest()
{
	bool                     failed             = false;
	parser->resolveDependenies();
//	SimulationInfo         * info               = parser->getSimInfo();
	BalanceAuthParams      * baParams           = parser->getBalanceAuthParams(0);
	ZoneParams             * zoneParams         = parser->getZoneParams(0);
//	ConventionalParams     * conventioaalParams = parser->getConventionalParams(0);
//	LoadParams             * loadParams         = parser->getLoadParams(0);
//	WindParams             * windParams         = parser->getWindParams(0);
//	SolarParams            * solarParams        = parser->getSolarParams(0);

	if (baParams[0].nZones != 3) {
		failed   = true;
		*validationLog << "\t\tFAIL (resolveDependenciesTest.positive) : n zones in BA[0] != 3 : " << baParams[0].nZones << endl;
	}

	if (baParams[1].nZones != 2) {
		failed   = true;
		*validationLog << "\t\tFAIL (resolveDependenciesTest.positive) : n zones in BA[0] != 2 : " << baParams[0].nZones << endl;
	}

	if (zoneParams[0].nLoads != 2) {
		failed   = true;
		*validationLog << "\t\tFAIL (resolveDependenciesTest.positive) : n loads in Zone[0] != 2 : " << zoneParams[0].nLoads << endl;
	}

	if (zoneParams[1].nLoads != 3) {
		failed   = true;
		*validationLog << "\t\tFAIL (resolveDependenciesTest.positive) : n loads in Zone[1] != 3 : " << zoneParams[1].nLoads << endl;
	}

	if (zoneParams[2].nLoads != 4) {
		failed   = true;
		*validationLog << "\t\tFAIL (resolveDependenciesTest.positive) : n loads in Zone[2] != 4 : " << zoneParams[2].nLoads << endl;
	}

	if (zoneParams[3].nLoads != 5) {
		failed   = true;
		*validationLog << "\t\tFAIL (resolveDependenciesTest.positive) : n loads in Zone[3] != 5 : " << zoneParams[3].nLoads<< endl;
	}

	if (zoneParams[4].nLoads != 6) {
		failed   = true;
		*validationLog << "\t\tFAIL (resolveDependenciesTest.positive) : n loads in Zone[4] != 6  : " << zoneParams[4].nLoads<< endl;
	}

	if (zoneParams[0].nWindFarms != 2) {
		failed   = true;
		*validationLog << "\t\tFAIL (resolveDependenciesTest.positive) : n wind farms in Zone[0] != 2 : " << zoneParams[0].nWindFarms << endl;
	}

	if (zoneParams[1].nWindFarms != 3) {
		failed   = true;
		*validationLog << "\t\tFAIL (resolveDependenciesTest.positive) : n wind farms in Zone[1] != 3 : " << zoneParams[1].nWindFarms << endl;
	}

	if (zoneParams[2].nWindFarms != 4) {
		failed   = true;
		*validationLog << "\t\tFAIL (resolveDependenciesTest.positive) : n wind farms in Zone[2] != 4 : " << zoneParams[2].nWindFarms << endl;
	}

	if (zoneParams[3].nWindFarms != 5) {
		failed   = true;
		*validationLog << "\t\tFAIL (resolveDependenciesTest.positive) : n wind farms in Zone[3] != 5 : " << zoneParams[3].nWindFarms << endl;
	}

	if (zoneParams[4].nWindFarms != 6) {
		failed   = true;
		*validationLog << "\t\tFAIL (resolveDependenciesTest.positive) : n wind farms in Zone[4] != 6 : " << zoneParams[4].nWindFarms << endl;
	}

	if (zoneParams[0].nSolarFarms != 2) {
		failed   = true;
		*validationLog << "\t\tFAIL (resolveDependenciesTest.positive) : n solar farms in Zone[0] != 2 : " << zoneParams[0].nSolarFarms << endl;
	}

	if (zoneParams[1].nSolarFarms != 3) {
		failed   = true;
		*validationLog << "\t\tFAIL (resolveDependenciesTest.positive) : n solar farms in Zone[1] != 3 : " << zoneParams[1].nSolarFarms << endl;
	}

	if (zoneParams[2].nSolarFarms != 4) {
		failed   = true;
		*validationLog << "\t\tFAIL (resolveDependenciesTest.positive) : n solar farms in Zone[2] != 4 : " << zoneParams[2].nSolarFarms << endl;
	}

	if (zoneParams[3].nSolarFarms != 5) {
		failed   = true;
		*validationLog << "\t\tFAIL (resolveDependenciesTest.positive) : n solar farms in Zone[3] != 5 : " << zoneParams[3].nSolarFarms << endl;
	}

	if (zoneParams[4].nSolarFarms != 6) {
		failed   = true;
		*validationLog << "\t\tFAIL (resolveDependenciesTest.positive) : n solar farms in Zone[4] != 6 : " << zoneParams[4].nSolarFarms << endl;
	}

	return failed;
}

} /* namespace testing */
