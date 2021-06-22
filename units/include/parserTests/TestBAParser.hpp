/*
 * TestBAParser.hpp
 *
 *  Created on: Jan 19, 2012
 *      Author: kglass
 */

#ifndef TestBAParser_HPP_
#define TestBAParser_HPP_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "interpreter/BAParser.hpp"
#include "model/administrative/BalancingAuthority.hpp"
#include "structures.hpp"

namespace testing {

class TestBAParser : public CPPUNIT_NS :: TestFixture
{
	CPPUNIT_TEST_SUITE (TestBAParser);
	CPPUNIT_TEST (inputTest);
	CPPUNIT_TEST_SUITE_END ();
public:
	TestBAParser(){};
	virtual ~TestBAParser(){};
	void setUp(void);
	void tearDown(void);

protected:
	void inputTest(void);
private:
	interpreter::BAParser  * baParser;
	bool                     rimTest;
	char                   * paramSpace;
	char                   * interfaceSpace;
	SimulationInfo         * info;
};

} /* namespace testing */
#endif /* TestBAParser_HPP_ */
