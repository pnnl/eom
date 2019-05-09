#pragma once

/* ***************************************************************************
 * PURPOSE:
 * This implements the line constraint between two zones in a single BA. The
 * constraint is:
 *
 * 	Load[z] =(SUM{0->nZones}P[z]) + Pin[z] - Pout[z]
 * ---------------------------------------------------------------------------
 */

#include "optimizers/UnitCommitmentConstraint.hpp"

namespace model {

class InterzoneMaxLinePowerUC : public UnitCommitmentConstraint
{
public:
	InterzoneMaxLinePowerUC(UCData & data, UCOptData & optData): UnitCommitmentConstraint(data, optData) {};
	virtual ~InterzoneMaxLinePowerUC() {};
	virtual void load();
private:
	InterzoneMaxLinePowerUC(InterzoneMaxLinePowerUC & izlc);
	InterzoneMaxLinePowerUC & operator = (InterzoneMaxLinePowerUC & izlc);
};

} /* END OF NAMESPACE model */
