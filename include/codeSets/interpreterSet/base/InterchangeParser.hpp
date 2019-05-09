#ifndef INTERCHANGEPARSER_HPP_
#define INTERCHANGEPARSER_HPP_

#include "structures.hpp"
#include "ModelParser.hpp"

namespace interpreter {
class InterchangeParser : public ModelParser
{
public:
	InterchangeParser(const char * fileName);
	virtual ~InterchangeParser(){};
	virtual void setModels(SimulationInfo * info);
protected:
	virtual INTEGER getCount(){return countLines() - 1;};
private:
	size_t nInterchanges;
};

} /* END OF NAMESPACE interpreter */

#endif /* INTERCHANGEPARSER_HPP_ */
