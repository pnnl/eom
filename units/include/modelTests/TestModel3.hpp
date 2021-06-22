/*
 * TestModel3.hpp
 *
 *  Created on: Jan 19, 2012
 *      Author: kglass
 */

#ifndef TESTMODEL3_HPP_
#define TESTMODEL3_HPP_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "Parser.hpp"
#include "simulation/Scheduler.hpp"
#include "model/generation/ConventionalGenerator.hpp"
#include "model/generation/StorageGenerator.hpp"
#include "model/generation/HydroGenerator.hpp"

namespace testing {

class TestModel3 : public CPPUNIT_NS :: TestFixture
{
	CPPUNIT_TEST_SUITE (TestModel3);
	CPPUNIT_TEST (generatorInitTests);
	CPPUNIT_TEST_SUITE_END ();
public:
	TestModel3(){};
	virtual ~TestModel3(){};
	void setUp(void);
	void tearDown(void);
protected:
	void generatorInitTests(void);
	void compareGenParams(ConventionalParams & expectedParams,
			ConventionalParams & initialValues, INTEGER expectedIndex,
			INTEGER actualIndex);
	void compareSharedParams(ConventionalParams & expectedParams,
			INTEGER expectedIndex, INTEGER actualIndex);
private:
	simulator::Scheduler   * scheduler;
	interpreter::RIMParser * parser;
};

} /* namespace testing */
#endif /* TESTMODEL3_HPP_ */
