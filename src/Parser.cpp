#include "Parser.hpp"
#include "FileControl.hpp"
#include "exceptions/Exception.hpp"
#include <algorithm>
#include <stdexcept>

#define COMMENT_CHAR   '#'

using std::string;
using std::endl;
using std::cerr;
using std::clog;

namespace interpreter {

const INTEGER elapsedDays[] = {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365};

/* ****************************************************************************
 * Parser(const char * fileName)
 *************************************************************************** */
/**
 * \details Opens the file specified by the input parameter and initializes the
 * parser
 *
 * @param fileName name of the file to parse
 */
Parser::Parser(const string &fileName) :
	file(NULL)
{
	file                = utility::FileControl::loadFile(fileName.c_str());
	nLines              =  0;

	current.position    =  0;
	current.line        =  1;
	current.column      =  1;
}

/* ****************************************************************************
 * ~Parser()
 *************************************************************************** */
/**
 * The Parser <i>destructor</i> closes the open file and deletes the buffer
 */
Parser::~Parser()
{
	if (file != NULL) {
		utility::FileControl::closeFile(file);
		file = NULL;
	}
}

/* ************************************************************************
 * File methods
************************************************************************ */
/**
 *
 */
void
Parser::reduceFile()
{
	INTEGER                  size           = file->fileBuffSize;
	INTEGER                  iter           = 0;
	INTEGER                  pos            = 0;
	char                   * reduced        = new char[size + 1];

	// copy file->fileBuffer to reduced
	for (pos = 0; pos < size; ++pos) {
		// if the EOL is reached, replace the existing EOL character with \n
		// and add the size of the EOL character minus 1 to the position,
		// this deals with EOLs with more than one character and ensures
		// a uniform EOL character
		if (!strncmp(&file->fileBuffer[pos], EOL, EOL_CHARACTERS)) {
			reduced[iter]    = '\n';
			pos        += EOL_CHARACTERS - 1; // the for loop will ensure the correct increment of pos
			++iter;
		} else if (file->fileBuffer[pos] == COMMENT_CHAR){
			// if the character is a comment, move the pos index to the EOL,
			// the EOL test should deal with this
			while(strncmp(&file->fileBuffer[pos], EOL, EOL_CHARACTERS)) {
				++pos;
			}
		} else {
			// copy the string to the reduced buffer
			reduced[iter]      = file->fileBuffer[pos];
			++iter;
		}
	}

	delete [] file->fileBuffer;
	file->fileBuffSize  = iter;
	file->fileBuffer    = reduced;
	file->fileBuffer[iter] = 0;
}

/* ************************************************************************
 * File methods
************************************************************************ */
// convert \r to \n, trim trailing space, end with \n\0
// assumption: file->fileBuffer can fit an extra character
// assumption: file->fileBuffer is initialized to 0
void
Parser::reduceCSV()
{
	char *write = &file->fileBuffer[0];
	char *read  = &file->fileBuffer[0];
	while (*read != '\0') {
		switch(*read) {
			case '\n':
			case '\r':
				*write = '\n';
				read++;
				write++;
				while (*read != '\0' && isspace(*read)) read++;
				break;
			default:
				*write = *read;
				read++;
				write++;
				break;
		}
	}
	if (*(write-1) != '\n') {
		*write = '\n';
		write++;
	}
	*write = '\0';
	file->fileBuffSize = write - &file->fileBuffer[0];
}

/**
 * The Parser <i>resetParser</i> method initializes the current and next
 * cursors to the start of the file
 */
void
Parser::countLines()
{
	resetParser();
	INTEGER                  size           = file->fileBuffSize;

	for (INTEGER pos = 0; pos < size+1; ++pos) {
		if (file->fileBuffer[pos] == '\n') {
			pos         += EOL_CHARACTERS - 1;
			++nLines;
		}
	}
 }

/* ****************************************************************************
 * ****************************************************************************
 *                      Cursor methods
 * ****************************************************************************
 * ************************************************************************* */

/* ****************************************************************************
 * void Parser::resetParser()
 *************************************************************************** */
/**
 * The Parser <i>resetParser</i> method initializes the current and next
 * cursors to the start of the file
 */
void
Parser::resetParser()
{
	current.position    = 0;
	current.line        = 1;
	current.column      = 1;
}

/* ****************************************************************************
 * void Parser::increment(Cursor & cursor)
 * ************************************************************************* */

/**
 * The Parser <i>increment(Cursor & cursor)</i> performs the same action as the
 * <i>increment()</i> except it updates an arbitrary cursor. These allows
 * parsing methods to look ahead.
 *
 * \param cursor : Cursor & -- any cursor used by the parser
 * \return NONE
 * \throws ParserException EOF_REACHED exception it the cursor reaches
 * the end of file. NOTE: the size of the buffer is the actual end of file + 1
 */
void
Parser::increment(Cursor & cursor)
{
	++cursor.position;
	if (!isInBounds(cursor.position)) {
		//cerr << "ERROR (";
		//cerr <<  utility::FileControl::getFileName(__FILE__);
		//cerr << ":" << __LINE__ << "): cursor is outside of the valid bounds." << endl;
		throw Exception(__FILE__, __LINE__, Exception::INVALID_ARRAY);
	} else if (!strncmp(&file->fileBuffer[cursor.position], EOL, EOL_CHARACTERS)) {
		++cursor.line;
		cursor.column   = 1;
	} else {
		++cursor.column;
	}
}

/* ****************************************************************************
 * LOGICAL Parser::validBounds(INTEGER pos)
 *************************************************************************** */
/**
 * The Parser <i>validBounds(Cursor & cursor)</i> method ensures that a given
 * position is within the start of the file and the end of the file.
 *
 * \param pos : INTEGER any integer
 * \return valid : LOGICAL true means pos is valid, false means it is not
 */
LOGICAL
Parser::isInBounds(INTEGER pos)
{
	bool                     valid          = true;
	if (pos < 0 || pos > file->fileBuffSize) {
		valid           = false;
	}
	return valid;
}
/* ****************************************************************************
 * LOGICAL Parser::isLineEmpty()
 *************************************************************************** */
/**
 * The Parser <i>isLineEmpty()</i> method checks if the line is empty after
 * the cursor.  A line is empty if the cursor is positioned at a sequence
 * of whitespace containing a newline.
 *
 * \return valid : LOGICAL true means pos is valid, false means it is not
 */
LOGICAL
Parser::isLineEmpty()
{
	Cursor                   iter           = current;
	while(strchr(" \t",file->fileBuffer[iter.position])) {
		increment(iter);
	}
	return strchr(EOL,file->fileBuffer[iter.position]);
}

/* ****************************************************************************
 *
 *************************************************************************** */
Parser::Cursor
Parser::skipLine(Cursor & cursor)
{
	Cursor                   iter           = cursor;

	try {
		while(strncmp(&file->fileBuffer[iter.position], EOL, EOL_CHARACTERS)) {
			increment(iter);
		}
		increment(iter);
	} catch (Exception & e) {
		cerr << "ERROR (";
		cerr <<  utility::FileControl::getFileName(__FILE__);
		cerr << ":" << __LINE__ << "): ." << endl;
		throw Exception(__FILE__, __LINE__, Exception::INVALID_ARRAY);
	}

	return iter;
}

/* ****************************************************************************
 * White space until character or EOF reached
 *************************************************************************** */
Parser::Cursor
Parser::skipSpaces(Cursor & cursor)
{
	Cursor                   iter           = cursor;

	try {
		while(isspace(file->fileBuffer[iter.position])) {
			increment(iter);
		}
	} catch (Exception & e) {
		cerr << "ERROR (";
		cerr <<  utility::FileControl::getFileName(__FILE__);
		cerr << ":" << __LINE__ << "): ." << endl;
		throw Exception(__FILE__, __LINE__, Exception::INVALID_ARRAY);
	}

	return iter;
}

/* ****************************************************************************
 *
 *************************************************************************** */
Parser::Cursor
Parser::skipCharacters(Cursor & cursor)
{
	Cursor                   iter           = cursor;

	try {
		while(!isspace(file->fileBuffer[iter.position])) {
			increment(iter);
		}
	} catch (Exception & e) {
		cerr << "ERROR (";
		cerr <<  utility::FileControl::getFileName(__FILE__);
		cerr << ":" << __LINE__ << "): ." << endl;
		throw Exception(__FILE__, __LINE__, Exception::INVALID_ARRAY);
	}
	return iter;
}

/* ****************************************************************************
 * Set the cursor to the next non-comment occurrence of character c then move
 * it one space further
 *************************************************************************** */
Parser::Cursor
Parser::skipCharacter(Cursor & cursor, char c)
{
	Cursor                   iter           = cursor;

	try {
		while(file->fileBuffer[iter.position] != c) {
			increment(iter);
		}
		increment(iter);
	} catch (Exception & e) {
		cerr << "ERROR (";
		cerr <<  utility::FileControl::getFileName(__FILE__);
		cerr << ":" << __LINE__ << "): ." << endl;
		throw Exception(__FILE__, __LINE__, Exception::INVALID_ARRAY);
	}
	return iter;
}

/* ****************************************************************************
 *
 *************************************************************************** */
Parser::Cursor
Parser::findNextCharacter(Cursor & cursor, char c)
{
	Cursor                   iter           = cursor;

	try {
		while(file->fileBuffer[iter.position] != c) {
			increment(iter);
		}
	} catch (Exception & e) {
		//cerr << "ERROR (";
		//cerr <<  utility::FileControl::getFileName(__FILE__);
		//cerr << ":" << __LINE__ << "): cannot find character " << c << "." << endl;
		throw Exception(__FILE__, __LINE__, Exception::EOF_REACHED);
	}


	return iter;
}

/* ****************************************************************************
 *
 *************************************************************************** */
Parser::Cursor
Parser::findCharacterInLine(Cursor & cursor, char c)
{
	Cursor                   iter           = cursor;

	try {
		while(file->fileBuffer[iter.position] &&
				strncmp(&file->fileBuffer[iter.position], EOL, EOL_CHARACTERS)) {
			increment(iter);
		}
		if (!strncmp(&file->fileBuffer[iter.position], EOL, EOL_CHARACTERS)) {
			cerr << "ERROR (";
			cerr <<  utility::FileControl::getFileName(__FILE__);
			cerr << ":" << __LINE__ << "): cannot find character in current line " << c << "." << endl;
			throw Exception(__FILE__, __LINE__, Exception::EOF_REACHED);
		}
	} catch (Exception & e) {
		cerr << "ERROR (";
		cerr <<  utility::FileControl::getFileName(__FILE__);
		cerr << ":" << __LINE__ << "): cannot find character in current line " << c << "." << endl;
		throw Exception(__FILE__, __LINE__, Exception::EOF_REACHED);
	}


	return iter;
}

// ============================================================================

/* ****************************************************************************
 *
 *************************************************************************** */

/**
 * \brief searches for a string between the next character and the first
 * white space after that.
 */
char
Parser::getCurrentCharacter ()
{
	return file->fileBuffer[current.position];
};


string & ltrim(string & s) {
	s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
	return s;
}
string & rtrim(string & s) {
	s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
	return s;
}
string & trim(string & s) {
	ltrim(rtrim(s));
	return s;
}
/* ****************************************************************************
 *
 *************************************************************************** */
/**
 * \brief searches for a string between the next non-space character and the
 * first white space after that. It ignores " characters
 */
string
Parser::getString()
{
	return getString(" \t\r\f\n", false);
}

string
Parser::getString(char delimiter)
{
	char s[2] = {delimiter, '\0'};
	return getString(string(s), true);
}
string
Parser::getString(string delimiters, bool skipNextCharacter)
{
	Cursor                   iter           = current;
	try {
		iter            = skipSpaces(iter);
		while(string::npos == delimiters.find(file->fileBuffer[iter.position])) {
			increment(iter);
		}
		string s = string(file->fileBuffer, current.position, iter.position - current.position);
		trim(s);
        if (2 <= s.length() && '"' == s.at(0) && '"' == s.at(s.length()-1)) {
        	s = s.substr(1, s.length()-2);
        }
		// move to the first character after the delimiter
        if (skipNextCharacter) {
        	increment(iter);
		}
		current = iter;
		return s;
	} catch (Exception & e) {
		cerr << "ERROR (";
		cerr <<  utility::FileControl::getFileName(__FILE__);
		cerr << ":" << __LINE__ << "): could not find string." << endl;
		throw Exception(__FILE__, __LINE__, Exception::EOF_REACHED);
	}
	throw std::logic_error("reached unreachable code");
}

/* ****************************************************************************
 *
 *************************************************************************** */
/**
 * \brief searches for an integer between the next non-space character and the
 * first white space after that.
 */
INTEGER
Parser::getInteger()
{
	INTEGER                  rtn            = -1;

	try {
		string               value          = getString();
		rtn             = atoi(value.c_str());
	} catch (Exception & e) {
		clog << "ERROR (" <<  utility::FileControl::getFileName(__FILE__) << ":" << __LINE__ << "): failed to find an integer." << endl;
		throw e;
	}

	return rtn;
}
INTEGER
Parser::getInteger(char delimiter)
{
	INTEGER                  rtn            = -1;

	try {
		string               value          = getString(delimiter);
		rtn             = atoi(value.c_str());
	} catch (Exception & e) {
		clog << "ERROR (" <<  utility::FileControl::getFileName(__FILE__) << ":" << __LINE__ << "): failed to find an integer." << endl;
		throw e;
	}

	return rtn;
}

/* ****************************************************************************
 *
 *************************************************************************** */
/**
 * \brief searches for an integer between the next non-space character and the
 * first white space after that.
 */
FLOAT
Parser::getFloat()
{
	FLOAT                    rtn            = -1.0;

	try {
		string               value          = getString();
		rtn             = atof(value.c_str());
	} catch (Exception & e) {
		clog << "ERROR (" <<  utility::FileControl::getFileName(__FILE__) << ":" << __LINE__ << "): failed to find an integer." << endl;
		throw e;
	}

	return rtn;
}
double
Parser::getFloat(char delimiter)
{
	double                    rtn            = -1.0;

	try {
		string               value          = getString(delimiter);
		rtn             = atof(value.c_str());
	} catch (Exception & e) {
		clog << "ERROR (" <<  utility::FileControl::getFileName(__FILE__) << ":" << __LINE__ << "): failed to find an integer." << endl;
		throw e;
	}

	return rtn;
}
FLOAT
Parser::getFloat(string delimiters, bool skipNextCharacter)
{
  FLOAT rtn = -23.42;
 
	try {
		string value = getString(delimiters, skipNextCharacter);
		rtn = atof(value.c_str());
	} catch (Exception & e) {
		clog << "ERROR (" <<  utility::FileControl::getFileName(__FILE__) << ":" << __LINE__ << "): failed to find an integer." << endl;
		throw e;
	}
  return rtn;
}

/* ****************************************************************************
 *
 *************************************************************************** */
char
Parser::getCharacter()
{
	char                     rtn            = file->fileBuffer[current.position];

	try {
		increment(current);
	} catch (Exception & e) {
		clog << "ERROR (" <<  utility::FileControl::getFileName(__FILE__) << ":" << __LINE__ << "): failed to find an integer." << endl;
		throw e;
	}

	return rtn;
}

/* ****************************************************************************
 *
 *************************************************************************** */
LOGICAL
Parser::getBoolean()
{
	LOGICAL                  rtn            = false;

	try {
		string               value          = getString();
		std::transform(value.begin(), value.end(), value.begin(), ::tolower);

		if (value == "true") {
			rtn         = true;
		}

	} catch (Exception & e) {
		clog << "ERROR (" <<  utility::FileControl::getFileName(__FILE__) << ":" << __LINE__ << "): failed to find an boolean." << endl;
	}

	return rtn;
}

/* ****************************************************************************
 *
 *************************************************************************** */
LOGICAL
Parser::getBoolean(char delimiter)
{
	LOGICAL                  rtn            = false;

	try {
		string               value          = getString(delimiter);
		std::transform(value.begin(), value.end(), value.begin(), ::tolower);

		if (value == "true") {
			rtn         = true;
		}

	} catch (Exception & e) {
		clog << "ERROR (" <<  utility::FileControl::getFileName(__FILE__) << ":" << __LINE__ << "): failed to find an boolean." << endl;
	}

	return rtn;
}

/* ****************************************************************************
 *
 *************************************************************************** */
Date
Parser::getDate(DATE_FORMAT dateFmt)
{
	Date                     date;

	switch (dateFmt) {
	case YYYYMMDD_HYPEN :
		date.year       = getInteger('-');
		date.month      = getInteger('-');
		date.day        = getInteger();
		break;
	case MMDDYYYY_HYPEN :
		date.month      = getInteger('-');
		date.day        = getInteger('-');
		date.year       = getInteger();
		break;
	case YYYYMMDD_SLASH :
		date.year       = getInteger('/');
		date.month      = getInteger('/');
		date.day        = getInteger();
		break;
	case MMDDYYYY_SLASH :
		date.month      = getInteger('/');
		date.day        = getInteger('/');
		date.year       = getInteger();
		break;
	default:
		break;
	}

	return date;
}

/* ****************************************************************************
 *
 *************************************************************************** */
Date
Parser::getDate(DATE_FORMAT dateFmt, char delimiter)
{
	Date                     date;

	switch (dateFmt) {
	case YYYYMMDD_HYPEN :
		date.year       = getInteger('-');
		date.month      = getInteger('-');
		date.day        = getInteger(delimiter);
		break;
	case MMDDYYYY_HYPEN :
		date.month      = getInteger('-');
		date.day        = getInteger('-');
		date.year       = getInteger(delimiter);
		break;
	case YYYYMMDD_SLASH :
		date.year       = getInteger('/');
		date.month      = getInteger('/');
		date.day        = getInteger(delimiter);
		break;
	case MMDDYYYY_SLASH :
		date.month      = getInteger('/');
		date.day        = getInteger('/');
		date.year       = getInteger(delimiter);
		break;
	default:
		break;
	}

	return date;
}

/* ****************************************************************************
 *
 *************************************************************************** */
TimeOfDay
Parser::getTimeOfDay(TIME_FORMAT timeFmt)
{
	TimeOfDay                tod;

	switch (timeFmt) {
	case HHMMSS_HYPEN :
		tod.hours       = getInteger('-');
		tod.minutes     = getInteger('-');
		tod.seconds     = getInteger();
		break;
	case HHMM_HYPEN :
		tod.hours       = getInteger('-');
		tod.minutes     = getInteger('-');
		tod.seconds     = 0;
		break;
	case HHMMSS_COLON :
		tod.hours       = getInteger(':');
		tod.minutes     = getInteger(':');
		tod.seconds     = getInteger();
		break;
	case HHMM_COLON :
		tod.hours       = getInteger(':');
		tod.minutes     = getInteger(':');
		tod.seconds     = 0;
		break;
	default:
		break;
	}

	return tod;

}

/* ****************************************************************************
 *
 *************************************************************************** */
TimeOfDay
Parser::getTimeOfDay(TIME_FORMAT timeFmt, char delimiter)
{
	TimeOfDay                tod;

	switch (timeFmt) {
	case HHMMSS_HYPEN :
		tod.hours       = getInteger('-');
		tod.minutes     = getInteger('-');
		tod.seconds     = getInteger(delimiter);
		break;
	case HHMM_HYPEN :
		tod.hours       = getInteger('-');
		tod.minutes     = getInteger(delimiter);
		tod.seconds     = 0;
		break;
	case HHMMSS_COLON :
		tod.hours       = getInteger(':');
		tod.minutes     = getInteger(':');
		tod.seconds     = getInteger(delimiter);
		break;
	case HHMM_COLON :
		tod.hours       = getInteger(':');
		tod.minutes     = getInteger(delimiter);
		tod.seconds     = 0;
		break;
	default:
		break;
	}

	return tod;

}

/* ****************************************************************************
 *
 *************************************************************************** */
Time
Parser::getTime(DATE_FORMAT dateFmt, TIME_FORMAT timeFmt)
{
	Time                     time;

	time.date                = getDate(dateFmt);
	time.tod                 = getTimeOfDay(timeFmt);
	setMinutesPastZero(time);

	return time;
}

/* ****************************************************************************
 *
 *************************************************************************** */
Time
Parser::getTime(DATE_FORMAT dateFmt, TIME_FORMAT timeFmt, char delimiter)
{
	Time                     time;

	time.date                = getDate(dateFmt, delimiter);
	time.tod                 = getTimeOfDay(timeFmt, delimiter);
	setMinutesPastZero(time);

	return time;
}

/* ****************************************************************************
 *
 *************************************************************************** */
void
Parser::setMinutesPastZero(Time & time)
{
	time.minutesPastZero     = 0L;
	time.minutesPastZero    += 525600L*(LONG_INTEGER)time.date.year;
	time.minutesPastZero    += 1440L*(LONG_INTEGER)elapsedDays[time.date.month - 1];
	time.minutesPastZero    += 1440L*((LONG_INTEGER)time.date.day - 1L);
	time.minutesPastZero    += 60L*(LONG_INTEGER)time.tod.hours;
	time.minutesPastZero    += (LONG_INTEGER)time.tod.minutes;
}

/* ****************************************************************************
 *
 *************************************************************************** */
void
Parser::setPWCurve(PiecewiseLinear & curve)
{
	LOGICAL                  done           = false;
	curve.nSegments  = 0;
	while (!done) {
		INTEGER seg = curve.nSegments;
		// set the string between
		current         = skipCharacter(current, '(');
		curve.segment[seg].startPoint[POWER]  = getFloat(';');
		curve.segment[seg].startPoint[COST]   = getFloat(')');
		current         = skipCharacter(current, ';');
		current         = skipCharacter(current, '(');
		curve.segment[seg].endPoint[POWER]    = getFloat(';');
		curve.segment[seg].endPoint[COST]     = getFloat(')');

		// if next character is a comma, then all pieces are found
		// else check for the next piece
		current = skipSpaces(current);
		char                 c              = file->fileBuffer[current.position];
		if (c == ',') {
			current           = skipCharacter(current, ',');
			++curve.nSegments;
			done = true;
		} else if (c == '#' || c == ';') {
			increment(current);
			++curve.nSegments;
		} else {
			cerr << "ERROR (";
			cerr <<  utility::FileControl::getFileName(__FILE__);
			cerr << ":" << __LINE__ << "):  heat rate curve is incorrectly structured." << endl;
			throw Exception(__FILE__, __LINE__, Exception::INVALID_ARRAY);
		}
		curve.segment[seg].slope      = (curve.segment[seg].endPoint[COST] - curve.segment[seg].startPoint[COST])/
				(curve.segment[seg].endPoint[POWER] - curve.segment[seg].startPoint[POWER]);
		curve.segment[seg].intercept  = curve.segment[seg].startPoint[COST] -
				curve.segment[seg].slope * curve.segment[seg].startPoint[POWER];
	}
}

// ========================================================================

/* ****************************************************************************
 * ASSUME: the current tag is like the requested tag, the while loop will
 * either demonstrate that the assumption is false or confirm it is true
 *************************************************************************** */
LOGICAL
Parser::matchTag(Cursor & iter, const char * tag)
{
	INTEGER                  i              = 0;
	INTEGER                  count          = strlen(tag);
	bool                     likeTag        = true;

	while (i < count && iter.position != EOF && likeTag) {
		if (tag[i] != file->fileBuffer[iter.position]) {
			likeTag     = false;
		}
		++i;
		increment(iter);
	}

	return likeTag;
}

/* ****************************************************************************
 *
 *************************************************************************** */
LOGICAL
Parser::findTag(const char * tag, char startDelimiter, char stopDelimiter)
{
	resetParser();
	Cursor                   iter           = current;
	bool                     tagFound       = false;

	while(!tagFound && iter.position != EOF) {
		try {
			iter        = findNextCharacter(iter, startDelimiter);
			// find next lands on the delimiter, skip it by incrementing
			increment(iter);
			tagFound = matchTag(iter, tag);
			if (tagFound) {
				iter    = findNextCharacter(iter, stopDelimiter);
			}
			increment(iter);
			current     = iter;
		} catch (Exception & e) {
			clog << "ERROR (" <<  utility::FileControl::getFileName(__FILE__) << ":" << __LINE__ << "): tag " << tag << " not found." << endl;
			return false;
		}
	}

	return tagFound;
}

/* ****************************************************************************
 *
 *************************************************************************** */
string
Parser::getTagValue(const char * tag, char startDelimiter, char stopDelimiter,
		char separator)
{
	Cursor                   iter           = current;
	string                   rtn   = "";

	try {
		 LOGICAL             tagFound       = findTag(tag, startDelimiter, stopDelimiter);
		 if (tagFound) {
			 // iter was pointing at current in this method, findTag made it point
			 // at the tag, but is lost on the return. The method stores the
			 // correct value in current
			 iter            = current;
			 // read past the ending delimiter
			 iter            = findNextCharacter(iter, separator);
			 increment(iter);
			 iter            = skipSpaces(iter);
			 current         = iter;
			 rtn             = getString();
		 }
	} catch (Exception & e) {
		cerr << "ERROR (";
		cerr <<  utility::FileControl::getFileName(__FILE__);
		cerr << ":" << __LINE__ << "): ." << endl;
		throw Exception(__FILE__, __LINE__, Exception::INVALID_ARRAY);
	}
	return rtn;
}

} /* END OF NAMESPACE interpreter */
