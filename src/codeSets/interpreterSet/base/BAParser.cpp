#include "BAParser.hpp"
#include "exceptions/Exception.hpp"
#include "FileControl.hpp"

using std::vector;
using std::string;
using std::endl;
using std::clog;

namespace interpreter {

BAParser::BAParser(const string & fileName, vector<BalanceAuthParams> & balancingAuthorityParameters, SimulationInfo & simulationInfo) :
	ModelParser(fileName), defaultData(false), balancingAuthorityParameters(balancingAuthorityParameters)
{
	// account for wind.csv header (1 line);
	nLines             -= 1;
	setModels(simulationInfo);
};


BAParser::~BAParser() {};

void BAParser::setModels(SimulationInfo & info)
{
	info.nBalancingAuths = nLines;
	if (!defaultData) {
		resetParser();
		current             = skipLine(current);
		try {
			for (size_t ba = 0; ba < info.nBalancingAuths; ba++) {
				BalanceAuthParams parameters;
				string           name           = getString(',');
				strcpy(parameters.name, name.c_str());
				parameters.cps1            = getFloat(',');
				parameters.cps2            = getFloat(',');
				parameters.bias            = getFloat(',');
				parameters.agcLower        = getFloat(',');
				parameters.agcUpper        = getFloat(',');
				parameters.reserveFactor   = getFloat();
				current             = skipLine(current);
				balancingAuthorityParameters.push_back(parameters);
			}
		} catch (Exception & e) {
			clog << "ERROR (" <<  utility::FileControl::getFileName(__FILE__) << ":" << __LINE__ << "failed to parse BA model" << endl;
			throw;
		}
	} else {
		for (size_t ba = 0; ba < info.nBalancingAuths; ba++) {
			BalanceAuthParams parameters;
			strcpy(parameters.name, "DEFAULT");
			parameters.cps1            = 1.0;
			parameters.cps2            = 1.0;
			parameters.bias            = 1.0;
			parameters.agcLower        = 1.0;
			parameters.agcUpper        = 1.0;
			parameters.reserveFactor   = 0.0;
			balancingAuthorityParameters.push_back(parameters);
		}
	}
	setDerivedData(info);
}

void
BAParser::setDerivedData(SimulationInfo & info)
{
	for (size_t ba = 0; ba < info.nBalancingAuths; ba++) {
		balancingAuthorityParameters[ba].nZones             = 0;
		balancingAuthorityParameters[ba].nGenerators        = 0;
		balancingAuthorityParameters[ba].nConventional      = 0;
		balancingAuthorityParameters[ba].nStorage           = 0;
		balancingAuthorityParameters[ba].nHydro             = 0;
		balancingAuthorityParameters[ba].currentPower       = 0.0;
		balancingAuthorityParameters[ba].currentLoad        = 0.0;
		balancingAuthorityParameters[ba].outputPower        = 0.0;
		balancingAuthorityParameters[ba].outputLoad         = 0.0;
	}
};

} /* end of NAMESPACE */
