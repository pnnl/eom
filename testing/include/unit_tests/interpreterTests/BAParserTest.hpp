/*
 * BAParserTest.hpp
 *
 *  Created on: Sep 13, 2011
 *      Author: kglass
 */

#ifndef BAPARSERTEST_HPP_
#define BAPARSERTEST_HPP_

#include "../../Validation.hpp"
#include "BAParser.hpp"

namespace testing {

class BAParserTest: public testing::Validation {
public:
	BAParserTest();
	virtual ~BAParserTest();

#if defined RIM_DEBUG
	friend class testing::ParserTest;
#endif
protected:
	virtual void             setupTests(){};

private:
	void constructBAParserTest(const char * fileName, INTEGER nBAs);
	void deconstructBAParserTest(const char * fileName);

	void setModelsTest(char * parameters, char * interface, SimulationInfo * info);

	void loadBATwst(INTEGER i, char * parameters, char * interface);
	void setDerivedDataTest(char * parameters, char * interface, SimulationInfo * info);
	void getCountTest();

	string                   srcName;
};

} /* namespace testing */
#endif /* BAPARSERTEST_HPP_ */
