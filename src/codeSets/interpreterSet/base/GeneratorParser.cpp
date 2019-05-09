#include "GeneratorParser.hpp"
#include "generic.hpp"
#include "structures.hpp"
#include "csv.hpp"
#include "location.hpp"
#include "weather.hpp"
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string/case_conv.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/assign.hpp>
#include <algorithm>

using ::gov::pnnl::rim::csv::DictionaryReader;
using ::gov::pnnl::rim::csv::Dictionary;
using ::boost::lexical_cast;
using ::boost::algorithm::to_lower_copy;
using ::boost::algorithm::split;
using ::boost::algorithm::is_any_of;
using ::boost::algorithm::trim_copy;
using ::std::map;
using ::std::ifstream;
using ::std::min;
using ::std::auto_ptr;
using ::std::vector;
using ::std::string;

/*
count technology category
 1509         CT CT Gas
  497         CT CT Oil
   87         CT CT Other
  676         CC Combined Cycle (Existing)
    2         CC IGCC
  243         IC Internal Combustion
  109         ?? Nuclear (Existing)
 1009       Coal ST Coal
  217         ?? ST Gas
  102         ?? ST Oil
  173         ?? ST Other
*/

AirTemperatureDeratingTable::AirTemperatureDeratingTable(std::vector<std::pair<double, double> > fahrenheitVsFactor) : fahrenheitVsFactor(fahrenheitVsFactor) { }
double AirTemperatureDeratingTable::deratingFactorFor(const ::Temperature & temperature) {
	double fahrenheit = temperature.fahrenheit();
	if (fahrenheit < fahrenheitVsFactor[0].first)
		return fahrenheitVsFactor[0].second;
	if (fahrenheit > fahrenheitVsFactor[fahrenheitVsFactor.size()-1].first)
		return fahrenheitVsFactor[fahrenheitVsFactor.size()-1].second;
	size_t index = 0;
	while (fahrenheitVsFactor[index+1].first < fahrenheit) // guaranteed to terminate before overflowing by returns above TODO make sure I have < and <= used properly
		index++;
	double f0 = fahrenheitVsFactor[index  ].first;
	double f1 = fahrenheitVsFactor[index+1].first;
	double d0 = fahrenheitVsFactor[index  ].second;
	double d1 = fahrenheitVsFactor[index+1].second;
	return d0 + (d1-d0) * (fahrenheit - f0) / (f1 - f0);
}
AirTemperatureefficientTable::AirTemperatureefficientTable(std::vector<std::pair<double, double> > fahrenh) : fahrenh(fahrenh) { }
double AirTemperatureefficientTable::deratingFactorForef(const ::Temperature & temperature) {
	double fahrenheit = temperature.fahrenheit();
	if (fahrenheit < fahrenh[0].first)
		return fahrenh[0].second;
	if (fahrenheit > fahrenh[fahrenh.size()-1].first)
		return fahrenh[fahrenh.size()-1].second;
	size_t index = 0;
	while (fahrenh[index+1].first < fahrenheit) // guaranteed to terminate before overflowing by returns above TODO make sure I have < and <= used properly
		index++;
	double f0 = fahrenh[index  ].first;
	double f1 = fahrenh[index+1].first;
	double d0 = fahrenh[index  ].second;
	double d1 = fahrenh[index+1].second;
	return d0 + (d1-d0) * (fahrenheit - f0) / (f1 - f0);
}
namespace interpreter {

struct StringEmpty
{
  bool operator()(const std::string& s) { return s.empty(); }
};

PiecewiseLinear readCurve(const string & input) {
	PiecewiseLinear curve;
	vector<string> values;
	// TODO regex match the format
	split(values, input, is_any_of(");("));
	values.erase(std::remove_if(values.begin(), values.end(), StringEmpty()), values.end());
	for (size_t index = 0; index < values.size(); index+=4) {
		double x0 = lexical_cast<double>(trim_copy(values[index  ]));
		double y0 = lexical_cast<double>(trim_copy(values[index+1]));
		double x1 = lexical_cast<double>(trim_copy(values[index+2]));
		double y1 = lexical_cast<double>(trim_copy(values[index+3]));
		LineSegment segment(x0, y0, x1, y1);
		curve.segment[index/4] = segment;
		++curve.nSegments;
	}
	return curve;
}

GeneratorParser::GeneratorParser(
		const string & fileName,
		vector<ConventionalParams> & conventionalParameters,
		vector<ConventionalData> & conventionalData,
		vector<Location> & locations,
		map<string, AirTemperatureDeratingTable> & deratingTables,
		map<string, AirTemperatureefficientTable> & deratingTabletwo,
		SimulationInfo & info
)
{
	DictionaryReader reader(new ifstream(fileName.c_str()));
	bool emittedLocationWarning = false;
	while (! reader.end()) {
		++info.nConventionalGenerators;
		++info.nGenerators;
		Dictionary row = reader.next();
		ConventionalParams parameters;
		string           name           = row["name"];
		strcpy(parameters.genParams.name, name.c_str());
		parameters.genParams.heatRateCurve = readCurve(row["Input-Output curve (Power output in MW; Fuel input in MMBtu/hr)"]);
		parameters.genParams.genPowerLimits[LOWER_BOUND] = parameters.genParams.heatRateCurve.segment[0].startPoint[POWER];
		parameters.genParams.genPowerLimits[UPPER_BOUND] = parameters.genParams.heatRateCurve.segment[parameters.genParams.heatRateCurve.nSegments-1].endPoint[POWER];
		parameters.genParams.genRampRates[DOWN] = lexical_cast<double>(row["ramp down rate (MW/hr)"]);
		parameters.genParams.genRampRates[UP]   = lexical_cast<double>(row["ramp up rate (MW/hr)"]);
		parameters.genTransitionCost[DOWN]      = lexical_cast<double>(row["shutdown cost (K$)"]) * 1000; // *1000 to convert from kilo-dollars to dollars
		parameters.genTransitionCost[UP]        = lexical_cast<double>(row["startup cost (K$)"]) * 1000; // *1000 to convert from kilo-dollars to dollars
		parameters.genParams.balancingAuth      = lexical_cast<int>(row["Balancing Authority"]);
		parameters.genParams.zone               = lexical_cast<int>(row["Zone"]);
		parameters.genParams.fuelType           = setFuelType(row["Fuel Type"]);
		parameters.genParams.fuelCost           = lexical_cast<double>(row["Fuel cost ($/MMBtu)"]);
		parameters.genParams.canCommit          = "true" == to_lower_copy(row["can commit"]);
		parameters.genParams.canLoadFollow      = "true" == to_lower_copy(row["can load follow"]);
		parameters.genParams.canRegulate        = "true" == to_lower_copy(row["can regulate"]);
		parameters.minTimeInState[OFF_STATE]    = lexical_cast<int>(row["minimum downtime (hours)"]);
		parameters.minTimeInState[ON_STATE]     = lexical_cast<int>(row["minimum uptime (hours)"]);
		parameters.genParams.noxRate            = lexical_cast<double>(row["NOx Rate (lbs/MMBtu)"]);
		parameters.genParams.soxRate            = lexical_cast<double>(row["SOx Rate (lbs/MMBtu)"]);
		parameters.genParams.co2Rate            = lexical_cast<double>(row["CO2 Rate (lbs/MMBtu)"]);
		if (row.contains("must on") && "" != row["must on"]) {
			parameters.genParams.muston          = "true" == to_lower_copy(row["must on"]);
		}
		if (row.contains("Variable O&M ($/MWh)") && "" != row["Variable O&M ($/MWh)"]) {
			parameters.genParams.operationsAndMaintenanceVariableCost = lexical_cast<double>(row["Variable O&M ($/MWh)"]);
		} else {
			parameters.genParams.operationsAndMaintenanceVariableCost = 0;
		}
		if (row.contains("Location") && "" != row["Location"]) {
			size_t locationIndex = lexical_cast<size_t>(row["Location"]);
			if (0 == locations.size()) {
				if (! emittedLocationWarning) {
					emittedLocationWarning = true;
					std::cerr << "WARNING: location data found for generators but no locations were loaded" << std::endl;
				}
			} else {
				parameters.genParams.location = &locations.at(locationIndex);
			}
		}
		if (row.contains("category") && "" != row["category"] && 0 < deratingTables.size()) // expect derating info for every generator category if there is any derating info at all
			parameters.genParams.airTemperatureDeratingTable = & deratingTables.at(row["category"]);
		else
			parameters.genParams.airTemperatureDeratingTable = NULL;

		if (row.contains("category") && "" != row["category"] && 0 < deratingTabletwo.size()) // expect derating info for every generator category if there is any derating info at all
			parameters.genParams.airTemperatureefficientTable = & deratingTabletwo.at(row["category"]);
		else
			parameters.genParams.airTemperatureefficientTable = NULL;
		parameters.status                       =  GENERATOR_INIT;
		conventionalParameters.push_back(parameters);
	}
	conventionalData.resize(info.nInterfaceSteps * info.nConventionalGenerators);
	setDerivedData(info, conventionalParameters);
}

void
GeneratorParser::setDerivedData(SimulationInfo & info, vector<ConventionalParams> & conventionalParameters)
{
	for (size_t gen = 0; gen < info.nConventionalGenerators; gen++) {
		// convert ramp rates from hours to base time units
		conventionalParameters[gen].genParams.genRampRates[DOWN] /= info.baseTimeStep;
		conventionalParameters[gen].genParams.genRampRates[UP]   /= info.baseTimeStep;
		conventionalParameters[gen].genParams.noxRate            /= info.baseTimeStep;
		conventionalParameters[gen].genParams.soxRate            /= info.baseTimeStep;
		conventionalParameters[gen].genParams.co2Rate            /= info.baseTimeStep;

		PiecewiseLinear & costCurve = conventionalParameters[gen].genParams.costCurve;
		PiecewiseLinear & heatRateCurve = conventionalParameters[gen].genParams.heatRateCurve;
		costCurve.nSegments = conventionalParameters[gen].genParams.heatRateCurve.nSegments;
		for (size_t segmentIndex = 0; segmentIndex < costCurve.nSegments; segmentIndex++) {
			LineSegment & costSegment = costCurve.segment[segmentIndex];
			LineSegment & heatSegment = heatRateCurve.segment[segmentIndex];
			if (heatSegment.startPoint[POWER] == heatSegment.endPoint[POWER]) { // solver has a hard time if start and end are the same
				FLOAT adjustment = min(1e-2,1e-2*heatSegment.endPoint[POWER]); // make sure our adjustment isn't too big
				heatSegment.endPoint[POWER] += adjustment;
			}
			double cost0 = heatSegment.startPoint[COST] * conventionalParameters[gen].genParams.fuelCost;
			double cost1 = heatSegment.endPoint[COST]   * conventionalParameters[gen].genParams.fuelCost;
			double power0 = heatSegment.startPoint[POWER];
			double power1 = heatSegment.endPoint[POWER];
			costSegment = LineSegment(power0, cost0, power1, cost1);
		}
	}
};

} /* END OF NAMESPACE interpreter */
