#pragma once

#include "optimizers/UnitCommitmentConstraint.hpp"

namespace model {

class PowerBalanceConstraintUC : public UnitCommitmentConstraint
{
public:
	PowerBalanceConstraintUC(UCData & data, UCOptData & optData): UnitCommitmentConstraint(data, optData) {};
	virtual ~PowerBalanceConstraintUC() {};
	virtual void load();
private:
	PowerBalanceConstraintUC(PowerBalanceConstraintUC &);
	PowerBalanceConstraintUC & operator = (PowerBalanceConstraintUC &);
};

} /* END OF NAMESPACE model */

