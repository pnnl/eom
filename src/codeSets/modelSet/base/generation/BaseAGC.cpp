/*
 * BaseAGC.cpp
 *
 *  Created on: Jan 15, 2011
 *      Author: kevin
 */

#include "generation/AGC.hpp"

BaseAGC::BaseAGC(RIMInterpreter & interpreter)
{


}

BaseAGC::~BaseAGC()
{

}
FLOAT
BaseAGC::computeAGC(FLOAT value)
{
	return agc;
}

FLOAT
BaseAGC::calcFreq(FLOAT rlDelta)
{
	FLOAT freqDelta  = 0.0;

	return freqDelta;
}

FLOAT
BaseAGC::calcACE(FLOAT actualInterchange, FLOAT scheduledInterchange)
{
	FLOAT ace  = 0.0;

	return ace;
}

FLOAT
BaseAGC::calcAGC(FLOAT ace)
{
	FLOAT age  = 0.0;

	return agc;
}

FLOAT
BaseAGC::control(FLOAT agc)
{
	FLOAT age  = 0.0;

	return agc;
}
