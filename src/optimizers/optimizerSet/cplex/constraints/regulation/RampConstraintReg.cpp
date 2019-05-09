/* ***************************************************************************
 * Author      : Kevin Glass
 * Date        : Jul 12, 2010
 * File        : ReserveConstraint.cpp
 * Project     : rim
 *
 *
 * Contents    :
 *
 * Assumptions :
 *
 * ---------------------------------------------------------------------------
 */

#include "constraints/regulation/RampConstraintReg.hpp"
#include "simulation/SDInterface.hpp"

namespace model {
/*
 * optimizer variables
 *
 *		generatorData[j].output.outputPower  -- generator
 *		powerTargetSchedule                  -- scheduleData array
 *		generatorDeltaPower                  -- Cplex variable
 *		maxGeneratorRampUp                   -- generator parameter
 *		maxGeneratorRampDown                 -- generator parameter
 */
void
RampConstraintReg::load()
{
	/*
	for (int32_t gen = 0; gen < data.nGenerators; gen++){
		optData.iloRampStatus[0][gen] = optData.iloGenIsOn[0][gen] + optData.iloGenStarting[0][gen];
		optData.iloRampUpConstraint[0][gen] = IloIfThen(optData.control.iloEnv,
				((data.ucRampSchedule[gen][23].targetPower <= optData.iloGenPower[0][gen]) &&
						optData.iloRampStatus[0][gen] == 1),
				((optData.iloGenPower[0][gen] - data.ucRampSchedule[gen][23].targetPower) <=
						data.genRampUpRate[gen]));
		optData.iloRampDownConstraint[0][gen] = IloIfThen(optData.control.iloEnv,
				((data.ucRampSchedule[gen][23].targetPower >= optData.iloGenPower[0][gen]) &&
						optData.iloRampStatus[0][gen] == 1),
				((data.ucRampSchedule[gen][23].targetPower - optData.iloGenPower[0][gen]) <=
						data.genRampDownRate[gen]) && (optData.iloGenStarting[0][gen]>=0));

		optData.control.iloModel.add(optData.iloRampUpConstraint[0][gen]);
		optData.control.iloModel.add(optData.iloRampDownConstraint[0][gen]);
	}

	for (INTEGER step = 1; step < data.ucLength; step++) {
		for (INTEGER gen = 0; gen < data.nGenerators; gen++){
			optData.iloRampStatus[step][gen] = optData.iloGenIsOn[step][gen] + optData.iloGenStarting[step][gen];
			optData.iloRampUpConstraint[step][gen] = IloIfThen(optData.control.iloEnv,
					((optData.iloGenPower[step-1][gen] <= optData.iloGenPower[step][gen]) && optData.iloRampStatus[step][gen] == 1),
					((optData.iloGenPower[step][gen] - optData.iloGenPower[step-1][gen]) <= data.genRampUpRate[gen]));
			optData.iloRampDownConstraint[step][gen] = IloIfThen(optData.control.iloEnv,
					((optData.iloGenPower[step-1][gen] >= optData.iloGenPower[step][gen]) && optData.iloRampStatus[step][gen] == 1),
					((optData.iloGenPower[step-1][gen] - optData.iloGenPower[step][gen]) <= data.genRampDownRate[gen]));

			optData.control.iloModel.add(optData.iloRampUpConstraint[step][gen]);
			optData.control.iloModel.add(optData.iloRampDownConstraint[step][gen]);
		}
	}
	*/
}

void
RampConstraintReg::printErrorState()
{
}

} /* END OF NAMESPACE model */

