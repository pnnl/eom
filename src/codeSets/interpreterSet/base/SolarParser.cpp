#include "SolarParser.hpp"
#include "exceptions/Exception.hpp"
#include "FileControl.hpp"

using std::vector;
using std::string;
using std::clog;
using std::endl;

namespace interpreter {

SolarParser::SolarParser(const string & fileName, vector<SolarParams> & solarParameters, SimulationInfo & simulationInfo) :
	ModelParser(fileName),
	solarParameters(solarParameters)
{
	// account for solar.csv header (1 line);
	nLines             -= 1;
	setModels(simulationInfo);
};



void
SolarParser::setModels(SimulationInfo & info)
{
	info.nSolarFarms = nLines;
	resetParser();
	current             = skipLine(current);
	try {
		for (size_t farm = 0; farm < info.nSolarFarms; farm++) {
			SolarParams parameters;
			string           name           = getString(',');
			strcpy(parameters.name, name.c_str());
			name        = getString(',');
			strcpy(parameters.fileName, name.c_str());
			parameters.balancingAuth  = getInteger(',');
			parameters.zone           = getInteger();
			parameters.currentSolarPower  = -1.0;
			parameters.outputSolarPower   = -1.0;
			current             = skipLine(current);
			solarParameters.push_back(parameters);
		}
	} catch (Exception & e) {
		clog << "ERROR (" <<  utility::FileControl::getFileName(__FILE__) << ":" << __LINE__ << "failed to parse solar model" << endl;
		throw;
	}
}

} /* END OF NAMESPACE interpreter */
