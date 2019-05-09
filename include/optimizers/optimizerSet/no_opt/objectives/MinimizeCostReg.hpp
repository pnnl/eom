/*
 * MinimizeCostReg.hpp :  NO OPTIMIZATION
 *
 *  Created on: Jan 13, 2011
 *      Author: kevin
 */

#ifndef MINIMIZECOSTREG_HPP_
#define MINIMIZECOSTREG_HPP_

#include "structures.hpp"
#include "optimization_structs.hpp"
#include "optimizers/ObjectiveFunction.hpp"

namespace model {

class MinimizeCostReg: public ObjectiveFunction {
public:
	MinimizeCostReg(RegData & data);
	virtual ~MinimizeCostReg();
protected:
	virtual void load()                                       = 0;
	virtual void reload()                                     = 0;
	virtual void printErrorState()                            = 0;

private:
	RegData                & data;
};

} /* END OF NAMESPACE model */

#endif /* MINIMIZECOSTREG_HPP_ */
