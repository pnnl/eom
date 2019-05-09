/*
 * UCOptConfigure.cpp : NO OPTIMIZATION
 *
 *  Created on: Nov 28, 2011
 *      Author: kglass
 */


#include "float.h"
#include "optimizer_structs.hpp"
#include "optimizers/optimization/UnitCommitOptimizer.hpp"

namespace model {
UnitCommitOptimizer::UnitCommitOptimizer(UCData & data, int ba) :
			ucData(ucData),
			smallestObjective(FLT_MAX),
			interface(NULL),
			baID(ba),
			currentTime(0),
			genStartIndex(0),
			convStartIndex(0),
			storageStartIndex(0),
			hydroStartIndex(0),
			zoneParams(NULL),
			zonePTDFData(NULL),
			zoneMaxLinePowerData(NULL),
			targetPowerData(NULL),
			targetPower(NULL),
			generatorOnData(NULL),
			generatorOn(NULL),
			switchOnData(NULL),
			switchOn(NULL),
			switchOffData(NULL),
			switchOff(NULL),
			scheduleData(NULL)
{
	interpreter::RIMParser * parser = interpreter::RIMParser::getRIMParser();
	interface              = simulator::SDInterface::getSDInterface();
	baParams               = *parser->getBalanceAuthParams(baID);
}

void
UnitCommitOptimizer::initializeModelSet()
{
}
// ############################################################################
// ## CHECK SOLUTION
// ############################################################################
void
UnitCommitOptimizer::handleSolution()
{
}

void
UnitCommitOptimizer::loadOptVariables()
{
}

void
UnitCommitOptimizer::handleNoSolution()
{
	/*
	 * This method is incomplete. It should:
	 * 		1) react to load > max possible generation by ramping all available
	 * 			generation to its maximum.
	 * 		2) react to load < min possible generation by ramping all available
	 * 			generation to its minimum
	 * This is not possible for the current optimization, so instead, it will
	 * terminate the code. In the future, the response of 'handleNoSolution'
	 * will be to optimize around the "out-of-bounds" loads.
	 *
	 * For the moment, this method will throw an exception, which will terminate
	 * the code.
	 */
	cerr << "WELL, that didn't work!!" << endl;
}

// ############################################################################
// ############################################################################
// ############################################################################
// ############################################################################
void
UnitCommitOptimizer::finalOptimizerCleanUp()
{
}

void
UnitCommitOptimizer::printOptimzationVariables(ostream & out)
{

}


} /* END OF NAMESPACE model */

