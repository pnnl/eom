/* ***************************************************************************
 * Author      : Kevin Glass
 * Date        : Jan 26, 2011
 * File        : BalancingAuthority.hpp
 * Project     : RIM
 *
 * Assumptions:
 *
 *
 * Testing:
 *      TBD
 *
 * Details:
 *
 *
 * STATUS:
 * not done     naming convention conformity
 * not done     doxygen comments
 * not done     private comments
 * not done     comment block conformity
 * not done     testing information
 * not done     ensure hidden methods
 *
 ************************************************************************** */
#ifndef BALANCINGAUTHORITY_HPP_
#define BALANCINGAUTHORITY_HPP_

#include "RIMParser.hpp"

#include "models/RIMModel.hpp"
#include "modelAbs/Generator.hpp"
#include "administrative/Zone.hpp"
#include "simulation/SDInterface.hpp"

#include "Optimization.hpp"
#include "optimizers/optimization/UnitCommitOptimizer.hpp"
#include "optimizers/optimization/LoadFollowingOpt.hpp"
#include "optimizers/optimization/RegulationOpt.hpp"

#include "models/timeSeries/AdjLoadTS.hpp"

#include "structures.hpp"

#include <boost/shared_ptr.hpp>

class Universe;

namespace model {

using boost::shared_ptr;

/**
 * \class BalancingAuthority
 *
 * The BalancingAuthority models the behavior of balancing authorities
 * including interactions between the BA's zones and the inter-zone optimizations
 * for day ahead (UC), load following and regulation generator scheduling.
 *
 * @ingroup basemodel
 */
class BalancingAuthority : public RIMModel
{
public:
	/**
	 * \brief Constructs a BA using parameter and initialization data from the
	 * RIM parser
	 */
	BalancingAuthority(const BalanceAuthParams & parameters, Zone * zoneSet, UCData & ucData, Universe & universe);
	virtual ~BalancingAuthority();
	/**
	 * \brief Triggers the unit commit optimizer, then updates the daily step
	 * of each zone in the BA
	 */
	virtual void setupTrigger();
	/**
	 * \brief Triggers the unit commit optimizer, then updates the daily step
	 * of each zone in the BA
	 */
	virtual void dailyTrigger();
	/**
	 * \brief Updates the unit commit step of each zone in the BA
	 */
	virtual void ucTrigger();
	/**
	 * \brief Triggers the load following optimizer, then updates the load
	 * following step of each zone in the BA
	 */
	virtual void lfTrigger();
	/**
	 * \brief Triggers the regulation optimizer, then updates the regulation
	 * step of each zone in the BA
	 */
	virtual void regTrigger();

	void dumpZoneTransfers();

private:
	INTEGER                  balancingAuthorityID;
	static INTEGER           BalancingAuthorityID;

	BalanceAuthParams        params;
	UCData                 * ucData;
	LFData                   lfData;
	RegData                  regData;
	Zone                   * zoneSet;
	bool                     implementsLoadFollowing;
	bool                     implementsRegulation;

	shared_ptr<Optimization<UnitCommitOptimizer> > unitCommit;
	shared_ptr<Optimization<LoadFollowingOpt> >    loadFollowing;
	shared_ptr<Optimization<RegulationOpt> >       regulation;
};

} /* END OF NAMESPACE model */

#endif /* BALANCINGAUTHORITY_HPP_ */
