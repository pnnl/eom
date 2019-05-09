/* ***************************************************************************
 * Author      : Kevin Glass
 * Date        : Jan 13, 2011
 * File        : LoadFollowingOpt.hpp
 * Project     : RIM
 *
 * PURPOSE:
 *
 *
 * STATUS:
 *     naming convention conformity
 *     doxygen comments
 *     private comments
 *     comment block conformity
 *     testing information
 *     ensure hidden methods
 *
 * TESTING:
 *
 * ---------------------------------------------------------------------------
 */

#ifndef LOADFOLLOWINGOPT_HPP_
#define LOADFOLLOWINGOPT_HPP_

#include "optimization_structs.hpp"
#include "optimizer_structs.hpp"

#include "optimizers/Optimizer.hpp"
#include "RIMParser.hpp"
#include "simulation/SDInterface.hpp"
#include "simulation/Clock.hpp"

#include "models/timeSeries/ScheduleData.hpp"
#include "models/timeSeries/AdjLoadTS.hpp"

#include "exceptions/Exception.hpp"
#include "exceptions/TerminationException.hpp"


namespace model {

class LoadFollowingOpt: public Optimizer
{
public:
	LoadFollowingOpt(UCData & ucData, LFData & lfData, size_t baID);
	virtual ~LoadFollowingOpt();

	virtual void initializeOptimizer(const SimulationInfo &simulationInfo);
	virtual void loadInitialOptimization();
	virtual void setupTrigger();
	virtual void dailyTrigger();
	virtual void ucTrigger(){};
	virtual void lfTrigger();
	virtual void regTrigger(){};
	virtual void configureRIMModel(){};

	virtual void cplexExceptionResponse(std::ostream & out);
	virtual void printState(std::ostream & out);
	virtual void printParameters(std::ostream & out);
	virtual void printOptimzationVariables(std::ostream & out);

	LFData * getLFData(){return &lfData;};
	LFOptData * getLFOptData(){return &optData;};

protected:
	UCData                   ucData;
	LFData                   lfData;
	LFOptData                optData;

//	virtual void updateState();
	void tune(IloCplex &cplex) {};
	INTEGER getNConstraints(){return N_LF_CONSTRAINTS;};

	virtual void setEnvironment(){};
	virtual void setInitialEnvironment(){};
	virtual void setModel(){};
	virtual void loadObjective(){};
	virtual void loadConstraint(INTEGER constraintIndex){};

	virtual void updateDataState();
	virtual void updateOptState();

	void dumpRampSchedule();
	void dumpRampSchedule(std::ostream & out, INTEGER timeLength);
	void dumpDailyProjection();

	virtual void setSchedules();
	void cleanupOptimizer();
	virtual void finalOptimizerCleanUp();

	virtual void loadOptVariables();

	// Optimization specific
	virtual void initializeModelSet();
	virtual void initBAInfo(const SimulationInfo &simulationInfo);
	virtual void initZoneInfo();
	virtual void initGenerators(const SimulationInfo &simulationInfo);

	virtual void initConventional(const SimulationInfo &simulationInfo);

	virtual void loadOptimization();

	virtual void handleNoSolution();
	virtual void handleSolution();

	// ================================================================

private:
	simulator::SDInterface * interface;
	INTEGER                  baID;
	INTEGER                  currentTime;
	INTEGER                  genStartIndex;
	INTEGER                  convStartIndex;
	INTEGER                  storageStartIndex;
	INTEGER                  hydroStartIndex;
	BalanceAuthParams        baParams;

	FLOAT                  * zonePTDFData;
	FLOAT                  * zoneMaxLinePowerData;
	FLOAT                  * targetPowerData;
	FLOAT                 ** targetPower;
	LOGICAL                * generatorOnData;
	LOGICAL               ** generatorOn;
	LOGICAL                * switchOnData;
	LOGICAL               ** switchOn;
	LOGICAL                * switchOffData;
	LOGICAL               ** switchOff;
};

} /* END OF NAMESPACE model */

#endif /* LOADFOLLOWINGOPT_HPP_ */
