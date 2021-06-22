/*
 * DataTestRIMParser.hpp
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
#ifndef DATATESTRIMPARSER_HPP_
#define DATATESTRIMPARSER_HPP_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "model/administrative/BalancingAuthority.hpp"
#include "RIMParser.hpp"
#include "parserTests/TestSimParser.hpp"
#include "parserTests/TestBAParser.hpp"
#include "parserTests/TestSimParser.hpp"
#include "parserTests/TestSimParser.hpp"
#include "parserTests/TestSimParser.hpp"
#include "parserTests/TestSimParser.hpp"

namespace testing {

class DataTestRIMParser : public CPPUNIT_NS :: TestFixture
{
	CPPUNIT_TEST_SUITE (DataTestRIMParser);
	CPPUNIT_TEST (rimSimulationLoadTests);
	CPPUNIT_TEST (rimBALoadTests);
	CPPUNIT_TEST (rimZoneLoadTests);
	CPPUNIT_TEST (rimConventionalTests);
	CPPUNIT_TEST (rimLoadTests);
	CPPUNIT_TEST (rimWindTests);
	CPPUNIT_TEST (rimSolarTests);
	CPPUNIT_TEST_SUITE_END ();
public:
	DataTestRIMParser(){};
	virtual ~DataTestRIMParser(){};
	void setUp(void);
	void tearDown(void);
protected:
	void rimSimulationLoadTests(void);
	void rimBALoadTests(void);
	void rimZoneLoadTests(void);
	void rimConventionalTests(void);
	void rimLoadTests(void);
	void rimWindTests(void);
	void rimSolarTests(void);

private:
	interpreter::RIMParser       * rimParser;
	char                         * parameters;
	int                          * offset;
	SimulationInfo               * info;
};

} /* namespace testing */
#endif /* DATATESTRIMPARSER_HPP_ */
