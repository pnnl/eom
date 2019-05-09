/*
 * RegOptConfigure.cpp :  NO OPTIMIZATION
 *
 *  Created on: Nov 28, 2011
 *      Author: kglass
 */


#include "optimizers/optimization/RegulationOpt.hpp"

namespace model {
// ############################################################################
// ## CHECK SOLUTION
// ############################################################################
void
RegulationOpt::handleSolution()
{
}

void
RegulationOpt::handleNoSolution()
{
	/*
	 * This method is incomplete. It should:
	 * 		1) react to load > max possible generation by ramping all available
	 * 			generation to its maximum.
	 * 		2) react to load < min possible generation by ramping all available
	 * 			generation to its minimum
	 * This is not possible for the current optimization, so instead, it will
	 * terminate the code. In the future, the response of 'handleNoSolution'
	 * will be to optimize around the "out-of-bounds" loads.
	 *
	 * For the moment, this method will throw an exception, which will terminate
	 * the code.
	 */
	cerr << "WELL, that didn't work!!" << endl;
}

// ############################################################################
// ############################################################################
// ############################################################################
// ############################################################################

void
RegulationOpt::printOptimzationVariables(ostream & out)
{

}

#if defined RIM_DEBUG
void
RegulationOpt::printOptimizer(ostream & out)
{

}
#endif


} /* END OF NAMESPACE model */

