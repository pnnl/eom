/*
 * SolarParserTest.cpp
 *
 *  Created on: Sep 15, 2011
 *      Author: kglass
 */

#include "SolarParserTest.hpp"

namespace testing {

SolarParserTest::SolarParserTest() {}

SolarParserTest::~SolarParserTest() {}

void
SolarParserTest::constructSolarParserTest(const char * fileName, INTEGER nSolars)
{
	incrementTest();
	interpreter::SolarParser   * parser         =
			new interpreter::SolarParser(fileName);

	char                * parameters        = new char[sizeof(SolarParams)*nSolars];
	char                * interface         = new char[sizeof(SolarData)*nSolars];
	SolarParams          * loadParams        = (SolarParams *)parameters;
	SolarData            * loadData          = (SolarData *)interface;
	SimulationInfo      * info              = (SimulationInfo *)parameters;
	string                baseName          = "Solar ";
	string                srcName           = "testing/data/validSrc/time_series/testSolar";
	info->nSolarFarms   = nSolars;

	// load all the data in the selected file
	*validationLog << "\tSolar Parser Constructor" << endl;
	parser->setModels(parameters, interface, info);
	INTEGER             base                = 100;
	INTEGER             offset              = 1;
	INTEGER             failed              = false;

	for (INTEGER i = 0; i < nSolars; i++) {
		cout << "\tSolar " << i << endl;
		INTEGER              inc            = 0;
		offset          = base;
		stringstream          loadName;
		loadName  << baseName << i;

		if (loadName.str() != loadParams[i].name) {
			*validationLog << "\tFAILED: incorrect load name " << endl;
			failed      |= true;
		}

		if (srcName != loadParams[i].fileName) {
			*validationLog << "\tFAILED: incorrect load file name " << endl;
			failed      |= true;
		}
		if (loadParams[i].balancingAuth != offset + inc) {
			*validationLog << "\tFAILED: incorrect balancing authority " << endl;
			failed      |= true;
		}
		++inc;

		if (loadParams[i].zone != offset + inc) {
			*validationLog << "\tFAILED: incorrect zone " << endl;
			failed      |= true;
		}
		++inc;

		if (loadData[i].state.currentSolarPower != -1) {
			*validationLog << "\tFAILED: incorrect current load " << endl;
			failed      |= true;
		}

		if (loadData[i].output.outputSolarPower != -1) {
			*validationLog << "\tFAILED: incorrect output load " << endl;
			failed      |= true;
		}

		base *= 10;
	}

	if(!failed) {
		*validationLog << "\tPASS (resetParserTest.positive) : reset passed" << endl;
	} else {
		incrementFailure();
		*validationLog << "\tFAIL (resetParserTest.positive) : reset failed" << endl;
	}
	// verify the correct load
}

void
SolarParserTest::deconstructSolarParserTest(const char * fileName)
{
}

void
SolarParserTest::setModelsTest(char * parameters, char * interface, SimulationInfo * info)
{
}

void
SolarParserTest::loadSolarTest(INTEGER i, char * parameters, char * interface)
{
}

void
SolarParserTest::setDerivedDataTest(char * parameters, char * interface, SimulationInfo * info)
{
}

void
SolarParserTest::getCountTest()
{
}

} /* namespace testing */
