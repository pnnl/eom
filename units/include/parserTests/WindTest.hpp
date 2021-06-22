/*
 * WindTest.hpp
 *
 *  Created on: Jan 19, 2012
 *      Author: kglass
 */

#ifndef WINDTEST_HPP_
#define WINDTEST_HPP_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

class WindTest
{
public:
	WindTest(WindParams (expectedParams[]),
			WindParams (actualValues[]), int nWinds)
	{
		for (int i = 0; i < nWinds; i++) {
			CPPUNIT_ASSERT_MESSAGE  (" input data: WIND name", !strcmp(expectedParams[i].name, actualValues[i].name));
			CPPUNIT_ASSERT_MESSAGE  (" input data: WIND fileName", !strcmp(expectedParams[i].fileName, actualValues[i].fileName));
			CPPUNIT_ASSERT_MESSAGE  (" input data: WIND balancingAuth", expectedParams[i].balancingAuth == actualValues[i].balancingAuth);
			CPPUNIT_ASSERT_MESSAGE  (" input data: WIND zone", expectedParams[i].zone == actualValues[i].zone);
			CPPUNIT_ASSERT_MESSAGE  (" input data: WIND currentWindPower", expectedParams[i].currentWindPower == actualValues[i].currentWindPower);
			CPPUNIT_ASSERT_MESSAGE  (" input data: WIND outputWindPower", expectedParams[i].outputWindPower == actualValues[i].outputWindPower);
		}
	};

protected:
private:
};

#endif /* WINDTEST_HPP_ */
