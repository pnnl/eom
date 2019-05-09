/* ***************************************************************************
 * Author      : Kevin Glass
 * Date        : Jul 14, 2010
 * File        : MinimizeCostLF.cpp
 * Project     : rim
 *
 * Inheritance
 *
 * Class members
 *
 * Class variables
 *
 * Purpose
 *
 ************************************************************************** */

#include "objectives/MinimizeCostLF.hpp"

namespace model {

void
MinimizeCostLF::load()
{
	try {
		for (INTEGER step = 0; step < lfData.lfLength; step++) {
			for (INTEGER gen = 0; gen < lfData.nGenerators; gen++) {
				FLOAT startPower = lfData.costCurve[gen].segment[0].startPoint[POWER];
				FLOAT endPower = lfData.costCurve[gen].segment[0].endPoint[POWER];
				FLOAT startCost = lfData.costCurve[gen].segment[0].startPoint[COST];
				FLOAT slope = lfData.costCurve[gen].segment[0].slope;
				IloSemiContVar  & power = optData.iloGenPower[step][gen];
				IloNumArray points = IloNumArray(optData.control.iloEnv, 2, startPower, endPower);
				IloNumArray slopes = IloNumArray(optData.control.iloEnv, 3, startCost/startPower, slope, 1e18);
				FLOAT x0 = 0;
				FLOAT y0 = 0;
				IloNumExpr cost = IloPiecewiseLinear(power, points, slopes, x0, y0);
				optData.control.iloObj += cost;
			}
		}
	} catch(IloException & e) {
		cerr << "ERROR: objective function failed to load" << endl;
	}
	optData.control.iloModel.add(IloMinimize(optData.control.iloEnv, optData.control.iloObj));
}

void
MinimizeCostLF::printErrorState()
{
}

} /* END OF NAMESPACE model */
