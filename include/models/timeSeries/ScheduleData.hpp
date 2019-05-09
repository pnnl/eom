/* ***************************************************************************
 * ScheduleData.hpp
 *
 *      Author: Kevin Glass
 *  Created on: May 28, 2010
 *
 * Inheritance
 *
 * Class members
 *
 * Class variables
 *
 * Purpose
 * The ScheduleData allocates the data spaces required by the simulation.It
 * maintains no control over the data spaces.
 *
 ************************************************************************** */

#ifndef SCHEDULEDATA_HPP_
#define SCHEDULEDATA_HPP_

/** \class ScheduleData
 * \brief
 *
 * @ingroup Construction
 */

#include "structures.hpp"
#include "optimization_structs.hpp"
#include "models/RIMModel.hpp"
#include "RIMParser.hpp"
#include "modelAbs/Generator.hpp"
#include "models/timeSeries/AdjLoadTS.hpp"

#include "optimization_structs.hpp"

namespace model {

class ScheduleData
{
	public:
		ScheduleData(const SimulationInfo & simulationInfo);
		~ScheduleData();
protected:
	void          setSchuduleData();
	void          setupPowerTargetSchedule();
	void          setupRampingSchedule();
private:
	static ScheduleData      * scheduleSet;

	// Schedule information
	FLOAT                    * lfPowerTargetData;     // UCTimeStep x nGenerators
	FLOAT                   ** genPowerTargetData;    // UCTimeStep x nGenerators

	INTEGER                    nBalancingAuths;
	INTEGER                    nZones;
	INTEGER                    nGenerators;
	INTEGER                    nConventionalGenerators;
	INTEGER                    ucTimeStepsPerDay;
	INTEGER                    lfLength;

	ScheduleData & operator = (ScheduleData & scheduleSet){return * this;};
};

} /* END OF NAMESPACE model */

#endif /* SCHEDULEDATA_HPP_ */
