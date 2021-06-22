#include "exceptions/Exception.hpp"
#include "FileControl.hpp"

using std::clog;
using std::cerr;
using std::endl;
using std::string;

namespace utility {
/* ****************************************************************************
 * INTEGER FileControl::validateFile(const char * fileName)
 *************************************************************************** */
/**
 * \param fileName : const char * -- name of the file being validated
 * \return size of the file in bytes
 * \throws Exception ACCESS1 exception if the file cannot be accessed (permissions, no such file, etc)
 * \throws Exception ACCESS2 exception if the file is not a regular file
 */
INTEGER
FileControl::validateFile(const char * fileName)
{
	struct stat              status;
	INTEGER                  error          = -1;

	error = stat(fileName, &status);
	if (error < 0) { // stat returns -1 for an invalid file (e.g. no such file).  errno will be set in this case to give more info.
		clog << "ERROR (" <<  utility::FileControl::getFileName(__FILE__) << ":" << __LINE__ << "): failed to access file " << fileName << endl;
		throw Exception(__FILE__, __LINE__ , Exception::ACCESS1);
	}

	if (!(S_ISREG(status.st_mode))) { // check that we're not a special file, like a directory
		clog << "ERROR (" <<  utility::FileControl::getFileName(__FILE__) << ":" << __LINE__ << "): file " << fileName << " is not a regular file." << endl;
		throw Exception(__FILE__, __LINE__ , Exception::ACCESS2);
	}

	return status.st_size;
}
/* ****************************************************************************
 * INTEGER FileControl::openInputFile(const char * fileName)
 *************************************************************************** */
/**
 * Use the <i>open</i> system command to open the file specified by fileName.
 * If the <i>open</i> command is successful, it will return an integer
 * specifying the file fileHandle, the method will in turn, return the fileHandle. If
 * the command is not successful, the command will throw an exception.
 *
 * \param fileName : const char * -- name of the file being validated
 * \return fileHandle : int           -- the file's fileHandle
 * \throws  ACCESS exception if the file fileHandle is less than zero.
 */
INTEGER
FileControl::openInputFile(const char * fileName)
{
	INTEGER                  fileHandle         = open(fileName, O_RDONLY);
	if (fileHandle < 0) {
		clog << "ERROR (" <<  utility::FileControl::getFileName(__FILE__) << ":" << __LINE__ << "): could not open " << fileName << endl;
		throw Exception(__FILE__, __LINE__ , Exception::OPEN1);
	}

	return fileHandle;
}
/* ****************************************************************************
 * FileData * FileControl::loadFile(const char * fileName)
 *************************************************************************** */
/**
 * The <i>loadFile</i> method allocates a FileData structure (see include/generic.hpp).
 * The structure holds pertinent file information including the file size, file
 * fileHandle and the file buffer. Once the file structure is allocated, the file size
 * is set from the return value of <i>validateFile</i>, the file is opened using
 * <i>openInputFile</i>the fileBuffer is allocated using <i>malloc</i> and the
 * file size + 1, and the data is read from the file into the buffer using the
 * system call <i>read</i>.If the size read is not the same as the file size,
 * the method will throw a Exception. As a security measure, a '\0'
 * character is added to the end of the buffer. The file is closed and the file
 * fileHandle is set to -1.
 *
 * If any of the FileControl calls throws an exception, the exception will be
 * caught, a message will be appended to the exceptions message and the
 * exception will be rethrown
 *
 * \param fileName : const char * -- name of the file being validated
 * \return file : FileData *      -- the file's fileHandle
 * \throws Exception ACCESS exception if file size != size read from file
 * \throws Exception ACCESS exception if a FileControl exception is caught,
 * the exception is re-thrown
 */
FileData *
FileControl::loadFile(const char * fileName)
{
	FileData           * file           = NULL;
	try {
		// FileData is a structure with no constructor or destructor
		file                 = new FileData;//(FileData *)malloc(sizeof(FileData));
		strncpy(file->fileName, fileName, 254);
		file->fileSize       = FileControl::validateFile(fileName);
		file->fileHandle     = FileControl::openInputFile(fileName);

		file->blockSize      = -1;
		file->fileBuffSize   =  file->fileSize;
		file->fileBuffer     = new char[file->fileSize + 2]();


		file->readPoint      = -1;
		file->loadPoint      = -1;

		file->nOutputBlocks  = -1;
		file->outputBuffer   = NULL;

		INTEGER readSize = read(file->fileHandle, file->fileBuffer, file->fileSize);

		if (readSize > file->fileSize || readSize == -1) {
			clog << "ERROR (" <<  utility::FileControl::getFileName(__FILE__) << ":" << __LINE__ << "): file size did not match read size." << endl;
			throw Exception(__FILE__, __LINE__ , Exception::ACCESS1);
		}
		file->fileBuffer[file->fileSize] = '\0';
		close(file->fileHandle);
		file->fileHandle         = -1;

	} catch (Exception & e) {
		clog << "ERROR (" <<  utility::FileControl::getFileName(__FILE__) << ":" << __LINE__ << "): could not load file " << fileName << endl;
		throw;
	}

	return file;
}
/* ****************************************************************************
 * FileData * FileControl::openIncremental(const char * fileName,
		INTEGER fileBuffSize, INTEGER outputSize, INTEGER loadSize)
  TODO: validate the success of the mallocs
 *************************************************************************** */
/**
 * The <i>openIncremental</i> method allocates a FileData data structure,
 * populates as follows:
 *		set the fileSize using <i>FileControl::validateFile</i>
 *		set the fileHandle using  <i>FileControl::openFile</i>
 *		set the buffer to the product of fileBuffSize (the number of blocks in the
 *			 buffer) times loadSize (the size of a block)
 *		set the point in the load point (the location in to which the data is to
 *			be loaded) to zero.
 *		set the size of a read/write block to loadPoint.
 *		set the read point (the location from which the data is to be read) to zero.
 *		set the destination buffer size to the product of outputSize (the number
 *			blocks in the destination buffer) times loadBlock (the size of a
 *			load)
 *		allocate the destination buffer.
 *
 * and returns it <u>without loading data</u>.
 *
 * A block (loadSize) is the size of read item (e.g., on days worth of data).
 * The number of blocks (fileBuffSize) is the number of blocks in the buffer, so the
 * the fileBuffSize recorded in the data structure is loadSize*fileBuffSize
 *
 * \param fileName  : INTEGER -- fileHandle of the file to be reset
 * \param nBlocks   : INTEGER -- fileHandle of the file to be reset
 * \param outputSize  : INTEGER -- fileHandle of the file to be reset
 * \param blockSize : INTEGER -- fileHandle of the file to be reset
 * \return file : FileData * --- a pointer to the fileData data structure.
 * \throws re-throws trapped Exception.
 */
FileData *
FileControl::openIncremental(const char * fileName, char * buffer,
		INTEGER nBlocks, INTEGER blockSize, INTEGER nOutputBlocks)
{
	FileData           * file           = NULL;
	try {
		file                 = new FileData;//(FileData *)malloc(sizeof(FileData));
		strncpy(file->fileName, fileName, 254);
		file->fileSize       = FileControl::validateFile(file->fileName);
		file->fileHandle     = FileControl::openInputFile(file->fileName);

		file->blockSize      = blockSize;
		file->fileBuffSize   = nBlocks * blockSize;
		file->fileBuffer     = new char[file->fileBuffSize];// (char *)malloc(sizeof(char)*file->fileBuffSize);

		file->readPoint      =  0;
		file->loadPoint      =  0;

		file->nOutputBlocks = nOutputBlocks;

		if (buffer == NULL) {
			file->outputBuffer   = new char[nOutputBlocks * blockSize];//(char *)malloc(sizeof(char)*file->outputSize);
		} else {
			file->fileBuffer = buffer;
		}

	} catch (Exception & e) {
		clog << "ERROR (" <<  utility::FileControl::getFileName(__FILE__) << ":" << __LINE__ << "): could not open file " << fileName << endl;
		throw;
	}

	return file;
}
/* ****************************************************************************
 * void FileControl::incrementalLoad(FileData * file)
 *************************************************************************** */
/**
 * The <i>incrementalLoad</i> method allocates a FileData data structure,
 * populates as follows:
 *		set the fileSize using <i>FileControl::validateFile</i>
 *		set the fileHandle using  <i>FileControl::openFile</i>
 *		set the buffer to the product of nBlocks (the number of blocks in the
 *			 buffer) times blockSize (the size of a block)
 *		set the point in the load point (the location in to which the data is to
 *			be loaded) to zero.
 *		set the size of a read/write block to loadPoint.
 *		set the read point (the location from which the data is to be read) to zero.
 *		set the destination buffer size to the product of outputSize (the number
 *			blocks in the destination buffer) times loadBlock (the size of a
 *			load)
 *		allocate the destination buffer.
 *
 *
 *
 * \param fileName: INTEGER -- fileHandle of the file to be reset
 * \return None
 * \throws Exception ACCESS exception if the reset failed.
 */
void
FileControl::incrementalLoad(FileData * file)
{
	if (file == NULL) {
		clog << "ERROR (" <<  utility::FileControl::getFileName(__FILE__) << ":" << __LINE__ << "): could not access file to be loaded." << endl;
		throw Exception(__FILE__, __LINE__ , Exception::ALLOCATION1);
	}
	if (file->outputBuffer == NULL) {
		clog << "ERROR (" <<  utility::FileControl::getFileName(__FILE__) << ":" << __LINE__ << "): output buffer was not allocated." << endl;
		throw Exception(__FILE__, __LINE__ , Exception::READING1);
	}
	if (file->loadPoint >= file->fileBuffSize) {
		clog << "ERROR (" <<  utility::FileControl::getFileName(__FILE__) << ":" << __LINE__ << "): attempted to access data outside of the file buffer." << endl;
		throw Exception(__FILE__, __LINE__ , Exception::ACCESS1);
	}

	try {
		INTEGER readSize = read(file->fileHandle,
				&file->fileBuffer[file->loadPoint], file->blockSize);
		if (readSize > file->blockSize || readSize == -1) {
			clog << "ERROR (" <<  utility::FileControl::getFileName(__FILE__) << ":" << __LINE__ << "): tried to read an input block larger then the input buffer." << endl;
			throw Exception(__FILE__, __LINE__ , Exception::ACCESS2);
		}

	} catch (Exception & e) {
		clog << "ERROR (" <<  utility::FileControl::getFileName(__FILE__) << ":" << __LINE__ << "): failed to load incremental block." << endl;
		throw;
	}

	// increment the load point
	file->loadPoint += file->blockSize;
	if (file->loadPoint >= file->fileBuffSize) {
		file->loadPoint = 0;
	}
}

/* ****************************************************************************
 * void FileControl::incrementalXferBlocks(FileData * file)
 * TODO: add a return check to the memcpy method
 *************************************************************************** */
/**
 * This method will transfer outputSize blocks from the fileBuffer, starting at
 * the readPoint index, to the destination buffer (outputBuffer) in time order.
 *
 * \param file: FileData -- fileData structure containing load information
 * \return NONE.
 */
void
FileControl::incrementalXferBlocks(FileData * file)
{
	if (file == NULL) {
		clog << "ERROR (" <<  utility::FileControl::getFileName(__FILE__) << ":" << __LINE__ << "): could not access file to be loaded." << endl;
		throw Exception(__FILE__, __LINE__ , Exception::ALLOCATION1);
	}
	if (file->outputBuffer == NULL) {
		clog << "ERROR (" <<  utility::FileControl::getFileName(__FILE__) << ":" << __LINE__ << "): output buffer was not allocated." << endl;
		throw Exception(__FILE__, __LINE__ , Exception::SETUP1);
	}
	if (file->loadPoint >= file->fileBuffSize) {
		clog << "ERROR (" <<  utility::FileControl::getFileName(__FILE__) << ":" << __LINE__ << "): attempted to access data outside of the file buffer." << endl;
		throw Exception(__FILE__, __LINE__ , Exception::ACCESS1);
	}

	INTEGER              blockPt        = 0;
	INTEGER              readPoint      = file->readPoint;
	// load output buffer starting at the current read point of the file buffer
	for (INTEGER i = 0; i < file->nOutputBlocks; i++) {
		memcpy(&file->outputBuffer[blockPt],&file->fileBuffer[readPoint], file->blockSize);
		blockPt     += file->blockSize;
		readPoint   += file->blockSize;

		if (readPoint >= file->fileBuffSize) {
			readPoint = 0;
		}
	}
	/* increment read point to next read point */
	file->readPoint   += file->blockSize;
	if (file->readPoint >= file->fileBuffSize) {
		file->readPoint = 0;
	}
}

/* ****************************************************************************
 * TODO: for now, there is no check for duplicate names, but in the
 * future, this should verify the uniqueness of the file name and
 * respond accordingly when the name is not unique
 *************************************************************************** */
/* ****************************************************************************
 * INTEGER FileControl::openOutputFile(const char * fileName)
 * TODO: change the privileges to 444
 *************************************************************************** */
/**
 * The <i>openOutputFile</i> uses the Posix open call, setting the file
 * privileges to 744. If the file fails to open, the method will throw an
 * exception
 *
 * \param fileHandle : INTEGER -- fileHandle of the open file
 * \return NONE
 * \throws Exception ACCESS exception if the file fails to open.
 */
INTEGER
FileControl::openOutputFile(const char * fileName)
{
	INTEGER                  fileHandle         =
		open(fileName, O_WRONLY | O_CREAT, S_IRWXU | S_IRGRP | S_IROTH);
	if (fileHandle < 0) {
		clog << "ERROR (" <<  utility::FileControl::getFileName(__FILE__) << ":" << __LINE__ << "): could not open the output file " << fileName << endl;
		throw Exception(__FILE__, __LINE__ , Exception::ACCESS1);
	}
	return fileHandle;
}
/* ****************************************************************************
 * FileData * closeFile(FileData ** fileData)
 *************************************************************************** */
/**
 * The <i>closeFile</i> method closes file if it was open, delete the read
 * and destination buffers, if necessary, deletes the fileData and sets the
 * value to NULL.
 *
 * \param fileData : FileData **  -- fileHandle of the open file
 * \return NONE
 */
void
FileControl::closeFile(FileData * fileData)
{
	if (fileData != NULL) {
		if (fileData->fileHandle > 2) {
			close(fileData->fileHandle);
		}

		delete [] (fileData->fileBuffer);
		delete [] (fileData->outputBuffer);
		delete (fileData);
	}
}
/* ****************************************************************************
 * string getFileName(const char * name)
 *************************************************************************** */
/**
 * The <i>getFileName</i> method assumes the file name may include a path. If
 * it does not, then the return value is <i>name</i>. However, if it does
 * include a path, the method will search for the last occurrence of a '/'
 * character. All characters after the last '/' are assumed to be the file
 * name.
 *
 * \param name : const char *  -- the name from which the file name is to be
 * extracted.
 * \return rtn : string -- the name of the file
 */
string
FileControl::getFileName(const char * name)
{
	string                   rtn;
	INTEGER                  len            = strlen(name);
	bool                     done           = false;
	try {
		INTEGER              i              = len;
		if (name[len-1] == '/') {
			clog << "ERROR (" <<  utility::FileControl::getFileName(__FILE__) << ":" << __LINE__ << "): name was a directory not a regular file." << name << endl;
			throw Exception(__FILE__, __LINE__ , Exception::UNCLEAR);
		} else {
			while (i > 0 && !done) {
				if (name[i] != '/') {
					--i;
				} else {
					++i;
					done    = true;
				}
			}
		}
		// search for last occurrence of '/'

		rtn = string(&name[i]);
	} catch (Exception & e) {
		clog << "ERROR (" <<  utility::FileControl::getFileName(__FILE__) << ":" << __LINE__ << "): could not get file name." << name << endl;
		throw;
	}

	return rtn;
}

} /* END OF NAMESPACE utility */
