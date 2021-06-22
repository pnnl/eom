/*
 * RIMParserTest.hpp
 *
 *  Created on: Apr 21, 2011
 *      Author: Kevin Glass
 */

#ifndef RIMPARSERTEST_HPP_
#define RIMPARSERTEST_HPP_

#include "structures.hpp"
#include "DirectoryControl.hpp"
#include "FileControl.hpp"
#include "interpreter/RIMParser.hpp"
#include "exceptions/FileException.hpp"

namespace testing {


class RIMParserTest : public interpreter::RIMParser
{
public:
	RIMParserTest(INTEGER count, char * argument[], ostream * out);
	virtual ~RIMParserTest();
protected:
	bool rimSetupTest1(string & outFile);
	bool rimLoadSourceFiles(string & outFile);
	bool rimModelInfoTest(string & outFile);
	bool rimModelParameterTest(string & outFile);
	bool rimModelInterfaceTest(string & outFile);
	bool rimResults(string & outFile);

	void runTests();
	bool rimParserTest();
private:

	string                   fileName;
	string                   outFile;
	string                   srcFile;
	string                   dirPath;
	ostream                * out;

	utility::DirectoryControl       * sourceDir;
	utility::DirectoryControl       * resultsDir;
	utility::DirectoryControl       * stdDir;
};

} /* END OF NAMESPACE testing */

#endif /* RIMPARSERTEST_HPP_ */
