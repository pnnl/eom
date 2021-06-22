/*
 * LoadTest.hpp
 *
 *  Created on: Jan 19, 2012
 *      Author: kglass
 */

#ifndef LOADTEST_HPP_
#define LOADTEST_HPP_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

class LoadTest
{
public:
	LoadTest(LoadParams (expectedParams[]),
			LoadParams (actualValues[]), int nLoads)
	{
		for (int i = 0; i < nLoads; i++) {
			CPPUNIT_ASSERT_MESSAGE  (" input data: LOAD name", !strcmp(expectedParams[i].name, actualValues[i].name));
			CPPUNIT_ASSERT_MESSAGE  (" input data: LOAD fileName", !strcmp(expectedParams[i].fileName, actualValues[i].fileName));
			CPPUNIT_ASSERT_MESSAGE  (" input data: LOAD balancingAuth", expectedParams[i].balancingAuth == actualValues[i].balancingAuth);
			CPPUNIT_ASSERT_MESSAGE  (" input data: LOAD zone", expectedParams[i].zone == actualValues[i].zone);
			CPPUNIT_ASSERT_MESSAGE  (" input data: LOAD currentLoad", expectedParams[i].currentLoad == actualValues[i].currentLoad);
			CPPUNIT_ASSERT_MESSAGE  (" input data: LOAD outputLoad", expectedParams[i].outputLoad == actualValues[i].outputLoad);
		}
	};

protected:
private:
};

#endif /* LOADTEST_HPP_ */
