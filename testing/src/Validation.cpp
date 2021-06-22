/* ***************************************************************************
 * Author      : Kevin Glass
 * Date        : Jul 3, 2011
 * File        : Validation.cpp
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

#include "Validation.hpp"
#include "FileControl.hpp"
#include "exceptions/Exception.hpp"

namespace testing {

Validation::Validation(const char * logName) :
		nFailed(0),
		nTests(0),
		nManual(0)
{
	validationLog   = new fstream(logName, ios::out | ios::app);
}

Validation::Validation(fstream * validationLog) :
		validationLog(validationLog),
		nFailed(0),
		nTests(0),
		nManual(0)
{
}

Validation::~Validation()
{
	cout << nTests << " tests were run. " << nTests - nFailed << " passed, " << nFailed << " failed." << endl;
	*validationLog << "\n" << nTests << " tests were run. " << nTests - nFailed << " passed, " << nFailed << " failed, to be checked  " << nManual  << endl;
	delete validationLog;
}

bool
Validation::diffFiles(const char * file1, const char * file2)
{
	try {
		INTEGER              size1          = utility::FileControl::validateFile(file1);
		INTEGER              size2          = utility::FileControl::validateFile(file1);
		if (size1 != size2) {
			return true;  // the files are different
		}
		INTEGER              handle1        = utility::FileControl::openInputFile(file1);
		INTEGER              handle2        = utility::FileControl::openInputFile(file2);
		char               * buffer1        = (char *)malloc(size1);
		char               * buffer2        = (char *)malloc(size2);
		read(handle1, buffer1, size1);
		read(handle2, buffer2, size1);
		for (INTEGER i = 0; i < size1; i++) {
			if (buffer1[i] != buffer2[i]) {
				return true;
			}
		}
	} catch (Exception & e) {

	} catch (...) {

	}

	return false;
}

} /* namespace testing */
