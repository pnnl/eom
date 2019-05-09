/* ***************************************************************************
 * Author      : Kevin Glass
 * Date        : Jan 13, 2011
 * File        : RegulationOpt.hpp
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

#ifndef REGULATIONOPT_HPP_
#define REGULATIONOPT_HPP_

#include "optimization_structs.hpp"
#include "optimizer_structs.hpp"

#include "optimizers/Optimizer.hpp"
#include "RIMParser.hpp"
#include "simulation/SDInterface.hpp"

#include "models/timeSeries/ScheduleData.hpp"
#include "models/timeSeries/AdjLoadTS.hpp"

#include "exceptions/Exception.hpp"

namespace model {

class RegulationOpt: public Optimizer
{
public:
	RegulationOpt(UCData & ucData, LFData & lfData, RegData & regData, size_t baID);
	virtual ~RegulationOpt();

	virtual void initializeOptimizer(){};
	virtual void loadInitialOptimization(){};
	virtual void dailyTrigger(){};
	virtual void ucTrigger(){};
	virtual void lfTrigger(){};
	virtual void regTrigger();
	virtual void configureRIMModel(){};

	virtual void cplexExceptionResponse(std::ostream & out);
	virtual void printState(std::ostream & out);
	virtual void printParameters(std::ostream & out);
	virtual void printOptimzationVariables(std::ostream & out);
protected:
	UCData                   ucData;
	LFData                   lfData;
	RegData                  regData;
	RegOptData               optData;

//	virtual void updateState();
	void tune(IloCplex &cplex) {};
	INTEGER getNConstraints(){return 10;};

	// ================================================================

	virtual void setEnvironment(){};
	virtual void setInitialEnvironment(){};
	virtual void setModel(){};
	virtual void loadObjective(){};
	virtual void loadConstraint(INTEGER constraintIndex){};

	virtual void updateDataState() {};
	virtual void updateOptState() {};

	virtual void setSchedules() {};
	virtual void finalOptimizerCleanUp() {};

	virtual void loadOptVariables(){};

	// Optimization specific
	virtual void initBAInfo() {};
	virtual void initializeModelSet() {};
	virtual void initZoneInfo() {};
	virtual void initGenerators() {};

	virtual void initConventional() {};
	virtual void initStorage(INTEGER offset) {};
	virtual void initHydro(INTEGER offset) {};

	virtual void loadOptimization() {};
	virtual void handleNoSolution();
	virtual void handleSolution();


	// ================================================================


private:
	INTEGER                  nGenerators;
	INTEGER                  baID;
	ScheduleData           * scheduleData;
	FLOAT                  * adjLoad;
};

} /* END OF NAMESPACE model */

#endif /* REGULATIONOPT_HPP_ */
