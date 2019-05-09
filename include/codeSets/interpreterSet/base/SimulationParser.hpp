/* ***************************************************************************
 * Author      : Kevin Glass
 * Date        : Jun 22, 2010
 * File        : SimulationParser.hpp
 * Project     : rim
 *
 * Testing:
 *      $(RIM)/test/construction/InterpreterTest
 *
 *
 * STATUS:
 *              naming convention conformity
 *              doxygen comments                                        done
 *              private comments
 *              comment block conformity
 *              testing information
 *              ensure hidden methods
 *
 ************************************************************************** */
#ifndef SIMULATIONPARSER_HPP_
#define SIMULATIONPARSER_HPP_

#include "structures.hpp"
#include "ModelParser.hpp"

namespace interpreter {

/** \class SimulationParser
 *
 * \brief The SimulationParser reads the Simulation parameter set and derives
 * initial conditions for the simulation.
 *
 * @ingroup interpreter
 */
class SimulationParser : public ModelParser
{
public:
	/**
	 * \brief Constructs a Simulation parser to parse the file <i>fileName</i>
	 *
	 * @param fileName name of the source file containing the Simulation
	 * parameter set
	 */
	SimulationParser(const std::string & fileName, SimulationInfo & simulationInfo);
	virtual ~SimulationParser(){};
	/**
	 * \brief Reads the parameter set for each Simulation model
	 *
	 * @param parameters is the parameter data space
	 * @param interface is the interface data space
	 * @param info : SimulationInfo is the initialized simulation data structure
	 */
	virtual void setModels(SimulationInfo &);
private:
	/**
	 * \brief Sets parameter and interface data derived from the model input
	 * file.
	 * @param info : SimulationInfo is the initialized simulation data structure
	 */
	void setDerivedData(SimulationInfo &);
	virtual LOGICAL validateInput(SimulationInfo &);
	REPEAT_TYPE setRepeatType();
};

} /* END OF NAMESPACE interpreter */

#endif /* SIMULATIONPARSER_HPP_ */
