#include "simulation/Clock.hpp"
#include "structures.hpp"
#include <boost/date_time/gregorian/gregorian.hpp>

using ::boost::posix_time::ptime;
using ::boost::posix_time::ptime;
using ::boost::gregorian::date;
using ::boost::posix_time::time_duration;

namespace simulator {

	Clock * Clock::clock = NULL;

	bool Clock::increment(){
		bool complete = Clock::clock->step >= Clock::clock->endTime;
		if (!complete) {
			Clock::clock->incrementClock();
		}
		return complete;
	}

	INTEGER Clock::getStep            () { return Clock::clock->step            ; }
	INTEGER Clock::getSDIPreviousIndex() { return Clock::clock->sdiPreviousIndex; }

	// CURRENT STEP DATA  =====================================================
	ptime   Clock::getCurrentTime     () { return Clock::clock->currentTime     ; }
	INTEGER Clock::getCurrentHourInDay() { return Clock::clock->currentHourInDay; }
	INTEGER Clock::getCurrentDayInSim () { return Clock::clock->currentDayInSim ; }
	INTEGER Clock::getCurrentUCStep   () { return Clock::clock->currentUCStep   ; }
	INTEGER Clock::getSDICurrentIndex () { return Clock::clock->sdiCurrentIndex ; }

	// NEXT STEP DATA  ========================================================
	INTEGER Clock::getSDINextIndex() { return Clock::clock->sdiNextIndex           ; }
	bool Clock::onRegStep         () { return Clock::clock->currentRegFlag         ; }
	bool Clock::onLFStep          () { return Clock::clock->currentLFFlag          ; }
	bool Clock::onUCStep          () { return Clock::clock->currentUCFlag          ; }
	bool Clock::onUCProjectionStep() { return Clock::clock->currentUCProjectionFlag; }
	bool Clock::onDumpStep        () { return Clock::clock->currentDumpFlag        ; }
	INTEGER Clock::getNumberOfUnitCommitStepsInProjection() {
		return Clock::clock->nUCStepsInProjection;
	}
	void Clock::incrementClock() {
		currentTime += stepDuration;
		step                    += baseTimeStep;

		// Time state
		previousHourInDay        = currentHourInDay;
		previousDayInSim         = currentDayInSim;

		currentHourInDay         = nextHourInDay;
		currentDayInSim          = nextDayInSim;

		// Clock update
		// ASSUMPTION: the base step is <= 60 min
		nextMinInHour           += baseTimeStep;
		if (nextMinInHour >= MINUTES_IN_HOUR) {
			nextMinInHour        = MINUTES_IN_HOUR - nextMinInHour;
			++nextHourInDay;
			if (nextHourInDay >= HOURS_IN_DAY) {
				nextHourInDay    = HOURS_IN_DAY - nextHourInDay;
				++nextDayInSim;
			}
		}

		// Time event state
		previousUCFlag           = currentUCFlag;
		previousUCProjectionFlag = currentUCProjectionFlag;

		previousLFFlag           = currentLFFlag;

		previousRegFlag          = currentRegFlag;

		currentUCStep            = nextUCStep;
		currentUCFlag            = nextUCFlag;
		currentUCProjectionFlag  = nextUCProjectionFlag;

		currentLFFlag            = nextLFFlag;

		currentRegFlag           = nextRegFlag;

		nextUCFlag            = false;
		nextUCProjectionFlag  = false;
		nextLFFlag            = false;
		nextRegFlag           = false;

		// increment index by baseTimeStep
		// TODO: these are not verified
		// TODO: convert the blocks to function calls
		if (implementsRegulation && implementsLoadFollowing) {
			// FUTURE_OPT
		} else if (implementsRegulation) {
			// FUTURE_OPT
		} else if (implementsLoadFollowing) {
			// FUTURE_OPT
		} else {
			nextUCStep               += 1;
			nextUCFlag                = true;
			if (nextUCStep >= nBaseStepsInProjection) {
				nextUCStep            = 0;
				nextUCProjectionFlag  = true;
			}
		}

		previousDumpFlag    = currentDumpFlag;
		currentDumpFlag     = nextDumpFlag;
		nextDumpFlag        = false;
		if (dumpStep == Clock::clock->nStepsInDump) {
			dumpStep        = 0;
			nextDumpFlag    = true; // dump current step
		}
		++dumpStep;

		sdiPreviousIndex         = sdiCurrentIndex;
		sdiCurrentIndex          = sdiNextIndex;
		++sdiNextIndex;
		if (sdiNextIndex >= nInterfaceSteps) {
			sdiNextIndex    = 0;
		}

	}

