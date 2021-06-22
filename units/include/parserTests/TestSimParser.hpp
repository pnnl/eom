/*
 * TestSimParser.hpp
 *
 *  Created on: Jan 19, 2012
 *      Author: kglass
 */

#ifndef TESTSIMPARSER_HPP_
#define TESTSIMPARSER_HPP_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "interpreter/SimulationParser.hpp"
#include "structures.hpp"

namespace testing {

class TestSimParser : public CPPUNIT_NS :: TestFixture
{
	CPPUNIT_TEST_SUITE (TestSimParser);
	CPPUNIT_TEST (inputTest);
	CPPUNIT_TEST_SUITE_END ();
public:
	TestSimParser(){};
	virtual ~TestSimParser(){};
	void setUp(void);
	void tearDown(void);

	friend class DataTestRIMParser;

protected:
	void inputTest(void);
private:
	interpreter::SimulationParser * simParser;
	char                   * paramSpace;
	char                   * interfaceSpace;
	SimulationInfo         * info;
};

} /* namespace testing */
#endif /* TESTSIMPARSER_HPP_ */
