#include "WindParser.hpp"
#include "exceptions/Exception.hpp"
#include "FileControl.hpp"

using std::vector;
using std::string;
using std::clog;
using std::endl;

namespace interpreter {

WindParser::WindParser(const string & fileName, vector<WindParams> & windParameters, SimulationInfo & simulationInfo) :
	ModelParser(fileName),
	windParameters(windParameters)
{
	// account for wind.csv header (1 line);
	nLines             -= 1;
	setModels(simulationInfo);
};


void
WindParser::setModels(SimulationInfo & info)
{
	info.nWindFarms = nLines;
	resetParser();
	current             = skipLine(current);
	try {
		for (size_t farm = 0; farm < info.nWindFarms; farm++) {
			WindParams parameters;
			string name                   = getString(',');
			strcpy(parameters.name, name.c_str());
			name                          = getString(',');
			strcpy(parameters.fileName, name.c_str());
			parameters.balancingAuth      = getInteger(',');
			parameters.zone               = getInteger();
			parameters.currentWindPower   = -1.0;
			parameters.outputWindPower    = -1.0;
			current             = skipLine(current);
			windParameters.push_back(parameters);
		}
	} catch (Exception & e) {
		clog << "ERROR (" <<  utility::FileControl::getFileName(__FILE__) << ":" << __LINE__ << "failed to parse wind model" << endl;
		throw;
	}
}

} /* END OF NAMESPACE interpreter */
