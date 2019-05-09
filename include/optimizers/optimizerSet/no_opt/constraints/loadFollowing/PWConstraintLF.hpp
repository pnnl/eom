/*
 * PWConstraintLF.hpp
 *
 *  Created on: Jan 15, 2011
 *      Author: kevin
 */

#ifndef PWCONSTRAINTLF_HPP_
#define PWCONSTRAINTLF_HPP_

#include "structures.hpp"
#include "optimization_structs.hpp"
#include "optimizers/Constraint.hpp"

namespace model {

class PWConstraintLF : public Constraint
{
public:
	PWConstraintLF(LFData & data) :
		data(data){};
	virtual ~PWConstraintLF();

	virtual void load();
	virtual void printErrorState();
protected:
private:
	LFData                 & data;

	PWConstraintLF(PWConstraintLF & rc) :
		data(rc.data){};
	PWConstraintLF & operator = (PWConstraintLF & rc){return *this;};
};

} /* END OF NAMESPACE model */

#endif /* PWCONSTRAINTLF_HPP_ */
