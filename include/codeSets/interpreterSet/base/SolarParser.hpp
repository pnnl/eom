#ifndef SOLARPARSER_HPP_
#define SOLARPARSER_HPP_

#include "structures.hpp"
#include "ModelParser.hpp"

namespace interpreter {

/** \class SolarParser
 *
 * \brief The SolarParser reads the Solar parameter set and derives initial
 * conditions for the Solar models.
 *
 * @ingroup interpreter
 */
class SolarParser : public ModelParser
{
public:
	/**
	 * \brief Constructs a Solar parser to parse the file <i>fileName</i>
	 *
	 * @param fileName name of the source file containing the Solar parameter
	 * set
	 */
	SolarParser(const std::string & fileName, std::vector<SolarParams> & solarParameters, SimulationInfo & simulationInfo);
	virtual ~SolarParser(){};
	/**
	 * \brief Reads the parameter set for each Solar model
	 * @param info : SimulationInfo is the initialized simulation data structure
	 */
	virtual void setModels(SimulationInfo & info);
private:
	std::vector<SolarParams> & solarParameters;
};

} /* END OF NAMESPACE interpreter */

#endif /* SOLARPARSER_HPP_ */
