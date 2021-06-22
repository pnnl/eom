/*
 * TestZoneParser.hpp
 *
 *  Created on: Jan 19, 2012
 *      Author: kglass
 */

#ifndef TestZoneParser_HPP_
#define TestZoneParser_HPP_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "interpreter/ZoneParser.hpp"
#include "structures.hpp"

namespace testing {

class TestZoneParser : public CPPUNIT_NS :: TestFixture
{
	CPPUNIT_TEST_SUITE (TestZoneParser);
	CPPUNIT_TEST (inputTest);
	CPPUNIT_TEST (derivedTest);
	CPPUNIT_TEST_SUITE_END ();
public:
	TestZoneParser(){};
	virtual ~TestZoneParser(){};
	void setUp(void);
	void tearDown(void);

	friend class DataTestRIMParser;

protected:
	void inputTest(void);
	void test0(ZoneParams * zoneParser);
	void test1(ZoneParams * zoneParser);
	void test2(ZoneParams * zoneParser);
	void test3(ZoneParams * zoneParser);
	void test4(ZoneParams * zoneParser);
	void derivedTest(void);
private:
	interpreter::ZoneParser  * zoneParser;
	char                   * paramSpace;
	char                   * interfaceSpace;
	SimulationInfo         * info;
};

} /* namespace testing */
#endif /* TestZoneParser_HPP_ */
