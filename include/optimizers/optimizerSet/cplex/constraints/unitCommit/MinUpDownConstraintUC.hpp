#pragma once

#include "optimizers/UnitCommitmentConstraint.hpp"

namespace model {

class MinUpDownConstraintUC: public UnitCommitmentConstraint
{
public:
	MinUpDownConstraintUC(UCData & data, UCOptData & optData) : UnitCommitmentConstraint(data, optData) {};
	virtual ~MinUpDownConstraintUC() {};
	virtual void load();
private:
	MinUpDownConstraintUC(MinUpDownConstraintUC &);
	MinUpDownConstraintUC & operator = (MinUpDownConstraintUC &);
};

} /* END OF NAMESPACE model */

