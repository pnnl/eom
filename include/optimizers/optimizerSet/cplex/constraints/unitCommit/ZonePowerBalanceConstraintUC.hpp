#pragma once

#include "optimizers/UnitCommitmentConstraint.hpp"

namespace model {

class ZonePowerBalanceConstraintUC : public UnitCommitmentConstraint
{
public:
	ZonePowerBalanceConstraintUC(UCData & data, UCOptData & optData): UnitCommitmentConstraint(data, optData) {};
	virtual ~ZonePowerBalanceConstraintUC() {};
	virtual void load();
private:
	ZonePowerBalanceConstraintUC(ZonePowerBalanceConstraintUC &);
	ZonePowerBalanceConstraintUC & operator = (ZonePowerBalanceConstraintUC &);
};

} /* END OF NAMESPACE model */

