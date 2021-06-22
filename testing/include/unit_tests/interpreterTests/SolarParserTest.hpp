/*
 * SolarParserTest.hpp
 *
 *  Created on: Sep 15, 2011
 *      Author: kglass
 */

#ifndef SOLARPARSERTEST_HPP_
#define SOLARPARSERTEST_HPP_

#include "SolarParser.hpp"
#include "../../Validation.hpp"

namespace testing {

class SolarParserTest: public testing::Validation {
public:
	SolarParserTest();
	virtual ~SolarParserTest();

#if defined RIM_DEBUG
	friend class testing::ParserTest;
#endif
protected:
	virtual void             setupTests(){};

private:
	void constructSolarParserTest(const char * fileName, INTEGER nSolars);
	void deconstructSolarParserTest(const char * fileName);

	void setModelsTest(char * parameters, char * interface, SimulationInfo * info);

	void loadSolarTest(INTEGER i, char * parameters, char * interface);
	void setDerivedDataTest(char * parameters, char * interface, SimulationInfo * info);
	void getCountTest();

	string                   srcName;
};

} /* namespace testing */
#endif /* SOLARPARSERTEST_HPP_ */
