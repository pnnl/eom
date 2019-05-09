/* ***************************************************************************
 * Author      : Kevin Glass
 * Date        : Jul 12, 2010
 * File        : PowerBalanceLF.cpp
 * Project     : rim
 *
 *
 * Contents    :
 *
 * Assumptions :
 *
 * ---------------------------------------------------------------------------
 */

#include "constraints/regulation/PowerBalanceConstraintReg.hpp"

namespace model {
/*
 * optimizer variables
 *
 *		generatorData[j].output.outputPower  -- generator
 *		onOffSchedule                        -- scheduleData array
 *		isLoadFollower                       -- generator parameter
 *
 *		generator power values, use signed values
 *		availableDeltaPower                       -- Cplex constant---sum of possible DeltaP of on generators
 *		generatorDeltaPower                  -- Cplex variable -- minimization variable, bound by signed [DeltaMinP, DeltaMaxP]
 *
 *		Load related values -- use abs of DeltaL
 *		isRampingUp                          -- sign of LF delta projection
 *		loadFollowingDeltaProjection         -- abs of LF delta projection
 *
 *		powerLoadBalance                     -- Cplex variable -- an unsigned value (
 *
 */

void
PowerBalanceConstraintReg::load()
{
	// the balancing authority has the 72 hour load projection
	/*
	for (INTEGER step = 0; step < data.regLength; step++){
		optData.iloPowerLoadBalance[step] =
				IloNumExprArg(IloSum(optData.iloGenPower[step]) ==
				(data.adjustedLoad[step]));

		optData.control.iloModel.add(optData.iloPowerLoadBalance[step]);
	}
		*/

}

void
PowerBalanceConstraintReg::printErrorState()
{
}

} /* END OF NAMESPACE model */
