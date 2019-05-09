/* ***************************************************************************
 * Purpose:
 * Constrain the power lines between zones using the PTDF model.
 */
#include "constraints/unitCommit/InterzonePowerTransferDistributionFactorUC.hpp"

namespace model {

void
InterzonePowerTransferDistributionFactorUC::load()
{
	//std::cout<<"r4"<<std::endl;
	for (size_t step = 0; step < data.ucLength; ++step) {
		for (size_t zoneFrom = 0; zoneFrom < data.nZones; ++zoneFrom) {
			for (size_t zoneTo = 0; zoneTo < data.nZones; ++zoneTo) {
				//cout << "zonePTDF[from=" << zoneFrom << "][to=" << zoneTo << "] = " << this->data.zonePTDF[zoneFrom][zoneTo] << endl;
				IloNumVar &powerReceived = optData.iloZonePowerTransferInPerZone[step][zoneTo][zoneFrom];
				IloNumVar &powerSurplus = optData.iloZonePowerTransferOut[step][zoneFrom];
				double ptdf = this->data.zonePTDF[zoneFrom][zoneTo];
				IloRange ptdfConstraint(powerReceived - ptdf * powerSurplus == 0);
				addRangeConstraint(ptdfConstraint, "ptdf[%d][%d][%d]", step, zoneFrom, zoneTo);
			}
		}
	}
}

} /* END OF NAMESPACE model */
