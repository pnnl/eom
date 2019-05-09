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
#include "optimizers/Constraint.hpp"

namespace model {

class PowerBalanceConstraintLF : public Constraint
{
public:
	PowerBalanceConstraintLF(LFData & data) :
		data(data){};
	virtual ~PowerBalanceConstraintLF();

	virtual void load();
	virtual void printErrorState();
protected:
private:
	LFData                 & data;


	PowerBalanceConstraintLF(PowerBalanceConstraintLF & rc) :
		data(rc.data){};
	PowerBalanceConstraintLF & operator = (PowerBalanceConstraintLF & rc){return *this;};
};

} /* END OF NAMESPACE model */

#endif /* POWERBALANCECONSTRAINTLF_HPP_ */
