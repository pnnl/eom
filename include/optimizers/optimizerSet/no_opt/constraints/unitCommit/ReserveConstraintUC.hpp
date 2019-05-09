/* ***************************************************************************
 * Author      : Kevin Glass
 * Date        : Jan 10, 2011
 * File        : ReserveConstraintUC.hpp
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

#ifndef RESERVECONSTRAINTUC_HPP_
#define RESERVECONSTRAINTUC_HPP_

#include "structures.hpp"
#include "optimization_structs.hpp"
#include "optimizers/Constraint.hpp"

namespace model {

class ReserveConstraintUC : public Constraint
{
public:
	ReserveConstraintUC(UCData & data);
	virtual ~ReserveConstraintUC();
	virtual void load() {};
protected:
private:
	UCData                 & data;

	ReserveConstraintUC(ReserveConstraintUC & rc) : data(rc.data){};
	ReserveConstraintUC & operator = (ReserveConstraintUC & rc){return *this;};
};

} /* END OF NAMESPACE model */

#endif /* RESERVECONSTRAINTUC_HPP_ */
