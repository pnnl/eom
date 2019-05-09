/*
 * MinimizeCostReg.hpp
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
	MinimizeCostReg(RegData & data){};
	/*
		MinimizeCostReg(RegData & data, RegOptData & optData):
			data(data),
			optData(optData){};
	*/
	virtual ~MinimizeCostReg(){};

		virtual void load();
		virtual void priceConstraint(){};
		virtual void printErrorState();
	protected:
private:
//	RegData                & data;
//	RegOptData             & optData;
};

} /* END OF NAMESPACE model */

#endif /* MINIMIZECOSTREG_HPP_ */
