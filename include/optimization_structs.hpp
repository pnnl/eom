#ifndef OPTIMIZATION_STRUCTS_HPP_
#define OPTIMIZATION_STRUCTS_HPP_

#include "structures.hpp"
#include <boost/date_time/posix_time/ptime.hpp>

enum UC_STATE {
	UC_INIT,
	WO_HYDRO,
	W_HYDRO,
	NO_HYDRO
};

// ###########################################################################
/**
 * The UC, LF and Reg data data structures allow constraints and objective
 * functions to share the same data spaces
 */
// ###########################################################################
class PowerTransfer {
public:
	PowerTransfer(size_t step, size_t zoneFrom, size_t zoneTo, double megawatts)
		: _step(step), _zoneFrom(zoneFrom), _zoneTo(zoneTo), _megawatts(megawatts) {}
	size_t step(void) const { return _step; }
	size_t zoneFrom(void) const { return _zoneFrom; }
	size_t zoneTo(void) const { return _zoneTo; }
	double megawatts(void) const { return _megawatts; }
private:
	size_t _step;
	size_t _zoneFrom;
	size_t _zoneTo;
	double _megawatts;
};
class UCData // UC data per BA
{
public:
	UCData(const SimulationInfo & simulationInfo, int baId);
	int                      baId                   ;
	std::vector<boost::posix_time::ptime> time      ; // time[step]
	PiecewiseLinear        * costCurve              ; // $ on y axis, power out on x axis
	PiecewiseLinear        * inputOutputCurve       ; // MMBtu on y axis, power out on x axis
	FLOAT                  * fuelCost               ; // $/MMBtu
	FLOAT                  * genTurnOnCost          ; // start up, shut down states
	FLOAT                  * genTurnOffCost         ; // start up, shut down states
	FLOAT                  * genPowerMinLimit       ; // pMin, pMax
	FLOAT                  * genPowerMaxLimit       ; // pMin, pMax
	FLOAT                  * genRampUpRate          ; // rampDownRate, rampUpRate
	FLOAT                  * genRampDownRate        ; // rampDownRate, rampUpRate
	INTEGER                * mustbeon		;
	FLOAT                  * noxRate                ; //
	FLOAT                  * soxRate                ; //
	FLOAT                  * co2Rate                ; //
	FLOAT                  * operationsAndMaintenanceVariableCost; // $/MWh
	FLOAT                  * zoneCo2Cost            ;
	FLOAT                 ** zonePTDF               ; //
	FLOAT                 ** zoneMaxLinePower       ; //
	std::vector<PowerTransfer> powerTransfer        ; // powerTransfer[zoneFrom][zoneTo]
	INTEGER                * minUpTime              ;
	INTEGER                * minDownTime            ;
	// these are computed on the fly
	FLOAT                 ** adjustedZoneLoad       ; // index by index-of-zone-in-ba, step
	FLOAT                  * adjustedBALoad         ;
	INTEGER                * zoneGeneratorOffset    ;
	INTEGER                * zoneNumberOfGenerators ;
	RampControl           ** rampSchedule           ;
	// vector of zone generator index vectors
	INTEGER                  nCommitGenerators      ;
	INTEGER                  nRegulationGenerators  ;
	// constants for the BA
	FLOAT                    reserveFactor          ;
	size_t                   nConventionalGenerators;
	size_t                   nStorageGenerators     ;
	size_t                   nHydroGenerators       ;
	size_t                   nGenerators            ;
	size_t                   nZones                 ;
	size_t                   ucLength               ; // # of steps in uc
	size_t                   rampLength             ;
	ConventionalData       * convGenCurrent         ;
	ConventionalData       * convGenPrev            ;
	ConventionalData       * convGenNext            ;
};
// ###########################################################################
/**
 *
 */
// ###########################################################################
struct LFData
{
	PiecewiseLinear        * costCurve;

	FLOAT                  * genPowerMinLimit;   // pMin, pMax
	FLOAT                  * genPowerMaxLimit;   // pMin, pMax
	FLOAT                  * genRampUpRate;      // rampDownRate, rampUpRate
	FLOAT                  * genRampDownRate;    // rampDownRate, rampUpRate

	FLOAT                  * noxRate;            //
	FLOAT                  * soxRate;            //
	FLOAT                  * co2Rate;            //

	FLOAT                 ** zonePTDF;           //
	FLOAT                 ** zoneMaxLinePower;           //

	LOGICAL                  isRampingUp;        // if D(load) >= 0, isRamping == true, else isRamping == false, this is done in updateState

	//	INTEGER                * actualTimeInState;
	//	GENERATOR_STATUS       * status;
	RampControl           ** rampSchedule;
	INTEGER                  rampLength;
	// these are computed on the fly
	FLOAT                 ** adjustedZoneLoad;      // index by index-of-zone-in-ba, step
	FLOAT                  * adjustedBALoad;
	INTEGER                * zoneGeneratorOffset;
	INTEGER                * zoneNumberOfGenerators;

	INTEGER                  nConventionalGenerators;
	INTEGER                  nStorageGenerators;
	INTEGER                  nHydroGenerators;
	INTEGER                  nGenerators;
	INTEGER                  nZones;
	INTEGER                  lfLength;
	ConventionalData       * convGenCurrent;
	ConventionalData       * convGenPrev;
	ConventionalData       * convGenNext;
};
// ###########################################################################
/**
 *
 */
// ###########################################################################
struct RegData
{
	FLOAT                    genPowerUpLimit;
	FLOAT                    genPowerDownLimit;
	FLOAT                    genRampUpRate;
	FLOAT                    genRampDownRate;

	FLOAT                    noxRate;
	FLOAT                    soxRate;
	FLOAT                    co2Rate;

	INTEGER                  nCommitGenerators;
	INTEGER                  nRegulatingGenerators;

	LOGICAL                  isRampingUp;        // if D(load) >= 0, isRamping == true, else isRamping == false, this is done in updateState
	// ------------------------------------------------------------------------
	// Regulation parameter
	FLOAT                    agc;
};

#endif /* OPTIMIZATION_STRUCTS_HPP_ */
