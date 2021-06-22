/*
 * SDITest.hpp
 *
 *  Created on: Oct 12, 2011
 *      Author: kglass
 */

#ifndef SDITEST_HPP_
#define SDITEST_HPP_

#include "../../Validation.hpp"
#include "simulation/Scheduler.hpp"
#include "simulation/SDInterface.hpp"
#include "RIMParser.hpp"

namespace testing {

class SDITest: public testing::Validation {
public:
	SDITest(const char * fileName);
	virtual ~SDITest();
	virtual void             setupTests(){}
	bool sdiConstructionTest();
	bool regTriggerTest();
	void setDaysValues(INTEGER i);
	bool validateDaysValues(INTEGER i);
	bool validateInterface();
protected:

private:
	simulator::Scheduler   * scheduler;
	simulator::SDInterface * interface;
	interpreter::RIMParser * parser;
};

} /* namespace testing */
#endif /* SDITEST_HPP_ */
