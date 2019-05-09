#include "constraints/loadFollowing/PowerBalanceConstraintLF.hpp"
#include "simulation/SDInterface.hpp"
#include "exceptions/OptimizationException.hpp"

using std::cerr;
using std::endl;

namespace model {

void
PowerBalanceConstraintLF::load()
{
	// the balancing authority has the 72 hour load projection
	for (INTEGER step = 0; step < lfData.lfLength; step++){
		try {
			optData.control.iloModel.add(IloSum(optData.iloGenPower[step]) == lfData.adjustedBALoad[step]);
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

void
PowerBalanceConstraintLF::printErrorState()
{
}

} /* END OF NAMESPACE model */

