/* ***************************************************************************
 * Author      : Kevin Glass
 * Date        : Dec 29, 2010
 * File        : MinimizeCostUC.hpp :  NO OPTIMIZATION
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

#ifndef MINIMIZECOSTUC_HPP_
#define MINIMIZECOSTUC_HPP_

#include "structures.hpp"
#include "optimization_structs.hpp"
#include "optimizers/ObjectiveFunction.hpp"

namespace model {

class MinimizeCostUC: public ObjectiveFunction
{
public:
	MinimizeCostUC(UCData & data);
	virtual ~MinimizeCostUC();
protected:
	virtual void load();
	virtual void printErrorState();

private:
	UCData                 & data;
};

} /* END OF NAMESPACE model */

#endif /* MINIMIZECOSTUC_HPP_ */
