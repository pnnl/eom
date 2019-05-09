/*
 * Optimization.hpp :  NO OPTIMIZATION
 *
 *  Created on: Dec 16, 2010
 *      Author: kevin
 */

#ifndef OPTIMIZATION_HPP_
#define OPTIMIZATION_HPP_

#include "structures.hpp"
#include "optimization_structs.hpp"
#include "RIMParser.hpp"
#include "models/RIMModel.hpp"
#include "optimizers/ObjectiveFunction.hpp"
#include "optimizers/Constraint.hpp"
#include "exceptions/OptimizationException.hpp"
#include "exceptions/Exception.hpp"

namespace model {

template <typename TYPE>
class Optimization : public TYPE
{
public:
	Optimization(INTEGER ba, INTEGER writeLength) : TYPE(ba, writeLength){};
	virtual ~Optimization(){};

	virtual void configure(interpreter::RIMParser & parser)
	{
		nConstraints         = TYPE::getNConstraints();
		constraintSet     = new Constraint *[nConstraints]; // available
		activeConstraint           = new Constraint *[nConstraints]; // in use

		objective            = NULL;
		clearConstraints();
	}
	void printLoadException(INTEGER element, OPTIMIZATION_TYPE optType, const char * file,
			const INTEGER line)
	{
		INTEGER              nConstraints   = 0;
		switch (optType) {
		case UNIT_COMMIT:
			nConstraints = N_UC_CONSTRAINTS;
			break;
		case LOAD_FOLLOWING:
			nConstraints = N_LF_CONSTRAINTS;
			break;
		case REGULATION:
			nConstraints = N_REG_CONSTRAINTS;
			break;
		default:
			nConstraints = 0;
			break;
		}
		/*
		cerr << "FATAL: in " << file << " " << line << endl;
		cerr << "\tattempted to set " << ucConstraintName[element];
		cerr << " (constraint number " << element << ") " << POWER_BALANCE_CONSTRAINT_UC;
		cerr << " to active element " << POWER_BALANCE_CONSTRAINT_UC << endl;
		cerr << "\tthe maximum number of active elements is " << nConstraints << endl;
		*/
	}

protected:

	virtual void solve()
	{
		try {

//			TYPE::loadOptVariables();

			// Load problem
			objective->load();
			for (INTEGER element = 0; element < nConstraints; element++) {
				if (activeConstraint[element] != NULL) {
					activeConstraint[element]->load();
				}
			}
			bool hasSolution = true;
			if (hasSolution) {
				TYPE::handleSolution();
			} else {
				TYPE::handleNoSolution();
			}
		} catch (OptimizationException & e) {
			cerr << __FILE__ << ":" << __LINE__ << " ";
			throw;
		} catch (...) {
			cerr << "Unknown exception caught" << endl;
			exit(-1);
		}
		cleanUpOptimizer();
	}
	virtual void loadConstraintSpace(){};
	virtual void reset(){};

	/**
	 * \brief Add and load the objective function the optimization.
	 */
	void setObjective() {
	};

	/**
	 * \brief Add and load a constraintSet from the constraintSet set to the
	 * active constraints.
	 */
	void setConstraint(INTEGER element, INTEGER set) {
		if (element < 0 || set < 0) {
			activeConstraint[element] = NULL;
		} else if (set >= nConstraints || element >= nConstraints) {
			// TODO: throw RUNTIME error
		}
		activeConstraint[element] = constraintSet[set];
	};

	/**
	 * \brief Add an objective function to the optimizer.
	 */
	void initObjective(ObjectiveFunction * objective) {
		this->objective = objective;
	};

	/**
	 * \brief Add a constraintSet to the optimizer's constraintSet set.
	 */
	void initConstraint(Constraint * constraint, INTEGER con) {
		if (con < 0 || con >= nConstraints) {
			// throw RUNTIME error
		}
		constraintSet[con] = constraint;
	};

	/**
	 * \brief Remove all of the constraints from active set.
	 */
	void clearConstraints()
	{
		for (INTEGER con = 0; con < nConstraints; con++) {
			activeConstraint[con]    = NULL;
		}
	};

	virtual void cleanUpOptimizer() {
	}

private:
	ObjectiveFunction      * objective;
	Constraint            ** activeConstraint;
	Constraint            ** constraintSet;
	INTEGER                  nConstraints;
};

} /* END OF NAMESPACE model */

#endif /* OPTIMIZATION_HPP_ */
