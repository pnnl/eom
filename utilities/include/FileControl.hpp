/* ***************************************************************************
 * Details:
 * The FileControl class contains a set of methods whose overriding purpose
 * is to create and populate a FileData data structure (see generic.hpp).
 * The data structure is designed to support two types of file reads: whole
 * file, in which the entire file is dumped into a single data space, and
 * rotating partial loads, which iteratively loads a section of the file data
 * into a circular buffer.
 *
 * For circular buffers, the data used by the system will be held in a
 * contiguous buffer. For example, consider a system that requires a data of
 * size three sets from the rotating load. If the desired data crosses the
 * boundary of the circular buffer (i.e., the index is reset to zero), the
 * contiguous buffer must reorganize the data by loading separate load blocks
 * in the proper order.
 *
 * The FileData data structure contains:
 * 		a handle to an opened file (handle)
 * 		the size of the file (fileSize0
 * 		a buffer to contain the data read from the file (readBuffer)
 * 		the size of the readBuffer (buffSize)
 * 		the index pointing to the load point of the readBuffer (loadPoint)
 * 		the amount of data to be loaded on each iteration (loadSize)
 * 		a buffer to store contiguous subsets of the readBuffer (destBuffer)
 * 		the size of the destination buffer (destSize)
 ************************************************************************** */
#ifndef FILECONTROL_HPP_
#define FILECONTROL_HPP_

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "generic.hpp"

namespace utility {

/**
 * \class FileControl
 *
 * \brief A generic interface between a program and the file system.
 *
 * The <i>FileControl</i> class acts as a buffer between a program and the
 * file system. The main purpose of this class is to ensure file access is
 * done using reasonable error checking and validation before assuming a file
 * is ready to use.
 *
 */
class FileControl {
public:
	// Complete file read methods
	/**
	 * \brief Verify the existence of the file specified by <i>fileName</i>.
	 */
	static INTEGER validateFile(const char * fileName);
	/**
	 * \brief Open the file specified by <i>fileName</i>.
	 */
	static INTEGER openInputFile(const char * fileName);
	/**
	 * \brief Load the file contents into a character buffer.
	 */
	static FileData * loadFile(const char * fileName);

	/* ***********************************************************************
	 * Rotating file read methods
	 * loadSize is the number of steps in a day
	 * buffSize is the number of days to be buffered
	 * destSize is the number of days
	 ********************************************************************** */
	/**
	 * \brief Open a file for reading into a rotating buffer.
	 */
	static FileData * openIncremental(const char * fileName, char * buffer,
			INTEGER nBlocks, INTEGER blockSize, INTEGER nOutputBlocks);
	/**
	 * \brief Load a block of data from the file to the rotating buffer.
	 */
	static void incrementalLoad(FileData * file);
	/* Verify this is still in use. It should be replaced with incrementalLoad
	static char * incrementalReadBlock(FileData * file);
	*/
	/**
	 * \brief Load the destination buffer from the read buffer.
	 */
	static void  incrementalXferBlocks(FileData * file);

	// Complete load methods
	/**
	 * \brief Open a file for writing.
	 */
	static INTEGER openOutputFile(const char * fileName);

	// Rotating file load methods : NONE

	// Other file methods
	/**
	 * \brief Close a file and delete the associated data.
	 */
	static void closeFile(FileData * fileData);

	// File/Path extraction methods
	// TODO: these methods make no assumptions regarding '//' type characters
	// there is also no rule regarding the inclusion or exclusion of '/' at
	// the end of a path name
	/**
	 * \brief Extract a file name from a path.
	 */
	static std::string getFileName(const char * name);
private:
	// The automatically generated methods are hidden in private
	FileControl(){};
	FileControl(FileControl & cntl){};
	FileControl & operator = (FileControl & cntl){return *this;};
	virtual ~FileControl(){};
};

} /* END OF NAMESPACE utility */

#endif /* FILECONTROL_HPP_ */
