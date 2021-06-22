/*
 * ConfigTestRIMParser.hpp
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
#ifndef CONFIGTESTRIMPARSER_HPP_
#define CONFIGTESTRIMPARSER_HPP_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "administrative/BalancingAuthority.hpp"
#include "RIMParser.hpp"
#include "parserTests/TestSimParser.hpp"
#include "parserTests/TestBAParser.hpp"
#include "parserTests/TestSimParser.hpp"
#include "parserTests/TestSimParser.hpp"
#include "parserTests/TestSimParser.hpp"
#include "parserTests/TestSimParser.hpp"

namespace testing {

class ConfigTestRIMParser : public CPPUNIT_NS :: TestFixture
{
	CPPUNIT_TEST_SUITE (ConfigTestRIMParser);
	CPPUNIT_TEST (baZoneSpaceTests);
	CPPUNIT_TEST (zoneGeneratorSpaceTests);
	CPPUNIT_TEST (baGeneratorSpaceTests);
	CPPUNIT_TEST (zoneLoadSpaceTests);
	CPPUNIT_TEST (zoneWindSpaceTests);
	CPPUNIT_TEST (zoneSolarSpaceTests);
	CPPUNIT_TEST (zoneCommitSpaceTests);
	CPPUNIT_TEST (zoneRegulateTests);
	CPPUNIT_TEST_SUITE_END ();

public:
	ConfigTestRIMParser() : rimParser(NULL), info(NULL), offset(NULL), parameters(NULL) {};
	virtual ~ConfigTestRIMParser(){};
	void setUp(void);
	void tearDown(void);
protected:
	void baZoneSpaceTests(void);
	void zoneGeneratorSpaceTests(void);
	void baGeneratorSpaceTests(void);
	void zoneLoadSpaceTests(void);
	void zoneWindSpaceTests(void);
	void zoneSolarSpaceTests(void);
	void zoneCommitSpaceTests(void);
	void zoneRegulateTests(void);

private:
	interpreter::RIMParser       * rimParser;
	char                         * parameters;
	int                          * offset;
	SimulationInfo               * info;
};

} /* namespace testing */
#endif /* CONFIGTESTRIMPARSER_HPP_ */
