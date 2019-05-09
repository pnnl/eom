/* ***************************************************************************
 * Assumptions :
 * The only models simulated by the scheduler are the BA models and the
 * adjusted load model. The BA controls the execution of the zone, load and
 * optimizer models. The zone controls the execution of the generator models
 * ---------------------------------------------------------------------------
 */

#include "simulation/Scheduler.hpp"
#include "simulation/Clock.hpp"
#include "exceptions/Exception.hpp"
#include "exceptions/TerminationException.hpp"
#include "RIMParser.hpp"
#include "universe.hpp"
#include <boost/shared_ptr.hpp>

using boost::shared_ptr;
using model::ConventionalGenerator;
using std::endl;
using std::cerr;
using std::cout;

namespace simulator {

Scheduler          * Scheduler::scheduler        = NULL;

#define throwIfArraySizeZero(variable) if (variable == 0) { \
	cerr << "\tERROR: " << #variable << " was set to zero" << endl; \
	throw Exception(__FILE__, __LINE__, Exception::INVALID_ARRAY_SIZE); \
}

/**
 * \throws Exception::INVALID_REALLOCATION when a method attempts to allocate
 * models after the models were already allocated
 */
Scheduler::Scheduler(Universe & universe) :
		simulationInfo(universe.simulationInfo),
		sdInterface(simulationInfo, universe.conventionalData),
		scheduleData(simulationInfo),
		ucData(),
		baSet(),
		zoneSet(),
		convGenSet(),
		baGeneratorData(new model::Generator  *[simulationInfo.nGenerators]()),
		zoneGenerator(new model::Generator **[simulationInfo.nZones]),
		adjLoadTS()
{
	if (Scheduler::scheduler != NULL) {
		cerr << "ERROR: attempted to define multiple schedulers" << endl;
		throw Exception(__FILE__, __LINE__, Exception::INVALID_REDEFINITION);
	}
	Scheduler::scheduler = this; // TODO not thread safe
	Clock::setClock(simulationInfo);

	throwIfArraySizeZero(simulationInfo.nGenerators);
	throwIfArraySizeZero(simulationInfo.nCanCommit);
	throwIfArraySizeZero(simulationInfo.nCanRegulate);
	throwIfArraySizeZero(simulationInfo.nZones);
	throwIfArraySizeZero(simulationInfo.nBalancingAuths);

	for (size_t generatorIndex = 0; generatorIndex < simulationInfo.nConventionalGenerators; generatorIndex++) {
		ConventionalGenerator generator(simulationInfo, convGenSet.size(), universe.conventionalParameters.at(convGenSet.size()));
		convGenSet.push_back(generator);
	}
	for (size_t zone = 0, baGeneratorIndex = 0, convIndex = 0; zone < simulationInfo.nZones; zone++) {
		zoneGenerator[zone] = &baGeneratorData[baGeneratorIndex];
		baGeneratorIndex += universe.zoneParameters[zone].nGenerators;
		for (size_t gen = 0; gen < universe.zoneParameters[zone].nConventional; gen++) {
			zoneGenerator[zone][gen] = &convGenSet[convIndex];
			++convIndex;
		}
	}
	for (size_t zone = 0; zone < simulationInfo.nZones; zone++) {
		adjLoadTS.push_back(shared_ptr<model::AdjLoadTS>(new model::AdjLoadTS(zone, simulationInfo)));
		zoneSet.push_back(model::Zone(zoneGenerator[zone], universe.zoneParameters[zone]));
	}
	for (size_t ba = 0; ba < simulationInfo.nBalancingAuths; ba++) {
		ucData.push_back(UCData(simulationInfo, ba));
		baSet.push_back(model::BalancingAuthority(universe.balancingAuthorityParameters[ba], &zoneSet[0], ucData[ba], universe)); // Sets up each BA's optimizer set.
	}
}

