/*
 * GeneratorTest.hpp
 *
 *  Created on: Jan 19, 2012
 *      Author: kglass
 */

#ifndef GENERATORTEST_HPP_
#define GENERATORTEST_HPP_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

class GeneratorTest
{
public:
	GeneratorTest(ConventionalParams (expectedParams[]),
			ConventionalParams (actualValues[]), int nGenerators){
		for (int i = 0; i < nGenerators; i++) {
			CPPUNIT_ASSERT_MESSAGE  (" input data: GENERATOR name", !strcmp(expectedParams[i].genParams.name, actualValues[i].genParams.name));
			CPPUNIT_ASSERT_MESSAGE  (" input data: GENERATOR heatRateCurve", !strncmp((char *)&expectedParams[i].genParams.heatRateCurve, (char *)&actualValues[i].genParams.heatRateCurve, sizeof(PiecewiseLinear)));
			CPPUNIT_ASSERT_MESSAGE  (" input data: GENERATOR costCurve", !strncmp((char *)&expectedParams[i].genParams.costCurve, (char *)&actualValues[i].genParams.costCurve, sizeof(PiecewiseLinear)));
			CPPUNIT_ASSERT_MESSAGE  (" input data: GENERATOR genPowerLimits lower", expectedParams[i].genParams.genPowerLimits[0] == actualValues[i].genParams.genPowerLimits[0]);
			CPPUNIT_ASSERT_MESSAGE  (" input data: GENERATOR genPowerLimits upper", expectedParams[i].genParams.genPowerLimits[1] == actualValues[i].genParams.genPowerLimits[1]);
			CPPUNIT_ASSERT_MESSAGE  (" input data: GENERATOR genRampRates down", expectedParams[i].genParams.genRampRates[0] == actualValues[i].genParams.genRampRates[0]);
			CPPUNIT_ASSERT_MESSAGE  (" input data: GENERATOR genRampRates up", expectedParams[i].genParams.genRampRates[1] == actualValues[i].genParams.genRampRates[1]);
			CPPUNIT_ASSERT_MESSAGE  (" input data: GENERATOR fuelCost", expectedParams[i].genParams.fuelCost == actualValues[i].genParams.fuelCost);
			CPPUNIT_ASSERT_MESSAGE  (" input data: GENERATOR balancingAuth", expectedParams[i].genParams.balancingAuth == actualValues[i].genParams.balancingAuth);
			CPPUNIT_ASSERT_MESSAGE  (" input data: GENERATOR zone", expectedParams[i].genParams.zone == actualValues[i].genParams.zone);
			CPPUNIT_ASSERT_MESSAGE  (" input data: GENERATOR canCommit", expectedParams[i].genParams.canCommit == actualValues[i].genParams.canCommit);
			CPPUNIT_ASSERT_MESSAGE  (" input data: GENERATOR canLoadFollow", expectedParams[i].genParams.canLoadFollow == actualValues[i].genParams.canLoadFollow);
			CPPUNIT_ASSERT_MESSAGE  (" input data: GENERATOR canRegulate", expectedParams[i].genParams.canRegulate == actualValues[i].genParams.canRegulate);
			CPPUNIT_ASSERT_MESSAGE  (" input data: GENERATOR noxRate", expectedParams[i].genParams.noxRate == actualValues[i].genParams.noxRate);
			CPPUNIT_ASSERT_MESSAGE  (" input data: GENERATOR soxRate", expectedParams[i].genParams.soxRate == actualValues[i].genParams.soxRate);
			CPPUNIT_ASSERT_MESSAGE  (" input data: GENERATOR co2Rate", expectedParams[i].genParams.co2Rate == actualValues[i].genParams.co2Rate);
			CPPUNIT_ASSERT_MESSAGE  (" input data: GENERATOR fuelType", expectedParams[i].genParams.fuelType == actualValues[i].genParams.fuelType);

			CPPUNIT_ASSERT_MESSAGE  (" input data: GENERATOR genTransitionCost down", expectedParams[i].genTransitionCost[0] == actualValues[i].genTransitionCost[0]);
			CPPUNIT_ASSERT_MESSAGE  (" input data: GENERATOR genTransitionCost up", expectedParams[i].genTransitionCost[1] == actualValues[i].genTransitionCost[1]);
			CPPUNIT_ASSERT_MESSAGE  (" input data: GENERATOR minTimeInState down", expectedParams[i].minTimeInState[0] == actualValues[i].minTimeInState[0]);
			CPPUNIT_ASSERT_MESSAGE  (" input data: GENERATOR minTimeInState up", expectedParams[i].minTimeInState[1] == actualValues[i].minTimeInState[1]);
			CPPUNIT_ASSERT_MESSAGE  (" input data: GENERATOR status", expectedParams[i].status == actualValues[i].status);
		}
	};

protected:
private:
};

#endif /* GENERATORTEST_HPP_ */
