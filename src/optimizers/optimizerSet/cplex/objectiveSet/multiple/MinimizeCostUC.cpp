#include "objectives/MinimizeCostUC.hpp"
#include "universe.hpp"
namespace model {

void
MinimizeCostUC::load()
{
	for (size_t zone = 0; zone < data.nZones; zone++) {
		for (int zoneGenIndex = 0; zoneGenIndex < data.zoneNumberOfGenerators[zone]; zoneGenIndex++) {
			int gen = zoneGenIndex + data.zoneGeneratorOffset[zone];
			FLOAT switchOnCost = this->data.genTurnOnCost[gen];
			FLOAT switchOffCost = this->data.genTurnOffCost[gen];
			INTEGER nSegments = data.costCurve[gen].nSegments;
			for (size_t step = 0; step < data.ucLength; step++) {
				IloNumExpr fuelInputInMMBtu = IloNumExpr(optData.control.iloEnv);
				IloNumVar  & switchOn  = optData.iloGenStartSupport[step][gen];
				//IloNumVar  & switchOff = optData.iloGenStopSupport[step][gen];
				//IloNumExpr & genIsOn   = optData.iloSegmentIsOn[step][gen];
				//optData.control.iloModel.add(genIsOn <= 1); // prevent multiple segments // TODO looks like it's in the wrong place
				optData.control.iloObj += switchOn*switchOnCost + switchOn *switchOffCost;
				IloBoolVar & isOn      = optData.iloSegmentIsOn[step][gen];
				double deratfac = 1;
				if (universe.airTemperatureDeratingTable.size() != 0){
					deratfac = universe.getderatfactor(data.time[step], data.baId, gen);
				}
				for (INTEGER seg = 0; seg < nSegments; seg++) {
					IloNumVar  & power     = optData.iloSegmentPower[step][gen][seg];
					FLOAT minp = deratfac*data.costCurve[gen].segment[seg].startPoint[POWER];
					FLOAT maxp = deratfac*data.costCurve[gen].segment[seg].endPoint[POWER];
					FLOAT minc = deratfac*data.costCurve[gen].segment[seg].startPoint[COST];
					FLOAT maxc = deratfac*data.costCurve[gen].segment[seg].endPoint[COST];

					//std::cout<<"gen "<<gen<<"sec "<<seg<<"r7"<<minp<<"r7 "<<maxp<<"r7  "<<minc<<"r7 "<<maxc<<std::endl;
					if (maxp - minp > 1){
						if (seg == 0){
							fuelInputInMMBtu += (power - minp*isOn ) * maxc / maxp;
							//std::cout<<"gen "<<gen<<"sec "<<seg<<"r7"<<maxc<<std::endl;

						} else {
							fuelInputInMMBtu += power*(maxc/maxp + minc/minp)/2;
						}

					} 
				}
				
				FLOAT initialcost = deratfac*data.costCurve[gen].segment[0].startPoint[COST];


				IloNumExpr fuelCostInDollars = IloNumExpr(optData.control.iloEnv);
				if (universe.airTemperatureefficientTable.size() == 0) {
					fuelCostInDollars = isOn*initialcost + fuelInputInMMBtu;

				} else {

					double derateff = universe.efficientyderat(data.time[step], data.baId, gen);
					fuelCostInDollars = isOn*initialcost + fuelInputInMMBtu / derateff;
				}
				IloNumExpr poundsCarbonDioxideEmitted = fuelInputInMMBtu * data.co2Rate[gen];
				IloNumExpr co2CostInDollars = poundsCarbonDioxideEmitted * data.zoneCo2Cost[zone] / 2000.;
				IloNumExpr operationsAndMaintenanceVariableCost = data.operationsAndMaintenanceVariableCost[gen] * optData.iloGenPower[step][gen];
				optData.control.iloObj +=
						fuelCostInDollars +
						co2CostInDollars +
						operationsAndMaintenanceVariableCost;
			}
		}
	}
	optData.control.iloModel.add(IloMinimize(optData.control.iloEnv, optData.control.iloObj));
}

void
MinimizeCostUC::printErrorState()
{
}

} /* END OF NAMESPACE model */
