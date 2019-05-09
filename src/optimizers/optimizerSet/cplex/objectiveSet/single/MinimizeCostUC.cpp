/* ***************************************************************************
 * Author      : Kevin Glass, Cesar A. Silva Monroy
 * Date        : Jul 14, 2010
 * File        : MinimizeCostUC.cpp
 * Project     : rim
 *
 * Purpose
 * 	Representing a linear cost curve as a single segment piecewise linear
 * 	cost curve.
 *
 * Revisions:
 * ---------------------------------------------------------------------------
 */
#include "objectives/MinimizeCostUC.hpp"

namespace model {

void
MinimizeCostUC::load()
{
	/*
	 * For each generator Cost = m*Power + b, assuming we have m and b then
	 * m * iloGenPower + b * iloGenIsOn = cost. If we do not ensure the
	 * generator is on before we compute the cost, then we will sum intercepts
	 * that do not belong in the computation.
	 *
	 * The m and b values are computed for each segment in the GeneratorParser.
	 *
	 * The problem that must solve the following:
	 *
	 * 		Sum{t = 0->ucLength(look ahead time)}Sum{g = 0->nGenerators} [cost slope * power + cost intercept * genIsOn]
	 *
	 * Since cost slope and intercept are a IloNumExprVar arrays and genPower is an
	 * IloSemiContinuous matrix[time][gen] CPLEX will not let us simply accumulate
	 * the values for costSlope * generatorPower.
	 *
	 * Try:
	 *
	 * Instead, we will accumulate
	 *
	 * The iloScalar product of slope[i]*genPower should be the cost
	 *
	 */
	try {
		optData.control.iloObj = IloExpr(optData.control.iloEnv, 0);
		for (INTEGER step = 0; step < data.ucLength; step++) {
			for (INTEGER gen = 0; gen < data.nGenerators; gen++) {
				FLOAT startCost = data.costCurve[gen].segment[0].startPoint[COST];
				FLOAT slope = data.costCurve[gen].segment[0].slope;
				FLOAT intercept = data.costCurve[gen].segment[0].intercept;
				FLOAT switchOnCost = this->data.genTurnOnCost[gen];
				FLOAT switchOffCost = this->data.genTurnOffCost[gen];
				IloNumVar  & switchOn  = optData.iloGenStartSupport[step][gen];
				//IloNumVar  & switchOff = optData.iloGenStopSupport[step][gen];
				IloBoolVar & isOn      = optData.iloSegmentIsOn[step][gen];
				IloNumExpr & power     = optData.iloGenPower[step][gen];
				optData.control.iloObj += power*slope + isOn * intercept + switchOn*switchOnCost + switchOn *switchOffCost;
			}
		}
	} catch(IloException & e) {
		std::cerr << "ERROR: objective function failed to load" << std::endl;
	}
	optData.control.iloModel.add(IloMinimize(optData.control.iloEnv, optData.control.iloObj));
}

void
MinimizeCostUC::printErrorState()
{
}

} /* END OF NAMESPACE model */
