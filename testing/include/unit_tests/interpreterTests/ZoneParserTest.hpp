/*
 * ZoneParserTest.hpp
 *
 *  Created on: Sep 14, 2011
 *      Author: kglass
 */

#ifndef ZONEPARSERTEST_HPP_
#define ZONEPARSERTEST_HPP_

#include "ZoneParser.hpp"
#include "../../Validation.hpp"

namespace testing {

class ZoneParserTest: public testing::Validation {
public:
	ZoneParserTest();
	virtual ~ZoneParserTest();

#if defined RIM_DEBUG
	friend class testing::ParserTest;
#endif
protected:
	virtual void             setupTests(){};

private:
	void constructZoneParserTest(const char * fileName, INTEGER nZones);
	void deconstructZoneParserTest(const char * fileName);

	void setModelsTest(char * parameters, char * interface, SimulationInfo * info);

	void loadZoneTwst(INTEGER i, char * parameters, char * interface);
	void setDerivedDataTest(char * parameters, char * interface, SimulationInfo * info);
	void getCountTest();

	string                   srcName;
};

} /* namespace testing */
#endif /* ZONEPARSERTEST_HPP_ */
