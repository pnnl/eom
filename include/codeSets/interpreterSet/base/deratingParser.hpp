#ifndef DERATING_PARSER_HPP_
#define DERATING_PARSER_HPP_

#include <string>
#include <map>

class AirTemperatureDeratingTable;

namespace interpreter {

class AirTemperatureDeratingParser {
	public: AirTemperatureDeratingParser(std::string & filename, std::map<std::string, AirTemperatureDeratingTable> & tables);
};

} /* END OF NAMESPACE */

#endif
