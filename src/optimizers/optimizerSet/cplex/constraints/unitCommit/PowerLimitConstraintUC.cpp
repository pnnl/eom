#include "constraints/unitCommit/PowerLimitConstraintUC.hpp"
#include "boost/format.hpp"
#include "optimizer_structs.hpp"
#include "optimization_structs.hpp"
#include "universe.hpp"
namespace model {

PowerLimitConstraintUC::PowerLimitConstraintUC(Universe & universe, UCData & data, UCOptData & optData): UnitCommitmentConstraint(data, optData), universe(universe) {};

void
PowerLimitConstraintUC::load(/*IloRangeArray &constraintsOfInterestInDual*/)
{
	//std::cout<<"r8"<<std::endl;
	// this ensures the power output is not more than the minimum or maximum
	// achievable limits based on the generator ramp rate
	for (size_t step = 0; step < data.ucLength; step++){
		for (size_t gen = 0; gen < data.nGenerators; gen++){
			IloBoolVar & isOn = optData.iloSegmentIsOn[step][gen];
			INTEGER muston=data.mustbeon[gen];
			if (muston==1){
				IloRange mustongenerator(isOn == 1);
				addRangeConstraint(mustongenerator, "muston[%d][%d]", step, gen);
			}
			for (size_t segment = 0; segment < data.costCurve[gen].nSegments; segment++) {
				IloNumVar & power = optData.iloSegmentPower[step][gen][segment];
				FLOAT min = data.costCurve[gen].segment[segment].startPoint[POWER];
				FLOAT max = data.costCurve[gen].segment[segment].endPoint[POWER];
				if (universe.airTemperatureDeratingTable.size() != 0){
					double deratfac = universe.getderatfactor(data.time[step], data.baId, gen);
					min = deratfac*min;
					max = deratfac*max;
				}
				if (max - min > 1){
					if (segment == 0){
						IloRange minimumPowerConstraint = isOn*min - power <= 0;
						IloRange maximumPowerConstraint = power - isOn*max <= 0;
						addRangeConstraint(minimumPowerConstraint, "MinimumPower[%d][%d][%d]", step, gen, segment);
						addRangeConstraint(maximumPowerConstraint, "MaximumPower[%d][%d][%d]", step, gen, segment);
					} else {
						IloRange minimumPowerConstraint = - power <= 0;
						IloRange maximumPowerConstraint = power - isOn*(max-min) <= 0;
						addRangeConstraint(minimumPowerConstraint, "MinimumPower[%d][%d][%d]", step, gen, segment);
						addRangeConstraint(maximumPowerConstraint, "MaximumPower[%d][%d][%d]", step, gen, segment);
					}

				} else if (segment == 0) {
					IloRange minimumPowerConstraint = isOn*max - power <= 0;
					IloRange maximumPowerConstraint = power - isOn*max <= 0;
					addRangeConstraint(minimumPowerConstraint, "MinimumPower[%d][%d][%d]", step, gen, segment);
					addRangeConstraint(maximumPowerConstraint, "MaximumPower[%d][%d][%d]", step, gen, segment);

				}
			}
		}
	}
}

} /* END OF NAMESPACE model */
