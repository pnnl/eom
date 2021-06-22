/*
 * BasicBasicTestRIMParser.hpp
 *
 *  Created on: Jan 19, 2012
 *      Author: kglass
 */
/*

setupRIMParsing          // allocates and initializes RIM parser spaces
loadSourceFiles          // sets up srcFileName array
	configureForParsing  // collects structure data
collectModelInformation  // sets up parameter and interface offset information
constructParameterSpace
constructInterfaceSpace
resolveDependenies       // this constitutes a separate test
dumpMetadata             //

NOTES =========================================================================
setupRIMParsing --
MUST initialize the all data space as the initialization values are assumed
	throughout the RIMParser

loadSourceFiles --
MUST have the correct file names for each parser type
MUST have correct data structure sizes for each requested data type

configureForParsing --
MUST create accurate model counts, parameter and interface offsets

constructParameterSpace --
MUST set model population information for the SimulationInfo part of the
	parameters space

constructInterfaceSpace --
MUST set the time step offsets correctly
MUST set the data dump size correctly





 */
#ifndef BASICTESTRIMPARSER_HPP_
#define BASICTESTRIMPARSER_HPP_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "RIMParser.hpp"

namespace testing {

class BasicTestRIMParser : public CPPUNIT_NS :: TestFixture
{
	CPPUNIT_TEST_SUITE (BasicTestRIMParser);
	CPPUNIT_TEST (rimLoadSourceFileTests);
	CPPUNIT_TEST (rimCollectModelInformationTests);
	CPPUNIT_TEST (rimConstructParameterSpaceTests);
	CPPUNIT_TEST (rimConstructInterfaceSpaceTests);
	CPPUNIT_TEST_SUITE_END ();
public:
	BasicTestRIMParser() : rimParser(NULL), info(NULL) {};
	virtual ~BasicTestRIMParser() : rimParser(NULL), info(NULL) {};
	void setUp(void);
	void tearDown(void);
protected:
	// loadSourceFiles
	void rimLoadSourceFileTests(void);
	// configureForParsing
	void rimCollectModelInformationTests(void);
	// constructParameterSpace
	void rimConstructParameterSpaceTests(void);
	// constructInterfaceSpace
	void rimConstructInterfaceSpaceTests(void);

private:
	interpreter::RIMParser       * rimParser;
	SimulationInfo         * info;
};

} /* namespace testing */
#endif /* BASICTESTRIMPARSER_HPP_ */
