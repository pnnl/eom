#ifndef HYDROPARSER_HPP_
#define HYDROPARSER_HPP_

#include "structures.hpp"
#include "ModelParser.hpp"

namespace interpreter {

class HydroParser : public ModelParser
{
public:
	HydroParser(const char * fileName);
	virtual ~HydroParser(){};

	virtual void setModels(char * interface, SimulationInfo & info);
private:
	INTEGER                  nGenerators;
};

} /* END OF NAMESPACE interpreter */

#endif /* HYDROPARSER_HPP_ */
