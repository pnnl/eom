#pragma once

#include "optimizers/UnitCommitmentConstraint.hpp"
class Universe;
namespace model {

class PowerLimitConstraintUC : public UnitCommitmentConstraint
{
public:
	PowerLimitConstraintUC(Universe & universe, UCData & data, UCOptData & optData);
	virtual ~PowerLimitConstraintUC() {};
	virtual void load();
private:
	Universe  & universe;
	PowerLimitConstraintUC(PowerLimitConstraintUC &);
	PowerLimitConstraintUC & operator = (PowerLimitConstraintUC &);
};

} /* END OF NAMESPACE model */

