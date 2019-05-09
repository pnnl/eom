#pragma once

#include "structures.hpp"
#include "optimization_structs.hpp"
#include "optimizer_structs.hpp"

#include <stdexcept>
#include <sstream>
#include <ilcplex/cplex.h>
#include <ilcplex/ilocplex.h>

#include "RIMParser.hpp"
#include "models/RIMModel.hpp"
#include "optimizers/ObjectiveFunction.hpp"
#include "optimizers/Constraint.hpp"

#include "exceptions/OptimizationException.hpp"
#include "exceptions/TerminationException.hpp"

class Universe;

namespace model {

template <typename TYPE>
class Optimization : public TYPE
{
public:
	Optimization(Universe & universe, UCData & ucData, size_t ba) : TYPE(universe, ucData, ba), fileNumber(0)
	{
		initializeOpt(ba);
	};
	Optimization(UCData & ucData, LFData & lfData, size_t ba) : TYPE(ucData, lfData, ba), fileNumber(0)
	{
		initializeOpt(ba);
	};
	Optimization(UCData & ucData, LFData & lfData, RegData & regData, size_t ba) : TYPE(ucData, lfData, regData, ba), fileNumber(0)
	{
		initializeOpt(ba);
	};
	virtual ~Optimization()
	{
		delete objective;
		delete [] activeConstraint;
		for (INTEGER element = 0; element < nConstraints; element++) {
			delete constraintSet[element];
		}
		delete [] constraintSet;
	};
	virtual void setEnvironment()
	{
		// Construct problem environment
		TYPE::optData.control.iloEnv    = IloEnv();
		TYPE::optData.control.iloModel  = IloModel(TYPE::optData.control.iloEnv);
		TYPE::optData.control.iloObj    = IloExpr(TYPE::optData.control.iloEnv);
		TYPE::loadOptimization();

		// Construct problem
		try {
			objective->load();
			for (INTEGER element = 0; element < nConstraints; element++) {
				if (activeConstraint[element] != NULL) {
					activeConstraint[element]->load();
				}
			}
		} catch (Exception & e) {
			std::cout << "\t" << __FILE__ << " " << __LINE__ << std::endl;
			std::cout << "\tERROR: could not load CPLEX model " << std::endl;
			throw;
		}
		solve();
	}
	virtual void setInitialEnvironment()
	{
		// Construct problem environment
		TYPE::optData.control.iloEnv    = IloEnv();
		TYPE::optData.control.iloModel  = IloModel(TYPE::optData.control.iloEnv);
		TYPE::optData.control.iloObj    = IloExpr(TYPE::optData.control.iloEnv);
		TYPE::initializeModelSet();
		TYPE::loadInitialOptimization();
		TYPE::loadOptVariables();

		// Construct problem
		try {
			objective->load();
			for (INTEGER element = 0; element < nConstraints; element++) {
				if (activeConstraint[element] != NULL) {
					activeConstraint[element]->load();
				}
			}
		} catch (Exception & e) {
			std::cerr << "\t" << __FILE__ << " " << __LINE__ << std::endl;
			std::cerr << "\tERROR: could not load CPLEX model " << std::endl;
			throw;
		}
		solve();
	}
protected:
private:
	void initializeOpt(INTEGER ba)
	{
		nConstraints         = TYPE::getNConstraints();
		constraintSet        = new Constraint *[nConstraints](); // available
		activeConstraint     = new Constraint *[nConstraints](); // in use
		objective            = NULL;
	}
	void cleanUpOptimizer()
	{
		TYPE::optData.control.iloEnv.end();
	}
	void solve()
	{
		using std::cerr;
		using std::endl;
		IloModel & model = TYPE::optData.control.iloModel;
		try {
			TYPE::optData.control.iloCplex = IloCplex(model);
		} catch(IloException &e) {
			cerr << "Error initializing CPLEX:" << endl;
			e.print(cerr);
			cerr << endl;
			throw;
		}
		IloCplex & cplex = TYPE::optData.control.iloCplex;
		TYPE::tune(cplex);
		try {

			std::stringstream ss;
			ss << "model" << fileNumber << ".lp";
			std::string modelFilename = ss.str();
			//cplex.exportModel(modelFilename.c_str());
			std::stringstream ss2;
			ss2 << "solution" << fileNumber;
			std::string solutionFilename = ss2.str();
			fileNumber++;

			bool hasSolution  = cplex.solve();
			if (hasSolution) {
				TYPE::handleSolution();
				//cplex.writeSolution(solutionFilename.c_str());

			} else {
				TYPE::handleNoSolution();
			}
			cleanUpOptimizer();
		} catch (OptimizationException & e) {
			std::cerr << __FILE__ << ":" << __LINE__ << " ";
			std::cerr << "ERROR: optimization failed" << std::endl;
			throw;
		} catch (IloException & e) {
			e.print(std::cerr);
			std::cerr << std::endl;
			TYPE::cplexExceptionResponse(std::cerr);
			cleanUpOptimizer();
			exit(-1);
		} catch (...) {
			std::cerr << __FILE__ << ":" << __LINE__ << " ";
			std::cerr << "Unknown exception caught" << std::endl;
			cleanUpOptimizer();
			exit(-1);
		}
	}

	/**
	 * \brief Add and load a constraintSet from the constraintSet set to the
	 * active constraints.
	 */
	void loadConstraint(INTEGER constraintIndex) {
		if (constraintIndex < 0 || constraintIndex >= nConstraints)
			TerminationException(__FILE__, __LINE__, Exception::INVALID_ARRAY);
		activeConstraint[constraintIndex] = constraintSet[constraintIndex];
	};

	/**
	 * \brief Add an objective function to the optimizer.
	 */
	void initObjective(ObjectiveFunction * objective) {
		this->objective = objective;
	};

	/**
	 * \brief Add a constraintSet to the optimizer's constraintSet set.
	 */
	void initConstraint(Constraint * constraint, INTEGER con) {
		if (con < 0 || con >= nConstraints) {
			throw std::runtime_error("constraint array index out of bounds");
		}
		constraintSet[con] = constraint;
	};

	ObjectiveFunction      * objective;
	Constraint            ** activeConstraint;
	Constraint            ** constraintSet;
	INTEGER                  nConstraints;
	int                      fileNumber;
};

} /* END OF NAMESPACE model */
