#ifndef LOCATIONPARSER_HPP_
#define LOCATIONPARSER_HPP_

#include <vector>
#include <string>

class Location;

namespace interpreter {

class LocationParser
{
	public:
		LocationParser(
			const std::string & fileName,
			std::vector<Location> & locations
		);
};

}

#endif
