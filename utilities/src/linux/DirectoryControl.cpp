/*
 * DirectoryControl.cpp
 *
 *  Created on: Mar 11, 2011
 *      Author: kevin
 */

#include "DirectoryControl.hpp"
#include "exceptions/FileException.hpp"

namespace utility {

DirectoryControl::DirectoryControl(const char * dirName) :
	dirent(NULL),
	name(dirName)
{
	try {
		DirectoryControl::verifyDirectory(dirName);

		dirent                   = opendir(dirName);
		if (dirent == NULL) {
			stringstream         msg;
			msg  << "ERROR: could not open directory " << dirName << endl;
			throw FileException(msg, Exception::ACCESS);
		}
	} catch (FileException & e) {
		stringstream         msg;
		msg  << "could not construct controller for directory " << dirName << endl;
		e.append(msg, __FILE__, __LINE__);
		throw;
	}
}

DirectoryControl::~DirectoryControl()
{
	closedir(dirent);
}

bool
DirectoryControl::verifyDirectory(const char * name)
{
	struct stat              status;
	INTEGER                  error          = -1;

	error = stat(name, &status);
	if (error < 0) {
		stringstream msg("ERROR: could not access ");
		msg << name;
		throw FileException(msg, Exception::ACCESS);
	}

	return S_ISDIR(status.st_mode);
}

} /* END OF NAMESPACE utility */
