/*
 * ZoneTest.hpp
 *
 *  Created on: Jan 19, 2012
 *      Author: kglass
 */

#ifndef ZONETEST_HPP_
#define ZONETEST_HPP_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "interpreter/SimulationParser.hpp"
#include "structures.hpp"

namespace testing {

class ZoneTest : public CPPUNIT_NS :: TestFixture
{
public:
	ZoneTest(ZoneParams (expectedParams[]),
			ZoneParams (actualValues[]), int nZones){
		for (int i = 0; i < nZones; i++) {
			CPPUNIT_ASSERT_MESSAGE  (" input data: Zone scenario name", !strcmp(expectedParams[i].name, actualValues[i].name));
			CPPUNIT_ASSERT_MESSAGE  (" input data: Zone start.date.month", expectedParams[i].noxCost == actualValues[i].noxCost);
			CPPUNIT_ASSERT_MESSAGE  (" input data: Zone start.date.day", expectedParams[i].soxCost == actualValues[i].soxCost);
			CPPUNIT_ASSERT_MESSAGE  (" input data: Zone start.date.year", expectedParams[i].co2Cost == actualValues[i].co2Cost);
			CPPUNIT_ASSERT_MESSAGE  (" input data: Zone stop.date.month", expectedParams[i].balancingAuthIndex == actualValues[i].balancingAuthIndex);
			CPPUNIT_ASSERT_MESSAGE  (" input data: Zone stop.date.day", expectedParams[i].zoneBAId == actualValues[i].zoneBAId);

			CPPUNIT_ASSERT_MESSAGE  (" input data: Zone nGenerators", expectedParams[i].nGenerators == actualValues[i].nGenerators);
			CPPUNIT_ASSERT_MESSAGE  (" input data: Zone nDumpSteps", expectedParams[i].nConventional == actualValues[i].nConventional);
			CPPUNIT_ASSERT_MESSAGE  (" input data: Zone ucTrigger", expectedParams[i].nStorage == actualValues[i].nStorage);
			CPPUNIT_ASSERT_MESSAGE  (" input data: Zone ucTimeStep", expectedParams[i].nHydro == actualValues[i].nHydro);
			CPPUNIT_ASSERT_MESSAGE  (" input data: Zone lfTimeStep", expectedParams[i].nLoads == actualValues[i].nLoads);
			CPPUNIT_ASSERT_MESSAGE  (" input data: Zone lfLength", expectedParams[i].nWindFarms == actualValues[i].nWindFarms);
			CPPUNIT_ASSERT_MESSAGE  (" input data: Zone regTimeStep", expectedParams[i].nSolarFarms == actualValues[i].nSolarFarms);
			CPPUNIT_ASSERT_MESSAGE  (" input data: Zone scenario name", !strcmp(expectedParams[i].interchangeFile, actualValues[i].interchangeFile));
		}
	};
	virtual ~ZoneTest(){};
protected:
private:
	ZoneTest(){};
};

} /* namespace testing */
#endif /* ZONETEST_HPP_ */
