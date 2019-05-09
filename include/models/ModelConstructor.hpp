/*
 * ModelConstructor.hpp
 *
 *  Created on: Apr 13, 2011
 *      Author: kevin
 */

#ifndef MODELCONSTRUCTOR_HPP_
#define MODELCONSTRUCTOR_HPP_

#include "structures.hpp"
#include "simulation/Scheduler.hpp"
#include "simulation/SDInterface.hpp"
#include "model/administrative/BalancingAuthority.hpp"
#include "model/administrative/Zone.hpp"
#include "model/generation/ConventionalGenerator.hpp"
#include "model/generation/StorageGenerator.hpp"
#include "model/generation/HydroGenerator.hpp"
#include "models/timeSeries/ScheduleData.hpp"
#include "models/timeSeries/PredictionData.hpp"

namespace model {

class ModelConstructor {
public:
	ModelConstructor(const char * fileName);
	virtual ~ModelConstructor();

protected:
	void setupModels(interpreter::RIMParser & parser);

	virtual void optimize(){};

private:
	simulator::Scheduler   * scheduler;
	ScheduleData           * scheduleData;
	simulator::SDInterface * sdInterface;

	BalancingAuthority     * authority;
	Zone                   * zone;
	ConventionalGenerator  * conventional;

	StorageGenerator       * storage;
	HydroGenerator         * hydro;

	INTEGER                  nZones;
	INTEGER                  nBAs;
	INTEGER                  nConventionalGenerators;
	INTEGER                  nStorageGenerators;
	INTEGER                  nHydroGenerators;

};

}

#endif /* MODELCONSTRUCTOR_HPP_ */
