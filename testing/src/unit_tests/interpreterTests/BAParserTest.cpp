/*
 * BAParserTest.cpp
 *
 *  Created on: Sep 13, 2011
 *      Author: kglass
 */

#include "BAParserTest.hpp"

namespace testing {

BAParserTest::BAParserTest() {}

BAParserTest::~BAParserTest() {}

void
BAParserTest::constructBAParserTest(const char * fileName, INTEGER nBAs)
{
	incrementTest();
	interpreter::BAParser        * parser         =
			new interpreter::BAParser(fileName);

	char * parameters = new char[sizeof(BalanceAuthParams)*nBAs];
	char * interface  = new char[sizeof(BalanceAuthData)*nBAs];
	string                baseName          = "BA ";
	BalanceAuthParams   * baParams = (BalanceAuthParams *)parameters;
	BalanceAuthData     * baData   = (BalanceAuthData *)interface;
	char                * simInfo           = new char[sizeof(SimulationInfo)];
	SimulationInfo      * info              = (SimulationInfo *)simInfo;

	info->nBalancingAuths = nBAs;

	// load all the data in the selected file
	*validationLog << "\tBalancing Authority Parser Constructor" << endl;
	parser->setModels(parameters, interface, info);
	INTEGER             base                = 100;
	INTEGER             offset              = 0;
	INTEGER             failed              = false;

	for (INTEGER i = 0; i < nBAs; i++) {
		cout << "\tBalancing Authority " << i << endl;
		INTEGER              inc            = 0;
		offset          = base;
		stringstream          loadName;
		loadName  << baseName << i;

		if (loadName.str() != baParams[i].name) {
			*validationLog << "\tFAILED: incorrect load name " << endl;
			failed      = true;
		}

		double               delta          = 0.0;

		delta           = baParams[i].cps1 - (offset + inc);
		delta           = delta > 0 ? delta :  -delta;
		if (delta > 0.00000001) {
			*validationLog << "\tFAILED: incorrect cps1 " << endl;
			failed      = true;
		}
		++inc;

		delta           = baParams[i].cps2 - (offset + inc);
		delta           = delta > 0 ? delta :  -delta;
		if (delta > 0.00000001) {
			*validationLog << "\tFAILED: incorrect cps2 " << endl;
			failed      = true;
		}
		++inc;

		delta           = baParams[i].bias - (offset + inc);
		delta           = delta > 0 ? delta :  -delta;
		if (delta > 0.00000001) {
			*validationLog << "\tFAILED: incorrect bias " << endl;
			failed      = true;
		}
		++inc;

		delta           = baParams[i].agcLower - (offset + inc);
		delta           = delta > 0 ? delta :  -delta;
		if (delta > 0.00000001) {
			*validationLog << "\tFAILED: incorrect agcLower " << endl;
			failed      = true;
		}
		++inc;

		delta           = baParams[i].agcUpper - (offset + inc);
		delta           = delta > 0 ? delta :  -delta;
		if (delta > 0.00000001) {
			*validationLog << "\tFAILED: incorrect agcUpper " << endl;
			failed      = true;
		}
		++inc;

		if (baParams[i].nZones != 0) {
			*validationLog << "\tFAILED: incorrect zone number " << endl;
			failed      = true;
		}
		++inc;

		delta           = baData[i].state.currentPower;
		delta           = delta > 0 ? delta :  -delta;
		if (delta > 0.00000001) {
			*validationLog << "\tFAILED: incorrect state.currentPower " << endl;
			failed      = true;
		}
		++inc;

		delta           = baData[i].state.currentLoad;
		delta           = delta > 0 ? delta :  -delta;
		if (delta > 0.00000001) {
			*validationLog << "\tFAILED: incorrect state.currentLoad " << endl;
			failed      = true;
		}
		++inc;

		delta           = baData[i].output.outputPower;
		delta           = delta > 0 ? delta :  -delta;
		if (delta > 0.00000001) {
			*validationLog << "\tFAILED: incorrect output.outputPower " << endl;
			failed      = true;
		}
		++inc;

		delta           = baData[i].output.outputLoad;
		delta           = delta > 0 ? delta :  -delta;
		if (delta > 0.00000001) {
			*validationLog << "\tFAILED: incorrect output.outputLoad " << endl;
			failed      = true;
		}
		++inc;

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
BAParserTest::deconstructBAParserTest(const char * fileName)
{
}

void
BAParserTest::setModelsTest(char * parameters, char * interface, SimulationInfo * info)
{
}

void
BAParserTest::loadBATwst(INTEGER i, char * parameters, char * interface)
{
}

void
BAParserTest::setDerivedDataTest(char * parameters, char * interface, SimulationInfo * info)
{
}

void
BAParserTest::getCountTest()
{
}

} /* namespace testing */
