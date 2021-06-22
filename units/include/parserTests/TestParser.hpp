/*
 * TestParser.hpp
 *
 *  Created on: Jan 19, 2012
 *      Author: kglass
 */

#ifndef TESTPARSER_HPP_
#define TESTPARSER_HPP_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "Parser.hpp"

namespace testing {

class TestParser : public CPPUNIT_NS :: TestFixture
{
	CPPUNIT_TEST_SUITE (TestParser);
	CPPUNIT_TEST (constructTests);
	CPPUNIT_TEST (basicMoveTests);
	CPPUNIT_TEST (skipMoveTests);
	CPPUNIT_TEST (characterMoveTests);
	CPPUNIT_TEST (valueTests);
	CPPUNIT_TEST (delimitedValueTests);
	CPPUNIT_TEST (dataTests);
	CPPUNIT_TEST (tagTests);
	CPPUNIT_TEST_SUITE_END ();
public:
	TestParser(){};
	virtual ~TestParser(){};
	void setUp(void);
	void tearDown(void);

	friend class DataTestRIMParser;

protected:
	void constructTests(void);
	void basicMoveTests(void);
	void skipMoveTests(void);
	void characterMoveTests(void);
	void valueTests(void);
	void delimitedValueTests(void);
	void dataTests(void);
	void tagTests(void);

	void writeBasic0File(void);
	void writeBasic1File(void);
	void increment(interpreter::Parser * parser, int steps);
	void skipLines(interpreter::Parser * parser, int steps);
private:
	interpreter::Parser    * simpleParser;
	interpreter::Parser    * characterParser;
	interpreter::Parser    * delimitedParser;
	interpreter::Parser    * tagParser;

	string                   dirName;
	string                   simpleFileName;
	string                   simpleFileData;
	string                   characterFileName;
	string                   characterFileData;
	string                   delimitedFileName;
	string                   delimitedFileData;
	string                   tagFileName;
};

} /* namespace testing */
#endif /* TESTPARSER_HPP_ */
