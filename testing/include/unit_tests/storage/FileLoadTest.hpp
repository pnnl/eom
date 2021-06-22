/* ***************************************************************************
 * Author      : Kevin Glass
 * Date        : Mar 7, 2011
 * File        : FileLoadTest.hpp
 * Project     : GENERAL
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
 * not done     doxygen comments
 * not done     private comments
 * not done     comment block conformity
 * not done     testing information
 * not done     ensure hidden methods
 *
 ************************************************************************** */

#ifndef FILELOADTEST_HPP_
#define FILELOADTEST_HPP_

#include "FileControl.hpp"
#include "DirectoryControl.hpp"
#include "generic.hpp"

class FileLoadTest
{
public:
	static bool fileLoadTest(DirectoryControl * sourceDir,
			DirectoryControl * resultsDir, string & fileName, ostream * out);
protected:
	bool testValidFile(DirectoryControl * sourceDir,
			DirectoryControl * resultsDir, string & fileName, ostream * out);
	bool testInvalidDir(DirectoryControl * sourceDir,
			DirectoryControl * resultsDir, string & fileName, ostream * out);
private:
	FileLoadTest(){};
	virtual ~FileLoadTest(){};
};

#endif /* FILELOADTEST_HPP_ */
