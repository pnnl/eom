/*
 * LoadParserTest.cpp
 *
 *  Created on: Sep 15, 2011
 *      Author: kglass
 */

#include "LoadParserTest.hpp"

namespace testing {

LoadParserTest::LoadParserTest() {}

LoadParserTest::~LoadParserTest() {}

void
LoadParserTest::constructLoadParserTest(const char * fileName, INTEGER nLoads)
{
	incrementTest();
	interpreter::LoadParser   * parser         =
			new interpreter::LoadParser(fileName);

	char                * parameters        = new char[sizeof(LoadParams)*nLoads];
	char                * interface         = new char[sizeof(LoadData)*nLoads];
	LoadParams          * loadParams        = (LoadParams *)parameters;
	LoadData            * loadData          = (LoadData *)interface;
	SimulationInfo      * info              = (SimulationInfo *)parameters;
	string                baseName          = "Load ";
	string                srcName           = "testing/data/validSrc/time_series/testLoad";
	info->nLoads        = nLoads;

	// load all the data in the selected file
	*validationLog << "\tLoad Parser Constructor" << endl;
	parser->setModels(parameters, interface, info);
	INTEGER             base                = 100;
	INTEGER             offset              = 1;
	INTEGER             failed              = false;

	for (INTEGER i = 0; i < nLoads; i++) {
		cout << "\tLoad " << i << endl;
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

		if (loadData[i].state.currentLoad != -1) {
			*validationLog << "\tFAILED: incorrect current load " << endl;
			failed      |= true;
		}

		if (loadData[i].output.outputLoad != -1) {
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
LoadParserTest::deconstructLoadParserTest(const char * fileName)
{
}

void
LoadParserTest::setModelsTest(char * parameters, char * interface, SimulationInfo * info)
{
}

void
LoadParserTest::loadLoadTest(INTEGER i, char * parameters, char * interface)
{
}

void
LoadParserTest::setDerivedDataTest(char * parameters, char * interface, SimulationInfo * info)
{
}

void
LoadParserTest::getCountTest()
{
}

} /* namespace testing */
