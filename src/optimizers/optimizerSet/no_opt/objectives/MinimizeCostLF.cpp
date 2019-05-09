/* ***************************************************************************
 * Author      : Kevin Glass
 * Date        : Jul 14, 2010
 * File        : MinimizeCostLF.cpp :  NO OPTIMIZATION
 *
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

#include "MinimizeCostLF.hpp"

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
MinimizeCostLF::load()
{
}

void
MinimizeCostLF::printErrorState()
{
}

} /* END OF NAMESPACE model */
