#ifndef GENERATORPARSER_HPP_
#define GENERATORPARSER_HPP_

#include <string>
#include <vector>
#include <map>
#include "structures.hpp"

namespace interpreter {

class GeneratorParser
{
public:
	GeneratorParser(
			const std::string & fileName,
			std::vector<ConventionalParams> & conventionalParameters,
			std::vector<ConventionalData> & conventionalData,
			std::vector<Location> & locations,
			std::map<std::string, AirTemperatureDeratingTable> & deratingTables,
			std::map<std::string, AirTemperatureefficientTable> & deratingTabletwo,
			SimulationInfo & simulationInfo
	);

private:
	// Sets parameter and interface data derived from the model input file.
	void setDerivedData(SimulationInfo & info, std::vector<ConventionalParams> & conventionalParameters);
};

} /* END OF NAMESPACE interpreter */

#endif /* GENERATORPARSER_HPP_ */
