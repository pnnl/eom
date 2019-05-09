#include "constraints/unitCommit/ZonePowerBalanceConstraintUC.hpp"

namespace model {

void
ZonePowerBalanceConstraintUC::load()
{
	//std::cout<<"r11"<<std::endl;
	for (size_t step = 0; step < data.ucLength; ++step) {
		for (size_t zone = 0; zone < data.nZones; ++zone) {
			IloNumExpr totalReceived = IloNumExpr(optData.control.iloEnv, 0);
			IloNumExpr zonePower = IloNumExpr(optData.control.iloEnv, 0);

			for (size_t zoneTo = 0; zoneTo < data.nZones; ++zoneTo){
				if (zone!=zoneTo){
					totalReceived =totalReceived + optData.iloZonePowerTransferInPerZone[step][zoneTo][zone]-optData.iloZonePowerTransferInPerZone[step][zone][zoneTo];
				 
				}

			}


			for (INTEGER gen = 0; gen < data.zoneNumberOfGenerators[zone]; ++gen) {
				int genIndex = gen + data.zoneGeneratorOffset[zone];
				zonePower += optData.iloGenPower[step][genIndex];
			}
			//IloNumExpr & powerIn = optData.iloZonePowerTransferIn[step][zone];
			//IloNumVar & powerOut = optData.iloZonePowerTransferOut[step][zone];
			double load = data.adjustedZoneLoad[zone][step];
			//cout << "adjustedZoneLoad[" << zone << "][" << step << "] = " << load << endl;
			IloRange zonePowerBalanceConstraint (zonePower - totalReceived - load == 0);
			addRangeConstraint(zonePowerBalanceConstraint, "ZonePowerBalance[%d][%d]", step, zone);
		}
	}
}

} /* END OF NAMESPACE model */
