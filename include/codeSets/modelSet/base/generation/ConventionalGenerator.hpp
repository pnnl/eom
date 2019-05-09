#ifndef CONVENTIONALGENERATOR_HPP_
#define CONVENTIONALGENERATOR_HPP_

#include "structures.hpp"
#include "modelAbs/Generator.hpp"
#include "models/timeSeries/ScheduleData.hpp"
#include "RIMParser.hpp"
#include "simulation/SDInterface.hpp"

namespace model {

/** \class ConventionalGenerator
 * \brief The ConventionalGenerator class the behavior of conventional
 * generators including: coal, natural gas, nuclear, etc.
 */
class ConventionalGenerator : public Generator
{
public:
	ConventionalGenerator(const SimulationInfo & simulationInfo, size_t id, ConventionalParams & params);
	virtual ~ConventionalGenerator();

	virtual void dailyTrigger();
	virtual void setupTrigger();
	virtual void ucTrigger();
	virtual void lfTrigger();
	virtual void regTrigger();
	virtual char * getName();

	virtual void printState(std::ostream & out);
	virtual void printParameters(std::ostream & out);
	bool getCommit  ();
	bool getRegulate();
	bool getZone    ();
	bool getBA      ();

	void dumpData();
private:
	size_t                   conventionalID;
	RampControl            * ucRampSchedule;
	ConventionalParams     * params;
	simulator::SDInterface * interface;
};

} /* END OF NAMESPACE model */

#endif /* CONVENTIONALGENERATOR_HPP_ */
