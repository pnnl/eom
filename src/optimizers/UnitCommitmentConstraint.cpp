#include "optimizers/UnitCommitmentConstraint.hpp"
#include <boost/format.hpp>

namespace model {

void UnitCommitmentConstraint::addRangeConstraint(IloRange constraint, std::string nameFormat, int a) {
	boost::format formatter(nameFormat);
	std::string name = boost::str(formatter % a);
	constraint.setName(name.c_str());
	optData.rangeConstraints.add(constraint);
	optData.control.iloModel.add(constraint);
}
void UnitCommitmentConstraint::addRangeConstraint(IloRange constraint, std::string nameFormat, int a, int b) {
	boost::format formatter(nameFormat);
	std::string name = boost::str(formatter % a % b);
	constraint.setName(name.c_str());
	optData.rangeConstraints.add(constraint);
	optData.control.iloModel.add(constraint);
}
void UnitCommitmentConstraint::addRangeConstraint(IloRange constraint, std::string nameFormat, int a, int b, int c) {
	boost::format formatter(nameFormat);
	std::string name = boost::str(formatter % a % b % c);
	constraint.setName(name.c_str());
	optData.rangeConstraints.add(constraint);
	optData.control.iloModel.add(constraint);
}

} /* END OF NAMESPACE model */
