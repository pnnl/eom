#include "structures.hpp"
#include "optimization_structs.hpp"
#include "exceptions/Exception.hpp"
#include "location.hpp"
#include "weather.hpp"

#include <sstream>
#include <algorithm>
#include <map>
#include <climits>
#include <cmath>

using std::string;
using std::endl;
using std::cerr;
using std::ostream;

SimulationInfo::SimulationInfo(void) :
	scenarioName           (         ),
	start                  (         ),
	stop                   (         ),
	baseTimeStep           (        1),
	nDumpSteps             (     1440),
	daStep                 (       24),
	ucTrigger              (        0),
	ucTimeStep             (       60),
	ucLength               (       72),
	lfTimeStep             (       10),
	lfLength               (      120),
	regTimeStep            (        1),
	hydroTimeStep          (       -1),
	forecastDays           (        4),
	repeatType             (NO_REPEAT),
	nRepeats               (        0),
	nInterfaceSteps        (       -1),
	nLoadRampSteps         (       -1),
	nBaseStepsInProjection (     4320),
	nBaseStepsInUCStep     (     1440),
	nBaseStepsInLFStep     (       10),
	nBaseStepsInRegStep    (        1),
	nUCTimeStepsInDay      (       24),
	nUCTimeStepsInHour     (        1),
	nLFStepsInDay          (      144),
	nLFStepsInHour         (       10),
	nLFStepsInUCStep       (       10),
	nRegStepsInDay         (     1440),
	nRegStepsInHour        (       60),
	nRegStepsInUCStep      (       60),
	nRegStepsInLFStep      (       10),
	duration               (        0),
	dumpSize               (        0),
	circularBuffer         (        0),
	nSimulations           (        0),
	nBalancingAuths        (        0),
	nSchedulers            (        0),
	nZones                 (        0),
	nConventionalGenerators(        0),
	nStorageGenerators     (        0),
	nHydroGenerators       (        0),
	nLines                 (        0),
	nDR                    (        0),
	nReplicates            (        0),
	nCanCommit             (        0),
	nCanRegulate           (        0),
	nLoads                 (        0),
	nWindFarms             (        0),
	nSolarFarms            (        0),
	nInterchanges          (        0),
	nGenerators            (        0),
	implementsLoadFollowing(    false),
	implementsRegulation   (    false),
	epGap                  (      .01)
{
}

GeneratorParams::GeneratorParams() :
	heatRateCurve                       (               ),
	costCurve                           (               ),
	fuelCost                            (nan("")        ),
	balancingAuth                       (INT_MIN        ),
	zone                                (INT_MIN        ),
	canCommit                           (false          ),
	canLoadFollow                       (false          ),
	canRegulate                         (false          ),
	muston	                            (false          ),
	noxRate                             (nan("")        ),
	soxRate                             (nan("")        ),
	co2Rate                             (nan("")        ),
	fuelType                            (NUM_FUEL_TYPES ),
	operationsAndMaintenanceVariableCost(nan("")        ),
	location                            (NULL           ),
	airTemperatureDeratingTable         (NULL           ),
	airTemperatureefficientTable        (NULL           )
{
	strcpy(name, "uninitialized");
	genPowerLimits[0] = nan("");
	genPowerLimits[1] = nan("");
	genRampRates  [0] = nan("");
	genRampRates  [1] = nan("");
}

ConventionalParams::ConventionalParams() :
	genParams           (),
	status              (GENERATOR_OFF)
{
	genTransitionCost[0] = nan("");
	genTransitionCost[1] = nan("");
	minTimeInState   [0] = INT_MAX;
	minTimeInState   [1] = INT_MAX;
}


double ConventionalParams::maxPowerAt(boost::posix_time::ptime time) {
	double maxPower = genParams.genPowerLimits[UPPER_BOUND];
	Location * location = genParams.location;
	if (location != NULL && genParams.airTemperatureDeratingTable != NULL) {
		maxPower *= genParams.airTemperatureDeratingTable->deratingFactorFor(location->temperatureAt(time));
	}
	return maxPower;
}

double ConventionalParams::efficien(boost::posix_time::ptime time) {
	double efficiency = 1;
	Location * location = genParams.location;
	if (location != NULL && genParams.airTemperatureefficientTable != NULL) {
		efficiency = genParams.airTemperatureefficientTable->deratingFactorForef(location->temperatureAt(time));
	}
	return efficiency;
}

double ConventionalParams::deratf(boost::posix_time::ptime time) {
	double deratfcy = 1;
	Location * location = genParams.location;
	if (location != NULL && genParams.airTemperatureDeratingTable != NULL) {
		deratfcy = genParams.airTemperatureDeratingTable->deratingFactorFor(location->temperatureAt(time));
	}
	return deratfcy;
}



string getRepeatType(REPEAT_TYPE type)
{
	string                   rtn;

	switch(type) {
	case NO_REPEAT :
		rtn =  "NO_REPEAT";
		break;
	case MONTE_CARLO :
		rtn =  "MONTE_CARLO";
		break;
	case SENSITIVITY_ANALYSIS :
		rtn =  "SENSITIVITY_ANALYSIS";
		break;
	default:
		break;
	}

	return rtn;
}

