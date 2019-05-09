#ifndef OPTIMIZER_HPP_
#define OPTIMIZER_HPP_

#include "optimization_structs.hpp"
#include "optimizer_structs.hpp"

#include "RIMParser.hpp"
#include "models/RIMModel.hpp"
#include "optimizers/Constraint.hpp"
#include "optimizers/ObjectiveFunction.hpp"

/*
 * A RIM optimizer is created  by instantiating an Optimization<T> class.
 * The Optimization<T> class implements:
 * 		configure, which allocates the constraintSet and constraintSet arrays and
 * 			sets the objective function to NULL
 * 		initConstraint, which collects the optimizers constraintSet set
 * 		initObjective, which sets the optimizes objective function
 * 		loadConstraint, which activates the constraints used in a particular
 * 			optimization
 * 		optimize, which configures an optimization, loads the objective function
 * 			and constraints, solves the optimization problem and deals with the
 * 			solution
 * The template parameter T is a specific optimization type. An object of this
 * type will
 * 		configure the object by setting up the optimization data structure
 * 		set up the shared schedules
 * 		configure one or more optimization problems
 * 		handleSolution (or no solution) from optimization solver
 */

namespace model {

class Optimizer
{
public:
	virtual ~Optimizer(){};

	virtual void initObjective(ObjectiveFunction * objective)           = 0;
	virtual void initConstraint(Constraint * constraint, INTEGER con)   = 0;
};

} /* END OF NAMESPACE model */

#endif /* OPTIMIZER_HPP_ */
