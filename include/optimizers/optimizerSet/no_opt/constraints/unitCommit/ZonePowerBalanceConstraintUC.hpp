/* ***************************************************************************
 * Author      : Kevin Glass
 * Date        : Jan 11, 2011
 * File        : ZonePowerBalanceConstraintUC.hpp
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

#ifndef ZPBUC_HPP_
#define ZPBUC_HPP_

#include "structures.hpp"
#include "optimization_structs.hpp"
#include "optimizers/Constraint.hpp"

namespace model {

class ZonePowerBalanceConstraintUC : public Constraint
{
public:
	ZonePowerBalanceConstraintUC(UCData & data);
	virtual ~ZonePowerBalanceConstraintUC();
	virtual void load() {};
protected:
private:
	UCData              & data;

	ZonePowerBalanceConstraintUC(ZonePowerBalanceConstraintUC & gssc) : data(gssc.data){};
	ZonePowerBalanceConstraintUC & operator = (ZonePowerBalanceConstraintUC & gssc){return *this;};
};

} /* END OF NAMESPACE model */

#endif /* ZPBUC_HPP_ */
