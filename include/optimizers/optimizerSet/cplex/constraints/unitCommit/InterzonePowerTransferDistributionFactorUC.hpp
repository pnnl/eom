#pragma once

#include "optimizers/UnitCommitmentConstraint.hpp"

namespace model {

class InterzonePowerTransferDistributionFactorUC : public UnitCommitmentConstraint
{
public:
	InterzonePowerTransferDistributionFactorUC(UCData & data, UCOptData & optData): UnitCommitmentConstraint(data, optData) {};
	virtual ~InterzonePowerTransferDistributionFactorUC() {};
	virtual void load();
private:
	InterzonePowerTransferDistributionFactorUC(InterzonePowerTransferDistributionFactorUC &);
	InterzonePowerTransferDistributionFactorUC & operator = (InterzonePowerTransferDistributionFactorUC & izlc);
};

} /* END OF NAMESPACE model */
