/* ***************************************************************************
 * Author      : Kevin Glass
 * Date        : Jun 22, 2010
 * File        : StorageParser.hpp
 * Project     : rim
 *
 * Testing:
 *      $(RIM)/test/construction/InterpreterTest
 *
 *
 * STATUS:
 *              naming convention conformity
 *              doxygen comments                                        done
 *              private comments
 *              comment block conformity
 *              testing information
 *              ensure hidden methods
 *
 ************************************************************************** */
#ifndef STORAGEPARSER_HPP_
#define STORAGEPARSER_HPP_

#include "structures.hpp"
#include "ModelParser.hpp"

namespace interpreter {
class StorageParser : public ModelParser
{
public:
	StorageParser(const char * fileName);
	virtual ~StorageParser(){};

	virtual void setModels(char * interface, SimulationInfo * info);

#if defined RIM_DEBUG
	virtual void printParameters(ostream * out, char * parameters);
	virtual void printInterface(ostream * out, char * interface);
#endif
protected:
	virtual void setDerivedData(char * parameters, char * interface, SimulationInfo * info);
	virtual LOGICAL validateInput(char * parameters, char * interface, SimulationInfo * info);
private:
	INTEGER                  nGenerators;
};

} /* END OF NAMESPACE interpreter */

#endif /* STORAGEPARSER_HPP_ */
