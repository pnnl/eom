#include "optimization_structs.hpp"
#include <cstddef>
#include <boost/date_time/posix_time/posix_time_types.hpp> //no i/o just types

using ::boost::posix_time::ptime;

UCData::UCData(const SimulationInfo & simulationInfo, int baId) :
	baId                   (                   baId),
	time                   (simulationInfo.ucLength),
	costCurve              (                   NULL),
	inputOutputCurve       (                   NULL),
	fuelCost               (                   NULL),
	genTurnOnCost          (                   NULL),
	genTurnOffCost         (                   NULL),
	genPowerMinLimit       (                   NULL),
	genPowerMaxLimit       (                   NULL),
	genRampUpRate          (                   NULL),
	genRampDownRate        (                   NULL),
	noxRate                (                   NULL),
	soxRate                (                   NULL),
	co2Rate                (                   NULL),
	operationsAndMaintenanceVariableCost(      NULL),
	zoneCo2Cost            (                   NULL),
	zonePTDF               (                   NULL),
	zoneMaxLinePower       (                   NULL),
	powerTransfer          (                       ),
	minUpTime              (                   NULL),
	minDownTime            (                   NULL),
	adjustedZoneLoad       (                   NULL),
	adjustedBALoad         (                   NULL),
	zoneGeneratorOffset    (                   NULL),
	zoneNumberOfGenerators (                   NULL),
	rampSchedule           (                   NULL),
	mustbeon	       (             	     0),
	nCommitGenerators      (                      0),
	nRegulationGenerators  (                      0),
	reserveFactor          (                      0),
	nConventionalGenerators(                      0),
	nStorageGenerators     (                      0),
	nHydroGenerators       (                      0),
	nGenerators            (                      0),
	nZones                 (                      0),
	ucLength               (simulationInfo.ucLength),
	rampLength             (                      0),
	convGenCurrent         (                   NULL),
	convGenPrev            (                   NULL),
	convGenNext            (                   NULL)
{
}
