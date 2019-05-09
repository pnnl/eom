/* ***************************************************************************
 * Purpose:
 * This is a constraint class responsible for constraining the capacity of
 * power lines between zones. The constraint is:
 *
 *  d[i][j]Pout[i] <= Line[i][j] i,j IN ZoneSet
 */

#include "constraints/unitCommit/InterzoneMaxLinePowerUC.hpp"

namespace model {

void
InterzoneMaxLinePowerUC::load()
{
	//std::cout<<"r3"<<std::endl;
	/*
	 * foreach time step in the projection
	 * 		foreach z = zone in the BA zone set
	 * 			foreach line between z' = {z each connection to z IN BA zone set}
	 * 				zonePTDF[z][z']Pout[z] <= zoneLine[z][z']
	 */
	for (size_t step = 0; step < data.ucLength; ++step) {
		for (size_t zoneFrom = 0; zoneFrom < data.nZones; ++zoneFrom) {
			for (size_t zoneTo = 0; zoneTo < data.nZones; ++zoneTo) {
				//cout << "zoneMaxLinePower[from=" << zoneFrom << "][to=" << zoneTo << "] = " << data.zoneMaxLinePower[zoneFrom][zoneTo] << endl;
				if (zoneTo != zoneFrom) 
				{
					IloRange maxLinePowerConstraint(
					optData.iloZonePowerTransferInPerZone[step][zoneTo][zoneFrom]
					- optData.iloZonePowerTransferInPerZone[step][zoneFrom][zoneTo]
					<= data.zoneMaxLinePower[zoneFrom][zoneTo]
					);
					addRangeConstraint(maxLinePowerConstraint, "MaxLinePower[%d][%d][%d]", step, zoneFrom, zoneTo);
				}
				
			}
		}
	}
}

} /* END OF NAMESPACE model */
