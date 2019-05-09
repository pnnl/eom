/* ***************************************************************************
 * Details:
 * The Parser class performs two general functions: identifying a token and
 * interpreting it. The identification methods manipulate the current cursor to
 * find specific locations (e.g., the next comma, the start of a line, the end
 * of the file. The parser uses specific methods to position the cursor, such
 * as <i>increment</i>. Restricting access to the cursor this way prevents (or
 * at least inhibits) the ability to
 ************************************************************************** */
#ifndef PARSER_HPP_
#define PARSER_HPP_

#include "generic.hpp"
#include "structures.hpp"

namespace interpreter {

/**
 * \brief The Parser opens files and provides a set of reads used by subclasses.
 *
 */

class Parser
{
public:
	/**
	 * \brief Start a parser and pass it the file specified by <i>fileName</i>.
	 */
	Parser(const std::string & fileName);
	/**
	 * \brief Start a parser and pass it the FileData structure specified by
	 * <i>fileData</i>.
	 */
	virtual ~Parser();

	/**
	 * \brief Return the number of lines in the parser's file
	 */
	virtual INTEGER getNLines() {return nLines;};


	struct Cursor {
		INTEGER              position;
		INTEGER              line;
		INTEGER              column;
	};

	struct ParserTag
	{
		std::string          modelTagName;
		INFORMATION_TYPE     modelType;
	};
protected:
	/* ************************************************************************
	 * File methods
	************************************************************************ */
	/**
	 * \brief Resets the current and next parser.
	 */
	void           reduceFile          ();

	/**
	 * \brief Resets the current and next parser.
	 */
	void           countLines          ();

	/* ************************************************************************
	 * Cursor methods
	************************************************************************ */
	/**
	 * \brief Resets the current and next parser.
	 */
	void           resetParser         ();

	/**
	 * \brief Resets the current and next parser.
	 */
	void           reduceCSV           ();

	/**
	 * \brief Controls the movement of the cursor passed through the formal
	 * argument
	 */
	void           increment           (Cursor & cursor);

	/**
	 * \brief Ensure the position value <i>pos</i> remains within the allowable
	 * range (0 to <i>fileSize</i>).
	 */
	LOGICAL        isInBounds          (INTEGER pos);

	/**
	 * \brief Determine whether a given line is empty, i.e., the first
	 * character in the line is NULL.
	 */
	LOGICAL        isLineEmpty         ();

	/**
	 * \brief moves cursor to first character after the next newline character
	 */
	Cursor         skipLine            (Cursor & cursor);

	/**
	 * \brief moves cursor to first non-white space character
	 */
	Cursor         skipSpaces          (Cursor & cursor);

	/**
	 * \brief moves cursor to first non-white space character
	 */
	Cursor         skipCharacters      (Cursor & cursor);

	/**
	 * \brief moves cursor past the first occurrence of the character c
	 */
	Cursor         skipCharacter       (Cursor & cursor, char c);

	/**
	 * \brief moves the next cursor to the first character after its current position
	 */
	Cursor         findNextCharacter   (Cursor & cursor, char c);

	/**
	 * \brief find the first occurrence of character c in the current line
	 */
	Cursor         findCharacterInLine (Cursor & cursor, char c);

	// ========================================================================

	/**
	 * \brief searches for a string between the next character and the first
	 * white space after that.
	 */
	char           getCurrentCharacter ();
	char           getCurrentCharacter (Cursor & position);

	/**
	 * \brief searches for a string between the next character and the first
	 * white space after that.
	 */
	std::string  getString();
	std::string  getString(char delimiter);
	std::string  getString(std::string delimiters, bool skipNextCharacter);

	/**
	 * \brief reads characters until no digit is found. Convert the string to an
	 * integer.
	 */
	INTEGER getInteger();
	INTEGER getInteger(char delimiter);

	/**
	 * \brief reads characters until the end float is found. Convert the string
	 * to an FLOAT.
	 */
	FLOAT   getFloat();
	FLOAT   getFloat(char delimiter);
	FLOAT   getFloat(std::string delimiters, bool skipNextCharacter);

	/**
	 * \brief finds the next character in the array and returns it
	 */
	char    getCharacter();
	char    getCharacter(const char * text);

	/**
	 * \brief reads characters to find either "true" or "false" where the
	 * strings are case insensitive.
	 */
	LOGICAL getBoolean();
	LOGICAL getBoolean(char delimiter);

	/**
	 * \brief read the data using the predefined format
	 */
	Date getDate(DATE_FORMAT dateFmt);
	Date getDate(DATE_FORMAT dateFmt, char delimiter);
	/**
	 * \brief read the time of day using the predefined format
	 */
	TimeOfDay getTimeOfDay(TIME_FORMAT timeFmt);
	TimeOfDay getTimeOfDay(TIME_FORMAT timeFmt, char delimiter);
	/**
	 * \brief read the time using the predefined date and time formats
	 */
	Time getTime(DATE_FORMAT dateFmt, TIME_FORMAT timeFmt);
	Time getTime(DATE_FORMAT dateFmt, TIME_FORMAT timeFmt, char delimiter);
	/**
	 * \brief compute the minutes past zero (minute*hour*day*month*year)
	 */
	void setMinutesPastZero(Time & start);

	void   setPWCurve(PiecewiseLinear & curve);
	// ========================================================================
	/**
	 * \brief searches for a tag in a <tag,value> pair between defined
	 * delimiters. Once the tag is found, the value is returned.
	 */
	std::string getTagValue(const char * tag, char startDelimiter, char stopDelimiter, char separator); // get the value associated with the tag

	/**
	 * \brief find the tag between the defined delimiters
	 */
	LOGICAL findTag(const char * tag, char startDelimiter, char stopDelimiter);

	/**
	 * \brief does a character-by-character match of a tag to the current string
	 */
	LOGICAL matchTag(Cursor & iter, const char * tag);

	Cursor                   current;
	FileData               * file;
	INTEGER                  nLines;

private:



	// The automatically generated methods are hidden in private
	Parser & operator = (Parser & cntl){return *this;};

};

} /* END OF NAMESPACE interpreter */
#endif /* PARSER_HPP_ */
