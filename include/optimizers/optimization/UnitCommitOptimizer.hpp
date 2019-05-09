/* ***************************************************************************
 * PURPOSE:
 * 		The purpose of the optimizer is twofold. First, it sets up generic
 * 		optimization information (UnitCommitOpt.cpp). Second, it set up the
 * 		solver specific optimization ucData (UCOptConfigure.cpp).
 *
 *		Each copy of the optimizer is associated with one balancing authority,
 *		so
 *
 * ---------------------------------------------------------------------------
 */

#pragma once

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

class Universe;

namespace model {

class UnitCommitOptimizer : public Optimizer
{
public:
	UnitCommitOptimizer(Universe & universe, UCData & ucData, size_t baID);
	virtual ~UnitCommitOptimizer();

	virtual void loadInitialOptimization();
	virtual void setupTrigger();
	virtual void dailyTrigger();
	virtual void ucTrigger();
	virtual void lfTrigger(){};
	virtual void regTrigger(){};
	virtual void configureRIMModel(){};

	virtual void cplexExceptionResponse(std::ostream & out);
	virtual void printState(std::ostream & out);
	virtual void printParameters(std::ostream & out);
	virtual void printOptimzationVariables(std::ostream & out);
protected:
	void tune(IloCplex &cplex);
	UCOptData optData;
	INTEGER getNConstraints(){return N_UC_CONSTRAINTS;};
	virtual void loadOptVariables();
	virtual void initializeModelSet();
	virtual void loadOptimization();
	virtual void handleNoSolution();
	virtual void handleSolution();
private:
	UCData & ucData;
	Universe & universe;

	virtual void setEnvironment() = 0;
	virtual void setInitialEnvironment() = 0;
	virtual void loadConstraint(INTEGER constraintIndex) = 0;

	virtual void updateDataState();
	virtual void updateOptState();

	void dumpZoneTransfers();
	void dumpRampSchedule();
	void dumpDailyProjection();

	virtual void finalOptimizerCleanUp();

	// Optimization specific
	virtual void initBAInfo(interpreter::RIMParser * parser);
	virtual void initZoneInfo(interpreter::RIMParser * parser);
	virtual void initGenerators(interpreter::RIMParser * parser);
	virtual void initConventional(interpreter::RIMParser * parser);

	void setSchedule();

	float                    smallestObjective;
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

	bool constraint_enabled(UC_CONSTRAINT c);
	bool constraint_enabled_for_initial_optimization(UC_CONSTRAINT c);
	void initializeOptimizer(void);
};

} /* END OF NAMESPACE model */