	void Clock::setClock(const SimulationInfo & simulationInfo)
	{
		if (Clock::clock == NULL) {
			Clock::clock = new Clock(simulationInfo);
		}
	}

	/*
	 * The sdiIndices refer to the index in the SDI into which current
	 * information is to be loaded.
	 *
	 * This simulation is predicated on the idea that the current time step
	 * (sdiCurrentIndex) is calculated from the next time step's data.
	 * Therefore, the current SDI time lags the current data times by one step.
	 */
	Clock::Clock(const SimulationInfo & simulationInfo) :
		currentTime(simulationInfo.start),
		stepDuration(0, simulationInfo.baseTimeStep, 0),
		endTime(simulationInfo.duration),
		baseTimeStep(simulationInfo.baseTimeStep),
		nInterfaceSteps(simulationInfo.nInterfaceSteps),

		nStepsInDump(simulationInfo.nDumpSteps),
		dumpStep(-1),  // this delays the dump by one time step behind the current time

		nBaseStepsInProjection(simulationInfo.nBaseStepsInProjection),
		nUCStepsInProjection(simulationInfo.nBaseStepsInProjection / simulationInfo.nBaseStepsInUCStep),
		step(0),

		previousDayInSim(0xDECAFBAD),
		previousHourInDay(0xDECAFBAD),
		previousMinInHour(0xDECAFBAD),
		sdiPreviousIndex(0xDECAFBAD),

		currentDayInSim(0),
		currentHourInDay(0),
		currentUCStep(0),
		sdiCurrentIndex(0),

		nextDayInSim(0),
		nextHourInDay(0),
		nextMinInHour(simulationInfo.baseTimeStep),
		nextProjStep(0),
		nextUCStep(0),
		nextLFStep(0),
		nextRegStep(simulationInfo.baseTimeStep),
		sdiNextIndex(1),

		previousUCProjectionFlag(false),
		previousUCFlag(false),
		previousLFFlag(false),
		previousRegFlag(false),
		previousDumpFlag(false),

		currentUCProjectionFlag(true),
		currentUCFlag(false),
		currentLFFlag(false),
		currentRegFlag(false),
		currentDumpFlag(true),

		nextUCProjectionFlag(false),
		nextUCFlag(false),
		nextLFFlag(false),
		nextRegFlag(true),
		nextDumpFlag(false),

		implementsLoadFollowing(simulationInfo.implementsLoadFollowing),
		implementsRegulation(simulationInfo.implementsRegulation)
	{
		endTime = endTime* baseTimeStep + simulationInfo.nLoadRampSteps*MINUTES_IN_DAY;
		if (implementsRegulation) {
			nextMinInHour           = simulationInfo.baseTimeStep;
			nextProjStep            = 0;
			nextUCStep              = 0;
			nextLFStep              = 0;
			nextRegStep             = simulationInfo.baseTimeStep;
			sdiNextIndex            = 1;

			nextUCFlag              = false;
			nextLFFlag              = false;
			nextRegFlag             = true;
			nextDumpFlag            = false;
		} else if (implementsLoadFollowing) {
			nextDayInSim            = 0;
			nextHourInDay           = 0;
			nextMinInHour           = simulationInfo.baseTimeStep;
			nextProjStep            = 0;
			nextUCStep              = 0;
			nextLFStep              = 1;
			nextRegStep             = simulationInfo.baseTimeStep;
			sdiNextIndex            = 1;

			nextUCFlag              = false;
			nextLFFlag              = true;
			nextRegFlag             = false;
			nextDumpFlag            = false;
		} else {
			nextDayInSim            = 0;
			nextHourInDay           = 1;
			nextMinInHour           = 0;
			nextProjStep            = 0;
			nextUCStep              = 1;
			nextLFStep              = 0;
			nextRegStep             = 0;
			sdiNextIndex            = 1;

			nextUCFlag              = true;
			nextLFFlag              = false;
			nextRegFlag             = false;
			nextDumpFlag            = false;
		}
	}

	Clock::~Clock() { }

} /* END OF NAMESPACE simulator */
