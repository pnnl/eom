/* ***************************************************************************
 * Author      : Kevin Glass
 * Date        : Feb 10, 2010
 * File        : OptimizationException.hpp
 * Project     : RIM
 *
 * Assumptions:
 *
 *
 * Testing:
 *      TBD
 *
 * Details:
 * The OptimizationException class defines the behavior of an exception
 * resulting from a problem with the optimization process. There are two
 * general types of optimization problems: internal solver problems and
 * inability to find solution problems. The first type will require an
 * investigation of the solver the second requires an investigation of the
 * problme construction.
 *
 *
 * STATUS:
 * not done     naming convention conformity
 *     done     doxygen comments
 *     done     private comments
 *     done     comment block conformity
 * not done     testing information
 *     done     ensure unused automatic methods hidden
 *
 ************************************************************************** */

#ifndef OPTIMIZATIONEXCEPTION_HPP_
#define OPTIMIZATIONEXCEPTION_HPP_

#include "Exception.hpp"

/** \class OptimizationException
 * \brief The OptimizationException class is used to handle exceptions from
 * RIM classes associated with CPLEX optimization problems.
 *
 * The OptimizationException handles exceptions found during the optimization
 * process. These can include traps of CPLEX errors or a failure to converge
 * by the optimizer.
 *
 * The OptimizerException will save the initial error message and allow the
 * program to attach extra information each time the exception is caught.
 *
 * NOTE: the default constructor and assignment operator are hidden in the
 * private section of the class.
 */

class OptimizationException : public Exception
{
public:

	OptimizationException(const char * fileName, int lineNum, EXCEPTION_TYPE flag) :
		Exception(fileName, lineNum, flag){};
	virtual ~OptimizationException() {};
protected:
	/**
	 * \brief Print the result of a optimizer exception
	 *
	 * <p>
	 * The result of the exception depends on the cause of the exception
	 * </p>
	 * <p>The types of problems are:
	 * <ul>
	 * <li>
	 * An internal error in the solver library has halted the solution process.
	 * These problems maybe difficult to debug if there is no built=in debug
	 * mode.
	 * </li>
	 * <li>
	 * The solver was unable to reach an answer.
 	 * </li>
	 * </ul>
	 *
	 * \param out : ostream & -- the output stream used to store the exception
	 * \return None
	 */
	virtual void printException(std::ostream & out)
	{
		if (flag == SOLVER) {
			out << "An internal solver error has produced a fatal error.";
		} else if (flag == CONVERGENCE) {
			out << "The solver has failed to converge.";
		} else {
			out << "An unexplained optimization error has occurred.";
		}
	}
private :

	OptimizationException & operator = (OptimizationException & e) {return *this;};
};

#endif /* OPTIMIZATIONEXCEPTION_HPP_ */
