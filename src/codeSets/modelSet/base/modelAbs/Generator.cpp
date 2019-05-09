/* ***************************************************************************
 * Author      : Kevin Glass
 * Date        : Feb 11, 2010
 * File        : Generator.cpp
 * Project     : RIM
 *
 *
 * Contents    :
 *
 * Assumptions :
 *
 * ---------------------------------------------------------------------------
 */

#include "modelAbs/Generator.hpp"
#include "simulation/Clock.hpp"

namespace model {

size_t Generator::GeneratorID = 0;

Generator::Generator() :
	generatorID(GeneratorID++),
	implementsLoadFollowing(true),
	implementsRegulation(true)
{
}

Generator::~Generator()
{
}

} /* END OF NAMESPACE model */
