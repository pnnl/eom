 /* ***************************************************************************
 * Author      : Cesar A. Silva Monroy, Kevin Glass
 * Date        : Sept 27, 2010
 * File        : PWConstraintLF.cpp
 * Project     : rim
 *
 *
 * Contents    :
 *
 * Assumptions :
 *
 * ---------------------------------------------------------------------------
 */

#include "constraints/loadFollowing/PWConstraintLF.hpp"
#include "simulation/SDInterface.hpp"

namespace model {

void
PWConstraintLF::load()
{
	/*
	 *
	 */
	for (INTEGER step = 0; step < lfData.lfLength; step++) {
		for (INTEGER gen = 0; gen < lfData.nGenerators; gen++){
			LineSegment & segment = lfData.costCurve[gen].segment[0];
			IloSemiContVar & generatorPower = optData.iloGenPower[step][gen];
			IloNumVar & zCoefficient = optData.iloZCoeff[step][gen][0];
			optData.iloZCoeffConstraint[step][gen][0] = IloIfThen(optData.control.iloEnv,
				segment.startPoint[POWER] <= generatorPower <= segment.endPoint[POWER],
				(segment.startPoint[POWER] + zCoefficient*segment.delta[POWER]) == generatorPower
			);

			// force the zcoeff to zero if generator is not on
			optData.iloZeroZCoeffConstraint[step][gen][0] = IloIfThen(optData.control.iloEnv,
					not(segment.startPoint[POWER] <= generatorPower <= segment.endPoint[POWER]),
					zCoefficient == 0
			);
			// Don't need to force to zero for above maximum because we're a SINGLE piecewise constraint,
			// power can't go above the max power for this segment

			optData.control.iloModel.add(optData.iloZCoeffConstraint[step][gen][0]);
			optData.control.iloModel.add(optData.iloZeroZCoeffConstraint[step][gen][0]);
		}
	}
}

void
PWConstraintLF::printErrorState()
{
}

} /* END OF NAMESPACE model */
