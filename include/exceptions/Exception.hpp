/* ***************************************************************************
 * Details:
 * The Exception class is a generic class used to define the behavior of all
 * RIM exceptions. The specific behavior consists of construction, appending
 * to an existing exception and printing of the exception message. Every
 * exception must include the name of the file in which the exception occurred
 * or was appended.
 ************************************************************************** */
#ifndef EXCEPTION_HPP_
#define EXCEPTION_HPP_

#include <string>
#include <ostream>

class Exception {
public:
	enum EXCEPTION_TYPE {
		NONE,
		DESIGN,
		ALLOCATION1,
		INITIALIZATION,
		SETUP1,
		ACCESS1,
		ACCESS2,
		ACCESS3,
		OPEN1,
		READING1,
		UNCLEAR,
		EOF_REACHED,
		INVALID_ARRAY,
		INVALID_ARRAY_SIZE,
		SOLVER,
		CONVERGENCE,
		PARSING,
		UNIT_COMMIT_FAILED,
		LOAD_FOLLOWING_FAILED,
		REGULATON_FAILED,
		INVALID_REALLOCATION,
		INVALID_REDEFINITION,
		CONSTRAINT_LOAD_FAILURE,
		ACCESSED_NULL
	};
	/**
	 * \brief Create an instance of an exception.
	 *
	 * The exception constructor takes a message explaining the nature of the
	 * exception, the name of the file from which the exception was thrown,
	 * the line number of the throw and a flag indicating the exception type.
	 * The method truncates the file name by removing the path from the
	 * file name (if any).
	 *
	 * The file name, line number and message are formatted in the exception's
	 * message
	 */
	Exception(const char * sourceFile, const int line, EXCEPTION_TYPE flag) :
		line(line), sourceFile(sourceFile), flag(flag) {};
	Exception(const Exception & e) :
		line(e.line), sourceFile(e.sourceFile), flag(e.flag) {};

	/**
	 * \brief Construct a new exception from an old one.
	 *
	 */
	virtual ~Exception() {};

	const int getLineNumber(){return line;};
	/**
	 * \brief print the formatted exception message then print additional
	 * exception specific information.
	 */
	friend std::ostream & operator << (std::ostream & out, Exception & e) {
		using std::endl;
		out << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
		out << e.sourceFile << ": " << e.line << " ERROR: " << e.flag << endl;
		out << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
		e.printException(out);
		return out;
	}
	EXCEPTION_TYPE getReason(){return flag;};
protected:
	const int                line;
	const std::string        sourceFile;
	const EXCEPTION_TYPE     flag;

	virtual void printException(std::ostream & out){};

};

#endif /* EXCEPTION_HPP_ */
