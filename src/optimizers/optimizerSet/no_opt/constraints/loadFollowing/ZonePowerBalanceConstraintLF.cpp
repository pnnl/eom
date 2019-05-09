/* ***************************************************************************
 * Author      : Kevin Glass
 * Date        : Jul 12, 2010
 * File        : ZonePowerBalanceConstraintLF.cpp
 * Project     : rim
 *
 *
 * Contents    :
 *
 * Assumptions :
 *
 * ---------------------------------------------------------------------------
 */

#include "constraints/loadFollowing/ZonePowerBalanceConstraintLF.hpp"
#include "simulation/Clock.hpp"

namespace model {

void
ZonePowerBalanceConstraintLF::load()
{
}

void
ZonePowerBalanceConstraintLF::printErrorState()
{
	cerr << "NO ERROR STATE TO PRINT" << endl;
}

} /* END OF NAMESPACE model */
