/* ***************************************************************************
 * Author      : Kevin Glass
 * Date        : Jan 13, 2011
 * File        : MinimizeCostLF.hpp
 * Project     : RIM
 *
 * PURPOSE:
 *
 *
 * STATUS:
 *     naming convention conformity
 *     doxygen comments
 *     private comments
 *     comment block conformity
 *     testing information
 *     ensure hidden methods
 *
 * TESTING:
 *
 * ---------------------------------------------------------------------------
 */

#ifndef MINIMIZECOSTLF_HPP_
#define MINIMIZECOSTLF_HPP_

#include "structures.hpp"
#include "optimization_structs.hpp"
#include "optimizer_structs.hpp"
#include "optimizers/ObjectiveFunction.hpp"

namespace model {

class MinimizeCostLF: public ObjectiveFunction {
public:
	MinimizeCostLF(UCData & ucData, LFData & lfData, LFOptData & optData) {};

	/*
	MinimizeCostLF(LFData & data, LFOptData & optData):
		data(data),
		optData(optData){
		scheduleData = ScheduleData::getScheduleData();
	};
	*/
	virtual ~MinimizeCostLF(){};

	virtual void load();
	virtual void priceConstraint(){};
	virtual void printErrorState();

protected:
private:
};

} /* END OF NAMESPACE model */


#endif /* MINIMIZECOSTLF_HPP_ */
