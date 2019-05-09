#include "structures.hpp"
#include "models/RIMModel.hpp"
#include "models/timeSeries/AdjLoadTS.hpp"
#include "FileControl.hpp"

using std::string;
using std::vector;
using std::ostream;

namespace model {

/*
 * The AdjLoadTS object has to translate its Zone position within the BA to
 * access its initialization data
 */
AdjLoadTS::AdjLoadTS(INTEGER zoneID, const SimulationInfo &simulationInfo) :
	ucLoadFile(NULL),
	ucWindFile(NULL),
	ucSolarFile(NULL),
	ucLoadData(NULL),
	ucWindData(NULL),
	ucSolarData(NULL),
	ucAdjSeries(NULL),
	ucSeriesLength(simulationInfo.ucLength),

	lfLoadFile(NULL),
	lfWindFile(NULL),
	lfSolarFile(NULL),
	lfLoadData(NULL),
	lfWindData(NULL),
	lfSolarData(NULL),
	lfAdjSeries(NULL),
	lfSeriesLength(simulationInfo.lfLength),

	regLoadFile(NULL),
	regWindFile(NULL),
	regSolarFile(NULL),
	regLoadData(NULL),
	regWindData(NULL),
	regSolarData(NULL),
	regAdjSeries(NULL),
	regSeriesLength(simulationInfo.regTimeStep), // TODO verify; this looks odd

