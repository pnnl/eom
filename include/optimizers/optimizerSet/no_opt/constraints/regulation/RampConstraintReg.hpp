/*
 * RampConstraintReg.hpp
 *
 *  Created on: Jan 13, 2011
 *      Author: kevin
 */

#ifndef RAMPCONSTRAINTREG_HPP_
#define RAMPCONSTRAINTREG_HPP_

#include "structures.hpp"
#include "optimization_structs.hpp"
#include "optimizers/Constraint.hpp"

namespace model {

class RampConstraintReg: public Constraint {
public:
	RampConstraintReg(RegData & data) :
		data(data){};
	virtual ~RampConstraintReg();

	virtual void load();
	virtual void printErrorState();
protected:
private:
	RegData                & data;

	RampConstraintReg(RampConstraintReg & rc)  :
		data(rc.data){};
	RampConstraintReg & operator = (RampConstraintReg & RampConstraintReg){return *this;};
};

} /* END OF NAMESPACE model */

#endif /* RAMPCONSTRAINTREG_HPP_ */
