/*
 * WindParserTest.hpp
 *
 *  Created on: Sep 15, 2011
 *      Author: kglass
 */

#ifndef WINDPARSERTEST_HPP_
#define WINDPARSERTEST_HPP_

#include "WindParser.hpp"
#include "../../Validation.hpp"

namespace testing {

class WindParserTest: public testing::Validation {
public:
	WindParserTest();
	virtual ~WindParserTest();

#if defined RIM_DEBUG
	friend class testing::ParserTest;
#endif
protected:
	virtual void             setupTests(){};

private:
	void constructWindParserTest(const char * fileName, INTEGER nWinds);
	void deconstructWindParserTest(const char * fileName);

	void setModelsTest(char * parameters, char * interface, SimulationInfo * info);

	void loadWindTest(INTEGER i, char * parameters, char * interface);
	void setDerivedDataTest(char * parameters, char * interface, SimulationInfo * info);
	void getCountTest();

	string                   srcName;
};

} /* namespace testing */
#endif /* WINDPARSERTEST_HPP_ */
