#ifndef TERMINATIONEXCEPTION_HPP_
#define TERMINATIONEXCEPTION_HPP_

#include "Exception.hpp"


class TerminationException : public Exception
{
public:

	TerminationException(const char * fileName, int lineNum, EXCEPTION_TYPE flag) :
		Exception(fileName, lineNum, flag){};
	virtual ~TerminationException() {};
protected:
	virtual void printException(std::ostream & out)
	{
	}
private :

	TerminationException & operator = (TerminationException & e) {return *this;};
};

#endif /* TERMINATIONEXCEPTION_HPP_ */