REPEAT_TYPE getRepeatType(string & name)
{
	REPEAT_TYPE              type           = NO_REPEAT;

	if (name == "none") {
		type = NO_REPEAT;
	} else if (name == "mc") {
		type = MONTE_CARLO;
	} else if (name == "sa") {
		type = SENSITIVITY_ANALYSIS;
	} else {
		cerr << "FATAL: invalid model name " << name << endl;
		throw Exception(__FILE__, __LINE__, Exception::INITIALIZATION);
	}

	return type;

}

string getGeneratorStatus(GENERATOR_STATE status)
{
	string                   rtn;

	switch(status) {
	case GENERATOR_RAMPUP :
		rtn =  "GENERATOR_RAMPUP";
		break;
	case GENERATOR_SUPPORTING :
		rtn =  "GENERATOR_SUPPORTING";
		break;
	case GENERATOR_RAMPDOWN :
		rtn =  "GENERATOR_RAMPDOWN";
		break;
	case GENERATOR_OFF :
		rtn =  "GENERATOR_OFF";
		break;
	case GENERATOR_INIT :
		rtn =  "GENERATOR_INIT";
		break;
	default:
		break;
	}

	return rtn;
}

string getSwitchStatus(SW_STATE swStatus)
{
	string                   rtn;

	switch(swStatus) {
	case SW_OFF_STATE :
		rtn =  "SW_OFF_STATE";
		break;
	case SW_ON_STATE :
		rtn =  "SW_ON_STATE";
		break;
	case NO_SWITCH :
		rtn =  "NO_SWITCH";
		break;
	default:
		rtn = "INVALID_SWITCH_STATE";
		break;
	}

	return rtn;
}


string getBooleanValue(bool value)
{
	return value?"true":"false";
}

void printPWCurve(PiecewiseLinear & curve, ostream & out)
{
	out << "Curve with " << curve.nSegments << " segments" << endl;
	for (INTEGER seg = 0; seg < N_PW_SEGMENTS; seg++) {
		out << "\t\t" << seg << ":";
		out << "{";
		out << "(" << curve.segment[seg].startPoint[POWER] << ", ";
		out << curve.segment[seg].startPoint[COST]<< "),";
		out << "(" << curve.segment[seg].endPoint[POWER] << ", ";
		out << curve.segment[seg].endPoint[COST]<< "),";
		out << curve.segment[seg].slope << ", ";
		out << curve.segment[seg].intercept;
		out << "}" << endl;
	}
}

FUEL_TYPE setFuelType(const string & fuel)
{
	std::map<string, FUEL_TYPE> map;
	map["nuclear"   ] = NUCLEAR;
	map["coal"      ] = COAL;
	map["naturalgas"] = NATURALGAS;
	map["diesel"    ] = DIESEL;
	map["cc"        ] = GAS_CC;
	map["ct"        ] = GAS_CT;

	string lowercaseFuel = fuel;
	std::transform(lowercaseFuel.begin(), lowercaseFuel.end(), lowercaseFuel.begin(), ::tolower);
	if (1 >= map.count(lowercaseFuel))
		return map[lowercaseFuel];
	cerr << "FATAL: invalid fuel name " << fuel << endl;
	throw Exception(__FILE__, __LINE__, Exception::INITIALIZATION);
}

string getFuelType(FUEL_TYPE type)
{
	string                   rtn;
	switch(type) {
	case NUCLEAR :
		rtn =  "NUCLEAR";
		break;
	case COAL :
		rtn =  "COAL";
		break;
	case NATURALGAS :
		rtn =  "NATURALGAS";
		break;
	case DIESEL :
		rtn =  "DIESEL";
		break;
	default:
		cerr << "FATAL: invalid fuel type " << type << endl;
		throw Exception(__FILE__, __LINE__, Exception::INITIALIZATION);
	}

	return rtn;
}

string getModelType(INFORMATION_TYPE type)
{
	std::map<INFORMATION_TYPE, string> map;
	map[SIMULATION       ] = "SIMULATION"       ;
	map[BALANCE_AUTHS    ] = "BALANCE_AUTHS"    ;
	map[ZONE             ] = "ZONE"             ;
	map[CONVENTIONAL     ] = "CONVENTIONAL"     ;
	map[LOAD_TIMESERIES  ] = "LOAD_TIMESERIES"  ;
	map[WIND_TIMESERIES  ] = "WIND_TIMESERIES"  ;
	map[SOLAR_TIMESERIES ] = "SOLAR_TIMESERIES" ;
	map[ZONE_INTRACONNECT] = "ZONE_INTRACONNECT";
	map[ZONE_INTERCONNECT] = "ZONE_INTERCONNECT";
	map[ZONE_CONNECTIONS ] = "ZONE_CONNECTIONS" ;
	map[NUM_MODELS       ] = "NUM_MODELS"       ;

	if (1 >= map.count(type))
		return map[type];
	cerr << "FATAL: invalid model type " << type << endl;
	throw Exception(__FILE__, __LINE__, Exception::INITIALIZATION);
}
