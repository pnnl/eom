/* ***************************************************************************
 * Author      : Kevin Glass
 * Date        : Jan 13, 2011
 * File        : MinimizeCostLF.hpp :  NO OPTIMIZATION
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
#include "optimizers/ObjectiveFunction.hpp"

namespace model {

class MinimizeCostLF: public ObjectiveFunction {
public:
	MinimizeCostLF(LFData & data);
	virtual ~MinimizeCostLF();
protected:
	virtual void load()                                       = 0;
	virtual void reload()                                     = 0;
	virtual void printErrorState()                            = 0;

private:
	LFData                 & data;
};

} /* END OF NAMESPACE model */


#endif /* MINIMIZECOSTLF_HPP_ */
