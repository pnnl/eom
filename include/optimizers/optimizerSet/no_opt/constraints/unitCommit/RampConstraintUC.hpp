/* ***************************************************************************
 * Author      : Kevin Glass
 * Date        : Dec 30, 2010
 * File        : RampConstraintUC.hpp
 * Project     : RIM
 *
 * PURPOSE:
 *
 *
 * STATUS:
 *     naming convention conformity
 *     doxygen comments
 *     private comments
 *     comment block conformity
 *     testing information
 *     ensure hidden methods
 *
 * TESTING:
 *
 * ---------------------------------------------------------------------------
 */

#ifndef RAMPCONSTRAINTUC_HPP_
#define RAMPCONSTRAINTUC_HPP_

#include "structures.hpp"
#include "optimization_structs.hpp"
#include "optimizers/Constraint.hpp"

namespace model {

class RampConstraintUC: public Constraint {
public:
	RampConstraintUC(UCData & data);
	virtual ~RampConstraintUC();
	virtual void load() {};
protected:
private:
	UCData                 & data;

	RampConstraintUC(RampConstraintUC & cons) : data(cons.data){};
	RampConstraintUC & operator = (RampConstraintUC & cons){return *this;};
};

} /* END OF NAMESPACE model */

#endif /* RAMPCONSTRAINTUC_HPP_ */
