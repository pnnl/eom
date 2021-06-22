/* ***************************************************************************
 * Author      : Kevin Glass
 * Date        : Mar 11, 2011
 * File        : DirectoryControl.hpp
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
 *     done     doxygen comments
 *     done     private comments
 *     done     comment block conformity
 *     done     testing information
 *     done     ensure hidden methods
 *
 ************************************************************************** */
#ifndef DIRECTORYCONTROL_HPP_
#define DIRECTORYCONTROL_HPP_

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include "generic.hpp"

namespace utility {

class DirectoryControl {
public:
	DirectoryControl(const char * dirName);
	virtual ~DirectoryControl();
	const char * getName(){return name.c_str();};
	static bool verifyDirectory(const char * name);
protected:
private:
	DIR                    * dirent;
	string                   name;

	DirectoryControl(){};
	DirectoryControl(DirectoryControl & cntl){};
	DirectoryControl & operator = (DirectoryControl & cntl){return *this;};
};

} /* END OF NAMESPACE utility */

#endif /* DIRECTORYCONTROL_HPP_ */
