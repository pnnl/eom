/* ***************************************************************************
 * Author      : Kevin Glass
 * Date        : Jul 3, 2011
 * File        : Validation.hpp
 * Project     : RIM
 *
 * PURPOSE:
 *
 *
 * STATUS:
 *     naming convention conformity
 *     doxygen comments
 *     private comments
 *     comment block conformity
 *     testing information
 *     ensure hidden methods
 *
 * TESTING:
 *
 * ---------------------------------------------------------------------------
 */

#ifndef VALIDATION_HPP_
#define VALIDATION_HPP_

#include "generic.hpp"

namespace testing {

class Validation
{
public:
	Validation(const char * logName);
	Validation(fstream * validationLog);
	virtual ~Validation();
protected:
	fstream                * validationLog;

	bool diffFiles(const char * file1, const char * file2);
	void incrementTest(){++nTests;};
	void incrementFailure(){++nFailed;};
	void incrementManual(){++nManual;};
private:
	INTEGER                  nFailed;
	INTEGER                  nTests;
	INTEGER                  nManual;

	Validation(Validation & val) :
		validationLog(NULL),
		nFailed(0),
		nTests(0),
		nManual(0)
	{};
	Validation & operator = (Validation & v){return *this;};
};

} /* namespace testing */
#endif /* VALIDATION_HPP_ */
