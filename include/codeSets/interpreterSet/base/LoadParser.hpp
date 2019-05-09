#ifndef LOADPARSER_HPP_
#define LOADPARSER_HPP_

#include "structures.hpp"
#include "ModelParser.hpp"

namespace interpreter {

/** \class LoadParser
 *
 * \brief The LoadParser reads the Load parameter set and derives initial
 * conditions for the Load models.
 *
 * @ingroup interpreter
 */
class LoadParser : public ModelParser
{
public:
	/**
	 * \brief Constructs a Load parser to parse the file <i>fileName</i>
	 *
	 * @param fileName name of the source file containing the Load parameter set
	 */
	LoadParser(const std::string & fileName, std::vector<LoadParams> & loadParameters, SimulationInfo & simulationInfo);
	virtual ~LoadParser(){};
	/**
	 * \brief Reads the parameter set for each Load model
	 * @param info : SimulationInfo is the initialized simulation data structure
	 */
	virtual void setModels(SimulationInfo & info);
private:
	void setDerivedData( SimulationInfo & info);
	std::vector<LoadParams> & loadParameters;
};

} /* END OF NAMESPACE interpreter */

#endif /* LOADPARSER_HPP_ */
