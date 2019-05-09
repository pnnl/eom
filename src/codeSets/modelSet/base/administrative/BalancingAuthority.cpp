#include "administrative/BalancingAuthority.hpp"
#include "simulation/Scheduler.hpp"
#include "models/timeSeries/AdjLoadTS.hpp"
#include "administrative/Zone.hpp"
#include "exceptions/Exception.hpp"
#include "universe.hpp"

#include <string.h>
#include <boost/format.hpp>

using std::vector;
using std::cerr;
using std::endl;
using std::string;

namespace model {

INTEGER BalancingAuthority::BalancingAuthorityID = 0;

BalancingAuthority::BalancingAuthority(const BalanceAuthParams & parameters, Zone * zoneSet, UCData & ucData, Universe & universe) :
	balancingAuthorityID(BalancingAuthorityID++),
	params(parameters),
	ucData(&ucData),
	zoneSet(zoneSet),
	implementsLoadFollowing(universe.simulationInfo.implementsLoadFollowing),
	implementsRegulation(universe.simulationInfo.implementsRegulation),
	unitCommit(),
	loadFollowing(),
	regulation()
{
	if (params.nZones > 0) {
		unitCommit = shared_ptr<Optimization<UnitCommitOptimizer> >(new Optimization<UnitCommitOptimizer>(universe, ucData, balancingAuthorityID));
		if (implementsLoadFollowing) {
			loadFollowing = shared_ptr<Optimization<LoadFollowingOpt> >(new Optimization<LoadFollowingOpt>(ucData, lfData, balancingAuthorityID));
		}
		if (implementsRegulation) {
			regulation = shared_ptr<Optimization<RegulationOpt> >(new Optimization<RegulationOpt>(ucData, lfData, regData, balancingAuthorityID));
		}
	}
}

BalancingAuthority::~BalancingAuthority()
{
	--BalancingAuthorityID; // Why?  Only works if objects destructed in reverse order of creation.  Is the constructor being called again after the destructor runs?
}

void
BalancingAuthority::dailyTrigger()
{
	// call optimizer
	// TODO: when this is done for a multi-zone BA, the BA adj load must be computed from the zone Adj load
	try {
		// optimize for this time step
		unitCommit->dailyTrigger();

		// zone and generator response to dailyTrigger
		for (size_t i = 0; i < params.nZones; i++) {
			zoneSet[i].dailyTrigger();
		}
	} catch (Exception & e) {
		cerr << "\t" << __FILE__ << " " << __LINE__ << endl;
		cerr << "\tERROR: could not setup initial environment " << endl;
		throw;
	}
	// need to collect zone input
	for (size_t i = 0; i < params.nZones; i++) {
		zoneSet[i].dailyTrigger();
	}

};

void
BalancingAuthority::setupTrigger()
{
	try {
		unitCommit->setupTrigger();

		// zone and generator response to setupTrigger
		for (size_t zoneIndex = 0; zoneIndex < this->params.nZones; zoneIndex++) {
			zoneSet[zoneIndex].setupTrigger();
		}
	} catch (Exception & e) {
		cerr << "\t" << __FILE__ << " " << __LINE__ << endl;
		cerr << "\tERROR: could not setup initial environment " << endl;
		throw;
	}
	// UC will compute optimal generator set using the generator data directly
	// to compute the zone information, the UC must constrain the costs using
	// the total zone power + incoming and outgoing power as a function of line
	// power limits and PTDFs. Therefore each zone need to account for its
	// generation and
};

void
BalancingAuthority::ucTrigger()
{
	unitCommit->ucTrigger();
	for (size_t i = 0; i < params.nZones; i++) {
		zoneSet[i].ucTrigger();
	}
};

void
BalancingAuthority::lfTrigger()
{
	if (implementsLoadFollowing) {
		loadFollowing->lfTrigger();
		for (size_t i = 0; i < params.nZones; i++) {
			zoneSet[i].lfTrigger();
		}
	}
};

void
BalancingAuthority::regTrigger()
{
	if (implementsLoadFollowing) {
		regulation->regTrigger();
		for (size_t i = 0; i < params.nZones; i++) {
			zoneSet[i].regTrigger();
		}
	}
};

void BalancingAuthority::dumpZoneTransfers(void) {
	if (2 > this->ucData->nZones)
		return;
	const string filename = str(boost::format("results/zoneTransfers-%d.csv") % this->balancingAuthorityID);
	std::ofstream out(filename.c_str(), std::ios_base::trunc);
	out << "Step,Zone From,Zone To,Transfer Amount (MW)" << endl;
	for (vector<PowerTransfer>::iterator it = this->ucData->powerTransfer.begin(); it != this->ucData->powerTransfer.end(); it++) {
		const PowerTransfer & transfer = *it;
		out << boost::format("%d,%d,%d,%.1f")
			% transfer.step()
			% transfer.zoneFrom()
			% transfer.zoneTo()
			% transfer.megawatts()
			<< endl;
	}
}

} /* END OF NAMESPACE model */
