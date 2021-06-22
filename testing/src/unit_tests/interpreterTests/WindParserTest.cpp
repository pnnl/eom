/*
 * WindParserTest.cpp
 *
 *  Created on: Sep 15, 2011
 *      Author: kglass
 */

#include "WindParserTest.hpp"

namespace testing {

WindParserTest::WindParserTest() {
	// TODO Auto-generated constructor stub

}

WindParserTest::~WindParserTest() {
	// TODO Auto-generated destructor stub
}

void
WindParserTest::constructWindParserTest(const char * fileName, INTEGER nWinds)
{
	incrementTest();
	interpreter::WindParser   * parser         =
			new interpreter::WindParser(fileName);

	char                * parameters        = new char[sizeof(WindParams)*nWinds];
	char                * interface         = new char[sizeof(WindData)*nWinds];
	WindParams          * loadParams        = (WindParams *)parameters;
	WindData            * loadData          = (WindData *)interface;
	SimulationInfo      * info              = (SimulationInfo *)parameters;
	string                baseName          = "Wind ";
	string                srcName           = "testing/data/validSrc/time_series/testWind";
	info->nWindFarms    = nWinds;

	// load all the data in the selected file
	*validationLog << "\tWind Parser Constructor" << endl;
	parser->setModels(parameters, interface, info);
	INTEGER             base                = 100;
	INTEGER             offset              = 1;
	INTEGER             failed              = false;

	for (INTEGER i = 0; i < nWinds; i++) {
		cout << "\tWind " << i << endl;
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

		if (loadData[i].state.currentWindPower != -1) {
			*validationLog << "\tFAILED: incorrect current load " << endl;
			failed      |= true;
		}

		if (loadData[i].output.outputWindPower != -1) {
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
WindParserTest::deconstructWindParserTest(const char * fileName)
{
}

void
WindParserTest::setModelsTest(char * parameters, char * interface, SimulationInfo * info)
{
}

void
WindParserTest::loadWindTest(INTEGER i, char * parameters, char * interface)
{
}

void
WindParserTest::setDerivedDataTest(char * parameters, char * interface, SimulationInfo * info)
{
}

void
WindParserTest::getCountTest()
{
}

} /* namespace testing */
