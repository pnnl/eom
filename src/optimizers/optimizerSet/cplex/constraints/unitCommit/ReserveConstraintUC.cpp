#include "constraints/unitCommit/ReserveConstraintUC.hpp"
#include "boost/format.hpp"
#include "optimizer_structs.hpp"
#include "optimization_structs.hpp"
#include "universe.hpp"
namespace model {
ReserveConstraintUC::ReserveConstraintUC(Universe & universe, UCData & data, UCOptData & optData): UnitCommitmentConstraint(data, optData), universe(universe) {};

void
ReserveConstraintUC::load()
{
	//std::cout<<"r10"<<std::endl;
	/*
	IloNumArray maxPower = IloNumArray(optData.control.iloEnv, data.nConventionalGenerators);
	for (INTEGER gen = 0; gen < data.nGenerators; gen++) {
		maxPower[gen] = data.genPowerMaxLimit[gen];
	}
	*/
	if (data.reserveFactor>0){
		for (size_t step = 0; step < data.ucLength; step++) {
			FLOAT requiredAvailablePower = (1.+data.reserveFactor)*data.adjustedBALoad[step];
		//IloExpr availablePower = IloScalProd(optData.iloGenIsOn[step], maxPower);
			IloExpr availablePower = IloExpr(optData.control.iloEnv);
			for (size_t gen = 0; gen < data.nGenerators; gen++) {
				double deratfac = 1;
				if (universe.airTemperatureDeratingTable.size() != 0){
					deratfac = universe.getderatfactor(data.time[step], data.baId, gen);
				}
				if (data.costCurve[gen].segment[data.costCurve[gen].nSegments-1].endPoint[POWER]<10000){
					availablePower += deratfac *data.genPowerMaxLimit[gen] *optData.iloSegmentIsOn[step][gen];
				}
			}
			IloRange reserveConstraint(availablePower >= requiredAvailablePower);
			addRangeConstraint(reserveConstraint, "Reserve[%d]", step);
		}
	}
}

} /* END OF NAMESPACE model */
