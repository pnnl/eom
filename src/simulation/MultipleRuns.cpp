/*
 * MultipleRuns.cpp
 *
 *  Created on: Oct 30, 2010
 *      Author: Kevin Glass
 */

#include "simulation/MultipleRuns.hpp"
#include "interpreter/ModelInterpreter.hpp"
#include "models/generic/ModelConstructor.hpp"

MultipleRuns::MultipleRuns(ModelInterpreter * interpreter)
{
	ModelConstructor       * constructor    = new ModelConstructor(interpreter);
	SimulationInfo         * simInfo        = interpreter->getSimulationInfo();
	INTEGER                  nRuns          = simInfo->nRuns;

//	scheduler           = Scheduler::getScheduler(interpreter);

	Scheduler * simulator = Scheduler::getScheduler(interpreter);
	for (INTEGER i = 0; i < nRuns; i++) {
		simulator->simulate();
		constructor->reset(); // resetting constructor reload the initial data and parameters
	}

}

MultipleRuns::~MultipleRuns()
{
}

void
usage(int argc, char * argv[])
{
	cout << "USAGE: \n";
}

int
main(int argc, char * argv[])
{
	try {
		usage(argc, argv);
	} catch (...) {
		exit(-1);
	}
	try {
		ModelInterpreter   * interpreter    = NULL;
#if defined XMLPARSER
		interpreter                 = new ModelInterpreter(definition, netCDF_wind_datafile, sourceFile);
#else
		interpreter                 = new ModelInterpreter(argv[1]);
#endif
		MultipleRuns runs(interpreter);
	} catch (...) {
	}
}
