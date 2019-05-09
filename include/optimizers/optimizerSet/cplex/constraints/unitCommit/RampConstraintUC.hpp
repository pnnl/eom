#pragma once

#include "optimizers/UnitCommitmentConstraint.hpp"

namespace model {

class RampConstraintUC: public UnitCommitmentConstraint
{
public:
	RampConstraintUC(UCData & data, UCOptData & optData): UnitCommitmentConstraint(data, optData) {};
	virtual ~RampConstraintUC() {};
	virtual void load();
private:
	RampConstraintUC(RampConstraintUC &);
	RampConstraintUC & operator = (RampConstraintUC &);
};

} /* END OF NAMESPACE model */

