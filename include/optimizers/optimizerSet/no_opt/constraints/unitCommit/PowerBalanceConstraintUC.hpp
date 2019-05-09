/* ***************************************************************************
 * Author      : Kevin Glass
 * Date        : Dec 29, 2010
 * File        : PowerBalanceConstraintUC.hpp
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

#ifndef POWERBALANCECONSTRAINTUC_HPP_
#define POWERBALANCECONSTRAINTUC_HPP_

#include "structures.hpp"
#include "optimization_structs.hpp"
#include "optimizers/Constraint.hpp"

namespace model {

class PowerBalanceConstraintUC : public Constraint
{
public:
	PowerBalanceConstraintUC(UCData & data);
	virtual ~PowerBalanceConstraintUC();
	virtual void load() {};
protected:
private:
	UCData                 & data;


	PowerBalanceConstraintUC(PowerBalanceConstraintUC & pbc) : data(pbc.data){};
	PowerBalanceConstraintUC & operator = (PowerBalanceConstraintUC & pbc){return *this;};
};

} /* END OF NAMESPACE model */

#endif /* POWERBALANCECONSTRAINTUC_HPP_ */
