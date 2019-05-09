#ifndef SDINTERFACE_HPP_
#define SDINTERFACE_HPP_

#include "structures.hpp"
#include "optimization_structs.hpp"
#include "RIMParser.hpp"
#include "models/RIMModel.hpp"
#include "exceptions/Exception.hpp"

#if defined RIM_DEBUG
namespace testing {
class TestModel;
}
#endif

namespace simulator {

class SDInterface : public model::RIMModel
{
public:
	SDInterface(const SimulationInfo & simulationInfo, std::vector<ConventionalData> & conventionalData);
	static SDInterface * getSDInterface();
	virtual ~SDInterface();

	virtual void dailyTrigger(){};
	virtual void ucTrigger(){};
	virtual void lfTrigger(){};
	virtual void regTrigger();
	void configureRIMModel(){};

	FLOAT getEpGap() { return this->epGap; }

	// returns the set of requested model values at t = time
	ConventionalData   * getConventionalData(INTEGER time);
	ConventionalData   * getConventionalData(INTEGER time, size_t conventionalGeneratorIndex);

	RampControl       ** getBARampSchedule(size_t ba){return &rampSchedule[ba];};
	RampControl        * getGenRampSchedule(size_t gen){return rampSchedule[gen];};
private:
	INTEGER              getTimeIndex(INTEGER time);
	LOGICAL              isTimeValid(INTEGER time);
	static SDInterface     * interface;
	std::vector<ConventionalData>&conventionalData;
	FLOAT                         epGap;
	size_t                        nGenerators;
	size_t                        nConventionalGenerators;
	size_t                        ucLength;
	INTEGER                       sdiTimeSteps;
	std::vector<RampControl>      rampScheduleData;
	std::vector<RampControl*>     rampSchedule;

	SDInterface & operator = (SDInterface & interface){return *this;};

	void dumpSDIStep(INTEGER step);
};

} /* END OF NAMESPACE simulator */

#endif /* SDINTERFACE_HPP_ */
