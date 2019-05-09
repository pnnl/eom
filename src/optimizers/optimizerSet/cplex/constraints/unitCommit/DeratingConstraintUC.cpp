#include "constraints/unitCommit/DeratingConstraintUC.hpp"
#include "optimizer_structs.hpp"
#include "optimization_structs.hpp"
#include "universe.hpp"
#include <boost/format.hpp>

namespace model {

DeratingConstraintUC::DeratingConstraintUC(Universe & universe, UCData & data, UCOptData & optData) :
	UnitCommitmentConstraint(data, optData),
	universe(universe)
{
}

DeratingConstraintUC::~DeratingConstraintUC() {
}

void
DeratingConstraintUC::load()
{
	//std::cout<<"r1"<<std::endl;
	for (size_t step = 0; step < data.ucLength; step++) {
		for (size_t gen = 0; gen < data.nGenerators; gen++) {
			IloNumExpr & power = optData.iloGenPower[step][gen];
			double maxPower = data.genPowerMaxLimit[gen];
			double deratedMaxPower = universe.getMaxPower(data.time[step], data.baId, gen);
			if (deratedMaxPower < maxPower)
				addRangeConstraint(power <= deratedMaxPower, "derating[%d][%d]", step, gen);
		}
	}
}

} /* END OF NAMESPACE model */
