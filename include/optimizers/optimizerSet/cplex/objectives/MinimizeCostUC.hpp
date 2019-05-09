
#ifndef MINIMIZECOSTUC_HPP_
#define MINIMIZECOSTUC_HPP_

#include "structures.hpp"
#include "optimization_structs.hpp"
#include "optimizer_structs.hpp"
#include "optimizers/ObjectiveFunction.hpp"
#include "universe.hpp"
#include <boost/format.hpp>
namespace model {

class MinimizeCostUC: public ObjectiveFunction
{
public:

	MinimizeCostUC(Universe & universe, UCData & data, UCOptData & optData):
		universe(universe), data(data), optData(optData){};

	virtual ~MinimizeCostUC(){};

	virtual void load();
	virtual void priceConstraint(){};
	virtual void printErrorState();
protected:
private:
	Universe  & universe;
	UCData                 & data;
	UCOptData              & optData;
};

} /* END OF NAMESPACE model */

#endif /* MINIMIZECOSTUC_HPP_ */
