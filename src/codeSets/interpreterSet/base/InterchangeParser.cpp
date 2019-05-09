#include "InterchangeParser.hpp"

namespace interpreter {

InterchangeParser::InterchangeParser(const char * fileName) :
		ModelParser(fileName),
		nInterchanges(-1)
{
	// account for interchange.csv header (1 line);
	nLines             -= 1;
};


void
InterchangeParser::setModels(char * data, SimulationInfo * info)
{
}

#if defined RIM_DEBUG
void
InterchangeParser::printParameters(ostream * out, char * params)
{
}

void
InterchangeParser::printInterface(ostream * out, char * data)
{
}
#endif

} /* END OF NAMESPACE interpreter */
