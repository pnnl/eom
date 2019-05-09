#pragma once

#include "optimizers/UnitCommitmentConstraint.hpp"

namespace model {

class InterzoneTransportConstraintUC : public UnitCommitmentConstraint
{
public:
	InterzoneTransportConstraintUC(UCData & data, UCOptData & optData): UnitCommitmentConstraint(data, optData) {};
	virtual ~InterzoneTransportConstraintUC() {};
	virtual void load();
private:
	InterzoneTransportConstraintUC(InterzoneTransportConstraintUC &);
	InterzoneTransportConstraintUC & operator = (InterzoneTransportConstraintUC &);
};

} /* END OF NAMESPACE model */

