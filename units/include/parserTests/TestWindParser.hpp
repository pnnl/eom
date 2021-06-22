/*
 * TestWindParser.hpp
 *
 *  Created on: Jan 19, 2012
 *      Author: kglass
 */

#ifndef TESTWINDPARSER_HPP_
#define TESTWINDPARSER_HPP_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "interpreter/WindParser.hpp"
#include "structures.hpp"

namespace testing {

class TestWindParser : public CPPUNIT_NS :: TestFixture
{
	CPPUNIT_TEST_SUITE (TestWindParser);
	CPPUNIT_TEST (inputTest);
	CPPUNIT_TEST_SUITE_END ();
public:
	TestWindParser(){};
	virtual ~TestWindParser(){};
	void setUp(void);
	void tearDown(void);

protected:
	void inputTest(void);
private:
	interpreter::WindParser  * windParser;
	char                   * paramSpace;
	char                   * interfaceSpace;
	SimulationInfo         * info;
};

} /* namespace testing */
#endif /* TESTWINDPARSER_HPP_ */
