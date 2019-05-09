/*
 * PWConstraintReg.hpp
 *
 *  Created on: Jan 15, 2011
 *      Author: kevin
 */

#ifndef PWCONSTRAINTREG_HPP_
#define PWCONSTRAINTREG_HPP_

#include "structures.hpp"
#include "optimization_structs.hpp"
#include "optimizers/Constraint.hpp"

namespace model {

class PWConstraintReg : public Constraint
{
public:
	PWConstraintReg(RegData & data) :
		data(data){};
	virtual ~PWConstraintReg();

	virtual void load();
	virtual void printErrorState();
protected:
private:
	RegData                & data;

	PWConstraintReg(PWConstraintReg & plc)  :
		data(plc.data){};
	PWConstraintLF & operator = (PWConstraintLF & plc){return *this;};
};

} /* END OF NAMESPACE model */

#endif /* PWCONSTRAINTREG_HPP_ */
