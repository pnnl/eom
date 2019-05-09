#pragma once

#include "optimizers/UnitCommitmentConstraint.hpp"

namespace model {

class GenStartStopConstraintUC : public UnitCommitmentConstraint
{
public:
	GenStartStopConstraintUC(UCData & data, UCOptData & optData): UnitCommitmentConstraint(data, optData) {};
	virtual ~GenStartStopConstraintUC() {};
	virtual void load();
private:
	GenStartStopConstraintUC(GenStartStopConstraintUC &);
	GenStartStopConstraintUC & operator = (GenStartStopConstraintUC &);
};

} /* END OF NAMESPACE model */
