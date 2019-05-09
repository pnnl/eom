/* ***************************************************************************
 * Assumptions :
 *  1) The parser and sdInterface have been successfully constructed. NOTE: if
 *  they have not been successfully constructed, the code will not reach this
 *  point
 */

#include "simulation/SDInterface.hpp"
#include "simulation/Clock.hpp"
#include "FileControl.hpp"
#include "exceptions/Exception.hpp"
#include <stdexcept>

using std::vector;
using std::cerr;
using std::endl;

namespace simulator {
SDInterface        * SDInterface::interface      = NULL;

SDInterface *
SDInterface::getSDInterface()
{
	if (SDInterface::interface == NULL) {
		throw std::logic_error("attempted to get SDI interface singleton before it was instantiated.");
	}
	return SDInterface::interface;
}

SDInterface::SDInterface(const SimulationInfo & simulationInfo, vector<ConventionalData> & conventionalData) :
	conventionalData       (conventionalData                      ),
	epGap                  (simulationInfo.epGap                  ),
	nGenerators            (simulationInfo.nGenerators            ),
	nConventionalGenerators(simulationInfo.nConventionalGenerators),
	ucLength               (simulationInfo.ucLength               ),
	sdiTimeSteps           (simulationInfo.nInterfaceSteps        ),
	rampScheduleData       (ucLength*nGenerators                  ),
	rampSchedule           (nGenerators                           )
{
	if (SDInterface::interface != NULL)
		throw std::logic_error("attempted to create second instance of singleton SDInterface");
	size_t offset = 0;
	for (size_t gen = 0; gen < nGenerators; gen++){
		rampSchedule[gen]  = &rampScheduleData[offset];
		offset += ucLength;
	}
	for (size_t gen = 0; gen < ucLength*nGenerators; gen++){
		rampScheduleData[gen].targetPower = 0.0;
		rampScheduleData[gen].state       = GENERATOR_INIT;
		rampScheduleData[gen].timeInState = 0;
		rampScheduleData[gen].genSwitch   = NO_SWITCH;
	}
	SDInterface::interface = this;
};

SDInterface::~SDInterface()
{
	SDInterface::interface = NULL;
};

void
SDInterface::regTrigger()
{
}

/* ===========================================================================
 *  INTEGER getTimeIndex(INTEGER time)
 *
 *  Get a the SDI index corresponding to the formal parameter "time." If "time"
 *  is greater then the "next step," the request is for an invalid time
 *  point. If time is less then the "current step" - (sdiTimeSteps - 2), then
 *  the time is already removed from the SDI and no longer valid.
 *
 *  To determine if a step is in the SDI:
 *  	determine the number of steps between the current time (Clock->steps)
 *  		and "time." deltaSteps = Clock::clock->step - time
 *		let index = Clock::clock->step - deltaSteps
 *		if index < 0, then it has wrapped around, set index to
 *			sdiTimeSteps + index (since index < 0 under this condition,
 *			the effect is to subtract index from sdiTimeSteps)
  ========================================================================= */
INTEGER
SDInterface::getTimeIndex(INTEGER time)
{
	INTEGER                  index          = 0;

	if (!isTimeValid(time)) {
		throw Exception(__FILE__, __LINE__, Exception::INVALID_ARRAY);
	}
	// if the time is valid, compute its index in the SDI
	if (time  == Clock::getSDICurrentIndex() + 1) {
		index = Clock::clock->sdiNextIndex;
	} else {
		INTEGER              deltaSteps     = Clock::clock->getSDICurrentIndex() - time;
		if (deltaSteps < (sdiTimeSteps - 1)){
			index = Clock::clock->getSDICurrentIndex() - deltaSteps;
			if (index < 0 && -index < sdiTimeSteps) {
				index = sdiTimeSteps + index;
			}
		}
	}
	return index;
}

/* ===========================================================================
 * Time is valid when:
 * 1) the time is less then the size of the SDI buffer. The simulator has to
 *    catch up to the
 * 2) the time is within "now" and "now" - the size of the SDI buffer.
  ===========================================================================*/
LOGICAL
SDInterface::isTimeValid(INTEGER time)
{
	LOGICAL                  valid          = false;

	if (time <= Clock::getSDICurrentIndex() + sdiTimeSteps - 1) {
		// check first load of SDI
		valid           = true;
	} else
	if (time <= Clock::getSDICurrentIndex() &&
			time > Clock::getSDICurrentIndex() - sdiTimeSteps) {
		// check after wrap around
		valid           = true;
	}

	return valid;
};

ConventionalData *
SDInterface::getConventionalData(INTEGER time)
{
	INTEGER                  index          = 0;
	//ConventionalData       * data           = NULL;
	try {
		index               = getTimeIndex(time);
	} catch (Exception & e) {
		cerr << "ERROR: requested time was not in the interface" << endl;
		throw;
	}
	//char                   * timeStep       = &sdiDataSpace[sdiStepOffset[index]];
	//data = (ConventionalData *)(&timeStep[sdiOffset[CONVENTIONAL]]);
	//return data;
	return &conventionalData[index * nConventionalGenerators];
};

ConventionalData *
SDInterface::getConventionalData(INTEGER time, size_t generatorIndex)
{
	if (nConventionalGenerators <= generatorIndex)
		throw std::range_error("requested conventional generator at index outside range");
	size_t timeIndex;
	try {
		timeIndex = getTimeIndex(time);
	} catch (Exception &) {
		cerr << "ERROR: requested time, index was not in the interface" << endl;
		throw;
	}
	return &conventionalData[timeIndex * nConventionalGenerators + generatorIndex];
}

void
SDInterface::dumpSDIStep(INTEGER step)
{
	// print simulation data
	// print ba data
	// print zone data
	// print conventional data
	// print load data
	// print wind data
	// print solar data
	// print ramp data
};

} /* END OF NAMESPACE simulator */
