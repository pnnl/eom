/* ***************************************************************************
 * Author      : Kevin Glass
 * Date        : Jan 10, 2011
 * File        : PowerLimitConstraintUC.hpp
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

#ifndef POWERLIMITCONSTRAINTUC_HPP_
#define POWERLIMITCONSTRAINTUC_HPP_

#include "structures.hpp"
#include "optimization_structs.hpp"
#include "optimizers/Constraint.hpp"

namespace model {

class PowerLimitConstraintUC : public Constraint
{
public:
	PowerLimitConstraintUC(UCData & data);
	virtual ~PowerLimitConstraintUC();
	virtual void load() {};
protected:
private:
	UCData                 & data;

	PowerLimitConstraintUC(PowerLimitConstraintUC & plc) : data(plc.data){};
	PowerLimitConstraintUC & operator = (PowerLimitConstraintUC & plc){return *this;};
};

} /* END OF NAMESPACE model */

#endif /* POWERLIMITCONSTRAINTUC_HPP_ */
