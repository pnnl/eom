#pragma once

#include "optimizers/Constraint.hpp"
#include "optimizer_structs.hpp"
#include "optimization_structs.hpp"

class IloRange;

namespace model {

class UnitCommitmentConstraint : public Constraint
{
public:
	UnitCommitmentConstraint(UCData &data, UCOptData &optData) : data(data), optData(optData) {};
	virtual ~UnitCommitmentConstraint(){};
	virtual void load() = 0;
protected:
	void addRangeConstraint(IloRange constraint, std::string nameFormat, int a);
	void addRangeConstraint(IloRange constraint, std::string nameFormat, int a, int b);
	void addRangeConstraint(IloRange constraint, std::string nameFormat, int a, int b, int c);
	UCData                 & data;
	UCOptData              & optData;
};

} /* END OF NAMESPACE model */

