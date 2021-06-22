/*
 * GeneratorParserTest.cpp
 *
 *  Created on: Aug 25, 2011
 *      Author: kglass
 */


#include "GeneratorParserTest.hpp"

namespace testing {

GeneratorParserTest::GeneratorParserTest()
{
}

GeneratorParserTest::~GeneratorParserTest()
{
}

void
GeneratorParserTest::constructGeneratorParserTest(const char * fileName, INTEGER nGenerators)
{
	incrementTest();
	interpreter::GeneratorParser        * parser         =
			new interpreter::GeneratorParser(fileName);

	char                * parameters        = new char[sizeof(ConventionalParams)*nGenerators];
	char                * interface         = new char[sizeof(ConventionalData)*nGenerators];
	ConventionalParams  * genParams         = (ConventionalParams *)parameters;
	ConventionalData    * genData           = (ConventionalData *)interface;
	char                * simInfo           = new char[sizeof(SimulationInfo)];
	string                baseName          = "Generator ";
	SimulationInfo      * info              = (SimulationInfo *)simInfo;

	info->nGenerators             = nGenerators;
	info->nConventionalGenerators = nGenerators;

	// load all the data in the selected file
	*validationLog << "\tGenerator Parser Constructor" << endl;
	parser->setModels(parameters, interface, info);
	INTEGER             base                = 200;
	INTEGER             offset              = 1;
	INTEGER             failed              = false;

	for (INTEGER i = 0; i < nGenerators; i++) {
		cout << "\tGenerator " << i << endl;
		INTEGER              inc            = 0;
		offset          = base + 1;
		stringstream          loadName;
		loadName  << baseName << i;

		if (loadName.str() != genParams[i].genParams.name) {
			*validationLog << "\tFAILED: incorrect load name " << endl;
			failed      |= true;
		}
		for (INTEGER j = 0; j < genParams[i].genParams.heatRateCurve.nSegments + 1; j++) {
			if (genParams[i].genParams.heatRateCurve.segment[j].startPoint[INDEPENDENT] != offset + inc) {
				*validationLog << "\tFAILED: incorrect value for cost curve, segment " << endl;
				failed      |= true;
			}
			++inc;
			if (genParams[i].genParams.heatRateCurve.segment[j].startPoint[DEPENDENT] != offset + inc) {
				*validationLog << "\tFAILED: incorrect value for cost curve, segment " << endl;
				failed      |= true;
			}
			++inc;
			if (genParams[i].genParams.heatRateCurve.segment[j].endPoint[INDEPENDENT] != offset + inc) {
				*validationLog << "\tFAILED: incorrect value for cost curve, segment " << endl;
				failed      |= true;
			}
			++inc;
			if (genParams[i].genParams.heatRateCurve.segment[j].endPoint[DEPENDENT] != offset + inc) {
				*validationLog << "\tFAILED: incorrect value for cost curve, segment " << endl;
				failed      |= true;
			}
			++inc;
		}

		if (genParams[i].genParams.genRampRates[DOWN] != offset + inc) {
			*validationLog << "\tFAILED: incorrect ramp down rate " << endl;
			failed      |= true;
		}
		++inc;

		if (genParams[i].genParams.genRampRates[UP] != offset + inc) {
			*validationLog << "\tFAILED: incorrect ramp up rate " << endl;
			failed      |= true;
		}
		++inc;

		if (genParams[i].genTransitionCost[DOWN] != offset + inc) {
			*validationLog << "\tFAILED: incorrect shutdown cost " << endl;
			failed      |= true;
		}
		++inc;

		if (genParams[i].genTransitionCost[UP] != offset + inc) {
			*validationLog << "\tFAILED: incorrect startup cost " << endl;
			failed      |= true;
		}
		++inc;

		if (genParams[i].genParams.balancingAuth != offset + inc) {
			*validationLog << "\tFAILED: incorrect balancing authority " << endl;
			failed      |= true;
		}
		++inc;

		if (genParams[i].genParams.zone != offset + inc) {
			*validationLog << "\tFAILED: incorrect zone " << endl;
			failed      |= true;
		}
		++inc;

		if (i == 0 ) {
			if (genParams[i].genParams.fuelType != NUCLEAR) {
				*validationLog << "\tFAILED: incorrect fuel type " << endl;
				failed      |= true;
			}
		} else if (i == 1) {
			if (genParams[i].genParams.fuelType != COAL) {
				*validationLog << "\tFAILED: incorrect fuel type " << endl;
				failed      |= true;
			}
		} else {
			if (genParams[i].genParams.fuelType != NATURALGAS) {
				*validationLog << "\tFAILED: incorrect fuel type " << endl;
				failed      |= true;
			}
		}

		if (genParams[i].genParams.fuelCost != offset + inc) {
			*validationLog << "\tFAILED: incorrect fuel cost " << endl;
			failed      |= true;
		}
		++inc;

		if (i == 0 || i == 2) {
			if (genParams[i].genParams.canCommit) {
				*validationLog << "\tFAILED: incorrect commit state " << endl;
				failed      |= true;
			}

			if (!genParams[i].genParams.canLoadFollow) {
				*validationLog << "\tFAILED: incorrect load following state " << endl;
				failed      |= true;
			}

			if (genParams[i].genParams.canRegulate) {
				*validationLog << "\tFAILED: incorrect regulation state " << endl;
				failed      |= true;
			}
		} else {
			if (!genParams[i].genParams.canCommit) {
				*validationLog << "\tFAILED: incorrect commit state " << endl;
				failed      |= true;
			}

			if (genParams[i].genParams.canLoadFollow) {
				*validationLog << "\tFAILED: incorrect load following state " << endl;
				failed      |= true;
			}

			if (!genParams[i].genParams.canRegulate) {
				*validationLog << "\tFAILED: incorrect regulation state " << endl;
				failed      |= true;
			}
		}

		if (genParams[i].minTimeInState[OFF_STATE] != offset + inc) {
			*validationLog << "\tFAILED: incorrect time in off state " << endl;
			failed      |= true;
		}
		++inc;

		if (genParams[i].minTimeInState[ON_STATE] != offset + inc) {
			*validationLog << "\tFAILED: incorrect time in on state " << endl;
			failed      |= true;
		}
		++inc;

		if (genParams[i].genParams.noxRate != offset + inc) {
			*validationLog << "\tFAILED: incorrect nox rate " << endl;
			failed      |= true;
		}
		++inc;

		if (genParams[i].genParams.soxRate != offset + inc) {
			*validationLog << "\tFAILED: incorrect sox rate " << endl;
			failed      |= true;
		}
		++inc;

		if (genParams[i].genParams.co2Rate != offset + inc) {
			*validationLog << "\tFAILED: incorrect co2 rate " << endl;
			failed      |= true;
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
GeneratorParserTest::deconstructGeneratorParserTest(const char * fileName)
{
}

void
GeneratorParserTest::setModelsTest(char * parameters, char * interface, SimulationInfo * info)
{
}

void
GeneratorParserTest::loadGeneratorTest(INTEGER i, char * parameters, char * interface)
{
}

void
GeneratorParserTest::setDerivedDataTest(char * parameters, char * interface, SimulationInfo * info)
{
}

void
GeneratorParserTest::getCountTest()
{
}

} /* namespace testing */
