
/* ***************************************************************************
 * Author      : Kevin Glass
 * Date        : Jul 14, 2010
 * File        : MinimizeCostReg.cpp
 * Project     : rim
 *
 * Inheritance
 *
 * Class members
 *
 * Class variables
 *
 * Purpose
 *
 ************************************************************************** */

#include "objectives/MinimizeCostReg.hpp"

namespace model {
/*
 * optimizer variables
 *
 *		iLoModel                             -- sum of DeltaP * cost without sign
 *		onOffSchedule                        -- scheduleData array
 *		isLoadFollower                       -- generator parameter
 *		lfCoeffMUp                           -- unsgined scalars
 *		lfCoeffMDown                         -- unsgined scalars
 *
 *		generator power values, use signed values
 *		generatorDeltaPower                  -- Cplex variable -- minimization variable, bound by signed [DeltaMinP, DeltaMaxP]
 *
 */
void
MinimizeCostReg::load()
{
	/*
	priceConstraint();
	for (int32_t time = 0; time < data.regLength; time++) {
		for (int32_t gen = 0; gen < data.nGenerators; gen++){
		*/
			// NOTE: mxb[INTERCEPT] is really (delta C)

			/*
			NEED TO REPLAC genIsOn
			optData.control.iloObj +=
					(data.costCurve[gen].segment[0].startPoint[INDEPENDENT]*optData.iloGenIsOn[time][gen])+
					(optData.iloZCoeff[time][gen][0]*data.costCurve[gen].segment[0].intercept);
			*/
			 /*
		}
	}
	*/
}


void
MinimizeCostReg::printErrorState()
{
}

} /* END OF NAMESPACE model */
