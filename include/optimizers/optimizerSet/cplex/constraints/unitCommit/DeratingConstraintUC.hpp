#pragma once

#include "optimizers/UnitCommitmentConstraint.hpp"

class Universe;

namespace model {

class DeratingConstraintUC : public UnitCommitmentConstraint
{
	public:
		DeratingConstraintUC(Universe & universe, UCData & data, UCOptData & optData);
		virtual ~DeratingConstraintUC();
		virtual void load();
	private:
		Universe  & universe;
		DeratingConstraintUC(DeratingConstraintUC &);
		DeratingConstraintUC & operator = (DeratingConstraintUC &);
};

} /* END OF NAMESPACE model */

