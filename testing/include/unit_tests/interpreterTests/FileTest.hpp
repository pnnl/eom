/*
 * FileTest.hpp
 *
 *  Created on: Jul 17, 2011
 *      Author: kglass
 */

#ifndef FILETEST_HPP_
#define FILETEST_HPP_

#include "structures.hpp"
#include "Exception.hpp"
#include "Validation.hpp"

namespace testing {

class FileTest : public Validation
{
public:
	FileTest();
	virtual ~FileTest();
protected:
	virtual void setupTests();
	void validateTest();
	void openInputFileTest();
	void loadFileTest();
	void openIncrementalTest();
	void incrementalLoadTest();
	void incrementalXferBlocksTest();
	void resetFileTest();
	void openOutputFileTest();
	void dumpToFileTest();
	void closeFileTest();
	void getFileNameTest();
	void chopLastPathTest();
private:
	INTEGER                  sizeOfTestFile;
	INTEGER                  sizeOfNumbersFile;
};

} /* namespace testing */
#endif /* FILETEST1_HPP_ */
