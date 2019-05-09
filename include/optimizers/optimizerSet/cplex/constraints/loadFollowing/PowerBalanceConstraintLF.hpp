/*
 * PowerBalanceConstraintLF.hpp
 *
 *  Created on: Jan 15, 2011
 *      Author: kevin
 */

#ifndef POWERBALANCECONSTRAINTLF_HPP_
#define POWERBALANCECONSTRAINTLF_HPP_

#include "structures.hpp"
#include "optimization_structs.hpp"
#include "optimizer_structs.hpp"
#include "optimizers/Constraint.hpp"

namespace model {

class PowerBalanceConstraintLF : public Constraint
{
public:
	PowerBalanceConstraintLF(UCData & ucData, LFData & lfData, LFOptData & optData) :
		ucData(ucData),
		lfData(lfData),
		optData(optData){};
	virtual ~PowerBalanceConstraintLF(){};

	virtual void load();
	virtual void printErrorState();
protected:
private:
	UCData                 & ucData;
	LFData                 & lfData;
	LFOptData              & optData;

	PowerBalanceConstraintLF(PowerBalanceConstraintLF & rc) :
		ucData(rc.ucData),
		lfData(rc.lfData),
		optData(rc.optData){};
	PowerBalanceConstraintLF & operator = (PowerBalanceConstraintLF & rc){return *this;};
};

} /* END OF NAMESPACE model */

#endif /* POWERBALANCECONSTRAINTLF_HPP_ */
