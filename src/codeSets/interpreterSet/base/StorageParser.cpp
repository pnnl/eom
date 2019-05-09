/* ***************************************************************************
 * Author      : Kevin Glass
 * Date        : Jun 22, 2010
 * File        : StorageParser.cpp
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
#include "StorageParser.hpp"
#include "exceptions/Exception.hpp"

namespace interpreter {

StorageParser::StorageParser(const char * fileName) :
	ModelParser(fileName)
{
	// account for storage.csv header (1 line);
	nLines             -= 1;
};


void
StorageParser::setModels(char * params, char * data, SimulationInfo * info)
{
}

void
StorageParser::setDerivedData(char * params, char * data, SimulationInfo * info)
{
};

LOGICAL
StorageParser::validateInput(char * params, char * data, SimulationInfo * info)
{
	return false;
};

#if defined RIM_DEBUG
void
StorageParser::printParameters(ostream * out, char * params)
{
}

void
StorageParser::printInterface(ostream * out, char * data)
{
}
#endif

} /* END OF NAMESPACE interpreter */