	nWindFarms(0),
	nSolarFarms(0),
	circularBuffer(simulationInfo.circularBuffer),
	daStep(simulationInfo.daStep),
	regTimeStep(simulationInfo.regTimeStep),
	lfTimeStep(simulationInfo.lfTimeStep),
	ucTimeStep(simulationInfo.ucTimeStep),
	nForecastDays(simulationInfo.forecastDays)
,	lookAheadDays(simulationInfo.ucLength / HOURS_IN_DAY),
	implementsLoadFollowing(simulationInfo.implementsLoadFollowing),
	implementsRegulation(simulationInfo.implementsRegulation),
	zoneId(zoneID)
{
	interpreter::RIMParser * parser       = interpreter::RIMParser::getRIMParser();
	ZoneParams    * params = parser->getZoneParams(zoneID);
	nWindFarms             = params->nWindFarms;
	nSolarFarms            = params->nSolarFarms;

	setupUCAdjTS();
	if (implementsLoadFollowing) {
		setupLFAdjTS();
	}
	if (implementsRegulation) {
		setupRegAdjTS();
	}
}

AdjLoadTS::~AdjLoadTS()
{
	// there is a single load per TS and multiple negative loads
	utility::FileControl::closeFile(ucLoadFile);
	utility::FileControl::closeFile(lfLoadFile);
	utility::FileControl::closeFile(regLoadFile);

	delete [] ucAdjSeries;
	if(nSolarFarms != 0) {
		for (INTEGER solar = 0; solar < nSolarFarms; solar++) {
			utility::FileControl::closeFile(ucSolarFile[solar]);
		}
		delete [] ucSolarFile;
		delete [] ucSolarData;

		if (implementsLoadFollowing) {
			for (INTEGER solar = 0; solar < nSolarFarms; solar++) {
				utility::FileControl::closeFile(lfSolarFile[solar]);
			}
			delete [] lfSolarFile;
			delete [] lfSolarData;
		}
		if (implementsRegulation) {
			for (INTEGER solar = 0; solar < nSolarFarms; solar++) {
				utility::FileControl::closeFile(regSolarFile[solar]);
			}
			delete [] regSolarFile;
			delete [] regSolarData;
		}

	}
	if(nWindFarms != 0) {
		for (INTEGER wind = 0; wind < nWindFarms; wind++) {
			utility::FileControl::closeFile(ucWindFile[wind]);
			utility::FileControl::closeFile(lfWindFile[wind]);
			utility::FileControl::closeFile(regWindFile[wind]);
		}
		delete [] ucWindData;
		delete [] ucWindFile;

		if (implementsLoadFollowing) {
			delete [] lfWindData;
			delete [] lfAdjSeries;
			delete [] lfWindFile;
		}

		if (implementsRegulation) {
			delete [] regWindData;
			delete [] regWindFile;
			delete [] regAdjSeries;
		}
	}
};

void
AdjLoadTS::setupUCAdjTS()
{
	ucAdjSeries         = new FLOAT[ucSeriesLength];
	for (INTEGER len = 0; len < ucSeriesLength; len++) {
		ucAdjSeries[len]   = 0.0;
	}

	openUCLoadFile();
	for (size_t blocks = 0; blocks < nForecastDays; blocks++) {
		utility::FileControl::incrementalLoad(ucLoadFile);
	}
	utility::FileControl::incrementalXferBlocks(ucLoadFile);

	if (nWindFarms != 0) {
		openUCWindFile();
		for (size_t blocks = 0; blocks < nForecastDays; blocks++) {
			for (INTEGER wind = 0; wind < nWindFarms; wind++) {
				utility::FileControl::incrementalLoad(ucWindFile[wind]);
			}
		}
		for (INTEGER wind = 0; wind < nWindFarms; wind++) {
			utility::FileControl::incrementalXferBlocks(ucWindFile[wind]);
		}
	}

	if (nSolarFarms != 0) {
		openUCSolarFile();
		for (size_t blocks = 0; blocks < nForecastDays; blocks++) {
			for (INTEGER solar = 0; solar < nSolarFarms; solar++) {
				utility::FileControl::incrementalLoad(ucSolarFile[solar]);
			}
		}
		for (INTEGER solar = 0; solar < nSolarFarms; solar++) {
			utility::FileControl::incrementalXferBlocks(ucSolarFile[solar]);
		}
	}
}

void
AdjLoadTS::setupLFAdjTS()
{
	lfAdjSeries         = new FLOAT[lfSeriesLength];
	for (INTEGER len = 0; len < lfSeriesLength; len++) {
		lfAdjSeries[len]   = 0.0;
	}

	openLFLoadFile();
	if (nWindFarms != 0) {
		openLFWindFile();
	}
	if (nSolarFarms != 0) {
		openLFSolarFile();
	}
	for (size_t blocks = 0; blocks < nForecastDays; blocks++) {
		utility::FileControl::incrementalLoad(lfLoadFile);

		for (INTEGER wind = 0; wind < nWindFarms; wind++) {
			utility::FileControl::incrementalLoad(lfWindFile[wind]);
		}

		for (INTEGER solar = 0; solar < nSolarFarms; solar++) {
			utility::FileControl::incrementalLoad(lfSolarFile[solar]);
		}
	}

	utility::FileControl::incrementalXferBlocks(lfLoadFile);
	for (INTEGER wind = 0; wind < nWindFarms; wind++) {
		utility::FileControl::incrementalXferBlocks(lfWindFile[wind]);
	}

	for (INTEGER solar = 0; solar < nSolarFarms; solar++) {
		utility::FileControl::incrementalXferBlocks(lfSolarFile[solar]);
	}
}

void
AdjLoadTS::setupRegAdjTS()
{
	regAdjSeries        = new FLOAT[regSeriesLength];
	for (INTEGER len = 0; len < regSeriesLength; len++) {
		regAdjSeries[len]  = 0.0;
	}
	openRegLoadFile();
	if (nWindFarms != 0) {
		openRegWindFile();
	}
	if (nSolarFarms != 0) {
		openRegSolarFile();
	}
	for (size_t blocks = 0; blocks < nForecastDays; blocks++) {
		utility::FileControl::incrementalLoad(regLoadFile);

		for (INTEGER wind = 0; wind < nWindFarms; wind++) {
			utility::FileControl::incrementalLoad(regWindFile[wind]);
		}

		for (INTEGER solar = 0; solar < nSolarFarms; solar++) {
			utility::FileControl::incrementalLoad(regSolarFile[solar]);
		}
	}

	utility::FileControl::incrementalXferBlocks(regLoadFile);
	for (INTEGER wind = 0; wind < nWindFarms; wind++) {
		utility::FileControl::incrementalXferBlocks(regWindFile[wind]);
	}

	for (INTEGER solar = 0; solar < nSolarFarms; solar++) {
		utility::FileControl::incrementalXferBlocks(regSolarFile[solar]);
	}
}

void
AdjLoadTS::openUCLoadFile()
{
	interpreter::RIMParser * parser       = interpreter::RIMParser::getRIMParser();
	const vector<size_t> &   zoneLoad       = parser->getZoneLoadSet(zoneId);
	INTEGER                  zoneLoadIndex  = zoneLoad.at(0);
	INTEGER                  ucBlockSize    = (daStep/HOURS_IN_DAY) * MINUTES_IN_DAY*sizeof(FLOAT)/ucTimeStep;

	string                   fullName;

	LoadParams             * params     = parser->getLoadParams(zoneLoadIndex);

	fullName        = params->fileName;
	fullName        += ".uc";
	ucLoadFile      = utility::FileControl::openIncremental(fullName.c_str(),
			NULL, circularBuffer, ucBlockSize, lookAheadDays);
	ucLoadData      = (FLOAT *)ucLoadFile->outputBuffer;
}

void
AdjLoadTS::openLFLoadFile()
{
	interpreter::RIMParser * parser       = interpreter::RIMParser::getRIMParser();

	const vector<size_t> &   zoneLoad       = parser->getZoneLoadSet(zoneId);
	INTEGER                  zoneLoadIndex  = zoneLoad[zoneId];
	INTEGER                  lfBlockSize      = MINUTES_IN_DAY*sizeof(FLOAT)/lfTimeStep;
	string                   fullName;

	LoadParams             * params     = parser->getLoadParams(zoneLoadIndex);

	fullName        = params->fileName;
	fullName       += ".lf";
	lfLoadFile      = utility::FileControl::openIncremental(fullName.c_str(),
			NULL, circularBuffer, lfBlockSize, lookAheadDays);
	lfLoadData      = (FLOAT *)lfLoadFile->outputBuffer;
}

void
AdjLoadTS::openRegLoadFile()
{
	interpreter::RIMParser * parser       = interpreter::RIMParser::getRIMParser();

	const vector<size_t> &   zoneLoad       = parser->getZoneLoadSet(zoneId);
	INTEGER                  zoneLoadIndex  = zoneLoad[zoneId];
	INTEGER                  regBlockSize     = MINUTES_IN_DAY*sizeof(FLOAT)/regTimeStep;

	string                   fullName;

	LoadParams             * params     = parser->getLoadParams(zoneLoadIndex);

	fullName        = params->fileName;
	fullName       += ".reg";
	regLoadFile     = utility::FileControl::openIncremental(fullName.c_str(),
			NULL, circularBuffer, regBlockSize, lookAheadDays);
	regLoadData     = (FLOAT *)regLoadFile->outputBuffer;
}

void
AdjLoadTS::openUCWindFile()
{
	interpreter::RIMParser * parser       = interpreter::RIMParser::getRIMParser();

	INTEGER                  ucBlockSize      = MINUTES_IN_DAY*sizeof(FLOAT)/ucTimeStep;
	string                   fullName;
	ZoneParams             * zoneParams     = parser->getZoneParams(zoneId);

	nWindFarms                              = zoneParams->nWindFarms;

	ucWindFile     = new FileData*[nWindFarms];
	ucWindData     = new FLOAT*[nWindFarms];
	for (INTEGER wind = 0; wind < nWindFarms; wind++) {
		const vector<size_t> &   zoneWind       = parser->getZoneWindSet(zoneId);
		INTEGER                  zoneWindIndex  = zoneWind[0];
		WindParams             * params           = parser->getWindParams(zoneWindIndex);

		fullName            = params->fileName;
		fullName           += ".uc";
		ucWindFile[wind]    = utility::FileControl::openIncremental(fullName.c_str(),
				NULL, circularBuffer, ucBlockSize, lookAheadDays);
		ucWindData[wind]    = (FLOAT *)ucWindFile[wind]->outputBuffer;
	}
}

void
AdjLoadTS::openLFWindFile()
{
	interpreter::RIMParser * parser       = interpreter::RIMParser::getRIMParser();

	INTEGER                  lfBlockSize      = MINUTES_IN_DAY*sizeof(FLOAT)/lfTimeStep;

	string                   fullName;

	ZoneParams             * zoneParams     = parser->getZoneParams(zoneId);
	nWindFarms                              = zoneParams->nWindFarms;

	lfWindFile     = new FileData*[nWindFarms];
	lfWindData     = new FLOAT*[nWindFarms];
	for (INTEGER wind = 0; wind < nWindFarms; wind++) {
		const vector<size_t> &   zoneWind       = parser->getZoneWindSet(zoneId);
		INTEGER                  zoneWindIndex  = zoneWind[0];
		WindParams             * params         = parser->getWindParams(zoneWindIndex);

		fullName            = params->fileName;
		fullName           += ".lf";
		lfWindFile[wind]    = utility::FileControl::openIncremental(fullName.c_str(),
				NULL, circularBuffer, lfBlockSize, lookAheadDays);
		lfWindData[wind]    = (FLOAT *)lfWindFile[wind]->outputBuffer;
	}
}

void
AdjLoadTS::openRegWindFile()
{
	interpreter::RIMParser * parser       = interpreter::RIMParser::getRIMParser();

	INTEGER                  regBlockSize   = MINUTES_IN_DAY*sizeof(FLOAT)/regTimeStep;

	string                   fullName;

	ZoneParams             * zoneParams     = parser->getZoneParams(zoneId);
	nWindFarms                              = zoneParams->nWindFarms;

	regWindFile    = new FileData*[nWindFarms];
	regWindData    = new FLOAT*[nWindFarms];
	for (INTEGER wind = 0; wind < nWindFarms; wind++) {
		const vector<size_t> &   zoneWind       = parser->getZoneWindSet(zoneId);
		INTEGER                  zoneWindIndex  = zoneWind[0];
		WindParams             * params           = parser->getWindParams(zoneWindIndex);

		fullName            = params->fileName;
		fullName           += ".reg";
		regWindFile[wind]   = utility::FileControl::openIncremental(fullName.c_str(),
				NULL, circularBuffer, regBlockSize, lookAheadDays);
		regWindData[wind]   = (FLOAT *)regWindFile[wind]->outputBuffer;
	}
}

void
AdjLoadTS::openUCSolarFile()
{
	interpreter::RIMParser * parser       = interpreter::RIMParser::getRIMParser();

	INTEGER                  ucBlockSize    = MINUTES_IN_DAY*sizeof(FLOAT)/ucTimeStep;
	string                   fullName;

	ZoneParams             * zoneParams     = parser->getZoneParams(zoneId);
	nSolarFarms     = zoneParams->nSolarFarms;

	ucSolarFile     = new FileData*[nSolarFarms];
	ucSolarData     = new FLOAT*[nSolarFarms];

	for (INTEGER solar = 0; solar < nSolarFarms; solar++) {
		const vector<size_t> &   zoneSolar      = parser->getZoneSolarSet(zoneId);
		INTEGER                  zoneSolarIndex = zoneSolar[0];
		SolarParams            * params     = parser->getSolarParams(zoneSolarIndex);

		fullName              = params->fileName;
		fullName             += ".uc";
		ucSolarFile[solar]    = utility::FileControl::openIncremental(fullName.c_str(),
				NULL, circularBuffer, ucBlockSize, lookAheadDays);
		ucSolarData[solar]    = (FLOAT *)ucSolarFile[solar]->outputBuffer;
	}
}

void
AdjLoadTS::openLFSolarFile()
{
	interpreter::RIMParser * parser       = interpreter::RIMParser::getRIMParser();

	INTEGER                  lfBlockSize    = MINUTES_IN_DAY*sizeof(FLOAT)/lfTimeStep;
	string                   fullName;

	ZoneParams             * zoneParams     = parser->getZoneParams(zoneId);
	nSolarFarms     = zoneParams->nWindFarms;

	lfSolarFile    = new FileData*[nSolarFarms];
	lfSolarData    = new FLOAT*[nSolarFarms];

	for (INTEGER solar = 0; solar < nSolarFarms; solar++) {
		const vector<size_t> &   zoneSolar      = parser->getZoneWindSet(zoneId);
		INTEGER                  zoneSolarIndex = zoneSolar[0];
		SolarParams            * params     = parser->getSolarParams(zoneSolarIndex);

		fullName              = params->fileName;
		fullName             += ".lf";
		lfSolarFile[solar]    = utility::FileControl::openIncremental(fullName.c_str(),
				NULL, circularBuffer, lfBlockSize, lookAheadDays);
		lfSolarData[solar]    = (FLOAT *)lfSolarFile[solar]->outputBuffer;
	}
}

void
AdjLoadTS::openRegSolarFile()
{
	interpreter::RIMParser * parser       = interpreter::RIMParser::getRIMParser();

	INTEGER                  regBlockSize     = MINUTES_IN_DAY*sizeof(FLOAT)/regTimeStep;
	string                   fullName;

	ZoneParams             * zoneParams     = parser->getZoneParams(zoneId);
	nSolarFarms     = zoneParams->nWindFarms;

	regSolarFile   = new FileData*[nSolarFarms];
	regSolarData   = new FLOAT*[nSolarFarms];

	for (INTEGER solar = 0; solar < nSolarFarms; solar++) {
		const vector<size_t> &   zoneSolar      = parser->getZoneWindSet(zoneId);
		INTEGER                  zoneSolarIndex = zoneSolar[0];
		SolarParams            * params     = parser->getSolarParams(zoneSolarIndex);

		fullName              = params->fileName;
		fullName             += ".reg";
		regSolarFile[solar]   = utility::FileControl::openIncremental(fullName.c_str(),
				NULL, circularBuffer, regBlockSize, lookAheadDays);
		regSolarData[solar]   = (FLOAT *)regSolarFile[solar]->outputBuffer;
	}
}

void
AdjLoadTS::dailyTrigger()
{
	// compute adj load for UC steps during the current day
	for (INTEGER step = 0; step < ucSeriesLength; step++) {
		ucAdjSeries[step]  = 0;
	}
	// load - negative load
	for (INTEGER step = 0; step < ucSeriesLength; step++) {
		ucAdjSeries[step]  += ucLoadData[step];
		if (nWindFarms > 0) {
			for (INTEGER farm = 0; farm < nWindFarms; farm++) {
				ucAdjSeries[step]  -= ucWindData[farm][step];
			}
		}
		if (nSolarFarms > 0) {
			for (INTEGER farm = 0; farm < nSolarFarms; farm++) {
				ucAdjSeries[step]  -= ucSolarData[farm][step];
			}
		}
	}

	utility::FileControl::incrementalLoad(ucLoadFile);
	utility::FileControl::incrementalXferBlocks(ucLoadFile);

	if (nWindFarms > 0) {
		for (INTEGER wind = 0; wind < nWindFarms; wind++) {
			utility::FileControl::incrementalLoad(ucWindFile[wind]);
		}
		for (INTEGER wind = 0; wind < nWindFarms; wind++) {
			utility::FileControl::incrementalXferBlocks(ucWindFile[wind]);
		}
	}
	if (nSolarFarms > 0) {
		for (INTEGER solar = 0; solar < nSolarFarms; solar++) {
			utility::FileControl::incrementalLoad(ucSolarFile[solar]);
		}
		for (INTEGER solar = 0; solar < nSolarFarms; solar++) {
			utility::FileControl::incrementalXferBlocks(ucSolarFile[solar]);
		}
	}


	if (implementsLoadFollowing) {
		utility::FileControl::incrementalLoad(lfLoadFile);
		for (INTEGER wind = 0; wind < nWindFarms; wind++) {
			utility::FileControl::incrementalLoad(lfWindFile[wind]);
		}
		for (INTEGER solar = 0; solar < nSolarFarms; solar++) {
			utility::FileControl::incrementalLoad(lfSolarFile[solar]);
		}
		utility::FileControl::incrementalXferBlocks(lfLoadFile);
		for (INTEGER wind = 0; wind < nWindFarms; wind++) {
			utility::FileControl::incrementalXferBlocks(lfWindFile[wind]);
		}
		for (INTEGER solar = 0; solar < nSolarFarms; solar++) {
			utility::FileControl::incrementalXferBlocks(lfSolarFile[solar]);
		}
	}

	if (implementsRegulation) {
		utility::FileControl::incrementalLoad(regLoadFile);
		for (INTEGER wind = 0; wind < nWindFarms; wind++) {
			utility::FileControl::incrementalLoad(regWindFile[wind]);
		}
		for (INTEGER solar = 0; solar < nSolarFarms; solar++) {
			utility::FileControl::incrementalLoad(regSolarFile[solar]);
		}
		utility::FileControl::incrementalXferBlocks(regLoadFile);
		for (INTEGER wind = 0; wind < nWindFarms; wind++) {
			utility::FileControl::incrementalXferBlocks(regWindFile[wind]);
		}
		for (INTEGER solar = 0; solar < nSolarFarms; solar++) {
			utility::FileControl::incrementalXferBlocks(regSolarFile[solar]);
		}
	}
};

void
AdjLoadTS::ucTrigger()
{
};

void
AdjLoadTS::lfTrigger()
{
	for (INTEGER step = 0; step < lfSeriesLength; step++) {
		lfAdjSeries[step]  = 0;
	}

	for (INTEGER step = 0; step < lfSeriesLength; step++) {
		lfAdjSeries[step]  += lfLoadData[step];
		for (INTEGER farm = 0; farm < nWindFarms; farm++) {
			lfAdjSeries[step]  -= lfWindData[farm][step];
		}
		for (INTEGER farm = 0; farm < nSolarFarms; farm++) {
			lfAdjSeries[step]  -= lfSolarData[farm][step];
		}
	}
};

void
AdjLoadTS::regTrigger()
{
	for (INTEGER step = 0; step < regSeriesLength; step++) {
		regAdjSeries[step]  = 0;
	}

	for (INTEGER step = 0; step < regSeriesLength; step++) {
		regAdjSeries[step]  += regLoadData[step];

		for (INTEGER farm = 0; farm < nSolarFarms; farm++) {
			regAdjSeries[step]  -= regSolarData[farm][step];
		}
		for (INTEGER farm = 0; farm < nWindFarms; farm++) {
			regAdjSeries[step]  -= regWindData[farm][step];
		}
	}
};

void
AdjLoadTS::printState(ostream & out){};
void
AdjLoadTS::printParameters(ostream & out){};

} /* namespace model */
