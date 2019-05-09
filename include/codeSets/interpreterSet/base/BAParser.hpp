/* ***************************************************************************
 * Author      : Kevin Glass
 * Date        : Jun 3, 2011
 * File        : BAParser.hpp
 * Project     : RIM
 *
 * Assumptions:
 *
 *
 * Testing:
 *      TBD
 *
 * Details:
 *
 *
 * STATUS:
 * not done     naming convention conformity
 * not done     doxygen comments
 * not done     private comments
 * not done     comment block conformity
 * not done     testing information
 * not done     ensure hidden methods
 *
 ************************************************************************** */
/* ***************************************************************************
 * Author      : Kevin Glass
 * Date        : Jun 3, 2011
 * File        : BAParser.hpp
 * Project     : RIM
 *
 * Assumptions:
 *
 *
 * Testing:
 *      TBD
 *
 * Details:
 *
 *
 * STATUS:
 * not done     naming convention conformity
 * not done     doxygen comments
 * not done     private comments
 * not done     comment block conformity
 * not done     testing information
 * not done     ensure hidden methods
 *
 ************************************************************************** */
#ifndef BAPARSER_HPP_
#define BAPARSER_HPP_

#include "ModelParser.hpp"
#include "RIMParser.hpp"

namespace interpreter {

/**
 * \class BAParser
 *
 * \brief The BAParser reads the BA parameter set and derives initial
 * conditions for the BA models.
 *
 * @ingroup interpreter
 */
class BAParser: public ModelParser
{
public:
	/**
	 * \brief Constructs a BA parser to parse the file <i>fileName</i>
	 *
	 * @param fileName name of the source file containing the BA parameter set
	 */
	BAParser(const std::string & fileName, std::vector<BalanceAuthParams> & balancingAuthorityParameters, SimulationInfo & simulationInfo);
	virtual ~BAParser();
	/**
	 * \brief Reads the parameter set for each BA model
	 *
	 * @param parameters is the parameter data space
	 * @param interface is the interface data space
	 * @param info : SimulationInfo is the initialized simulation data structure
	 */
	virtual void setModels(SimulationInfo & info);

private:
	void setDerivedData(SimulationInfo & info);
	bool                             defaultData;
	std::vector<BalanceAuthParams> & balancingAuthorityParameters;
};

} /* END OF NAMESPACE interpreter */

#endif /* BAPARSER_HPP_ */
