#ifndef DERATINGEF_PARSER_HPP_
#define DERATINGEF_PARSER_HPP_

#include <string>
#include <map>

class AirTemperatureefficientTable;

namespace interpreter {

class AirTemperatureDeratingefParser {
	public: AirTemperatureDeratingefParser(std::string & filename, std::map<std::string, AirTemperatureefficientTable> & tables);
};

} /* END OF NAMESPACE */

#endif
