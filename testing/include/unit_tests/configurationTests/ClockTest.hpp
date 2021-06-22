/*
 * ClockTest.hpp
 *
 *  Created on: Oct 6, 2011
 *      Author: kglass
 */

#ifndef CLOCKTEST_HPP_
#define CLOCKTEST_HPP_

#include "simulation/Clock.hpp"
#include "Validation.hpp"

namespace testing {

class ClockTest: public testing::Validation {
public:
	ClockTest();
	virtual ~ClockTest();
protected:
	virtual void             setupTests(){};
	bool                     verify(INTEGER i);

private:
	INTEGER                  endTime;
	INTEGER                  baseTimeStep;
	INTEGER                  ucRecomputeLen;
	INTEGER                  ucTimeStep;
	INTEGER                  lfTimeStep;
	INTEGER                  regTimeStep;
	INTEGER                  dumpSize;
	INTEGER                  nDumpSteps;
	INTEGER                  nInterfaceSteps;

	INTEGER                  minuteStep;
	INTEGER                  dayStep;
	INTEGER                  hourStep;

	bool                     pass;
};

} /* namespace testing */
#endif /* CLOCKTEST_HPP_ */
