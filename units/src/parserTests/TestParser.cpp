/*
 * TestParser.cpp
 *
 *  Created on: Jan 19, 2012
 *      Author: kglass
 */

#include "parserTests/TestParser.hpp"
#include "Parser.hpp"
#include <string>

namespace testing {
CPPUNIT_TEST_SUITE_REGISTRATION (TestParser);

void
TestParser::setUp(void)
{
	dirName             = "units/data/parserTests/";
	writeBasic0File();
	writeBasic1File();
	delimitedFileName   = dirName + "delimitedData.inp";
	tagFileName         = dirName + "tag.inp";
	simpleParser        = new interpreter::Parser(simpleFileName.c_str());
	characterParser     = new interpreter::Parser(characterFileName.c_str());
	delimitedParser     = new interpreter::Parser(delimitedFileName.c_str());
	tagParser           = new interpreter::Parser(tagFileName.c_str());
	delimitedFileData   = delimitedParser->file->fileBuffer;
}

void
TestParser::tearDown(void)
{
	delete(simpleParser);
	delete(characterParser);
	delete(delimitedParser);
	delete(tagParser);
}

void
TestParser::constructTests(void)
{
	CPPUNIT_ASSERT (simpleParser != NULL);

	CPPUNIT_ASSERT_MESSAGE  ("INCORRECT CURRENT POSITION", simpleParser->current.position == 0);
	CPPUNIT_ASSERT_MESSAGE  ("INCORRECT CURRENT LINE", simpleParser->current.line == 1);
	CPPUNIT_ASSERT_MESSAGE  ("INCORRECT CURRENT COLUMN", simpleParser->current.column == 1);

	simpleParser->countLines();

	CPPUNIT_ASSERT_MESSAGE  ("countLines failed: miscounted lines", simpleParser->nLines == 3);
	CPPUNIT_ASSERT_MESSAGE  ("countLines failed: not at EOF", simpleParser->endOfFile == 11);
	// count lines moved the cursor to the EOF
	simpleParser->resetParser();
	CPPUNIT_ASSERT_MESSAGE  ("parserReset failed: current position wrong", simpleParser->current.position == 0);
	CPPUNIT_ASSERT_MESSAGE  ("parserReset failed: current line wrong", simpleParser->current.line == 1);
	CPPUNIT_ASSERT_MESSAGE  ("parserReset failed: current column wrong", simpleParser->current.column == 1);

	CPPUNIT_ASSERT_MESSAGE  ("parserReset failed: next position wrong", simpleParser->next.position == 0);
	CPPUNIT_ASSERT_MESSAGE  ("parserReset failed: next line wrong", simpleParser->next.line == 1);
	CPPUNIT_ASSERT_MESSAGE  ("parserReset failed: next column wrong", simpleParser->next.column == 1);
}

void
TestParser::basicMoveTests(void)
{
	simpleParser->getPopulation();
	// the buffer size is one larger than the file size os the buffer sees an extra line with one column
	CPPUNIT_ASSERT_MESSAGE  ("INCORRECT CURRENT POSITION", simpleParser->current.position == 11);
	CPPUNIT_ASSERT_MESSAGE  ("INCORRECT CURRENT LINE", simpleParser->current.line == 4);
	CPPUNIT_ASSERT_MESSAGE  ("INCORRECT CURRENT COLUMN", simpleParser->current.column == 1);
	// increment one space, this should throw an exception
	CPPUNIT_ASSERT_THROW (simpleParser->increment(simpleParser->next), Exception);
	simpleParser->resetParser();
	CPPUNIT_ASSERT_THROW (simpleParser->decrement(simpleParser->next), Exception);
	simpleParser->resetParser();
	simpleParser->current.position = -1;
	CPPUNIT_ASSERT_MESSAGE  ("INCORRECT CURRENT COLUMN", simpleParser->validBounds(simpleParser->current.position) == false);
	simpleParser->current.position = simpleParser->endOfFile + 4;
	CPPUNIT_ASSERT_MESSAGE  ("INCORRECT CURRENT COLUMN", simpleParser->validBounds(simpleParser->current.position) == false);
	simpleParser->current.position = simpleParser->endOfFile/2;
	CPPUNIT_ASSERT_MESSAGE  ("INCORRECT CURRENT COLUMN", simpleParser->validBounds(simpleParser->current.position) == true);
}

void
TestParser::skipMoveTests(void)
{
	characterParser->resetParser();
	increment(characterParser, 4);
	characterParser->skipLine();
	CPPUNIT_ASSERT_MESSAGE  ("skipLine failed", characterParser->next.position == 9);
	characterParser->skipSpace();
	CPPUNIT_ASSERT_MESSAGE  ("skipSpace failed", characterParser->next.position == 12);

}

void
TestParser::characterMoveTests(void)
{
	characterParser->resetParser();
	CPPUNIT_ASSERT_MESSAGE  ("isEmpty failed", !characterParser->isEmptyLine());
	characterParser->skipLine();
	CPPUNIT_ASSERT_MESSAGE  ("isEmpty failed", characterParser->isEmptyLine());
	characterParser->next.position = 3;
	char  c = characterParser->findNextCharacter();
	CPPUNIT_ASSERT_MESSAGE  ("findNextCharacter failed", c == '4');
	characterParser->resetParser();
	characterParser->findCharacter('6');
	CPPUNIT_ASSERT_MESSAGE  ("findCharacter failed", characterParser->next.position == 7);
	characterParser->increment(characterParser->next);
	characterParser->findCharacter('6');
	CPPUNIT_ASSERT_MESSAGE  ("findCharacter failed", characterParser->next.position == 18);
	CPPUNIT_ASSERT_THROW (characterParser->findCharacter('7'), Exception);
	characterParser->resetParser();
	skipLines(characterParser, 2);
	characterParser->findCharacterInLine('1');
	CPPUNIT_ASSERT_MESSAGE  ("findCharacterInLine failed", characterParser->next.position == 12);
	characterParser->resetParser();
	increment(characterParser, 2);
	bool value = characterParser->findCharacterInLine('1');
	CPPUNIT_ASSERT_MESSAGE  ("findCharacterInLine failed", !value);
	characterParser->resetParser();
	characterParser->findSpace();
	CPPUNIT_ASSERT_MESSAGE  ("findSpace failed", characterParser->next.position == 3);
}

void
TestParser::valueTests(void)
{
	delimitedParser->resetParser();
	CPPUNIT_ASSERT_MESSAGE  ("isComment failed", !delimitedParser->isComment());
	skipLines(delimitedParser, 6);
	CPPUNIT_ASSERT_MESSAGE  ("isComment failed", delimitedParser->isComment());
	delimitedParser->resetParser();
	// get integer
	increment(delimitedParser, 7);
	CPPUNIT_ASSERT_MESSAGE  ("getInteger failed", delimitedParser->getInteger() == 2);
	string                   buffer         = delimitedParser->file->fileBuffer;
	CPPUNIT_ASSERT_MESSAGE  ("getInteger failed", delimitedParser->next.position == 8);
	CPPUNIT_ASSERT_MESSAGE  ("getInteger altered input buffer", buffer == delimitedFileData);

	increment(delimitedParser, 6);
	CPPUNIT_ASSERT_THROW (delimitedParser->getInteger(), Exception);

	// get character
	increment(delimitedParser, 86);
	CPPUNIT_ASSERT_MESSAGE  ("MOVE TO CHARACTER", delimitedParser->next.position == 100);
	CPPUNIT_ASSERT_MESSAGE  ("getCharacter failed", delimitedParser->getCharacter() == 'a');
	buffer         = delimitedParser->file->fileBuffer;
	CPPUNIT_ASSERT_MESSAGE  ("getCharacter altered input buffer", buffer == delimitedFileData);

	// get float
	increment(delimitedParser, 6);
	CPPUNIT_ASSERT_MESSAGE  ("MOVE TO CHARACTER", delimitedParser->next.position == 106);
	buffer         = delimitedParser->file->fileBuffer;
	CPPUNIT_ASSERT_MESSAGE  ("getFloat failed", delimitedParser->getFloat() == -1.34);
	CPPUNIT_ASSERT_MESSAGE  ("getFloat altered input buffer", buffer == delimitedFileData);

	// get bool
	delimitedParser->resetParser();
	increment(delimitedParser, 20);
	CPPUNIT_ASSERT_MESSAGE  ("MOVE TO CHARACTER", delimitedParser->next.position == 20);
	buffer         = delimitedParser->file->fileBuffer;
	CPPUNIT_ASSERT_MESSAGE  ("getBool failed", delimitedParser->getBool());
	CPPUNIT_ASSERT_MESSAGE  ("getBool altered input buffer", buffer == delimitedFileData);
	increment(delimitedParser, 15);
	CPPUNIT_ASSERT_MESSAGE  ("MOVE TO CHARACTER", delimitedParser->next.position == 35);
	buffer         = delimitedParser->file->fileBuffer;
	CPPUNIT_ASSERT_MESSAGE  ("getBool failed", !delimitedParser->getBool());
	CPPUNIT_ASSERT_MESSAGE  ("getBool altered input buffer", buffer == delimitedFileData);

	// get string
	delimitedParser->resetParser();
	increment(delimitedParser, 20);
	CPPUNIT_ASSERT_MESSAGE  ("MOVE TO CHARACTER", delimitedParser->next.position == 20);
	buffer         = delimitedParser->file->fileBuffer;
	string          value = "true,";
	CPPUNIT_ASSERT_MESSAGE  ("getString failed", delimitedParser->getString() == value);
	CPPUNIT_ASSERT_MESSAGE  ("getBool altered input buffer", buffer == delimitedFileData);

	increment(delimitedParser, 45);
	CPPUNIT_ASSERT_MESSAGE  ("MOVE TO CHARACTER", delimitedParser->next.position == 70);
	buffer         = delimitedParser->file->fileBuffer;
	value = "quoted string";
	CPPUNIT_ASSERT_MESSAGE  ("getString failed", delimitedParser->getString() == value);
	CPPUNIT_ASSERT_MESSAGE  ("getBool altered input buffer", buffer == delimitedFileData);
}

void
TestParser::delimitedValueTests(void)
{
	delimitedParser->resetParser();
	increment(delimitedParser, 7);
	CPPUNIT_ASSERT_MESSAGE  ("getDelimitedInteger failed", delimitedParser->getDelimitedInteger(',') == 2);

	delimitedParser->resetParser();
	increment(delimitedParser, 100);
	CPPUNIT_ASSERT_MESSAGE  ("getDelimitedCharacter failed", delimitedParser->getDelimitedCharacter(',') == 'a');

	delimitedParser->resetParser();
	increment(delimitedParser, 106);
	CPPUNIT_ASSERT_MESSAGE  ("getDelimitedFloat failed", delimitedParser->getDelimitedFloat(',') == -1.34);

	delimitedParser->resetParser();
	increment(delimitedParser, 20);
	CPPUNIT_ASSERT_MESSAGE  ("getDelimitedBool failed", delimitedParser->getDelimitedBool(','));

	string value = "this is  a comment";
	delimitedParser->resetParser();
	increment(delimitedParser, 114);
	CPPUNIT_ASSERT_MESSAGE  ("getDelimitedString failed", delimitedParser->getDelimitedString(',') == value);
}

void
TestParser::dataTests(void)
{
	delimitedParser->resetParser();
	increment(delimitedParser, 48);
	Time     time1;
	time1.date = delimitedParser->getDate(MMDDYYYY_SLASH);
	CPPUNIT_ASSERT_MESSAGE  ("getDate failed: wrong month", time1.date.month == 12);
	CPPUNIT_ASSERT_MESSAGE  ("getDate failed: wrong day", time1.date.day == 31);
	CPPUNIT_ASSERT_MESSAGE  ("getDate failed: wrong year", time1.date.year == 2001);

	delimitedParser->resetParser();
	increment(delimitedParser, 60);
	Time     time2;
	time2.date = delimitedParser->getDate(MMDDYYYY_SLASH);
	CPPUNIT_ASSERT_MESSAGE  ("getDate failed: wrong month", time2.date.month == 1);
	CPPUNIT_ASSERT_MESSAGE  ("getDate failed: wrong day", time2.date.day == 1);
	CPPUNIT_ASSERT_MESSAGE  ("getDate failed: wrong year", time2.date.year == 2002);

	delimitedParser->resetParser();
	increment(delimitedParser, 86);
	time1.tod = delimitedParser->getTimeOfDay(HHMMSS_COLON);
	CPPUNIT_ASSERT_MESSAGE  ("getDate failed: wrong month", time1.tod.hours == 2);
	CPPUNIT_ASSERT_MESSAGE  ("getDate failed: wrong day", time1.tod.minutes == 12);
	CPPUNIT_ASSERT_MESSAGE  ("getDate failed: wrong year", time1.tod.seconds == 59);

	delimitedParser->resetParser();
	increment(delimitedParser, 95);
	time2.tod = delimitedParser->getTimeOfDay(HHMM_COLON);
	CPPUNIT_ASSERT_MESSAGE  ("getDate failed: wrong month", time2.tod.hours == 3);
	CPPUNIT_ASSERT_MESSAGE  ("getDate failed: wrong day", time2.tod.minutes == 23);
}

void
TestParser::tagTests(void)
{
	tagParser->resetParser();
	string tag = tagParser->getTagValue("tag1", '<', '>', ':');
	string value = "23";
	CPPUNIT_ASSERT_MESSAGE  ("getTagValue failed: wrong value", tag == value);

	tagParser->resetParser();
	tag = tagParser->getTagValue("tag2", '<', '>', ':');
	value = "-45.6";
	CPPUNIT_ASSERT_MESSAGE  ("getTagValue failed: wrong value", tag == value);


}

void
TestParser::writeBasic0File(void)
{
	simpleFileData      = "12345\n1\n12\n";
	simpleFileName      = dirName + "simple.inp";
	FILE                * fh   = fopen(simpleFileName.c_str(), "w");

	fprintf(fh, "%s", simpleFileData.c_str());
	fclose(fh);
}

void
TestParser::writeBasic1File(void)
{
	characterFileData   = "123 45 6\n\n  1   5 6\n    \n";
	characterFileName   = dirName + "character.inp";
	FILE                * fh   = fopen(characterFileName.c_str(), "w");

	fprintf(fh, "%s", characterFileData.c_str());
	fclose(fh);
}

void
TestParser::increment(interpreter::Parser * parser, int steps)
{
	for (int i = 0 ; i < steps ; i++) {
		parser->increment(parser->next);
	}
}

void
TestParser::skipLines(interpreter::Parser * parser, int steps)
{
	for (int i = 0 ; i < steps ; i++) {
		parser->skipLine();
	}
}

} /* namespace testing */
