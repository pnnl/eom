#ifndef CLOCK_HPP_
#define CLOCK_HPP_

#include "generic.hpp"
#include <boost/date_time/posix_time/ptime.hpp>

class SimulationInfo;

namespace simulator {

/**
 * \brief Simulation clock
 *
 * The clock is a singleton with one valid constructor (<em>startTime(...)</em>)and one
 * setter function (<em>increment()</em>). Both of these functions are protected,
 * making the friend class <em>SDInterface</em> the only way of creating or setting
 * the clock.
 *
 * Information regarding the current time are accessed through a set of static
 * accessor methods.
 *
 */
class Clock {
public:
	static bool increment();
	static INTEGER getStep();
	static INTEGER getSDIPreviousIndex(); // TODO make private

	// CURRENT STEP DATA  =====================================================
	static boost::posix_time::ptime getCurrentTime     ();
	static INTEGER                  getCurrentHourInDay();
	static INTEGER                  getCurrentDayInSim ();
	static INTEGER                  getCurrentUCStep   ();
	static INTEGER                  getSDICurrentIndex ();

	// NEXT STEP DATA  ========================================================
	static INTEGER getSDINextIndex();

	static bool onRegStep         ();
	static bool onLFStep          ();
	static bool onUCStep          ();
	static bool onUCProjectionStep();
	static bool onDumpStep        ();

	static INTEGER getNumberOfUnitCommitStepsInProjection();

	friend class Scheduler;
	friend class SDInterface;

protected:
	void incrementClock();
	static void setClock(const SimulationInfo & simulationInfo);

private:
	boost::posix_time::ptime         currentTime ;
	boost::posix_time::time_duration stepDuration;

	// Time information
	INTEGER                  endTime;               // simulation constant
	INTEGER                  baseTimeStep;          // simulation constant

	// Buffer time step information
	INTEGER                  nInterfaceSteps;       // simulation constant
	INTEGER                  nStepsInDump;          // simulation constant
	INTEGER                  dumpStep;              // counter index

	// Time event information
	INTEGER                  nBaseStepsInProjection;
	INTEGER                  nUCStepsInProjection;
	INTEGER                  step;

	// Time state
	INTEGER                  previousDayInSim; // 0..endTime for previous step, what is the day number in the simulation
	INTEGER                  previousHourInDay; // 0..23 for previous step, what is hour within the day
	INTEGER                  previousMinInHour; // 0..59 for previous step, what is minute within the hour
	// Time event state
	INTEGER                  sdiPreviousIndex;

	// Time state
	INTEGER                  currentDayInSim; // 0..endTime for current step, what is the day number in the simulation
	INTEGER                  currentHourInDay; // 0..23 for current step, what is hour within the day
	// Time event state
	INTEGER                  currentUCStep;

	INTEGER                  sdiCurrentIndex;

	// Time state
	INTEGER                  nextDayInSim; // 0..endTime for next step, what is the day number in the simulation
	INTEGER                  nextHourInDay; // 0..23 for next step, what is hour within the day
	INTEGER                  nextMinInHour; // 0..59 for next step, what is minute within the hour
	// Time event state
	INTEGER                  nextProjStep;
	INTEGER                  nextUCStep;
	INTEGER                  nextLFStep;
	INTEGER                  nextRegStep;

	INTEGER                  sdiNextIndex;

	// Time event flags
	bool                     previousUCProjectionFlag;
	bool                     previousUCFlag;
	bool                     previousLFFlag;
	bool                     previousRegFlag;
	bool                     previousDumpFlag;

	bool                     currentUCProjectionFlag;
	bool                     currentUCFlag;
	bool                     currentLFFlag;
	bool                     currentRegFlag;
	bool                     currentDumpFlag;

	bool                     nextUCProjectionFlag;
	bool                     nextUCFlag;
	bool                     nextLFFlag;
	bool                     nextRegFlag;
	bool                     nextDumpFlag;

	bool                     implementsLoadFollowing;
	bool                     implementsRegulation;

	static Clock        * clock;

	Clock(const SimulationInfo & simulationInfo);

	// ----------------------------------------------------------------------
	virtual ~Clock();
	Clock & operator = (Clock & timeInfo);
};

} /* END OF NAMESPACE simulator */

#endif /* CLOCK_HPP_ */
