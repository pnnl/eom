/*
 * ParserTest.hpp
 *
 *  Created on: Aug 10, 2011
 *      Author: kglass
 */

#ifndef PARSERTEST_HPP_
#define PARSERTEST_HPP_

#include "../../Validation.hpp"
#include "Parser.hpp"

namespace testing {

class ParserTest : public Validation, public interpreter::Parser
{
public:
	ParserTest(const char * name);
	virtual ~ParserTest();

	/*
	 * FILE WILL CONTAIN
	 * 12345
	 * 1
	 * 12
	 */
	static void setupBasic(const char * name);
	/*
	 * FILE WILL CONTAIN
	 * 123_45_6
	 *
	 *  1   5 6
	 * ____
	 *
	Char	Position	Line	Col
	1		0			1		1
	2		1			1		2
	3		2			1		3
	space	3			1		4
	4		4			1		5
	5		5			1		6
	space	6			1		7
	6		7			1		8
	cr		8			1		9
	cr		9			2		1
	space	10			3		1
	1		11			3		2
	space	12			3		3
	space	13			3		4
	space	14			3		5
	5		15			3		6
	space	16			3		7
	6		17			3		8
	space	18			3		9
	cr		19			3		10
	space	20			4		1
	space	21			4		2
	space	22			4		3
	space	23			4		4
	\0		24			4		5
	EOF		25			5		1
	 */
	static void setupCharacter(const char * name);

	void basicFunctions();
	void characterSearchFunctions();
	void valueSearchFunctions();
	void tagSearchFunctions();
	void modelParserTests();

protected:
	void setupTests(){};
	void validateParserTest();
	void validateParserFromFileTest();

	bool checkProgress(Cursor & cursor, char c, INTEGER pos, INTEGER line, INTEGER col);

	void countLinesTest();

	// cursor manipulation test
	void incrementCursorTest();
	void resetParserTest();
	void validBoundsTest();


	// character search tests
	void skipLineTest();
	void skipSpaceTest();
	void skipCharacterTest();

	void findNextCharacterTest();
	void findCharacterTest();
	void findSpaceTest();

	void matchTagTest();
	void findTagTest();

	// value parsing tests
	void getIntegerTest();
	void getCharacterTest();
	void getFloatTest();
	void getBoolTest();

	void getIntegerDelimiterTest();
	void getCharacterDelimiterTest();
	void getFloatDelimiterTest();
	void getBoolDelimiterTest();

	void getStringTest();
	void getStringDelimitedTest();

	void getTagValueTest();
	void getDateTest();
	void getTimeOfDayTest();
	void getTimeTest();
	void setSecondsPastZeroTest();

private:
	Cursor                   cursor;
};

} /* namespace testing */

#endif /* PARSERTEST_HPP_ */
