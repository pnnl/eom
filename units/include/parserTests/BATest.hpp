/*
 * BATest.cpp
 *
 *  Created on: Jan 23, 2012
 *      Author: kglass
 */
#ifndef BATEST_HPP_
#define BATEST_HPP_
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

class BATest {
public:
	BATest(BalanceAuthParams (expectedParams[]),
			BalanceAuthParams (actualValues[]), int nBAs){
		for (int i = 0; i < nBAs; i++) {
			CPPUNIT_ASSERT_MESSAGE  (" input data: BA 0 name", !strcmp(expectedParams[i].name, actualValues[i].name));
			CPPUNIT_ASSERT_MESSAGE  (" input data: BA 0 cps1", expectedParams[i].cps1 == actualValues[i].cps1);
			CPPUNIT_ASSERT_MESSAGE  (" input data: BA 0 cps2", expectedParams[i].cps2 == actualValues[i].cps2);
			CPPUNIT_ASSERT_MESSAGE  (" input data: BA 0 bias", expectedParams[i].bias == actualValues[i].bias);
			CPPUNIT_ASSERT_MESSAGE  (" input data: BA 0 agcLower", expectedParams[i].agcLower == actualValues[i].agcLower);
			CPPUNIT_ASSERT_MESSAGE  (" input data: BA 0 agcUpper", expectedParams[i].agcUpper == actualValues[i].agcUpper);
			CPPUNIT_ASSERT_MESSAGE  (" input data: BA 0 nZones", expectedParams[i].nZones == actualValues[i].nZones);
			CPPUNIT_ASSERT_MESSAGE  (" input data: BA 0 currentPower", expectedParams[i].currentPower == actualValues[i].currentPower);
			CPPUNIT_ASSERT_MESSAGE  (" input data: BA 0 currentLoad", expectedParams[i].currentLoad == actualValues[i].currentLoad);
			CPPUNIT_ASSERT_MESSAGE  (" input data: BA 0 outputPower", expectedParams[i].outputPower == actualValues[i].outputPower);
			CPPUNIT_ASSERT_MESSAGE  (" input data: BA 0 outputLoad", expectedParams[i].outputLoad == actualValues[i].outputLoad);
		}
	};
	virtual ~BATest(){};
protected:
private:
	BATest(){};
};

#endif
