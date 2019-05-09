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

#include "constraints/loadFollowing/RampConstraintLF.hpp"
#include "simulation/SDInterface.hpp"

namespace model {

void
RampConstraintLF::load()
{

	// loop through the commit generators in each zone

	for (INTEGER gen = 0; gen < lfData.nGenerators; ++gen) {
		FLOAT initialPower = lfData.convGenPrev[gen].generatorData.currentPower;
		if (ucData.rampSchedule[0][gen].state == GENERATOR_SUPPORTING) {
			optData.iloRampUpConstraint[0][gen] = IloIfThen(optData.control.iloEnv,
				(initialPower <= optData.iloGenPower[0][gen]),
				(optData.iloGenPower[0][gen] - initialPower) <= lfData.genRampUpRate[gen]);

			optData.iloRampDownConstraint[0][gen] = IloIfThen(optData.control.iloEnv,
				(initialPower >= optData.iloGenPower[0][gen]),
				(initialPower - optData.iloGenPower[0][gen]) <= lfData.genRampDownRate[gen]);

			optData.control.iloModel.add(optData.iloRampUpConstraint[0][gen]);
			optData.control.iloModel.add(optData.iloRampDownConstraint[0][gen]);
		}
		for (INTEGER step = 1; step < lfData.lfLength; step++) {
			if (ucData.rampSchedule[0][gen].state == GENERATOR_SUPPORTING) {
				optData.iloRampUpConstraint[step][gen] = IloIfThen(optData.control.iloEnv,
					(optData.iloGenPower[step-1][gen] <= optData.iloGenPower[step][gen]),
					(optData.iloGenPower[step][gen] - optData.iloGenPower[step-1][gen]) <= lfData.genRampUpRate[gen]);

				optData.iloRampDownConstraint[step][gen] = IloIfThen(optData.control.iloEnv,
					(optData.iloGenPower[step-1][gen] >= optData.iloGenPower[step][gen]),
					(optData.iloGenPower[step-1][gen] - optData.iloGenPower[step][gen]) <= lfData.genRampDownRate[gen]);

				optData.control.iloModel.add(optData.iloRampUpConstraint[step][gen]);
				optData.control.iloModel.add(optData.iloRampDownConstraint[step][gen]);
			}
		}
	}

}
void
RampConstraintLF::printErrorState()
{
}

} /* END OF NAMESPACE model */

