#pragma once

#include "optimizers/UnitCommitmentConstraint.hpp"
class Universe;
namespace model {

class ReserveConstraintUC : public UnitCommitmentConstraint
{
public:
	ReserveConstraintUC(Universe & universe, UCData & data, UCOptData & optData);
	virtual ~ReserveConstraintUC() {};
	virtual void load();
private:
	Universe  & universe;
	ReserveConstraintUC(ReserveConstraintUC &);
	ReserveConstraintUC & operator = (ReserveConstraintUC &);
};

} /* END OF NAMESPACE model */

