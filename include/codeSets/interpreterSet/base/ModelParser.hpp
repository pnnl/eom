#ifndef MODELPARSER_HPP_
#define MODELPARSER_HPP_

/* ***************************************************************************
 * Details:
 * The ModelParser class is an extension of the Parser class and an abstraction
 * for all model parsers (generator, load, etc.) The ModelParser extends the
 * Parser abstraction to include a method for adding the model and parameters
 * spaces, a method for counting the number of lines in the model's input file
 * and a method for setting data derived from the input files.
 ************************************************************************** */

#include "Parser.hpp"

namespace interpreter {

class ModelParser: protected Parser
{
protected:
	/**
	 * \brief Open the appropriate parser.
	 * @param fileName name of the parameter file for the parsed model.
	 */
	ModelParser(const std::string & fileName) :
		Parser(fileName)
	{
		reduceCSV();
		countLines();
	};
public:
	virtual ~ModelParser(){};
private:
	ModelParser & operator = (ModelParser & right); // hide assignment operator
};

} /* END OF NAMESPACE interpreter */

#endif /* MODELPARSER_HPP_ */
