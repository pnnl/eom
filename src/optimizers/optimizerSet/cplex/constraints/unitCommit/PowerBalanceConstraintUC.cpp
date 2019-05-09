#include "constraints/unitCommit/PowerBalanceConstraintUC.hpp"
#include "exceptions/Exception.hpp"
#include "exceptions/OptimizationException.hpp"

using std::cerr;
using std::endl;

namespace model {

void
PowerBalanceConstraintUC::load()
{
	//std::cout<<"r7"<<endl;
	for (size_t step = 0; step < data.ucLength; step++){
		try {
			double load = data.adjustedBALoad[step];
			IloNumExpr power = IloSum(optData.iloGenPower[step]);
			IloRange powerBalanceConstraint(power - load == 0);
			addRangeConstraint(powerBalanceConstraint, "PowerBalance[%d]", step);
		} catch (IloException & e) {
			cerr << "\tERROR: PowerBalanceConstraintUC::load() failed to load" << endl;
			cerr << "\tPowerBalanceConstraintUC load failed\n\t" << e.getMessage() << endl;
			throw OptimizationException(__FILE__, __LINE__, Exception::CONSTRAINT_LOAD_FAILURE);
		} catch (...) {
			cerr << "\tERROR: PowerBalanceConstraintUC::load() failed to load" << endl;
			throw OptimizationException(__FILE__, __LINE__, Exception::CONSTRAINT_LOAD_FAILURE);
		}
	}
}

} /* END OF NAMESPACE model */
