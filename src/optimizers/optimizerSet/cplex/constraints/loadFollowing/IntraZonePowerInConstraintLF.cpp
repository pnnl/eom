/* ***************************************************************************
 * Author      : Kevin Glass
 * Date        : May 8, 2012
 * File        : IntraZonePowerInConstraintLF.cpp
 * Project     : rim
 *
 * Purpose:
 * This is a constraint class responsible for constraining the capacity of
 * power lines between zones. The basic premise is:
 *
 *  Sum{
 *
 * Contents    :
 *
 * Assumptions :
 * ---------------------------------------------------------------------------
 */

#include "constraints/loadFollowing/IntraZonePowerInConstraintLF.hpp"
#include "simulation/Clock.hpp"

namespace model {

void
IntraZonePowerInConstraintLF::load()
{
	/*
	 * 	Assumes PTDF's are positive
	 */
	for (INTEGER step = 0; step < lfData.lfLength; ++step) {
		for (INTEGER zone = 0; zone < lfData.nZones; ++zone) {
			optData.iloZonePowerOutNonnegativeConstraint[step][zone] = IloBoolVar(optData.iloZonePowerTransferOut[step][zone] >= 0.0);
			optData.control.iloModel.add(optData.iloZonePowerOutNonnegativeConstraint[step][zone]);
		}
	}
	for (INTEGER step = 0; step < lfData.lfLength; ++step) {
		for (INTEGER zone = 0; zone < lfData.nZones; ++zone) {
			for (INTEGER connection = 0; connection < lfData.nZones; ++connection) {
				optData.iloZonePowerInPerConnection[step][zone] =
						IloNumExpr(lfData.zonePTDF[connection][zone]*optData.iloZonePowerTransferOut[step][connection]);
				optData.control.iloModel.add(optData.iloZonePowerInPerConnection[step][zone]);
			}
		}
	}

	for (INTEGER step = 0; step < lfData.lfLength; ++step) {
		for (INTEGER zone = 0; zone < lfData.nZones; ++zone) {
			optData.iloZoneInConstraint[step][zone] =
					IloNumExprArg(IloSum(optData.iloZonePowerInPerConnection[step]) ==
							optData.iloZonePowerTransferIn[step][zone]);
			optData.control.iloModel.add(optData.iloZoneInConstraint[step][zone]);
		}
	}
}

void
IntraZonePowerInConstraintLF::printErrorState()
{
}

} /* END OF NAMESPACE model */
