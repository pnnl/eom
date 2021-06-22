/* ***************************************************************************
 * Author      : Kevin Glass
 * Date        : Jun 22, 2010
 * File        : GeneratorParserTest.hpp
 * Project     : rim
 *
 *
 * Contents    :
 *
 * Assumptions :
 *
 * ---------------------------------------------------------------------------
 */

#ifndef GENERATORPARSERTEST_HPP_
#define GENERATORPARSERTEST_HPP_

#include "GeneratorParser.hpp"
#include "../../Validation.hpp"

namespace testing {

class GeneratorParserTest : public Validation
{
public:
	GeneratorParserTest();
	virtual ~GeneratorParserTest();

#if defined RIM_DEBUG
	friend class testing::ParserTest;
#endif
protected:
	virtual void             setupTests(){};

private:
	void constructGeneratorParserTest(const char * fileName, INTEGER nGenerators);
	void deconstructGeneratorParserTest(const char * fileName);

	void setModelsTest(char * parameters, char * interface, SimulationInfo * info);

	void loadGeneratorTest(INTEGER i, char * parameters, char * interface);
	void setDerivedDataTest(char * parameters, char * interface, SimulationInfo * info);
	void getCountTest();

	string                   srcName;
};

} /* namespace testing */
#endif /* GENERATORPARSERTEST_HPP_ */
