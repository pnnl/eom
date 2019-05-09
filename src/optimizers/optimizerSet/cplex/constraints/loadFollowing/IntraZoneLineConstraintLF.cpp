/* ***************************************************************************
 * Author      : Kevin Glass
 * Date        : May 8, 2012
 * File        : IntraZoneLineConstraintLF.cpp
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

#include "constraints/loadFollowing/IntraZoneLineConstraintLF.hpp"
#include "simulation/Clock.hpp"

namespace model {

void
IntraZoneLineConstraintLF::IntraZoneLineConstraintLF::load()
{
	/*
	 * foreach step step in the projection
	 * 		foreach z = zone in the BA zone set
	 * 			foreach line between z' = {z each connection to z IN BA zone set}
	 * 				zonePTDF[z][z']Pout[z] <= zoneLine[z][z']
	 */
	for (INTEGER step = 0; step < lfData.lfLength; ++step) {
		for (INTEGER zone = 0; zone < lfData.nZones; ++zone) {
			for (INTEGER connection = 0; connection < lfData.nZones; ++connection) {
				optData.iloZoneIntraLineConstraint[step][zone][connection] =
						(lfData.zonePTDF[zone][connection]*optData.iloZonePowerTransferOut[step][zone] <= lfData.zoneMaxLinePower[zone][connection]);
			}
		}
	}
}

void
IntraZoneLineConstraintLF::printErrorState()
{
}

} /* END OF NAMESPACE model */
