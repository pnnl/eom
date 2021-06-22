/* ***************************************************************************
 * Author      : Kevin Glass
 * Date        : Mar 18, 2011
 * File        : ParserTest.cpp
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
#include "unit_tests/ParserTest.hpp"
#include "TestHarness.hpp"
#include "FileControl.hpp"
#include "exceptions/FileException.hpp"

namespace interpreter {

bool
ParserTest::parserLoadTest(DirectoryControl * sourceDir,
		DirectoryControl * resultsDir, string & fileName,
		string & outFile, ostream * out)
{
	ParserTest               test;

	bool                     rtn            = true;
#if defined PARSER_LOAD_TEST || defined ALL
	*out << "\tParser loaded from file ---------------------------------" << endl;
	*out << "\t\topen file " << fileName << endl;
	rtn                 &= test.parserConstructionTest1(sourceDir, resultsDir, fileName, outFile, out);
	*out << "\tParser loaded from file data   --------------------------" << endl;
	rtn                 &= !test.parserConstructionTest2(sourceDir, resultsDir, fileName, outFile, out);
	*out << "\t---------------------------------------------------------" << endl;
#endif

#if defined PARSER_CONTROL_TEST || defined ALL
	*out << "\tParser control test -------------------------------------" << endl;
	rtn                 &= test.parserControlTest(sourceDir, resultsDir, fileName, outFile, out);
#endif

#if defined PARSER_NUMERICS_TEST || defined ALL
	*out << "\tParser control test -------------------------------------" << endl;
	rtn                 &= test.parserNumericsTest(sourceDir, resultsDir, fileName, outFile, out);
#endif
	return rtn;
}

bool
ParserTest::parserConstructionTest1(DirectoryControl * sourceDir,
		DirectoryControl * resultsDir, string & fileName,
		string & outFile, ostream * out)
{
	bool                     rtn            = false;
	char                     name[256];
	char                     output[256];

	try {
		sprintf(name, "%s/%s", sourceDir->getName(), fileName.c_str());
		Parser             * parser         = new Parser(name);
		FileData           * fileData       = parser->file;
		sprintf(output, "%s/%s", resultsDir->getName(), fileName.c_str());
		if (DirectoryControl::verifyDirectory(resultsDir->getName())) {
			FileControl::dumpToFile(fileData, output);
		}
		delete parser;
	} catch (ParserException & e) {
		stringstream         msg;
		msg << FileControl::getFileName(__FILE__) << ": " << __LINE__ << endl;
		msg << "\tERROR: The parsers starting index was less then zero";
		throw ParserException(msg, Exception::DESIGN);
	}
	return rtn;
}

bool
ParserTest::parserConstructionTest2(DirectoryControl * sourceDir,
		DirectoryControl * resultsDir, string & fileName,
		string & outFile, ostream * out)
{
	bool                     rtn            = false;
	char                     name[256];
	char                     output[256];

	try {
		sprintf(name, "%s/%s", sourceDir->getName(), fileName.c_str());
		FileData           * data           =  FileControl::loadFile(name);
		Parser             * parser         = new Parser(data);
		data            = NULL;
		FileData           * fileData       = parser->file;
		sprintf(output, "%s/%s1", resultsDir->getName(), fileName.c_str());
		if (DirectoryControl::verifyDirectory(resultsDir->getName())) {
			FileControl::dumpToFile(fileData, output);
		}
		delete parser;
	} catch (ParserException & e) {
		stringstream         msg;
		msg << FileControl::getFileName(__FILE__) << ": " << __LINE__ << endl;
		msg << "\tERROR: The parsers starting index was less then zero";
		throw ParserException(msg, Exception::DESIGN);
	}
	return rtn;
}

bool
ParserTest::parserControlTest(DirectoryControl * sourceDir,
		DirectoryControl * resultsDir, string & fileName,
		string & outFile, ostream * out)
{
	bool                     rtn            = false;
	char                     name[256];
	char                     output[256];

	try {
		sprintf(name, "%s/%s", sourceDir->getName(), fileName.c_str());
		FileData           * data           =  FileControl::loadFile(name);
		Parser             * parser         = new Parser(data);
		data            = NULL;

		sprintf(output, "%s/parserControl", resultsDir->getName());
		if (DirectoryControl::verifyDirectory(resultsDir->getName())) {
			rtn         &= propertiesTest(parser, out);
			rtn         &= incrementTest(parser, out);
			rtn         &= resetTest(parser, out);
			rtn         &= startOfLineTest(parser, out);
			rtn         &= spaceToEOLTest(parser, out);
		}
		delete parser;
	} catch (ParserException & e) {
		stringstream         msg;
		msg << FileControl::getFileName(__FILE__) << ": " << __LINE__ << endl;
		msg << "\tERROR: The parsers starting index was less then zero";
		throw ParserException(msg, Exception::DESIGN);
	}
	return rtn;
}

bool
ParserTest::parserNumericsTest(DirectoryControl * sourceDir,
		DirectoryControl * resultsDir, string & fileName,
		string & outFile, ostream * out)
{
	bool                     rtn            = false;
	char                     name[256];
	char                     output[256];

	try {
		sprintf(name, "%s/%s", sourceDir->getName(), fileName.c_str());
		FileData           * data           =  FileControl::loadFile(name);
		Parser             * parser         = new Parser(data);
		data            = NULL;

		sprintf(output, "%s/%s", resultsDir->getName(),outFile.c_str());
		ofstream             resFile;
		resFile.open(output);
		if (DirectoryControl::verifyDirectory(resultsDir->getName())) {
			resFile << parser->getInteger() << endl;
			parser->readDelimiter(parser->file->fileSize, ',');
			resFile << parser->getFloat() << endl;
			parser->readDelimiter(parser->file->fileSize, ',');
			parser->findCharacter(parser->file->fileSize);
			resFile << parser->getString(parser->file->fileSize, ',') << endl;
			parser->startOfLine();
			resFile << parser->getFloat() << endl;
			parser->readDelimiter(parser->file->fileSize, ':');
			resFile << parser->getCharacter() << endl;
		}
		resFile.close();
		delete parser;
	} catch (ParserException & e) {
		stringstream         msg;
		msg << FileControl::getFileName(__FILE__) << ": " << __LINE__ << endl;
		msg << "\tERROR: The parsers starting index was less then zero";
		throw ParserException(msg, Exception::DESIGN);
	}
	return rtn;
}

bool
ParserTest::propertiesTest(Parser * parser, ostream * out)
{
	bool                     rtn            = false;

	*out << "\tFile properties test" << endl;
	*out << "\t\tUsing source file " << parser->file->fileName << endl;
	*out << "\t\tThe number of lines is  " << parser->countLines();
	*out << "\t\tThe number of commas is " << parser->countCharacters(',', 0, parser->file->fileSize);

	return rtn;
}

bool
ParserTest::incrementTest(Parser * parser, ostream * out)
{
	bool                     rtn            = false;

	*out << "\tIncrement test" << endl;
	*out << "\t\tUsing source file " << parser->file->fileName << endl;
	for (INTEGER i = 0 ; i < parser->file->fileSize-1; i++) {
		*out << "\t\tCurrent = (" << parser->current.position << ", ";
		*out << parser->current.line << ", " << parser->current.column << ") ";
		*out << (INTEGER)parser->file->readBuffer[parser->current.position]<< endl;
		parser->increment();
	}
	*out << "\t\tThe number of lines is " << parser->countLines() << endl;

	return rtn;
}

bool
ParserTest::resetTest(Parser * parser, ostream * out)
{
	bool                     rtn            = false;

	*out << "\tReset test" << endl;
	*out << "\t\tUsing source file " << parser->file->fileName << endl;
	*out << "\t\tCurrent = (" << parser->current.position << ", ";
	*out << parser->current.line << ", " << parser->current.column << ") ";
	*out << (INTEGER)parser->file->readBuffer[parser->current.position]<< endl;

	parser->resetParser();

	*out << "\t\tCurrent = (" << parser->current.position << ", ";
	*out << parser->current.line << ", " << parser->current.column << ") ";
	*out << (INTEGER)parser->file->readBuffer[parser->current.position]<< endl;

	*out << "\t\tThe number of lines is " << parser->countLines() << endl;
	return rtn;
}

bool
ParserTest::startOfLineTest(Parser * parser, ostream * out)
{
	bool                     rtn            = false;

	*out << "\tStart Of Line test" << endl;
	*out << "\t\tUsing source file " << parser->file->fileName << endl;
	parser->resetParser();
	try {
	while (parser->validBounds(parser->current.position)){
		if (parser->startOfLine()) {
			*out << "\t\tCurrent = (" << parser->current.position << ", ";
			*out << parser->current.line << ", " << parser->current.column << ") ";
			*out << parser->file->readBuffer[parser->current.position]<< endl;
		}
		if (parser->current.position < parser->file->fileSize) {
			parser->increment();
		}
	}
	} catch (ParserException & e) {

	}

	return rtn;
}

bool
ParserTest::spaceToEOLTest(Parser * parser, ostream * out)
{
	bool                     rtn            = false;

	*out << "\tDistance to EOL test" << endl;
	*out << "\t\tUsing source file " << parser->file->fileName << endl;
	parser->resetParser();
	for (INTEGER i = 0 ; i < parser->file->fileSize-1; i++) {
		*out << "\t\tCurrent = (" << parser->current.position << ", ";
		*out << parser->current.line << ", " << parser->current.column << ") ";
		*out << parser->spacesToEOL()<< endl;
		parser->increment();
	}

	return rtn;
}

}
INTEGER
main(INTEGER count, char * argument[])
{
	if (count < 2 || count > 4) {
		cout << "USAGE: ParserTest <file name> [<output file name>] [<log file name>]" << endl;
		exit(-2);
	}
	string                   srcFile =
		FileControl::getFileName(argument[1]);
	string               temp              =
		FileControl::getFilePath(argument[1]);
	string                   dirPath =
		FileControl::chopLastPath(temp.c_str());
	string                   outFile;
	if (count == 2) {
		outFile        = srcFile;
	} else {
		outFile        = string(argument[2]);
	}
	char                     srcPath[256];
	char                     resPath[256];

	sprintf(srcPath, "%s/data", dirPath.c_str());
	sprintf(resPath, "%s/results", dirPath.c_str());

	DirectoryControl       * sourceDir;
	DirectoryControl       * resultsDir;
	try {
		sourceDir       = new DirectoryControl(srcPath);
		resultsDir      = new DirectoryControl(resPath);
	} catch (FileException & e) {
		cout << "TEST FAILED TO EXECUTE" << endl;
		cout << e << endl;
		exit(-1);
	}

	if (count == 3) {
		ofstream                 out;
		out.open(argument[2]);
		ParserTest::parserLoadTest(sourceDir, resultsDir, srcFile, outFile, &out);
	} else {
		ParserTest::parserLoadTest(sourceDir, resultsDir, srcFile, outFile, &cout);
	}
}
