/*
 * TestModel4.hpp
 *
 *  Created on: Jan 19, 2012
 *      Author: kglass
 */

#ifndef TESTMODEL4_HPP_
#define TESTMODEL4_HPP_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "Parser.hpp"
#include "simulation/Scheduler.hpp"
#include "model/generation/ConventionalGenerator.hpp"
#include "model/generation/StorageGenerator.hpp"
#include "model/generation/HydroGenerator.hpp"

namespace testing {

class TestModel4 : public CPPUNIT_NS :: TestFixture
{
	CPPUNIT_TEST_SUITE (TestModel4);
	CPPUNIT_TEST (adminInitTests);
	CPPUNIT_TEST_SUITE_END ();
public:
	TestModel4(){};
	virtual ~TestModel4(){};
	void setUp(void);
	void tearDown(void);
protected:
	void adminInitTests(void);
	void compareBAParams(BalanceAuthParams & expectedParams,
			BalanceAuthParams & initialValues);
	void compareZoneParams(ZoneParams & expectedParams,
			ZoneParams & initialValues, INTEGER expectedIndex,
			INTEGER actualIndex);
private:
	simulator::Scheduler   * scheduler;
	interpreter::RIMParser * parser;
};

} /* namespace testing */
#endif /* TESTMODEL4_HPP_ */
