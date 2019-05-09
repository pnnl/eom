#include "LoadParser.hpp"
#include "exceptions/Exception.hpp"
#include "FileControl.hpp"

using std::vector;
using std::string;
using std::endl;
using std::clog;

namespace interpreter {

LoadParser::LoadParser(const string & fileName, vector<LoadParams> & loadParameters, SimulationInfo & simulationInfo):
	ModelParser(fileName),
	loadParameters(loadParameters)
{
	// account for load.csv header (1 line);
	nLines             -= 1;
	setModels(simulationInfo);
};



void
LoadParser::setModels(SimulationInfo & info)
{
	info.nLoads = nLines;
	resetParser();
	try {
		current         = skipLine(current);
		for (size_t load = 0; load < info.nLoads; load++) {
			LoadParams parameters;
			string name               = getString(',');
			strcpy(parameters.name, name.c_str());
			name                      = getString(',');
			strcpy(parameters.fileName, name.c_str());
			parameters.balancingAuth  = getInteger(',');
			parameters.zone           = getInteger();
			current         = skipLine(current);
			loadParameters.push_back(parameters);
		}
		setDerivedData(info);
	} catch (Exception & e) {
		clog << "ERROR (";
		clog <<  utility::FileControl::getFileName(__FILE__);
		clog << ":" << __LINE__ << "failed to parse load model" << endl;
		throw;
	}
}

void
LoadParser::setDerivedData(SimulationInfo & info)
{
	for (size_t load = 0; load < info.nLoads; load++) {
		loadParameters[load].currentLoad        = 0.0;
		loadParameters[load].outputLoad         = 0.0;
	}
}

} /* END OF NAMESPACE interpreter */
