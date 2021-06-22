/*
 * modelReader.hpp
 *
 *  Created on: Oct 17, 2011
 *      Author: kglass
 */

#ifndef MODELREADER_HPP_
#define MODELREADER_HPP_

#include "structures.hpp"
#include "generic.hpp"
#include "FileControl.hpp"

namespace reader {


// : public interpreter::RIMParser
class ModelReader
{
public:
	ModelReader(const char * fileName, int duration);
	virtual ~ModelReader();

	void readDirectory(const char * dirName, int startDay, int stopDay);
	void writeCSV(const char * outputName);
	void writeCSVInv(const char * outputName);
protected:
private:
	INTEGER                  dataSpaceSize;
	INTEGER                  nBalanceAuths;
	INTEGER                  nZones;
	INTEGER                  nConventionalGenerators;
	INTEGER                  nLoads;
	INTEGER                  nWindFarms;
	INTEGER                  nSolarFarms;
	INTEGER                  nMinutes;
	// this stores the entire data space
	INTEGER                * dataSpaceOffset;
	char                   * dataSpace;
	char                   * parameterSpace;

	INTEGER                * interfaceOffset;
	INTEGER                * interfaceSize;
	INTEGER                * parameterSize;
	INTEGER                * parameterOffset;
	INTEGER                * modelCount;
};

} /* end of reader namespace */

#endif /* MODELREADER_HPP_ */
