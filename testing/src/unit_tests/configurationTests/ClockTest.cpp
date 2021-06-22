/*
 * ClockTest.cpp
 *
 *  Created on: Oct 6, 2011
 *      Author: kglass
 */

#include "ClockTest.hpp"

namespace testing {


ClockTest::ClockTest() :
		endTime(32),
		baseTimeStep(1),
		ucRecomputeLen(8),
		ucTimeStep(4),
		lfTimeStep(2),
		regTimeStep(1),
		dumpSize(0),
		nDumpSteps(8),
		nInterfaceSteps(16),
		pass(false)
{

	simulator::Clock::setClock(endTime, baseTimeStep, ucRecomputeLen, ucTimeStep,
			lfTimeStep, regTimeStep, nDumpSteps, dumpSize, nInterfaceSteps);

	//clock = new Clock(endTime, baseTimeStep, ucRecomputeLen, ucTimeStep,
	//		lfTimeStep, regTimeStep, nDumpSteps, nInterfaceSteps);

	if (simulator::Clock::clock->endTime != endTime) {
		*validationLog << "\tFAILED: incorrect load name " << endl;
		pass      |= true;
	}
	if (simulator::Clock::clock->baseTimeStep != baseTimeStep) {
		*validationLog << "\tFAILED: incorrect load name " << endl;
		pass      |= true;
	}
	if (simulator::Clock::clock->ucRecomputeLen != ucRecomputeLen) {
		*validationLog << "\tFAILED: incorrect load name " << endl;
		pass      |= true;
	}
	if (simulator::Clock::clock->nStepsInDump != nDumpSteps/baseTimeStep) {
		*validationLog << "\tFAILED: incorrect load name " << endl;
		pass      |= true;
	}
	if (simulator::Clock::clock->nInterfaceSteps != nInterfaceSteps) {
		*validationLog << "\tFAILED: incorrect load name " << endl;
		pass      |= true;
	}

	cout << "END TIME         = " << simulator::Clock::clock->endTime         << endl;
	cout << "BASE TIME STEP   = " << simulator::Clock::clock->baseTimeStep    << endl;

	cout << "INTERFACE LEN    = " << simulator::Clock::clock->nInterfaceSteps << endl;
	cout << "UC RECOMPUTE LEN = " << simulator::Clock::clock->ucRecomputeLen  << endl;

	cout << "LF STEP IN UC    = " << simulator::Clock::clock->nLFStepsInUCStep  << endl;
	cout << "DUMP LEN         = " << simulator::Clock::clock->nStepsInDump    << endl;

	for (INTEGER i = 0; i < endTime; i+=regTimeStep) {
		pass = verify(i);
		simulator::Clock::increment();
	}

}

ClockTest::~ClockTest(){}

bool
ClockTest::verify(INTEGER i)
{
	cout << "\n\nminute:   "      << simulator::Clock::clock->step  << endl;
	cout << "          \tPrevious\tCurrent\tNext" << endl;
	cout << "Index        \t" << simulator::Clock::getSDIPreviousIndex()          << "\t\t" << simulator::Clock::getSDICurrentIndex()          << "\t" << simulator::Clock::getSDINextIndex()          << endl;
	cout << "DUMP STEP    \t " << simulator::Clock::clock->dumpStep        << endl;
	cout << "Reg FLag     \t" << simulator::Clock::onRegStep()          << endl;
	cout << "LF FLag      \t" << simulator::Clock::onLFStep()           << endl;
	cout << "UC FLag      \t" << simulator::Clock::onUCStep()           << endl;
	cout << "UCP FLag     \t" << simulator::Clock::onUCProjectionStep() << endl;
	cout << "Dump FLag    \t" << simulator::Clock::onDumpStep()         << endl;
	return true;
}

} /* namespace testing */

int main (int argv, char * argc[])
{
	testing::ClockTest    clockTest;
	return 0 ;
}
