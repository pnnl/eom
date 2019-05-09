#include "ZoneParser.hpp"

using std::vector;
using std::string;
using std::endl;
using std::clog;

namespace interpreter {

ZoneParser::ZoneParser(const string & fileName, vector<ZoneParams> & zoneParameters, vector<BalanceAuthParams> & balancingAuthorityParameters, SimulationInfo & simulationInfo) :
	ModelParser(fileName), defaultData(false), zoneParameters(zoneParameters), balancingAuthorityParameters(balancingAuthorityParameters)
{
	// account for zone.csv header (1 line);
	nLines             -= 1;
	setModels(simulationInfo);
};

ZoneParser::~ZoneParser() {};

void
ZoneParser::setModels(SimulationInfo & info)
{
	info.nZones = nLines;
	if (!defaultData) {
		resetParser();
		current             = skipLine(current);
		//try {
			for (size_t zone = 0; zone < info.nZones; zone++) {
				ZoneParams parameters;
				string           name               = getString(',');
				strcpy(parameters.name, name.c_str());
				parameters.noxCost            = getFloat(',');
				parameters.soxCost            = getFloat(',');
				parameters.co2Cost            = getFloat(',');
				parameters.balancingAuthIndex = getInteger(',');
				strcpy(parameters.interchangeFile, name.c_str());
				current             = skipLine(current);
				zoneParameters.push_back(parameters);
			}
			setDerivedData(info);
		//} catch (Exception & e) {
			//clog << "ERROR (" <<  utility::FileControl::getFileName(__FILE__) << ":" << __LINE__ << "failed to parse zone model" << endl;
			//throw;
		//}
	} else {
		for (size_t zone = 0; zone < info.nZones; zone++) {
			ZoneParams parameters;
			strcpy(parameters.name, "DEFAULT");
			parameters.balancingAuthIndex  = zone; // This is the index of the default ba
			parameters.noxCost            = 1.0;
			parameters.soxCost            = 1.0;
			parameters.co2Cost            = 1.0;
			strcpy(parameters.interchangeFile, "");
			parameters.nConventional      = 0;
			zoneParameters.push_back(parameters);
		}
	}
}

void
ZoneParser::setDerivedData(SimulationInfo & info)
{
	int zoneOffset = 0;
	int baIndex = 0;
	for (size_t zone = 0; zone < info.nZones; zone++) {
		if (baIndex != zoneParameters[zone].balancingAuthIndex)
		{
			// TODO: assumes zones in correct order in input file, which currently agrees with RIMParser::loadBAZoneSet
			zoneOffset = zone;
			baIndex = zoneParameters[zone].balancingAuthIndex;
		}
		zoneParameters[zone].zoneBAId           = zone - zoneOffset;
		zoneParameters[zone].nGenerators        = 0;
		zoneParameters[zone].nConventional      = 0;
		zoneParameters[zone].nLoads             = 0;
		zoneParameters[zone].nWindFarms         = 0;
		zoneParameters[zone].nSolarFarms        = 0;
		++balancingAuthorityParameters[zoneParameters[zone].balancingAuthIndex].nZones;
	}
	// TODO parsing models occurs in order.  Should be able to rely on parsers to fill in these bits using information previously obtained.
};

} /* end of NAMESPACE */


