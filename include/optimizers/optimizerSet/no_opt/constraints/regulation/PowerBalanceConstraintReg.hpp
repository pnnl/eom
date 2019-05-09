/*
 * PowerBalanceConstraintReg.hpp
 *
 *  Created on: Jan 15, 2011
 *      Author: kevin
 */

#ifndef POWERBALANCECONSTRAINTREG_HPP_
#define POWERBALANCECONSTRAINTREG_HPP_

#include "structures.hpp"
#include "optimization_structs.hpp"
#include "optimizers/Constraint.hpp"

namespace model {

class PowerBalanceConstraintReg : public Constraint
{
public:
	PowerBalanceConstraintReg(RegData & data) :
		data(data){};
	virtual ~PowerBalanceConstraintReg();

	virtual void load();
	virtual void printErrorState();
protected:
private:
	RegData                & data;

	PowerBalanceConstraintReg(PowerBalanceConstraintReg & rc)  :
		data(rc.data){};
	PowerBalanceConstraintReg & operator = (PowerBalanceConstraintReg & rc){return *this;};
};

} /* END OF NAMESPACE model */

#endif /* POWERBALANCECONSTRAINTREG_HPP_ */
