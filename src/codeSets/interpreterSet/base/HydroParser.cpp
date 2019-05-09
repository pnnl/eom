/* ***************************************************************************
 * Author      : Kevin Glass
 * Date        : Jun 22, 2010
 * File        : HydroParser.cpp
 * Project     : rim
 *
 * Testing:
 *      TBD
 *
 * Details:
 *
 *
 * STATUS:
 * not done     naming convention conformity
 * not done     doxygen comments
 * not done     private comments
 * not done     comment block conformity
 * not done     testing information
 * not done     ensure hidden methods
 *
 ************************************************************************** */
#include "HydroParser.hpp"
#include "exceptions/Exception.hpp"

namespace interpreter {

HydroParser::HydroParser(const char * fileName) :
	ModelParser(fileName),
	nGenerators(-1)
{
	// account for wind.csv header (1 line);
	nLines             -= 1;
};


void
HydroParser::setModels(char * data, SimulationInfo & info)
{
}

} /* END OF NAMESPACE interpreter */
