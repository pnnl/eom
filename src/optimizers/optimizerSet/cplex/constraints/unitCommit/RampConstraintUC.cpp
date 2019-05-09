#include "constraints/unitCommit/RampConstraintUC.hpp"

namespace model {

void
RampConstraintUC::load()
{
	//std::cout<<"r9"<<std::endl;
	for (size_t gen = 0; gen < data.nGenerators; gen++){
		// when a generator turns on, it must ramp from zero to the target power output. If
		// the generator's ramp rate cannot support this, then it will not be selected under
		// any circumstances
		const int step = 0;
		IloNumExpr &power = optData.iloGenPower[step][gen];
		IloBoolVar &isOn = optData.iloSegmentIsOn[step][gen];
		double wasOn = GENERATOR_SUPPORTING == data.convGenPrev[gen].generatorData.status? 1 : 0;
		double previousPower = data.convGenPrev[gen].generatorData.currentPower;
		double rampUpRate = data.genRampUpRate[gen];
		double rampDownRate = data.genRampDownRate[gen];
		double minPower = data.genPowerMinLimit[gen];
		double rampSwitchRate = minPower; // min limit to force it to be near min power when turning on/off
		double maxPower = data.genPowerMaxLimit[gen];
		if (rampUpRate < maxPower) { // skip the constraint if it is not actually constraining
			//IloRange rampUpConstraint(power - previousPower - rampUpRate - rampSwitchRate*(1-wasOn) <= 0);
			IloRange rampUpConstraint(power - previousPower - rampUpRate <= 0);
			addRangeConstraint(rampUpConstraint, "RampUp[%d][%d]", step, gen);
		}
		if (rampDownRate < maxPower) { // skip the constraint if it is not actually constraining
			//IloRange rampDownConstraint(power - previousPower + rampDownRate + rampSwitchRate*(wasOn-isOn) >= 0);
			IloRange rampDownConstraint(power - previousPower + rampDownRate >= 0);
			addRangeConstraint(rampDownConstraint, "RampDown[%d][%d]", step, gen);
		}
	}

	for (size_t step = 1; step < data.ucLength; step++) {
		for (size_t gen = 0; gen < data.nGenerators; gen++) {
			IloNumExpr &power = optData.iloGenPower[step][gen];
			IloNumExpr &previousPower = optData.iloGenPower[step-1][gen];
			IloBoolVar &isOn = optData.iloSegmentIsOn[step][gen];
			IloBoolVar &wasOn = optData.iloSegmentIsOn[step-1][gen];
			double rampUpRate = data.genRampUpRate[gen];
			double rampDownRate = data.genRampDownRate[gen];
			double rampSwitchRate = data.genPowerMinLimit[gen]; // min limit to force it to be near min power when turning on/off
			double maxPower = data.genPowerMaxLimit[gen];
			if (rampUpRate < maxPower) { // skip the constraint if it is not actually constraining
				//IloRange rampUpConstraint(power - previousPower - rampUpRate - rampSwitchRate*(1-wasOn) <= 0);
				IloRange rampUpConstraint(power - previousPower - rampUpRate <= 0);
				addRangeConstraint(rampUpConstraint, "RampUp[%d][%d]", step, gen);
			}
			if (rampDownRate < maxPower) { // skip the constraint if it is not actually constraining
				//IloRange rampDownConstraint(power - previousPower + rampDownRate + rampSwitchRate*(wasOn-isOn) >= 0);
				IloRange rampDownConstraint(power - previousPower + rampDownRate >= 0);
				addRangeConstraint(rampDownConstraint, "RampDown[%d][%d]", step, gen);
			}
		}
	}

}

} /* END OF NAMESPACE model */
