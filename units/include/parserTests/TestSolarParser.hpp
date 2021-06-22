/*
 * TestSolarParser.hpp
 *
 *  Created on: Jan 19, 2012
 *      Author: kglass
 */

#ifndef TESTSOLARPARSER_HPP_
#define TESTSOLARPARSER_HPP_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "interpreter/SolarParser.hpp"
#include "structures.hpp"

namespace testing {

class TestSolarParser : public CPPUNIT_NS :: TestFixture
{
	CPPUNIT_TEST_SUITE (TestSolarParser);
	CPPUNIT_TEST (inputTest);
	CPPUNIT_TEST_SUITE_END ();
public:
	TestSolarParser(){};
	virtual ~TestSolarParser(){};
	void setUp(void);
	void tearDown(void);

protected:
	void inputTest(void);
private:
	interpreter::SolarParser  * solarParser;
	char                   * paramSpace;
	char                   * interfaceSpace;
	SimulationInfo         * info;
};

} /* namespace testing */
#endif /* TESTSOLARPARSER_HPP_ */
