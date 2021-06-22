/*
 * TestModel2.hpp
 *
 *  Created on: Jan 19, 2012
 *      Author: kglass
 */

#ifndef TESTMODEL2_HPP_
#define TESTMODEL2_HPP_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "Parser.hpp"
#include "simulation/Scheduler.hpp"
#include "model/generation/ConventionalGenerator.hpp"
#include "model/generation/StorageGenerator.hpp"
#include "model/generation/HydroGenerator.hpp"

namespace testing {

class TestModel2 : public CPPUNIT_NS :: TestFixture
{
	CPPUNIT_TEST_SUITE (TestModel2);
	CPPUNIT_TEST (generatorSetupTests);
	CPPUNIT_TEST_SUITE_END ();
public:
	TestModel2(){};
	virtual ~TestModel2(){};
	void setUp(void);
	void tearDown(void);
protected:
	void schedulerSetupTests(void);
	void generatorSetupTests(void);
	void zoneSetupTests(void);
	void baSetupTests(void);
private:
	simulator::Scheduler   * scheduler;
	interpreter::RIMParser * parser;
};

} /* namespace testing */
#endif /* TESTMODEL2_HPP_ */
