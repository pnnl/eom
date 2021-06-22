/*
 * TestGeneratorParser.hpp
 *
 *  Created on: Jan 19, 2012
 *      Author: kglass
 */

#ifndef TESTGENERATORPARSER_HPP_
#define TESTGENERATORPARSER_HPP_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "interpreter/GeneratorParser.hpp"
#include "structures.hpp"

namespace testing {

class TestGeneratorParser : public CPPUNIT_NS :: TestFixture
{
	CPPUNIT_TEST_SUITE (TestGeneratorParser);
	CPPUNIT_TEST (inputTest);
	CPPUNIT_TEST_SUITE_END ();
public:
	TestGeneratorParser(){};
	virtual ~TestGeneratorParser(){};
	void setUp(void);
	void tearDown(void);

	friend class DataTestRIMParser;

protected:
	void inputTest(void);
private:
	interpreter::GeneratorParser  * generatorParser;
	char                   * paramSpace;
	char                   * interfaceSpace;
	SimulationInfo         * info;
};

} /* namespace testing */
#endif /* TESTGENERATORPARSER_HPP_ */
