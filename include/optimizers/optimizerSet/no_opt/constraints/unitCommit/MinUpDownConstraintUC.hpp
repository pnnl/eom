/* ***************************************************************************
 * Author      : Kevin Glass
 * Date        : Jan 10, 2011
 * File        : MinUpDownConstraintUC.hpp
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
#ifndef MINUPDOWNCONSTRAINTUC_HPP_
#define MINUPDOWNCONSTRAINTUC_HPP_

#include "structures.hpp"
#include "optimization_structs.hpp"
#include "optimizers/Constraint.hpp"

namespace model {

class MinUpDownConstraintUC: public Constraint
{
public:
	MinUpDownConstraintUC(UCData & data);
	virtual ~MinUpDownConstraintUC();
	virtual void load() {};
protected:
private:
	UCData                 & data;

	MinUpDownConstraintUC(MinUpDownConstraintUC & mudc) : data(mudc.data){};
	MinUpDownConstraintUC & operator = (MinUpDownConstraintUC & mudc){return *this;};
};

} /* END OF NAMESPACE model */

#endif /* MINUPDOWNCONSTRAINTUC_HPP_ */
