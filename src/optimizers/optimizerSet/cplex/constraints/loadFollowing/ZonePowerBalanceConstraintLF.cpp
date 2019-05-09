/* ***************************************************************************
 * Author      : Kevin Glass
 * Date        : May 16, 2012
 * File        : ZonePowerBalanceConstraintLF.cpp
 * Project     : rim
 *
 * Purpose:
 * This is a constraint class responsible for constraining the capacity of
 * power lines between zones. The constraint is:
 *
 *  d[i][j]Pout[i] <= Line[i][j] i,j IN ZoneSet
 *
 * Contents    :
 *
 * Assumptions :
 * ---------------------------------------------------------------------------
 */

#include "constraints/loadFollowing/ZonePowerBalanceConstraintLF.hpp"
#include "simulation/Clock.hpp"

namespace model {

void
ZonePowerBalanceConstraintLF::load()
{
	for (INTEGER step = 0; step < lfData.lfLength; ++step) {
		for (INTEGER gen = 0; gen < lfData.nGenerators; ++gen) {
			/*
			optData.iloZonePowerConstraint[step][zone] =
					IloNumExprArg(IloSum(optData.iloGenPower[gen]) ==
							lfData.adjustedZoneLoad[zone][step]
							+ optData.iloZonePowerTransferOut[step][zone]
							- optData.iloZonePowerTransferIn[step][zone]);
							*/
		}
	}
}

void
ZonePowerBalanceConstraintLF::printErrorState()
{
}

} /* END OF NAMESPACE model */
