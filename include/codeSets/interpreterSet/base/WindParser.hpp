#ifndef WINDPARSER_HPP_
#define WINDPARSER_HPP_

#include "structures.hpp"
#include "ModelParser.hpp"

namespace interpreter {

/** \class WindParser
 *
 * \brief The WindParser reads the Wind parameter set and derives initial
 * conditions for the Wind models.
 *
 * @ingroup interpreter
 */
class WindParser : public ModelParser
{
public:
	/**
	 * \brief Constructs a Wind parser to parse the file <i>fileName</i>
	 * @param fileName name of the source file containing the BA parameter set
	 */
	WindParser(const std::string & fileName, std::vector<WindParams> & windParameters, SimulationInfo & simulationInfo);
	virtual ~WindParser(){};
	/**
	 * \brief Reads the parameter set for each Wind model
	 * @param info : SimulationInfo is the initialized simulation data structure
	 */
	virtual void setModels(SimulationInfo & info);

private:
	std::vector<WindParams> & windParameters;
};

} /* END OF NAMESPACE interpreter */

#endif /* WINDPARSER_HPP_ */
