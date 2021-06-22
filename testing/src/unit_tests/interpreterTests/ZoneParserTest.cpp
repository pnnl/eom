/*
 * ZoneParserTest.cpp
 *
 *  Created on: Sep 14, 2011
 *      Author: kglass
 */

#include "ZoneParserTest.hpp"

namespace testing {

ZoneParserTest::ZoneParserTest() {};

ZoneParserTest::~ZoneParserTest() {};


void
ZoneParserTest::constructZoneParserTest(const char * fileName, INTEGER nZones)
{
	incrementTest();
	interpreter::ZoneParser        * parser         =
			new interpreter::ZoneParser(fileName);

	char * parameters = new char[sizeof(ZoneParams)*nZones];
	char * interface  = new char[sizeof(ZoneData)*nZones];
	ZoneParams          * zoneParams = (ZoneParams *)parameters;
	ZoneData            * zoneData   = (ZoneData *)interface;
	string                baseName          = "Zone ";
	char                * simInfo           = new char[sizeof(SimulationInfo)];
	SimulationInfo      * info              = (SimulationInfo *)simInfo;

	info->nZones        = nZones;

	// load all the data in the selected file
	*validationLog << "\tZone Parser Constructor" << endl;
	parser->setModels(parameters, interface, info);
	INTEGER             base                = 100;
	INTEGER             offset              = 0;
	INTEGER             failed              = false;

	for (INTEGER i = 0; i < nZones; i++) {
		cout << "\tZone " << i << endl;
		INTEGER              inc            = 0;
		offset          = base;
		stringstream          loadName;
		loadName  << baseName << i;

		if (loadName.str() != zoneParams[i].name) {
			*validationLog << "\tFAILED: incorrect load name " << endl;
			failed      |= true;
		}

		double               delta          = 0.0;

		delta           = zoneParams[i].noxCost - (offset + inc);
		delta           = delta > 0 ? delta :  -delta;
		if (delta > 0.00000001) {
			*validationLog << "\tFAILED: incorrect noxCost " << endl;
			failed      |= true;
		}
		++inc;

		delta           = zoneParams[i].soxCost - (offset + inc);
		delta           = delta > 0 ? delta :  -delta;
		if (delta > 0.00000001) {
			*validationLog << "\tFAILED: incorrect soxCost " << endl;
			failed      |= true;
		}
		++inc;

		delta           = zoneParams[i].co2Cost - (offset + inc);
		delta           = delta > 0 ? delta :  -delta;
		if (delta > 0.00000001) {
			*validationLog << "\tFAILED: incorrect co2Cost " << endl;
			failed      |= true;
		}
		++inc;

		delta           = zoneParams[i].balancingAuthIndex - (offset + inc);
		delta           = delta > 0 ? delta :  -delta;
		if (delta > 0.00000001) {
			*validationLog << "\tFAILED: incorrect balancingAuthIndex " << endl;
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
ZoneParserTest::deconstructZoneParserTest(const char * fileName)
{
}

void
ZoneParserTest::setModelsTest(char * parameters, char * interface, SimulationInfo * info)
{
}

void
ZoneParserTest::loadZoneTwst(INTEGER i, char * parameters, char * interface)
{
}

void
ZoneParserTest::setDerivedDataTest(char * parameters, char * interface, SimulationInfo * info)
{
}

void
ZoneParserTest::getCountTest()
{
}


} /* namespace testing */