Scheduler::~Scheduler()
{
	delete [] baGeneratorData;
	delete [] zoneGenerator;
	delete Clock::clock;
	Scheduler::scheduler        = NULL;
}

Scheduler *
Scheduler::getScheduler()
{
	if (Scheduler::scheduler == NULL) {
		cerr << "ERROR: attempted to access unallocated scheduler" << endl;
		throw Exception(__FILE__, __LINE__, Exception::INITIALIZATION);
	}

	return Scheduler::scheduler;
}
/*
 * The simulation assume the time zero values SDI have been set
 */
void
Scheduler::simulate(void)
{
	try {
		// set up time step 0
		for (size_t zone = 0; zone < simulationInfo.nZones; zone++) {
			adjLoadTS[zone]->dailyTrigger();
		}
		// generates the initial UC schedule
		for (size_t ba = 0; ba < simulationInfo.nBalancingAuths; ba++) {
			baSet[ba].setupTrigger();
		}

		// generates the initial LF schedule
		sdInterface.regTrigger();
		for (size_t ba = 0; ba < simulationInfo.nBalancingAuths; ba++) {
			baSet[ba].ucTrigger();
		}

		for (size_t zone = 0; zone < simulationInfo.nZones; zone++) {
			adjLoadTS[zone]->lfTrigger();
		}
		for (size_t ba = 0; ba < simulationInfo.nBalancingAuths; ba++) {
//				baSet[ba].lfTrigger();
		}
		// generates the initial regulation signal
		for (size_t zone = 0; zone < simulationInfo.nZones; zone++) {
			adjLoadTS[zone]->regTrigger();
		}
		for (size_t ba = 0; ba < simulationInfo.nBalancingAuths; ba++){
//			baSet[ba].regTrigger();
		}
//	} catch (std::exception e) {
//		cerr << "ERROR: simulation failed: " << e.what() << endl;
//		throw;
	} catch(Exception e) {
		cerr << "ERROR: simulation failed" << endl << e << endl;
		throw;
	}
	try {
		while(!Clock::increment()) {
			if (Clock::onUCProjectionStep()) {
				// TODO: make sure this is the right step
				for (size_t zone = 0; zone < simulationInfo.nZones; zone++) {
					adjLoadTS[zone]->dailyTrigger();
				}
				// set projections
				for (size_t ba = 0; ba < simulationInfo.nBalancingAuths; ba++) {
					baSet[ba].dailyTrigger();
				}
				cout << "." << endl;
			}
			// set the clock data for the current time step in the SDI
			sdInterface.regTrigger();
			// make each BA update the its state (all zones and generators)
			if (Clock::onUCStep()) {
				for (size_t ba = 0; ba < simulationInfo.nBalancingAuths; ba++) {
					baSet[ba].ucTrigger();
				}
			}

			if (Clock::onLFStep()) {
				for (size_t zone = 0; zone < simulationInfo.nZones; zone++) {
					adjLoadTS[zone]->lfTrigger();
				}
				// make each BA update the its state (all zones and generators)
				for (size_t ba = 0; ba < simulationInfo.nBalancingAuths; ba++) {
	//				baSet[ba].lfTrigger();
				}
			}
			for (size_t zone = 0; zone < simulationInfo.nZones; zone++) {
				adjLoadTS[zone]->regTrigger();
			}
			// make each BA update the its state (all zones and generators)
			for (size_t ba = 0; ba < simulationInfo.nBalancingAuths; ba++){
	//			baSet[ba].regTrigger();
			}
		}

	} catch (TerminationException & e) {
		cerr << "FATAL: simulation failed" << endl;
		throw;
	} catch (...) {
		cerr << "ERROR: simulation failed" << endl;
	}
	for (size_t ba = 0; ba < simulationInfo.nBalancingAuths; ba++) {
		baSet[ba].dumpZoneTransfers();
	}
}

} /* END OF NAMESPACE simulator */
