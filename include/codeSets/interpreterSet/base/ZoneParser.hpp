#ifndef ZONEPARSER_HPP_
#define ZONEPARSER_HPP_

#include "ModelParser.hpp"

namespace interpreter {

/**
 * \class ZoneParser
 *
 * \brief The ZoneParser reads the Zone parameter set and derives initial
 * conditions for the Zone models.
 *
 */
class ZoneParser: public ModelParser
{
public:
	/**
	 * \brief Constructs a Zone parser to parse the file <i>fileName</i>
	 * @param fileName name of the source file containing the Zone parameter
	 * set
	 */
	ZoneParser(const std::string & fileName, std::vector<ZoneParams> & zoneParameters, std::vector<BalanceAuthParams> & balancingAuthorityParameters, SimulationInfo & simulationInfo);
	virtual ~ZoneParser();
	/**
	 * \brief Reads the parameter set for each Zone model
	 * @param info : SimulationInfo is the initialized simulation data structure
	 */
	virtual void setModels(SimulationInfo & info);
private:
	void setDerivedData(SimulationInfo & info);
	bool                 defaultData;
	std::vector<ZoneParams> & zoneParameters;
	std::vector<BalanceAuthParams> & balancingAuthorityParameters;
};

} // END NAMESPACE INTERPRETER
#endif /* ZONEPARSER_HPP_ */
