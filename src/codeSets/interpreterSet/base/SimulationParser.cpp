#include "SimulationParser.hpp"
#include "exceptions/Exception.hpp"
#include "FileControl.hpp"
#include <sstream>
#include <stdexcept>

using std::string;
using std::clog;
using std::endl;

namespace interpreter {

SimulationParser::SimulationParser(const string & fileName, SimulationInfo & simulationInfo) :
	ModelParser(fileName)
{
	// account for simulation.csv header (1 line);
	nLines             -= 1;
	setModels(simulationInfo);
};


void
SimulationParser::setModels(SimulationInfo & simulationInfo)
{
	simulationInfo.nSimulations = 1;
	resetParser();
	resetParser();
	current             = skipLine(current);
	try {
		string           name           = getString(',');
		strcpy(simulationInfo.scenarioName, name.c_str());
		simulationInfo.start.date         = getDate(MMDDYYYY_SLASH, ',');
		simulationInfo.stop.date          = getDate(MMDDYYYY_SLASH, ',');
		simulationInfo.nDumpSteps         = getInteger(',');
		// ignore the baseTimeStep, this is computed in the derived data section
		getInteger(',');
		simulationInfo.daStep             = getInteger(',');
		simulationInfo.ucTrigger          = getInteger(',');
		simulationInfo.ucTimeStep         = getInteger(',');
		simulationInfo.ucLength           = getInteger(',');
		simulationInfo.lfTimeStep         = getInteger(',');
		simulationInfo.lfLength           = getInteger(',');
		simulationInfo.regTimeStep        = getInteger(',');
		simulationInfo.hydroTimeStep      = getInteger(',');
		simulationInfo.forecastDays       = getInteger(',');
		simulationInfo.nLoadRampSteps     = getInteger(',');
		simulationInfo.nRepeats           = getInteger(',');
		string           repeatString     = getString(" \t,\n", false);
		simulationInfo.repeatType         = getRepeatType(repeatString);
		if (isLineEmpty()) {
			simulationInfo.epGap = 1e-4;
		} else {
			increment(current);
			simulationInfo.epGap = this->getFloat();
		}
		current = skipLine(current);
		setDerivedData(simulationInfo);
		if (! validateInput(simulationInfo))
			throw std::runtime_error("SimulationInfo validation failed.");
	} catch (Exception & e) {
		clog << "ERROR (" <<  utility::FileControl::getFileName(__FILE__) << ":" << __LINE__ << "): failed to parse simulation info" << endl;
		clog << e << endl;
		throw;
	}
};

void
SimulationParser::setDerivedData(SimulationInfo & simulationInfo)
{
	// get basic timing information
	if (simulationInfo.lfTimeStep != 0) {
		simulationInfo.implementsLoadFollowing  = true;
	} else {
		simulationInfo.implementsLoadFollowing  = false;
	}

	if (simulationInfo.regTimeStep != 0) {
		simulationInfo.implementsRegulation  = true;
	} else {
		simulationInfo.implementsRegulation  = false;
	}

	/*
	 * set the baseTimeStep to the smallest non-zero increment among
	 * UC, LF and Reg.
	 */
	if (simulationInfo.implementsRegulation) {
		simulationInfo.baseTimeStep   = simulationInfo.regTimeStep;
	} else if (simulationInfo.implementsLoadFollowing){
		simulationInfo.baseTimeStep   = simulationInfo.lfTimeStep;
	} else {
		simulationInfo.baseTimeStep   = simulationInfo.ucTimeStep;
	}

	simulationInfo.nBaseStepsInProjection = simulationInfo.daStep*MINUTES_IN_HOUR/simulationInfo.baseTimeStep;
	simulationInfo.nBaseStepsInUCStep  = simulationInfo.ucTimeStep/simulationInfo.baseTimeStep;

	simulationInfo.nBaseStepsInLFStep  = simulationInfo.lfTimeStep/simulationInfo.baseTimeStep;
	simulationInfo.nBaseStepsInRegStep = simulationInfo.regTimeStep/simulationInfo.baseTimeStep;

	// lookahead days is the ucLength (in hours) divided by the hours in a day
	// one is added to the step in case
	INTEGER                  lookAheadDays  = (simulationInfo.ucLength/HOURS_IN_DAY) + 1;

	simulationInfo.circularBuffer      = simulationInfo.forecastDays;
	if (lookAheadDays + 1 >= simulationInfo.forecastDays) {
		simulationInfo.circularBuffer  = lookAheadDays + 1;
	}

	// setup basic time steps. convert to minutes (base time unit)
	simulationInfo.nUCTimeStepsInDay   = MINUTES_IN_DAY/simulationInfo.ucTimeStep;
	simulationInfo.nUCTimeStepsInHour  = MINUTES_IN_HOUR/simulationInfo.ucTimeStep;

	simulationInfo.nLFStepsInDay    = 0xDECAFBAD; // -557122643
	simulationInfo.nLFStepsInHour   = 0xDECAFBAD;
	simulationInfo.nLFStepsInUCStep = 0xDECAFBAD;

	if (simulationInfo.implementsLoadFollowing) {
		simulationInfo.nLFStepsInDay        = MINUTES_IN_DAY/simulationInfo.lfTimeStep;
		simulationInfo.nLFStepsInHour       = MINUTES_IN_HOUR/simulationInfo.lfTimeStep;
		simulationInfo.nLFStepsInUCStep     = simulationInfo.ucTimeStep/simulationInfo.lfTimeStep;
	}

	simulationInfo.nRegStepsInDay      = 0xDECAFBAD;
	simulationInfo.nRegStepsInHour     = 0xDECAFBAD;
	simulationInfo.nRegStepsInUCStep   = 0xDECAFBAD;
	simulationInfo.nRegStepsInLFStep   = 0xDECAFBAD;

	if (simulationInfo.implementsRegulation) {
		simulationInfo.nRegStepsInDay        = MINUTES_IN_DAY/simulationInfo.regTimeStep;
		simulationInfo.nRegStepsInHour       = MINUTES_IN_HOUR/simulationInfo.regTimeStep;
		simulationInfo.nRegStepsInUCStep     = simulationInfo.ucTimeStep/simulationInfo.regTimeStep;
		simulationInfo.nRegStepsInLFStep     = simulationInfo.lfTimeStep/simulationInfo.regTimeStep;
	}

	simulationInfo.nCanCommit         = 0;
	simulationInfo.nCanRegulate       = 0;

	simulationInfo.dumpSize                 = -1; // this is not known until all the model space requirements are known

	// set start and stop time to the second. NOTE: only minute is really necessary
	simulationInfo.start.tod.hours          = 0;
	simulationInfo.start.tod.minutes        = 0;
	simulationInfo.start.tod.seconds        = 0;

	simulationInfo.stop.tod.hours           = 23;
	simulationInfo.stop.tod.minutes         = 59;
	simulationInfo.stop.tod.seconds         = 59;

	setMinutesPastZero(simulationInfo.start); // this is really minutes past 0 AD
	setMinutesPastZero(simulationInfo.stop);

	// duration is the DD:HH:00 to DD:HH:59 to make it a complete day, add 1
	simulationInfo.duration           = simulationInfo.stop.minutesPastZero - simulationInfo.start.minutesPastZero + 1;
	simulationInfo.duration          /= simulationInfo.baseTimeStep; // normalize minutes to base steps

	// time lengths are given in minutes, the dumpSteps depend on the
	// baseTimeStep, which in turn is based on the
	// Number of SDI steps in buffer
	simulationInfo.nDumpSteps       = simulationInfo.nDumpSteps/simulationInfo.baseTimeStep;
	simulationInfo.nInterfaceSteps  = 2*simulationInfo.nDumpSteps;
}

bool SimulationParser::validateInput(SimulationInfo & simulationInfo) {
	bool valid = true;
	valid = valid && (!simulationInfo.implementsRegulation ||  0 == simulationInfo.regTimeStep % simulationInfo.baseTimeStep);
	valid = valid && (!(simulationInfo.implementsRegulation && simulationInfo.implementsLoadFollowing) || 0 == simulationInfo.lfTimeStep % simulationInfo.regTimeStep);
	valid = valid && (!simulationInfo.implementsLoadFollowing || 0 == simulationInfo.ucTimeStep % simulationInfo.lfTimeStep);
	valid = valid && 0 == simulationInfo.ucTimeStep % 60;
	return valid;
};

} /* END OF NAMESPACE interpreter */
