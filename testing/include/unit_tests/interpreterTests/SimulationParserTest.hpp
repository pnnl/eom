/*
 * SimulationParserTest.hpp
 *
 *  Created on: Sep 15, 2011
 *      Author: kglass
 */

#ifndef SIMULATIONPARSERTEST_HPP_
#define SIMULATIONPARSERTEST_HPP_

#include "SimulationParser.hpp"
#include "../../Validation.hpp"

namespace testing {

class SimulationParserTest: public testing::Validation {
public:
	SimulationParserTest();
	virtual ~SimulationParserTest();

#if defined RIM_DEBUG
	friend class testing::ParserTest;
#endif
protected:
	virtual void             setupTests(){};

private:
	void constructSimulationParserTest(const char * fileName, INTEGER nZones);
	void deconstructSimulationParserTest(const char * fileName);

	void setModelsTest(char * parameters, char * interface, SimulationInfo * info);

	bool checkSim(INTEGER sim, SimulationInfo * info,
			SimulationData * data);
	void loadSimulationTwst(INTEGER i, char * parameters, char * interface);
	void setDerivedDataTest(char * parameters, char * interface, SimulationInfo * info);
	void getCountTest();

	string                   srcName;
};

} /* namespace testing */
#endif /* SIMULATIONPARSERTEST_HPP_ */
