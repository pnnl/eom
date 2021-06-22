/*
 * TestModel.hpp
 *
 *  Created on: Jan 19, 2012
 *      Author: kglass
 */

#ifndef TESTMODEL1_HPP_
#define TESTMODEL1_HPP_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "Parser.hpp"
#include "simulation/Scheduler.hpp"

namespace testing {

class TestModel : public CPPUNIT_NS :: TestFixture
{
	CPPUNIT_TEST_SUITE (TestModel);
	CPPUNIT_TEST (schedulerSetupTests);
//	CPPUNIT_TEST (generatorSetupTests);
	CPPUNIT_TEST_SUITE_END ();
public:
	TestModel()
	{
		scheduler = NULL;
		parser = NULL;
	};
	virtual ~TestModel(){};
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
#endif /* TESTMODEL1_HPP_ */
