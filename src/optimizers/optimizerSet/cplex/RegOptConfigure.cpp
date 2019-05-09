/*
 * RegOptConfigure.cpp
 *
 *  Created on: Nov 28, 2011
 *      Author: kglass
 */


#include "Optimization.hpp"
#include "optimizer_structs.hpp"
#include "cplex_data.hpp"
#include "optimizers/optimization/RegulationOpt.hpp"
#include "objectives/MinimizeCostReg.hpp"
#include "constraints/regulation/PowerBalanceConstraintReg.hpp"
#include "constraints/regulation/RampConstraintReg.hpp"

using std::ostream;
using std::endl;

namespace model {

// ############################################################################
// ## CHECK SOLUTION
// ############################################################################
void
RegulationOpt::handleSolution()
{
	// dump results to lfPowerTarget
/*
	for (int32_t gen = 0; gen < data.nGenerators; gen++){
//			data.lfPowerTarget[gen][step] = (FLOAT)optData.control.iloCplex.getValue(optData.iloGenPower[step][gen]);
	}

#if defined RIM_DEBUG
	printState(cerr);
	printParameters(cerr);
	printOptimizer(cerr);
	printOptimzationVariables(cerr);
#endif
	*/
}

void
RegulationOpt::handleNoSolution()
{
	/*
	 * Each on generator is increase by its maximum or minimums step
	 * The new load - total generator power is translated to a frequency change
	 */
}

// ############################################################################
// ############################################################################
// ############################################################################
// ############################################################################
void
RegulationOpt::printOptimzationVariables(ostream & out)
{
	out << "REGULATION OPTIMIZATION VARIABLES" << endl;
	/*
	for (INTEGER step = 0; step < data.ucLength; step++) {
		cerr << optData.iloPowerLoadBalance[step] << endl;
		cerr << optData.iloAvailablePower[step] << endl;
		cerr << optData.iloReserve[step] << endl;
		for (INTEGER gen = 0; gen < data.nGenerators; gen++) {
			cerr << optData.iloGenPower[step][gen] << endl;

			cerr << optData.iloGenIsOn[step][gen] << endl;

			cerr << optData.iloGenStartSupport[step][gen] << endl;
			cerr << optData.iloGenStopSupport[step][gen] << endl;

			cerr << optData.iloPowerLimit[step][gen] << endl;
			cerr << optData.iloPowerOffConstraint[step][gen] << endl;


			cerr << optData.iloOnOffError[step][gen] << endl;
			cerr << optData.iloStartStopConstraint[step][gen] << endl;

			cerr << optData.iloRampUpConstraint[step][gen] << endl;
			cerr << optData.iloRampDownConstraint[step][gen] << endl;

			cerr << optData.iloRampStatus[step][gen] << endl;

			cerr << optData.iloInitialMinUpTime << endl;
			cerr << optData.iloInitialMinDownTime << endl;
			cerr << optData.iloMinDownTimeConstraint[step][gen] << endl;
			cerr << optData.iloMinUpTimeConstraint[step][gen] << endl;
			cerr << optData.iloGeneratorMustBeOn[step][gen] << endl;
			cerr << optData.iloGeneratorMustBeOff[step][gen] << endl;

			cerr << optData.iloZCoeffConstraint[step][gen] << endl;
			cerr << optData.iloZValueConstraint[step][gen] << endl;
			for (INTEGER bp = 0; bp < x; bp++) {
				cerr << optData.iloZCoeff[step][gen][bp] << endl;
			}

		}
	}
	*/
}

} /* END OF NAMESPACE model */

