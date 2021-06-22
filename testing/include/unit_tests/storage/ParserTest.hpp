/* ***************************************************************************
 * Author      : Kevin Glass
 * Date        : Mar 18, 2011
 * File        : ParserTest.hpp
 * Project     : TESTING
 *
 * Assumptions:
 *
 *
 * Testing:
 *     Construction tests
 *         construct from file name
 *         construct from existing file data
 *     Control tests
 *         increment from start to finish
 *
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
#ifndef PARSERTEST_HPP_
#define PARSERTEST_HPP_
#include "Parser.hpp"
#include "DirectoryControl.hpp"
#include "FileControl.hpp"
#include "generic.hpp"

namespace interpreter {

class ParserTest {
public:
	static bool parserLoadTest(DirectoryControl * sourceDir,
			DirectoryControl * resultsDir, string & fileName,
			string & outFile, ostream * out);
protected:
	bool parserConstructionTest1(DirectoryControl * sourceDir,
			DirectoryControl * resultsDir, string & fileName,
			string & outFile, ostream * out);
	bool parserConstructionTest2(DirectoryControl * sourceDir,
			DirectoryControl * resultsDir, string & fileName,
			string & outFile, ostream * out);
	bool parserControlTest(DirectoryControl * sourceDir,
			DirectoryControl * resultsDir, string & fileName,
			string & outFile, ostream * out);
	bool parserNumericsTest(DirectoryControl * sourceDir,
			DirectoryControl * resultsDir, string & fileName,
			string & outFile, ostream * out);

	bool propertiesTest(Parser * parser, ostream * out);
	bool incrementTest(Parser * parser, ostream * out);
	bool resetTest(Parser * parser, ostream * out);
	bool startOfLineTest(Parser * parser, ostream * out);
	bool spaceToEOLTest(Parser * parser, ostream * out);
private:
	ParserTest(){};
	virtual ~ParserTest(){};
	ParserTest(ParserTest & test){};
	ParserTest & operator = (ParserTest & parser){return *this;};
};
}
#endif /* PARSERTEST_HPP_ */
