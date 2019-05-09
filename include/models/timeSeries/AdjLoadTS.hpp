/*
 * AdjLoadTS.hpp
 *
 *  Created on: Nov 19, 2011
 *      Author: kglass
 */

#ifndef ADJLOADTS_HPP_
#define ADJLOADTS_HPP_

#include "structures.hpp"
#include "models/RIMModel.hpp"

namespace model {
/*
 * The adjusted time series collects all of the time series for a given
 * optimization type (UC, LF or Reg). Every time series associated with the
 * optimization type is collected in the AdjLoadTS<OPT> object. These include
 * the load, solar, wind, ...).
 *
 * The adjusted time series for a BA the accumulation of the ZONE loads within
 * the BA less the accumulated negative loads (solar, wind, etc.). These values
 * are made available to each BA.
 *
 * The load and negative load information is also made available to each ZONE.
 *
 * The set of AdjLoads required by the simulation are created in ScheduleData
 */
#if defined RIM_DEBUG
namespace testing {
class AdjLoadTest;
}
#endif
class AdjLoadTS : public RIMModel{
public:
	AdjLoadTS(INTEGER zone, const SimulationInfo & simulationInfo);
	virtual ~AdjLoadTS();

	void configureRIMModel(){};

	virtual void dailyTrigger();
	virtual void ucTrigger();
	virtual void lfTrigger();
	virtual void regTrigger();

	FLOAT  * getUCAdjSeries(){return ucAdjSeries;};
	FLOAT  * getLFAdjSeries(){return lfAdjSeries;};
	FLOAT  * getRegAdjSeries(){return regAdjSeries;};

	virtual void printState(std::ostream & out);
	virtual void printParameters(std::ostream & out);

protected:
	void setupUCAdjTS();
	void setupLFAdjTS();
	void setupRegAdjTS();

	void openUCLoadFile();
	void openUCWindFile();
	void openUCSolarFile();

	void openLFLoadFile();
	void openLFWindFile();
	void openLFSolarFile();

	void openRegLoadFile();
	void openRegWindFile();
	void openRegSolarFile();

private:

	// array of pointers to FileData, these contain the desired data
	FileData               * ucLoadFile;
	FileData              ** ucWindFile;
	FileData              ** ucSolarFile;
	FLOAT                  * ucLoadData;
	FLOAT                 ** ucWindData;
	FLOAT                 ** ucSolarData;
	FLOAT                  * ucAdjSeries;
	INTEGER                  ucSeriesLength;

	FileData               * lfLoadFile;
	FileData              ** lfWindFile;
	FileData              ** lfSolarFile;
	FLOAT                  * lfLoadData;
	FLOAT                 ** lfWindData;
	FLOAT                 ** lfSolarData;
	FLOAT                  * lfAdjSeries;
	INTEGER                  lfSeriesLength;

	FileData               * regLoadFile;
	FileData              ** regWindFile;
	FileData              ** regSolarFile;
	FLOAT                  * regLoadData;
	FLOAT                 ** regWindData;
	FLOAT                 ** regSolarData;
	FLOAT                  * regAdjSeries;
	INTEGER                  regSeriesLength;

	INTEGER                  nWindFarms;
	INTEGER                  nSolarFarms;

	size_t                   circularBuffer;
	size_t                   daStep;
	size_t                   regTimeStep;
	size_t                   lfTimeStep;
	size_t                   ucTimeStep;
	size_t                   nForecastDays;
	INTEGER                  lookAheadDays;

	LOGICAL                  implementsLoadFollowing;
	LOGICAL                  implementsRegulation;


	INTEGER                  zoneId;
};
} /* namespace model */
#endif /* ADJLOADTS_HPP_ */
