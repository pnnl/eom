/*
 * RampConstraintLF.hpp
 *
 *  Created on: Jan 15, 2011
 *      Author: kevin
 */

#ifndef RAMPCONSTRAINTLF_HPP_
#define RAMPCONSTRAINTLF_HPP_

#include "structures.hpp"
#include "optimization_structs.hpp"
#include "optimizers/Constraint.hpp"

namespace model {

class RampConstraintLF : public Constraint
{
public:
	RampConstraintLF(LFData & data) :
		data(data){};
	virtual ~RampConstraintLF();

	virtual void load();
	virtual void printErrorState();
protected:
private:
	LFData                 & data;

	RampConstraintLF(RampConstraintLF & rc) :
		data(rc.data){};
	RampConstraintLF & operator = (RampConstraintLF & rc){return *this;};
};

} /* END OF NAMESPACE model */

#endif /* RAMPCONSTRAINTLF_HPP_ */
