/*
 * LoadParserTest.hpp
 *
 *  Created on: Sep 15, 2011
 *      Author: kglass
 */

#ifndef LOADPARSERTEST_HPP_
#define LOADPARSERTEST_HPP_

#include "LoadParser.hpp"
#include "Validation.hpp"

namespace testing {

class LoadParserTest: public testing::Validation {
public:
	LoadParserTest();
	virtual ~LoadParserTest();

#if defined RIM_DEBUG
	friend class testing::ParserTest;
#endif
protected:
	virtual void             setupTests(){};

private:
	void constructLoadParserTest(const char * fileName, INTEGER nLoads);
	void deconstructLoadParserTest(const char * fileName);

	void setModelsTest(char * parameters, char * interface, SimulationInfo * info);

	void loadLoadTest(INTEGER i, char * parameters, char * interface);
	void setDerivedDataTest(char * parameters, char * interface, SimulationInfo * info);
	void getCountTest();

	string                   srcName;
};

} /* namespace testing */
#endif /* LOADPARSERTEST_HPP_ */
