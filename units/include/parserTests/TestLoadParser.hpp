/*
 * TestLoadParser.hpp
 *
 *  Created on: Jan 19, 2012
 *      Author: kglass
 */

#ifndef TESTLOADPARSER_HPP_
#define TESTLOADPARSER_HPP_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "interpreter/LoadParser.hpp"
#include "structures.hpp"

namespace testing {

class TestLoadParser : public CPPUNIT_NS :: TestFixture
{
	CPPUNIT_TEST_SUITE (TestLoadParser);
	CPPUNIT_TEST (inputTest);
	CPPUNIT_TEST_SUITE_END ();
public:
	TestLoadParser(){};
	virtual ~TestLoadParser(){};
	void setUp(void);
	void tearDown(void);

protected:
	void inputTest(void);
private:
	interpreter::LoadParser  * loadParser;
	char                   * paramSpace;
	char                   * interfaceSpace;
	SimulationInfo         * info;
};

} /* namespace testing */
#endif /* TESTLOADPARSER_HPP_ */
