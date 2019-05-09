/* ***************************************************************************
 * Author      : Kevin Glass
 * Date        : Feb 12, 2010
 * File        : Zone.hpp
 * Project     : RIM
 *
 *
 * Contents    :
 *
 * Assumptions :
 * 1) Each Zone must have a unique id. The id is used as an index into the SDI
 * and the generator set in the ScheduleData singleton.
 * ---------------------------------------------------------------------------
 */

#ifndef ZONE_HPP_
#define ZONE_HPP_

#include "structures.hpp"
#include "optimization_structs.hpp"
#include "models/RIMModel.hpp"
#include "models/timeSeries/AdjLoadTS.hpp"
#include "modelAbs/Generator.hpp"
#include "RIMParser.hpp"
#include "generation/ConventionalGenerator.hpp"


namespace model {

/**
 * \class Zone
 *
 * In this version of the model, Zone's control the generator response to
 * various time steps. They have no independent behavior
 *
 * @ingroup basemodel
 */
class Zone: public RIMModel
{
public:
	/**
	 * \brief Constructs a Zone using parameter and initialization data from the
	 * RIM parser
	 */
	Zone(Generator ** generatorSet, const ZoneParams & zoneParameters);
	virtual ~Zone();

	virtual void dailyTrigger();
	virtual void setupTrigger();
	virtual void ucTrigger();
	virtual void lfTrigger();
	virtual void regTrigger();

#if defined RIM_DEBUG
	virtual void printState(ostream & out);
	virtual void printParameters(ostream & out);
#endif
private:
	INTEGER                  zoneID;
	static int32_t           ZoneID;

	Generator             ** generatorSet;
	size_t                   nGenerators;

};

} /* END OF NAMESPACE model */

#endif /* ZONE_HPP_ */
