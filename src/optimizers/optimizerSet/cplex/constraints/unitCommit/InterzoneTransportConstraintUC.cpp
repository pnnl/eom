#include "constraints/unitCommit/InterzoneTransportConstraintUC.hpp"

namespace model {

void
InterzoneTransportConstraintUC::load()
{
	//std::cout<<"r5"<<std::endl;
	for (size_t step = 0; step < data.ucLength; ++step) {
		for (size_t zoneFrom = 0; zoneFrom < data.nZones; ++zoneFrom) {
			IloNumExpr totalReceived = IloNumExpr(optData.control.iloEnv, 0);
			for (size_t zoneTo = 0; zoneTo < data.nZones; ++zoneTo) {
				totalReceived += optData.iloZonePowerTransferInPerZone[step][zoneTo][zoneFrom];
			}
			IloNumVar & totalSent = optData.iloZonePowerTransferOut[step][zoneFrom];
			IloRange transportConstraint(totalReceived - totalSent == 0);
			addRangeConstraint(transportConstraint, "transport[%d][%d]", step, zoneFrom);
		}
	}
}

} /* END OF NAMESPACE model */
