/* ***************************************************************************
 * Author      : Kevin Glass
 * Date        : Mar 7, 2011
 * File        : FileLoadTest.hpp
 * Project     : TESTING
 *
 * Assumptions:
 *
 *
 * Testing:
 *      TBD
 *
 *
 * STATUS:
 * not done     naming convention conformity
 *     done     doxygen comments
 *     done     private comments
 *     done     comment block conformity
 *     done     testing information
 *     done     ensure hidden methods
 *
 ************************************************************************** */
#include "unit_tests/FileLoadTest.hpp"
#include "DirectoryControl.hpp"
#include "exceptions/FileException.hpp"
#include "TestHarness.hpp"

bool
FileLoadTest::fileLoadTest(DirectoryControl * sourceDir,
		DirectoryControl * resultsDir, string & fileName, ostream * out)
{
	FileLoadTest             test;

	bool                     rtn            = true;
	*out << "\tVerify valid file succeeds ------------------------------" << endl;
	*out << "\t\topen file " << fileName << endl;
	rtn                 &= test.testValidFile(sourceDir, resultsDir, fileName, out);
	*out << "\tVerify invalid directory fails --------------------------" << endl;
	rtn                 &= !test.testInvalidDir(sourceDir, resultsDir, fileName, out);
	*out << "\t---------------------------------------------------------" << endl;
	return rtn;
}

bool
FileLoadTest::testValidFile(DirectoryControl * sourceDir,
		DirectoryControl * resultsDir, string & fileName, ostream * out)
{
	bool                     rtn            = false;
	char                     name[256];
	char                     output[256];
	// removes the last name after the directory separator
	try {
		sprintf(name, "%s/%s", sourceDir->getName(), fileName.c_str());
		FileData               * fileData       = FileControl::loadFile(name);

		sprintf(output, "%s/%s", resultsDir->getName(), fileName.c_str());
		if (DirectoryControl::verifyDirectory(resultsDir->getName())) {
			FileControl::dumpToFile(fileData, output);
			rtn  = TestHarness::compareFiles(output, name);
			if (rtn) {
				TestHarness::testPassed();
			} else {
				TestHarness::testFailed();
			}
		}
	} catch (...) {
		*out << "\t\tTest threw an exception." << endl;
	}
	return rtn;
}

bool
FileLoadTest::testInvalidDir(DirectoryControl * sourceDir,
		DirectoryControl * resultsDir, string & fileName, ostream * out)
{
	bool                     rtn            = false;
	char                     output[256];
	// removes the last name after the directory separator

	try {
		sprintf(output, "%sq", resultsDir->getName());
		if (DirectoryControl::verifyDirectory(output)) {
			TestHarness::testFailed();
			*out << "\t\tFailed." << endl;
		}
	} catch (FileException & e) {
		TestHarness::testPassed();
		*out << "\t\tPassed." << endl;
	} catch (...) {
		*out << "\t\tTest threw an undefined exception." << endl;
		exit(-1);
	}
	return rtn;
}
