/*
 * SolarTest.hpp
 *
 *  Created on: Jan 19, 2012
 *      Author: kglass
 */

#ifndef SOLARTEST_HPP_
#define SOLARTEST_HPP_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

class SolarTest
{
public:
	SolarTest(SolarParams (expectedParams[]),
			SolarParams (actualValues[]), int nSolars)
	{
		for (int i = 0; i < nSolars; i++) {
			CPPUNIT_ASSERT_MESSAGE  (" input data: SOLAR name", !strcmp(expectedParams[i].name, actualValues[i].name));
			CPPUNIT_ASSERT_MESSAGE  (" input data: SOLAR file name", !strcmp(expectedParams[i].fileName, actualValues[i].fileName));
			CPPUNIT_ASSERT_MESSAGE  (" input data: SOLAR balancingAuth", expectedParams[i].balancingAuth == actualValues[i].balancingAuth);
			CPPUNIT_ASSERT_MESSAGE  (" input data: SOLAR zone", expectedParams[i].zone == actualValues[i].zone);
			CPPUNIT_ASSERT_MESSAGE  (" input data: SOLAR currentSolarPower", expectedParams[i].currentSolarPower == actualValues[i].currentSolarPower);
			CPPUNIT_ASSERT_MESSAGE  (" input data: SOLAR outputSolarPower", expectedParams[i].outputSolarPower == actualValues[i].outputSolarPower);
		}
	};

protected:
private:
};

#endif /* SOLARTEST_HPP_ */
