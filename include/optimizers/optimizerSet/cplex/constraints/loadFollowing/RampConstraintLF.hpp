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
#include "optimizer_structs.hpp"
#include "optimizers/Constraint.hpp"

namespace model {

class RampConstraintLF : public Constraint
{
public:
	RampConstraintLF(UCData & ucData, LFData & lfData, LFOptData & optData) :
		ucData(ucData),
		lfData(lfData),
		optData(optData){};
	virtual ~RampConstraintLF(){};

	virtual void load();
	virtual void printErrorState();
protected:
private:
	UCData                 & ucData;
	LFData                 & lfData;
	LFOptData              & optData;


	RampConstraintLF(RampConstraintLF & rc) :
		ucData(rc.ucData),
		lfData(rc.lfData),
		optData(rc.optData){};
	RampConstraintLF & operator = (RampConstraintLF & rc){return *this;};
};

} /* END OF NAMESPACE model */

#endif /* RAMPCONSTRAINTLF_HPP_ */
