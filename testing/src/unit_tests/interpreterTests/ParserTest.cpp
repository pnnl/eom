/*
 * ParserTest.cpp
 *
 *  Created on: Aug 10, 2011
 *      Author: Kevin Glass
 */

#include "ParserTest.hpp"
#include "GeneratorParserTest.hpp"
#include "BAParserTest.hpp"
#include "ZoneParserTest.hpp"
#include "SimulationParserTest.hpp"
#include "LoadParserTest.hpp"
#include "WindParserTest.hpp"
#include "SolarParserTest.hpp"
#include "RIMParserTest.hpp"
#include "exceptions/Exception.hpp"

namespace testing {

ParserTest::ParserTest(const char * name) : Validation("results/log"), interpreter::Parser(name){}
ParserTest::~ParserTest(){}

void
ParserTest::setupBasic(const char * name)
{
	// generate simple.inp;
	FILE * fh = fopen(name, "w");

	fprintf(fh, "%d", 1);
	fprintf(fh, "%d", 2);
	fprintf(fh, "%d", 3);
	fprintf(fh, "%d", 4);
	fprintf(fh, "%d\n", 5);
	fprintf(fh, "%d\n", 1);
	fprintf(fh, "%d", 1);
	fprintf(fh, "%d", 2);

	fclose(fh);
}

void
ParserTest::setupCharacter(const char * name)
{
	// generate simple.inp;
	FILE * fh = fopen(name, "w");

	fprintf(fh, "%s\n", "123 45 6");
	fprintf(fh, "\n");
	fprintf(fh, "%s\n", " 1   5 6 ");
	fprintf(fh, "%s\n", "    ");

	fclose(fh);
}

void
ParserTest::basicFunctions()
{
	*validationLog << "Basic parser tests" << endl;
	cout << "Basic parser tests" << endl;
	setupTests();
	validateParserTest();
	validateParserFromFileTest();

	*validationLog << "COUNT LINES TEST" << endl;
	countLinesTest();
	*validationLog << "TESTING CURSOR MANIPULATION" << endl;
	// cursor manipulation test
	incrementCursorTest();
	resetParserTest();
	validBoundsTest();

	// character data

}

void
ParserTest::characterSearchFunctions()
{
	*validationLog << "Search tests" << endl;
	cout << "Character search tests" << endl;
	*validationLog << "CHARACTER SEARCH" << endl;

	skipLineTest();
	skipSpaceTest();
	skipCharacterTest();

	findNextCharacterTest();
	findCharacterTest();
	findSpaceTest();
}

void
ParserTest::valueSearchFunctions()
{
	*validationLog << "VALUE SEARCH" << endl;
	cout << "Value search tests" << endl;

	getIntegerTest();
	getCharacterTest();
	getFloatTest();
	getBoolTest();

	getIntegerDelimiterTest();
	getCharacterDelimiterTest();
	getFloatDelimiterTest();
	getBoolDelimiterTest();

	getStringTest();
	getStringDelimitedTest();

	getDateTest();
	getTimeOfDayTest();
	getTimeTest();
	setSecondsPastZeroTest();
}

void
ParserTest::tagSearchFunctions()
{
	*validationLog << "TAG SEARCH" << endl;
	cout << "Tag search tests" << endl;

	matchTagTest();
	findTagTest();

	getTagValueTest();
}

void
ParserTest::validateParserTest()
{
	try {
		incrementTest();
		bool                 failed         = false;

		if (current.position != 0) {
			failed   = true;
			*validationLog << "\t\tFAIL (validateParserTest.positive) : current.position == " << current.position << endl;
		}
		if (current.line != 1) {
			failed   = true;
			*validationLog << "\t\tFAIL (validateParserTest.positive) : current.line     == " << current.line << endl;
		}
		if (current.column != 1) {
			failed   = true;
			*validationLog << "\t\tFAIL (validateParserTest.positive) : current.colum    == " << current.column << endl;
		}

		if (next.position != 0) {
			failed   = true;
			*validationLog << "\t\tFAIL (validateParserTest.positive) : next.position == " << next.position << endl;
		}
		if (next.line != 1) {
			failed   = true;
			*validationLog << "\t\tFAIL (validateParserTest.positive) : next.line     == " << next.line << endl;
		}
		if (next.column != 1) {
			failed   = true;
			*validationLog << "\t\tFAIL (validateParserTest.positive) : next.colum    == " << next.column << endl;
		}

		if (file == NULL) {
			failed   = true;
			*validationLog << "\t\tFAIL (validateParserTest.positive) : file == NULL" << endl;
		}

		if (endOfFile <= 0) {
			failed   = true;
			*validationLog << "\t\tFAIL (validateParserTest.positive) : endOfFile == " << endOfFile << endl;
		}

		if (nLines != 0) {
			failed   = true;
			*validationLog << "\t\tFAIL (validateParserTest.positive) : nLines == " << nLines << endl;
		}

		if (failed) {
			incrementFailure();
			*validationLog << "\tFAIL (validateParserTest.positive) : Parser did not initialize correctly" << endl;
		} else {
			*validationLog << "\tPASS (validateParserTest.positive) : Parser initialized correctly" << endl;
		}
	} catch (Exception & e) {
		incrementFailure();
		*validationLog << "\tFAIL (validateTest.positive) : test failed due to file exception" << endl;
		*validationLog << e << endl;
	} catch (...) {
		incrementFailure();
		*validationLog << "\tFAIL (validateTest.positive) : test failed due to unknown exception" << endl;
		cout << "Validate Test for unknown reasons" << endl;
	}
}


void
ParserTest::validateParserFromFileTest()
{
	incrementTest();
	bool                 failed         = false;
	try {
		FileData           * data           = utility::FileControl::loadFile("testing/standards/parserTests/delimitedData.inp");
		interpreter::Parser      parser(data);

		if (parser.current.position != 0) {
			failed   = true;
			*validationLog << "\t\tFAIL (validateParserFromFileTest.positive) : current.position == " << parser.current.position << endl;
		}
		if (parser.current.line != 1) {
			failed   = true;
			*validationLog << "\t\tFAIL (validateParserFromFileTest.positive) : current.line     == " << parser.current.line << endl;
		}
		if (parser.current.column != 1) {
			failed   = true;
			*validationLog << "\t\tFAIL (validateParserFromFileTest.positive) : current.colum    == " << parser.current.column << endl;
		}

		if (parser.next.position != 0) {
			failed   = true;
			*validationLog << "\t\tFAIL (validateParserFromFileTest.positive) : next.position == " << parser.next.position << endl;
		}
		if (parser.next.line != 1) {
			failed   = true;
			*validationLog << "\t\tFAIL (validateParserFromFileTest.positive) : next.line     == " << parser.next.line << endl;
		}
		if (parser.next.column != 1) {
			failed   = true;
			*validationLog << "\t\tFAIL (validateParserFromFileTest.positive) : next.colum    == " << parser.next.column << endl;
		}

		if (parser.file == NULL) {
			failed   = true;
			*validationLog << "\t\tFAIL (validateParserFromFileTest.positive) : file == NULL" << endl;
		}

		if (parser.endOfFile <= 0) {
			failed   = true;
			*validationLog << "\t\tFAIL (validateParserFromFileTest.positive) : endOfFile == " << parser.endOfFile << endl;
		}

		if (parser.nLines != 0) {
			failed   = true;
			*validationLog << "\t\tFAIL (validateParserFromFileTest.positive) : nLines == " << nLines << endl;
		}
		if (failed) {
			incrementFailure();
			*validationLog << "\tFAIL (validateParserFromFileTest.positive) : Parser did not initialize from file correctly" << endl;
		} else {
			*validationLog << "\tPASS (validateParserFromFileTest.positive) : Parser initialized from file correctly" << endl;
		}
	} catch (Exception & e) {
		incrementFailure();
		*validationLog << "\tFAIL (validateParserFromFileTest.positive) : test failed due to file exception" << endl;
		*validationLog << e << endl;
	} catch (...) {
		incrementFailure();
		*validationLog << "\tFAIL (validateParserFromFileTest.positive) : test failed due to unknown exception" << endl;
		cout << "Validate Test for unknown reasons" << endl;
	}
}

bool
ParserTest::checkProgress(Cursor & cursor, char c, INTEGER pos, INTEGER line, INTEGER col)
{
	bool                     rtn            = true;

	if (file->fileBuffer[cursor.position] != c ||
			cursor.position != pos ||
			cursor.line != line ||
			cursor.column != col) {
		rtn = false;
	}
	return rtn;
}

void
ParserTest::countLinesTest()
{
	incrementTest();
	INTEGER                  spaces         = countLines();

	if (spaces != 3) {
		incrementFailure();
		*validationLog << "\tFAIL (countLinesTest.positive) : number of chars =  " << spaces << endl;
	} else {
		*validationLog << "\tPASS (countLinesTest.positive) : count chars passed" << endl;
	}
}

void
ParserTest::resetParserTest()
{
	incrementTest();
	bool                     failed         = false;
	bool                     check          = false;

	check = checkProgress(next, '2', 9, 3, 2);
	if (!check) {
		failed   = true;
		*validationLog << "\t\tFAIL (resetParserTest.positive) : current.position == " << current.position << endl;
	}

	resetParser();

	check = checkProgress(next, '1', 0, 1, 1);
	if (!check) {
		failed   = true;
		*validationLog << "\t\tFAIL (resetParserTest.positive) : current.position == " << current.position << endl;
	}

	if(!failed) {
		*validationLog << "\tPASS (resetParserTest.positive) : reset passed" << endl;
	} else {
		incrementFailure();
		*validationLog << "\tFAIL (resetParserTest.positive) : reset failed" << endl;
	}
}

void
ParserTest::incrementCursorTest()
{
	incrementTest();
	bool                     failed         = false;
	bool                     check          = false;
	resetParser();
	check = checkProgress(next, '1', 0, 1, 1);
	if (!check) {
		failed   = true;
		*validationLog << "\t\tFAIL (incrementCursorTest.positive) : next.position == " << next.position << endl;
	}
	increment(next);

	check = checkProgress(next, '2', 1, 1, 2);
	if (!check) {
		failed   = true;
		*validationLog << "\t\tFAIL (incrementCursorTest.positive) : next.position == " << next.position << endl;
	}
	increment(next);

	check = checkProgress(next, '3', 2, 1, 3);
	if (!check) {
		failed   = true;
		*validationLog << "\t\tFAIL (incrementCursorTest.positive) : next.position == " << next.position << endl;
	}
	increment(next);

	check = checkProgress(next, '4', 3, 1, 4);
	if (!check) {
		failed   = true;
		*validationLog << "\t\tFAIL (incrementCursorTest.positive) : next.position == " << next.position << endl;
	}
	increment(next);

	check = checkProgress(next, '5', 4, 1, 5);
	if (!check) {
		failed   = true;
		*validationLog << "\t\tFAIL (incrementCursorTest.positive) : next.position == " << next.position << endl;
	}
	increment(next);

	check = checkProgress(next, '\n', 5, 1, 6);
	if (!check) {
		failed   = true;
		*validationLog << "\t\tFAIL (incrementCursorTest.positive) : next.position == " << next.position << endl;
	}
	increment(next);

	check = checkProgress(next, '1', 6, 2, 1);
	if (!check) {
		failed   = true;
		*validationLog << "\t\tFAIL (incrementCursorTest.positive) : next.position == " << next.position << endl;
	}
	increment(next);

	check = checkProgress(next, '\n', 7, 2, 2);
	if (!check) {
		failed   = true;
		*validationLog << "\t\tFAIL (incrementCursorTest.positive) : next.position == " << next.position << endl;
	}
	increment(next);

	check = checkProgress(next, '1', 8, 3, 1);
	if (!check) {
		failed   = true;
		*validationLog << "\t\tFAIL (incrementCursorTest.positive) : next.position == " << next.position << endl;
	}
	increment(next);

	check = checkProgress(next, '2', 9, 3, 2);
	if (!check) {
		failed   = true;
		*validationLog << "\t\tFAIL (incrementCursorTest.positive) : next.position == " << next.position << endl;
	}

	if(!failed) {
		*validationLog << "\tPASS (incrementCursorTest.positive) : increment next passed" << endl;
	} else {
		incrementFailure();
		*validationLog << "\tFAIL (incrementCursorTest.positive) : increment next failed" << endl;
	}

}

void
ParserTest::validBoundsTest()
{
	incrementTest();
	bool                     failed         = false;
	bool                     check          = false;

	check = validBounds(endOfFile - 1);
	if (!check) {
		failed   = true;
		*validationLog << "\t\tFAIL (validBoundsTest.positive) : position within bounds test failed" << endl;
	}

	try {
		for(INTEGER i = 0; i < endOfFile; i++) {
			increment(current);
		}
		check = false;
		increment(current);
	} catch (Exception & e) {
		check = true;
	}

	if (!check) {
		failed   = true;
		*validationLog << "\t\tFAIL (validBoundsTest.positive) : failed to catch parser exception from increment" << endl;
	}

	check = validBounds(endOfFile+1);
	if (check) {
		failed   = true;
		*validationLog << "\t\tFAIL (validBoundsTest.positive) : position > bounds test failed" << endl;
	}

	check = validBounds(-1);
	if (check) {
		failed   = true;
		*validationLog << "\t\tFAIL (validBoundsTest.positive) : position < bounds test failed" << endl;
	}

	if(!failed) {
		*validationLog << "\tPASS (validBoundsTest.positive) : valid bounds check passed" << endl;
	} else {
		incrementFailure();
		*validationLog << "\tFAIL (validBoundsTest.positive) : valid bounds check failed" << endl;
	}
}


// END OF BASIC TESTS

void
ParserTest::skipLineTest()
{
	incrementTest();
	bool                     failed         = false;
	bool                     check          = false;

	resetParser();

	skipLine();
	check = checkProgress(next, '\n', 9, 2, 1);
	if (!check) {
		failed   = true;
		*validationLog << "\t\tFAIL (skipLineTest.positive) : cursor is in the wrong position" << endl;
	}

	skipLine();
	check = checkProgress(next, ' ', 10, 3, 1);
	if (!check) {
		failed   = true;
		*validationLog << "\t\tFAIL (skipLineTest.positive) : cursor is in the wrong position" << endl;
	}

	skipLine();
	check = checkProgress(next, ' ', 20, 4, 1);
	if (!check) {
		failed   = true;
		*validationLog << "\t\tFAIL (skipLineTest.positive) : cursor is in the wrong position" << endl;
	}

	skipLine();
	skipLine();
	skipLine();
	check = checkProgress(next, '\0', 25, 5, 1);
	if (!check) {
		failed   = true;
		*validationLog << "\t\tFAIL (skipLineTest.positive) : cursor is in the wrong position" << endl;
	}

	if(!failed) {
		*validationLog << "\tPASS (skipLineTest.positive) : skip line passed" << endl;
	} else {
		incrementFailure();
		*validationLog << "\tFAIL (skipLineTest.positive) : skip line failed" << endl;
	}
}

void
ParserTest::skipSpaceTest()
{
	incrementTest();
	bool                     failed         = false;
	bool                     check          = false;

	resetParser();
	check = checkProgress(next, '1', 0, 1, 1);
	if (!check) {
		failed   = true;
		*validationLog << "\t\tFAIL (skipSpaceTest.positive) : cursor is in the wrong position" << endl;
	}
	increment(next);

	skipSpace();
	check = checkProgress(next, '2', 1, 1, 2);
	if (!check) {
		failed   = true;
		*validationLog << "\t\tFAIL (skipSpaceTest.positive) : cursor is in the wrong position" << endl;
	}
	increment(next);

	skipSpace();
	check = checkProgress(next, '3', 2, 1, 3);
	if (!check) {
		failed   = true;
		*validationLog << "\t\tFAIL (skipSpaceTest.positive) : cursor is in the wrong position" << endl;
	}
	increment(next);


	skipSpace();
	check = checkProgress(next, '4', 4, 1, 5);
	if (!check) {
		failed   = true;
		*validationLog << "\t\tFAIL (skipSpaceTest.positive) : cursor is in the wrong position" << endl;
	}
	increment(next);


	skipSpace();
	check = checkProgress(next, '5', 5, 1, 6);
	if (!check) {
		failed   = true;
		*validationLog << "\t\tFAIL (skipSpaceTest.positive) : cursor is in the wrong position" << endl;
	}
	increment(next);


	skipSpace();
	check = checkProgress(next, '6', 7, 1, 8);
	if (!check) {
		failed   = true;
		*validationLog << "\t\tFAIL (skipSpaceTest.positive) : cursor is in the wrong position" << endl;
	}
	increment(next);


	skipSpace();
	check = checkProgress(next, '1', 11, 3, 2);
	if (!check) {
		failed   = true;
		*validationLog << "\t\tFAIL (skipSpaceTest.positive) : cursor is in the wrong position" << endl;
	}
	increment(next);


	skipSpace();
	check = checkProgress(next, '5', 15, 3, 6);
	if (!check) {
		failed   = true;
		*validationLog << "\t\tFAIL (skipSpaceTest.positive) : cursor is in the wrong position" << endl;
	}
	increment(next);


	skipSpace();
	check = checkProgress(next, '6', 17, 3, 8);
	if (!check) {
		failed   = true;
		*validationLog << "\t\tFAIL (skipSpaceTest.positive) : cursor is in the wrong position" << endl;
	}
	increment(next);


	skipSpace();
	check = next.position == endOfFile;
	if (!check) {
		failed   = true;
		*validationLog << "\t\tFAIL (skipSpaceTest.positive) : cursor is in the wrong position" << endl;
	}

	if(!failed) {
		*validationLog << "\tPASS (skipSpaceTest.positive) : skip line passed" << endl;
	} else {
		incrementFailure();
		*validationLog << "\tFAIL (skipSpaceTest.positive) : skip line failed" << endl;
	}
}

void
ParserTest::skipCharacterTest()
{
	incrementTest();
	LOGICAL                  check          = false;
	LOGICAL                  failed         = false;

	resetParser();

	skipCharacter('1');
	check = checkProgress(next, '2', 1, 1, 2);
	if (!check) {
		failed   = true;
		*validationLog << "\t\tFAIL (skipCharacterTest.positive) : cursor is in the wrong position" << endl;
	}

	skipCharacter('1');
	check = checkProgress(next, ' ', 12, 3, 3);
	if (!check) {
		failed   = true;
		*validationLog << "\t\tFAIL (skipCharacterTest.positive) : cursor is in the wrong position" << endl;
	}

	try {
		check = false;
		skipCharacter('1');
		check = checkProgress(next, ' ', 12, 3, 3);
		if (!check) {
			failed   = true;
			*validationLog << "\t\tFAIL (skipCharacterTest.positive) : cursor is in the wrong position" << endl;
		}
	} catch (Exception & e) {
		check = true;
	}
	if (!check) {
		failed   = true;
		*validationLog << "\t\tFAIL (skipCharacterTest.positive) : failed to catch the end of file" << endl;
	}

	if(!failed) {
		*validationLog << "\tPASS (skipCharacterTest.positive) : skip character passed" << endl;
	} else {
		incrementFailure();
		*validationLog << "\tFAIL (skipCharacterTest.positive) : skip character failed" << endl;
	}
}

void
ParserTest::findNextCharacterTest()
{
	incrementTest();
	LOGICAL                  check          = false;
	LOGICAL                  failed         = false;

	resetParser();
	next.position = 12;
	next.line     = 3;
	next.column   = 3;

	findNextCharacter();
	check = checkProgress(next, '5', 15, 3, 6);
	if (!check) {
		failed   = true;
		*validationLog << "\t\tFAIL (findNextCharacterTest.positive) : cursor is in the wrong position" << endl;
	}

	if(!failed) {
		*validationLog << "\tPASS (findNextCharacterTest.positive) : skip character passed" << endl;
	} else {
		incrementFailure();
		*validationLog << "\tFAIL (findNextCharacterTest.positive) : skip character failed" << endl;
	}
}

void
ParserTest::findCharacterTest()
{
	incrementTest();
	LOGICAL                  check          = false;
	LOGICAL                  failed         = false;

	resetParser();

	findCharacter('1');
	check = checkProgress(next, '1', 0, 1, 1);
	if (!check) {
		failed   = true;
		*validationLog << "\t\tFAIL (findCharacterTest.positive) : cursor is in the wrong position" << endl;
	}
	increment(next);
	findCharacter('1');
	check = checkProgress(next, '1', 11, 3, 2);
	if (!check) {
		failed   = true;
		*validationLog << "\t\tFAIL (findCharacterTest.positive) : cursor is in the wrong position" << endl;
	}

	try {
		check = false;
		increment(next);
		findCharacter('1');
		check = checkProgress(next, ' ', 12, 3, 3);
		if (!check) {
			failed   = true;
			*validationLog << "\t\tFAIL (findCharacterTest.positive) : cursor is in the wrong position" << endl;
		}
	} catch (Exception & e) {
		check = true;
	}
	if (!check) {
		failed   = true;
		*validationLog << "\t\tFAIL (findCharacterTest.positive) : failed to catch the end of file" << endl;
	}

	if(!failed) {
		*validationLog << "\tPASS (findCharacterTest.positive) : skip character passed" << endl;
	} else {
		incrementFailure();
		*validationLog << "\tFAIL (findCharacterTest.positive) : skip character failed" << endl;
	}
}

void
ParserTest::findSpaceTest()
{
	incrementTest();
	LOGICAL                  check          = false;
	LOGICAL                  failed         = false;

	resetParser();

	findSpace();
	check = checkProgress(next, ' ', 3, 1, 4);
	if (!check) {
		failed   = true;
		*validationLog << "\t\tFAIL (findSpaceTest.positive) : cursor is in the wrong position" << endl;
	}

	increment(next);
	findSpace();
	check = checkProgress(next, ' ', 6, 1, 7);
	if (!check) {
		failed   = true;
		*validationLog << "\t\tFAIL (findSpaceTest.positive) : cursor is in the wrong position" << endl;
	}

	increment(next);
	findSpace();
	check = checkProgress(next, '\n', 8, 1, 9);
	if (!check) {
		failed   = true;
		*validationLog << "\t\tFAIL (findSpaceTest.positive) : cursor is in the wrong position" << endl;
	}

	increment(next);
	findSpace();
	check = checkProgress(next, '\n', 9, 2, 1);
	if (!check) {
		failed   = true;
		*validationLog << "\t\tFAIL (findSpaceTest.positive) : cursor is in the wrong position" << endl;
	}

	increment(next);
	findSpace();
	check = checkProgress(next, ' ', 10, 3, 1);
	if (!check) {
		failed   = true;
		*validationLog << "\t\tFAIL (findSpaceTest.positive) : cursor is in the wrong position" << endl;
	}
	next.position = 24;
	next.line     = 4;
	next.column   = 5;

	try {
		check = false;
		increment(next);
		findSpace();
		check = checkProgress(next, ' ', 12, 3, 3);
		if (!check) {
			failed   = true;
			*validationLog << "\t\tFAIL (findSpaceTest.positive) : cursor is in the wrong position" << endl;
		}
	} catch (Exception & e) {
		check = true;
	}
	if (!check) {
		failed   = true;
		*validationLog << "\t\tFAIL (findSpaceTest.positive) : failed to catch the end of file" << endl;
	}

	if(!failed) {
		*validationLog << "\tPASS (findSpaceTest.positive) : skip character passed" << endl;
	} else {
		incrementFailure();
		*validationLog << "\tFAIL (findSpaceTest.positive) : skip character failed" << endl;
	}
}

// END OF CHARACTER TESTS
void
ParserTest::getIntegerTest()
{
	incrementTest();
	INTEGER                  value          = 0;
	LOGICAL                  failed         = false;

	resetParser();
	skipCharacter(',');
	value = getInteger();
	if (value != 2) {
		failed   = true;
		*validationLog << "\t\tFAIL (getIntegerTest.positive) : misread 2" << endl;
	}

	skipCharacter(',');
	skipCharacter(',');
	skipCharacter(',');
	value = getInteger();
	if (value != 29) {
		failed   = true;
		*validationLog << "\t\tFAIL (getIntegerTest.positive) : misread 29" << endl;
	}
	skipLine();
	skipLine();
	skipLine();
	skipLine();
	skipLine();
	skipCharacter(',');
	value = getInteger();
	if (value != -34) {
		failed   = true;
		*validationLog << "\t\tFAIL (getIntegerTest.positive) : misread -34" << endl;
	}

	if(!failed) {
		*validationLog << "\tPASS (getIntegerTest.positive) : get integer passed" << endl;
	} else {
		incrementFailure();
		*validationLog << "\tFAIL (getIntegerTest.positive) : get integer failed" << endl;
	}
}

void
ParserTest::getCharacterTest()
{
	incrementTest();
	char                     value          = 0;
	LOGICAL                  failed         = false;

	resetParser();
	skipLine();
	skipCharacter(',');
	value = getCharacter();
	if (value != 'a') {
		failed   = true;
		*validationLog << "\t\tFAIL (getCharacterTest.positive) : misread a" << endl;
	}
	skipCharacter(',');
	value = getCharacter();
	if (value != 'b') {
		failed   = true;
		*validationLog << "\t\tFAIL (getCharacterTest.positive) : misread b" << endl;
	}
	skipCharacter(',');
	value = getCharacter();
	if (value != 'c') {
		failed   = true;
		*validationLog << "\t\tFAIL (getCharacterTest.positive) : misread c" << endl;
	}
	skipCharacter(',');
	value = getCharacter();
	if (value != 'd') {
		failed   = true;
		*validationLog << "\t\tFAIL (getCharacterTest.positive) : misread d" << endl;
	}

	skipLine();
	skipLine();
	skipLine();
	skipLine();
	value = getCharacter();
	if (value != 'a') {
		failed   = true;
		*validationLog << "\t\tFAIL (getCharacterTest.positive) : misread a at start of line" << endl;
	}

	if(!failed) {
		*validationLog << "\tPASS (getCharacterTest.positive) : get character passed" << endl;
	} else {
		incrementFailure();
		*validationLog << "\tFAIL (getCharacterTest.positive) : get character failed" << endl;
	}
}

void
ParserTest::getBoolTest()
{
	incrementTest();
	LOGICAL                  value          = 0;
	LOGICAL                  failed         = false;

	resetParser();
	skipLine();
	value = getBool();
	if (!value) {
		failed   = true;
		*validationLog << "\t\tFAIL (getBoolTest.positive) : misread true" << endl;
	}
	skipCharacter(',');
	skipCharacter(',');
	skipCharacter(',');
	skipCharacter(',');
	value = getBool();
	if (value) {
		failed   = true;
		*validationLog << "\t\tFAIL (getBoolTest.positive) : misread f" << endl;
	}

	if(!failed) {
		*validationLog << "\tPASS (getBoolTest.positive) : get bool passed" << endl;
	} else {
		incrementFailure();
		*validationLog << "\tFAIL (getBoolTest.positive) : get bool failed" << endl;
	}
}

void
ParserTest::getFloatTest()
{
	incrementTest();
	FLOAT                    value          = 0;
	LOGICAL                  failed         = false;

	resetParser();
	value = getFloat();
	if (value != 1.334) {
		failed   = true;
		*validationLog << "\t\tFAIL (getFloatTest.positive) : misread 1.334" << endl;
	}
	skipCharacter(',');
	skipCharacter(',');
	value = getFloat();
	if (value != 5.22) {
		failed   = true;
		*validationLog << "\t\tFAIL (getFloatTest.positive) : misread 5.22" << endl;
	}
	skipLine();
	skipLine();
	value = getFloat();
	if (value != -23.4) {
		failed   = true;
		*validationLog << "\t\tFAIL (getFloatTest.positive) : misread 5.22" << endl;
	}
	skipLine();
	skipLine();
	skipLine();
	skipCharacter(',');
	skipCharacter(',');

	value = getFloat();
	if (value != -1.34) {
		failed   = true;
		*validationLog << "\t\tFAIL (getFloatTest.positive) : misread -1.34 at end of file" << endl;
	}

	if(!failed) {
		*validationLog << "\tPASS (getFloatTest.positive) : get float passed" << endl;
	} else {
		incrementFailure();
		*validationLog << "\tFAIL (getFloatTest.positive) : get float failed" << endl;
	}
}

void
ParserTest::getIntegerDelimiterTest()
{
	incrementTest();
	INTEGER                  value          = 0;
	LOGICAL                  failed         = false;

	resetParser();
	skipCharacter(',');
	value = getDelimitedInteger(',');
	if (value != 2) {
		failed   = true;
		*validationLog << "\t\tFAIL (getIntegerDelimiterTest.positive) : misread 2" << endl;
	}

	skipCharacter(',');
	skipCharacter(',');
	value = getDelimitedInteger(',');
	if (value != 29) {
		failed   = true;
		*validationLog << "\t\tFAIL (getIntegerDelimiterTest.positive) : misread 29" << endl;
	}
	skipLine();
	skipLine();
	skipLine();
	skipLine();
	skipCharacter(',');
	value = getDelimitedInteger(',');
	if (value != -34) {
		failed   = true;
		*validationLog << "\t\tFAIL (getIntegerDelimiterTest.positive) : misread -34" << endl;
	}

	if(!failed) {
		*validationLog << "\tPASS (getIntegerDelimiterTest.positive) : get integer passed" << endl;
	} else {
		incrementFailure();
		*validationLog << "\tFAIL (getIntegerDelimiterTest.positive) : get integer failed" << endl;
	}
}

void
ParserTest::getCharacterDelimiterTest()
{
	incrementTest();
	char                     value          = 0;
	LOGICAL                  failed         = false;

	resetParser();
	skipLine();
	skipCharacter(',');
	value = getDelimitedCharacter(',');
	if (value != 'a') {
		failed   = true;
		*validationLog << "\t\tFAIL (getCharacterDelimiterTest.positive) : misread a" << endl;
	}
	value = getDelimitedCharacter(',');
	if (value != 'b') {
		failed   = true;
		*validationLog << "\t\tFAIL (getCharacterDelimiterTest.positive) : misread b" << endl;
	}
	value = getDelimitedCharacter(',');
	if (value != 'c') {
		failed   = true;
		*validationLog << "\t\tFAIL (getCharacterDelimiterTest.positive) : misread c" << endl;
	}
	value = getDelimitedCharacter(',');
	if (value != 'd') {
		failed   = true;
		*validationLog << "\t\tFAIL (getCharacterDelimiterTest.positive) : misread d" << endl;
	}

	skipLine();
	skipLine();
	skipLine();
	skipLine();
	value = getDelimitedCharacter(',');
	if (value != 'a') {
		failed   = true;
		*validationLog << "\t\tFAIL (getCharacterDelimiterTest.positive) : misread a at start of line" << endl;
	}

	if(!failed) {
		*validationLog << "\tPASS (getCharacterDelimiterTest.positive) : get character passed" << endl;
	} else {
		incrementFailure();
		*validationLog << "\tFAIL (getCharacterDelimiterTest.positive) : get character failed" << endl;
	}
}

void
ParserTest::getBoolDelimiterTest()
{
	incrementTest();
	LOGICAL                  value          = 0;
	LOGICAL                  failed         = false;

	resetParser();
	skipLine();
	value = getDelimitedBool(',');
	if (!value) {
		failed   = true;
		*validationLog << "\t\tFAIL (getBoolDelimiterTest.positive) : misread true" << endl;
	}
	skipCharacter(',');
	skipCharacter(',');
	skipCharacter(',');
	skipCharacter(',');
	value = getDelimitedBool(',');
	if (value) {
		failed   = true;
		*validationLog << "\t\tFAIL (getBoolDelimiterTest.positive) : misread f" << endl;
	}

	if(!failed) {
		*validationLog << "\tPASS (getBoolDelimiterTest.positive) : get bool passed" << endl;
	} else {
		incrementFailure();
		*validationLog << "\tFAIL (getBoolDelimiterTest.positive) : get bool failed" << endl;
	}
}

void
ParserTest::getFloatDelimiterTest()
{
	incrementTest();
	FLOAT                    value          = 0;
	LOGICAL                  failed         = false;

	resetParser();
	value = getFloat();
	if (value != 1.334) {
		failed   = true;
		*validationLog << "\t\tFAIL (getFloatDelimiterTest.positive) : misread 1.334" << endl;
	}

	skipCharacter(',');
	skipCharacter(',');
	value = getFloat();
	if (value != 5.22) {
		failed   = true;
		*validationLog << "\t\tFAIL (getFloatDelimiterTest.positive) : misread 5.22" << endl;
	}

	skipLine();
	skipLine();
    value = getFloat();
    if (value != -23.4) {
        failed   = true;
        *validationLog << "\t\tFAIL (getFloatDelimiterTest.positive) : misread 5.22" << endl;
    }

	skipLine();
	skipLine();
	skipLine();
    skipCharacter(',');
	skipCharacter(',');
	value = getFloat();
	if (value != -1.34) {
		failed   = true;
		*validationLog << "\t\tFAIL (getFloatDelimiterTest.positive) : misread -1.34 at end of file" << endl;
	}

	if(!failed) {
		*validationLog << "\tPASS (getFloatDelimiterTest.positive) : get float passed" << endl;
	} else {
		incrementFailure();
		*validationLog << "\tFAIL (getFloatDelimiterTest.positive) : get float failed" << endl;
	}
}

void
ParserTest::getStringTest()
{
	incrementTest();
	string                   value;
	LOGICAL                  failed         = false;

	resetParser();
	value = getString();
	if (value != "1.334") {
		failed   = true;
		*validationLog << "\t\tFAIL (getStringTest.positive) : misread \"1.334\"" << endl;
	}
	skipCharacter(',');
	skipCharacter(',');
	skipCharacter(',');

	skipLine();
	skipCharacter(',');
	skipCharacter(',');
	skipCharacter(',');
	skipCharacter(',');
	skipCharacter(',');
	current = next;
	value = getString();
	if (value != "false") {
		failed   = true;
		*validationLog << "\t\tFAIL (getStringTest.positive) : misread \"false\"" << endl;
	}

	skipLine();
	skipLine();
	skipCharacter(',');
	skipCharacter(',');
	current = next;
	findCharacter('\n');
	value = getString();
	if (value != "quoted string") {
		failed   = true;
		*validationLog << "\t\tFAIL (getStringTest.positive) : misread \"quoted string\"" << endl;
	}

	if(!failed) {
		*validationLog << "\tPASS (getStringTest.positive) : get string passed" << endl;
	} else {
		incrementFailure();
		*validationLog << "\tFAIL (getStringTest.positive) : get string failed" << endl;
	}
}

void
ParserTest::getStringDelimitedTest()
{
	incrementTest();
	string                   value;
	LOGICAL                  failed         = false;

	resetParser();
	value = getDelimitedString(',');
	if (value != "1.334 ") {
		failed   = true;
		*validationLog << "\t\tFAIL (getStringDelimitedTest.positive) : misread \"1.334\"" << endl;
	}

	skipCharacter(',');
	current = next;
	value = getDelimitedString(',');
	if (value != "5.22") {
		failed   = true;
		*validationLog << "\t\tFAIL (getStringDelimitedTest.positive) : misread \"5.22\"" << endl;
	}

	skipLine();
	skipCharacter(',');
	skipCharacter(',');
	skipCharacter(',');
	skipCharacter(',');
	skipCharacter(',');
	current = next;
	value = getDelimitedString('\n');
	if (value != "false") {
		failed   = true;
		*validationLog << "\t\tFAIL (getStringDelimitedTest.positive) : misread \"false\"" << endl;
	}

	skipLine();
	skipCharacter(',');
	skipCharacter(',');
	current = next;
	findCharacter('\n');
	value = getString();
	if (value != "quoted string") {
		failed   = true;
		*validationLog << "\t\tFAIL (getStringDelimitedTest.positive) : misread \"quoted string\"" << endl;
	}

	if(!failed) {
		*validationLog << "\tPASS (getStringDelimitedTest.positive) : get string passed" << endl;
	} else {
		incrementFailure();
		*validationLog << "\tFAIL (getStringDelimitedTest.positive) : get string failed" << endl;
	}
}

void
ParserTest::getDateTest()
{
	incrementTest();
	Date                     value;
	LOGICAL                  failed         = false;

	resetParser();
	skipLine();
	skipLine();
	skipLine();
	value = getDate(MMDDYYYY_SLASH);
	if (value.year != 2001 || value.month != 12 || value.day != 31) {
		failed   = true;
		*validationLog << "\t\tFAIL (getDateTest.positive) : misread data 12/31/2001" << endl;
	}
	skipCharacter(',');

	findNextCharacter();
	value = getDate(MMDDYYYY_SLASH);
	if (value.year != 2002|| value.month != 1 || value.day != 1) {
		failed   = true;
		*validationLog << "\t\tFAIL (getDateTest.positive) : misread data 12/31/2001" << endl;
	}

	if(!failed) {
		*validationLog << "\tPASS (getDateTest.positive) : get string passed" << endl;
	} else {
		incrementFailure();
		*validationLog << "\tFAIL (getDateTest.positive) : get string failed" << endl;
	}
}

void
ParserTest::getTimeOfDayTest()
{
	incrementTest();
	TimeOfDay                value;
	LOGICAL                  failed         = false;

	value.hours = 0;
	value.minutes = 0;
	value.seconds = 0;
	resetParser();
	skipLine();
	skipLine();
	skipLine();
	skipLine();
	findNextCharacter();
	value = getTimeOfDay(HHMMSS_COLON);
	if (value.hours != 2 || value.minutes != 12 || value.seconds != 59) {
		failed   = true;
		*validationLog << "\t\tFAIL (getTimeOfDayTest.positive) : misread data 12/31/2001" << endl;
	}
	skipCharacter(',');

	findNextCharacter();
	value = getTimeOfDay(HHMM_COLON);
	if (value.hours != 3 || value.minutes != 23 || value.seconds != 0) {
		failed   = true;
		*validationLog << "\t\tFAIL (getTimeOfDayTest.positive) : misread data 12/31/2001" << endl;
	}

	if(!failed) {
		*validationLog << "\tPASS (getTimeOfDayTest.positive) : get string passed" << endl;
	} else {
		incrementFailure();
		*validationLog << "\tFAIL (getTimeOfDayTest.positive) : get string failed" << endl;
	}
}

void
ParserTest::getTimeTest()
{
}

void
ParserTest::setSecondsPastZeroTest()
{
	incrementTest();
	Time                     time1;
	Time                     time2;

	resetParser();
	time1.date.year          = 0;
	time1.date.month         = 0;
	time1.minutesPastZero    = 0L;

	time1.tod.hours          = 0;
	time1.tod.minutes        = 0;
	time1.tod.seconds        = 0;

	time2.date.year          = 0;
	time2.date.month         = 0;
	time2.date.day           = 0;
	time2.minutesPastZero    = 0L;

	time2.tod.hours          = 23;
	time2.tod.minutes        = 59;
	time2.tod.seconds        = 59;

	skipLine();
	skipLine();
	skipLine();
	time1.date = getDate(MMDDYYYY_SLASH);
	skipCharacter(',');
	findNextCharacter();
	time2.date = getDate(MMDDYYYY_SLASH);

	setMinutesPastZero(time1);
	setMinutesPastZero(time2);

	if(time2.minutesPastZero - time1.minutesPastZero == 2879) {
		*validationLog << "\tPASS (setSecondsPastZeroTest.positive) : get string passed" << endl;
	} else {
		incrementFailure();
		*validationLog << "\tFAIL (setSecondsPastZeroTest.positive) : get string failed" << endl;
	}
}

// tag testing
void
ParserTest::matchTagTest()
{
	incrementTest();
	bool                     value;
	LOGICAL                  failed         = false;

	resetParser();
	skipCharacter('<');
	value = matchTag("tag1");
	if (!value) {
		failed   = true;
		*validationLog << "\t\tFAIL (matchTagTest.positive) : misread data 12/31/2001" << endl;
	}

	skipCharacter('<');
	value = matchTag("tag2");
	if (!value) {
		failed   = true;
		*validationLog << "\t\tFAIL (matchTagTest.positive) : misread data 12/31/2001" << endl;
	}

	if(!failed) {
		*validationLog << "\tPASS (matchTagTest.positive) : get string passed" << endl;
	} else {
		incrementFailure();
		*validationLog << "\tFAIL (matchTagTest.positive) : get string failed" << endl;
	}
}

void
ParserTest::findTagTest()
{
	incrementTest();
	bool                     value;
	LOGICAL                  failed         = false;

	resetParser();

	value = findTag("tag1", '<', '>');
	if (!value) {
		failed   = true;
		*validationLog << "\t\tFAIL (matchTagTest.positive) : misread data 12/31/2001" << endl;
	}

	value = findTag("tag2", '<', '>');
	if (!value) {
		failed   = true;
		*validationLog << "\t\tFAIL (matchTagTest.positive) : misread data 12/31/2001" << endl;
	}

	if(!failed) {
		*validationLog << "\tPASS (matchTagTest.positive) : get string passed" << endl;
	} else {
		incrementFailure();
		*validationLog << "\tFAIL (matchTagTest.positive) : get string failed" << endl;
	}
}

void
ParserTest::getTagValueTest()
{
	incrementTest();
	string                   value;
	LOGICAL                  failed         = false;

	resetParser();

	value = getTagValue("tag1", '<', '>',':');
	if (value != "23") {
		failed   = true;
		*validationLog << "\t\tFAIL (matchTagTest.positive) : misread data 12/31/2001" << endl;
	}

	value = getTagValue("tag2", '<', '>',':');
	if (value != "-45.6") {
		failed   = true;
		*validationLog << "\t\tFAIL (matchTagTest.positive) : misread data 12/31/2001" << endl;
	}

	if(!failed) {
		*validationLog << "\tPASS (matchTagTest.positive) : get string passed" << endl;
	} else {
		incrementFailure();
		*validationLog << "\tFAIL (matchTagTest.positive) : get string failed" << endl;
	}
}

void
ParserTest::modelParserTests()
{

	GeneratorParserTest    * generatorParser = new GeneratorParserTest();
	INTEGER                  nGenerators    = 3; // NOTE nGenerators must match the number of generators in "testing/data/generators.csv"
	*validationLog << "Generator Parser Tests" << endl;
	generatorParser->constructGeneratorParserTest("testing/data/validSrc/base/generators.csv", nGenerators);

	delete generatorParser;

	BAParserTest           * baParser = new BAParserTest();
	INTEGER                  nBAs      = 3; // NOTE nGenerators must match the number of generators in "testing/data/generators.csv"
	*validationLog << "Balancing Parser Tests" << endl;
	baParser->constructBAParserTest("testing/data/validSrc/base/ba.csv", nBAs);

	delete baParser;

	ZoneParserTest         * zoneParser = new ZoneParserTest();
	INTEGER                  nZones     = 3; // NOTE nGenerators must match the number of generators in "testing/data/generators.csv"
	*validationLog << "Zone Parser Tests" << endl;
	zoneParser->constructZoneParserTest("testing/data/validSrc/base/zone.csv", nZones);

	delete zoneParser;

	SimulationParserTest   * simParser = new SimulationParserTest();
	INTEGER                  nSims     = 4; // NOTE nGenerators must match the number of generators in "testing/data/generators.csv"
	*validationLog << "Simulation Parser Tests" << endl;
	simParser->constructSimulationParserTest("testing/data/validSrc/base/simulation.csv", nSims);

	delete simParser;

	LoadParserTest         * loadParser     = new LoadParserTest();
	INTEGER                  nLoads         = 3; // NOTE nGenerators must match the number of generators in "testing/data/generators.csv"
	*validationLog << "Load Parser Tests" << endl;
	loadParser->constructLoadParserTest("testing/data/validSrc/base/load.csv", nLoads);

	delete loadParser;

	WindParserTest         * windParser     = new WindParserTest();
	INTEGER                  nWinds         = 3; // NOTE nGenerators must match the number of generators in "testing/data/generators.csv"
	*validationLog << "Wind Parser Tests" << endl;
	windParser->constructWindParserTest("testing/data/validSrc/base/wind.csv", nWinds);

	delete windParser;

	SolarParserTest         * solarParser    = new SolarParserTest();
	INTEGER                  nSolars         = 3; // NOTE nGenerators must match the number of generators in "testing/data/generators.csv"
	*validationLog << "Solar Parser Tests" << endl;
	solarParser->constructSolarParserTest("testing/data/validSrc/base/solar.csv", nSolars);

	delete solarParser;

	RIMParserTest          * rimParser       = new RIMParserTest();
	rimParser->constructRIMParserTest("testing/data/validSrc/configureModel/sources.txt");
	*validationLog << "RIM Parser Tests" << endl;

	delete rimParser;
}

} /* namespace testing */

int main(int argc, char * argv[])
{
	testing::ParserTest::setupBasic("testing/data/parserTests/simple.inp");
	testing::ParserTest    * test = new testing::ParserTest("testing/data/parserTests/simple.inp");
	test->basicFunctions();
	delete test;

	testing::ParserTest::setupCharacter("testing/data/parserTests/character.inp");
	test                = new testing::ParserTest("testing/data/parserTests/character.inp");
	test->characterSearchFunctions();
	delete test;

	test                = new testing::ParserTest("testing/standards/parserTests/delimitedData.inp");
	test->valueSearchFunctions();
	delete test;

	test                = new testing::ParserTest("testing/standards/parserTests/tag.inp");
	test->tagSearchFunctions();
	delete test;

	test                = new testing::ParserTest("testing/data/parserTests/simple.inp");
	test->modelParserTests();
	return 0;
}

