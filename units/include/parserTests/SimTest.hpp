/*
 * SimTest.hpp
 *
 *  Created on: Jan 19, 2012
 *      Author: kglass
 */

#ifndef SIMTEST_HPP_
#define SIMTEST_HPP_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "interpreter/SimulationParser.hpp"
#include "structures.hpp"

namespace testing {

class SimTest : public CPPUNIT_NS :: TestFixture
{
public:
	SimTest(SimulationInfo (expectedParams[]),
		SimulationInfo (actualValues[]), int nSimulations){
		for (int i = 0; i < nSimulations; i++) {
			CPPUNIT_ASSERT_MESSAGE  (" input data: Simulation 0 scenario name", !strcmp(expectedParams[i].scenarioName, actualValues[i].scenarioName));
			CPPUNIT_ASSERT_MESSAGE  (" input data: Simulation 0 start.date.month", expectedParams[i].start.date.month == actualValues[i].start.date.month);
			CPPUNIT_ASSERT_MESSAGE  (" input data: Simulation 0 start.date.day", expectedParams[i].start.date.day == actualValues[i].start.date.day);
			CPPUNIT_ASSERT_MESSAGE  (" input data: Simulation 0 start.date.year", expectedParams[i].start.date.year == actualValues[i].start.date.year);
			CPPUNIT_ASSERT_MESSAGE  (" input data: Simulation 0 stop.date.month", expectedParams[i].stop.date.month == actualValues[i].stop.date.month);
			CPPUNIT_ASSERT_MESSAGE  (" input data: Simulation 0 stop.date.day", expectedParams[i].stop.date.day == actualValues[i].stop.date.day);
			CPPUNIT_ASSERT_MESSAGE  (" input data: Simulation 0 stop.date.year", expectedParams[i].stop.date.year == actualValues[i].stop.date.year);

			CPPUNIT_ASSERT_MESSAGE  (" input data: Simulation 0 baseTimeStep", expectedParams[i].baseTimeStep == actualValues[i].baseTimeStep);
			CPPUNIT_ASSERT_MESSAGE  (" input data: Simulation 0 nDumpSteps", expectedParams[i].nDumpSteps == actualValues[i].nDumpSteps);
			CPPUNIT_ASSERT_MESSAGE  (" input data: Simulation 0 ucTrigger", expectedParams[i].ucTrigger == actualValues[i].ucTrigger);
			CPPUNIT_ASSERT_MESSAGE  (" input data: Simulation 0 ucTimeStep", expectedParams[i].ucTimeStep == actualValues[i].ucTimeStep);
			CPPUNIT_ASSERT_MESSAGE  (" input data: Simulation 0 lfTimeStep", expectedParams[i].lfTimeStep == actualValues[i].lfTimeStep);
			CPPUNIT_ASSERT_MESSAGE  (" input data: Simulation 0 lfLength", expectedParams[i].lfLength == actualValues[i].lfLength);
			CPPUNIT_ASSERT_MESSAGE  (" input data: Simulation 0 regTimeStep", expectedParams[i].regTimeStep == actualValues[i].regTimeStep);
			CPPUNIT_ASSERT_MESSAGE  (" input data: Simulation 0 hydroTimeStep", expectedParams[i].hydroTimeStep == actualValues[i].hydroTimeStep);
			CPPUNIT_ASSERT_MESSAGE  (" input data: Simulation 0 forecastDays", expectedParams[i].forecastDays == actualValues[i].forecastDays);
			CPPUNIT_ASSERT_MESSAGE  (" input data: Simulation 0 repeatType", expectedParams[i].repeatType == actualValues[i].repeatType);
			CPPUNIT_ASSERT_MESSAGE  (" input data: Simulation 0 nRepeats", expectedParams[i].nRepeats == actualValues[i].nRepeats);
			CPPUNIT_ASSERT_MESSAGE  (" input data: Simulation 0 regTimeStep", expectedParams[i].regTimeStep == actualValues[i].regTimeStep);
			CPPUNIT_ASSERT_MESSAGE  (" input data: Simulation 0 hydroTimeStep", expectedParams[i].hydroTimeStep == actualValues[i].hydroTimeStep);

			CPPUNIT_ASSERT_MESSAGE  (" input data: Simulation 0 start.tod.hours", expectedParams[i].start.tod.hours == actualValues[i].start.tod.hours);
			CPPUNIT_ASSERT_MESSAGE  (" input data: Simulation 0 start.tod.minutes", expectedParams[i].start.tod.minutes == actualValues[i].start.tod.minutes);
			CPPUNIT_ASSERT_MESSAGE  (" input data: Simulation 0 start.tod.seconds", expectedParams[i].start.tod.seconds == actualValues[i].start.tod.seconds);
			CPPUNIT_ASSERT_MESSAGE  (" input data: Simulation 0 stop.tod.hours", expectedParams[i].stop.tod.hours == actualValues[i].stop.tod.hours);
			CPPUNIT_ASSERT_MESSAGE  (" input data: Simulation 0 stop.tod.minutes", expectedParams[i].stop.tod.minutes == actualValues[i].stop.tod.minutes);
			CPPUNIT_ASSERT_MESSAGE  (" input data: Simulation 0 stop.tod.seconds", expectedParams[i].stop.tod.seconds == actualValues[i].stop.tod.seconds);

			CPPUNIT_ASSERT_MESSAGE  (" input data: Simulation 0 duration", expectedParams[i].duration == actualValues[i].duration);
			CPPUNIT_ASSERT_MESSAGE  (" input data: Simulation 0 nSimulations", expectedParams[i].nSimulations == actualValues[i].nSimulations);
			CPPUNIT_ASSERT_MESSAGE  (" input data: Simulation 0 nBalancingAuths", expectedParams[i].nBalancingAuths == actualValues[i].nBalancingAuths);
			CPPUNIT_ASSERT_MESSAGE  (" input data: Simulation 0 nSchedulers", expectedParams[i].nSchedulers == actualValues[i].nSchedulers);
			CPPUNIT_ASSERT_MESSAGE  (" input data: Simulation 0 nZones", expectedParams[i].nZones == actualValues[i].nZones);
			CPPUNIT_ASSERT_MESSAGE  (" input data: Simulation 0 nConventionalGenerators", expectedParams[i].nConventionalGenerators == actualValues[i].nConventionalGenerators);
			CPPUNIT_ASSERT_MESSAGE  (" input data: Simulation 0 nStorageGenerators", expectedParams[i].nStorageGenerators == actualValues[i].nStorageGenerators);
			CPPUNIT_ASSERT_MESSAGE  (" input data: Simulation 0 nHydroGenerators", expectedParams[i].nHydroGenerators == actualValues[i].nHydroGenerators);
			CPPUNIT_ASSERT_MESSAGE  (" input data: Simulation 0 nLines", expectedParams[i].nLines == actualValues[i].nLines);
			CPPUNIT_ASSERT_MESSAGE  (" input data: Simulation 0 nDR", expectedParams[i].nDR == actualValues[i].nDR);
			CPPUNIT_ASSERT_MESSAGE  (" input data: Simulation 0 nReplicates", expectedParams[i].nReplicates == actualValues[i].nReplicates);

			CPPUNIT_ASSERT_MESSAGE  (" input data: Simulation 0 nLoads", expectedParams[i].nLoads == actualValues[i].nLoads);
			CPPUNIT_ASSERT_MESSAGE  (" input data: Simulation 0 nWindFarms", expectedParams[i].nWindFarms == actualValues[i].nWindFarms);
			CPPUNIT_ASSERT_MESSAGE  (" input data: Simulation 0 nSolarFarms", expectedParams[i].nSolarFarms == actualValues[i].nSolarFarms);
			CPPUNIT_ASSERT_MESSAGE  (" input data: Simulation 0 nInterchanges", expectedParams[i].nInterchanges == actualValues[i].nInterchanges);
			CPPUNIT_ASSERT_MESSAGE  (" input data: Simulation 0 nGenerators", expectedParams[i].nGenerators == actualValues[i].nGenerators);
		}
	};
	virtual ~SimTest(){};
protected:
private:
	SimTest(){};
};

} /* namespace testing */
#endif /* SIMTEST_HPP_ */
