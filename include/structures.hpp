/* ***************************************************************************
 * Assumptions :
 *
 * RIM design philosophy
 *
 *   Configuration
 *
 *   Memory management
 *
 *     RIM requires a substantial amount of data. All of this data must be
 *     collected for and accessible by the user. Printing formatted ASCII data
 *     for the user would produce a significant performance hit. Given that the
 *     target is for TBs of output and the user will need to filter the
 *     collected data storing it in an ASCII representation will create another
 *     performance hit.
 *
 *     RIM will represent all model I/O and optimization output data in a
 *     contiguous and amorphous data structure. The design of the data structure
 *     must store data for an arbitrary number of models and a fixed number of
 *     time steps. To ensure controlled access to the data by the various models
 *     the data structure is wrapped in the SDInterface class. SDInterface is
 *     a singleton that allows requests for data at a specific time for a model
 *     or set models of a single type.
 *
 *     NOTES:
 *
 *     * This needs to be refactored. In general methods need read only
 *     access to parts of the data and editing privilege to other parts of the
 *     data. Using const return values from the SDInterface and clear
 *     definitions regarding the read-only/edit privilege for each class.
 *
 *     * The SDInterface should not implement the singleton design pattern.
 *
 *   Discrete time vs. discrete event simulation
 *
 *     RIM implements a discrete time simulation because the simulation updates
 *     happen on one of four well defined boundaries, daily UC projection,
 *     hourly UC step, periodic load following, periodic regulation. Though
 *     discrete events are not required or requested, the system is designed for
 *     a hybrid approach if the user decides it is necessary. To implement a
 *     discrete event scheduler, an event list could be included in
 *     Scheduler::simulation. On each step, the simulation method should collect
 *     the events for the current step and execute them.
 *
 *   Time Series/Agent/Optimization model division
 *
 *     RIM provide three model types: time series, agent models and
 *     optimization models.
 *
 *     Time series models
 *     The time series models read data from one or more files into a local
 *     buffer. This data is copied to an output buffer which is distributed to
 *     models that require the data. In the case of multiple files, the file
 *     buffer data is accumulated in the output buffer.
 *
 *     Agent models
 *     Agent models represent specific things in the simulated system. Each
 *     agent model takes commit data from the SDInterface operates on the data
 *     and stores it in the state and output sections of the SDInterface.
 *
 *     This structure allows us to represent the system realistically. As RIM
 *     evolves, these models will support discrete event transitions that alter
 *     the behavior of the model independently of the optimizer.
 *
 *     Optimization models
 *     RIM uses three optimizers: unit commit, load following and regulation.
 *     Each optimizer uses a cost minimization objective function that can
 *     change based on user requirements. The optimizer will also use
 *     constraints. The set of constraints may vary depending on circumstances.
 *
 *     Each optimizer will read data from the output section of the SDInterface
 *     and write the the commit section of SDInterface.
 *
 *     Additional commentary
 *
 *     The agent and optimization models do not directly interact. All
 *     communications take place through the SDInterface because all the data
 *     must be recorded there. Rather than pushing the data to each agent and
 *     optimization models, each model can pull the data from the SDInterface.
 *
 *     NOTES:
 *
 *     * Access to the time series buffer should be read-only. Only the time
 *     series will have write permission to the buffer.
 *
 *     * The commit data associated with the interface data should called input
 *     data rather than commit data
 *
 *   Optimization model design
 *     The optimization code must be flexible enough to accommodate different
 *     optimization engines. This requirement led to a complex representation
 *     for optimizations. Some of the functionality is separated into
 *
 * ---------------------------------------------------------------------------
 */

#pragma once

#if defined OS_WINDOWS
#  define int32_t int
#else
#  include <stdint.h>
#endif

#include "generic.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstring>
#include <cstdio>

class Location;
class Temperature;

/**
 *
 */
#define MAX_NAME_LEN 256        // maximum size of name character array
#define N_REPLICATES  10        // maximum number of parameters for probability distributions
#define POWER_EPS      0.0001   // maximum number of parameters for probability distributions

 /**
  * \class FileControl
  *
  * \brief A generic interface between a program and the file system.
  *
  * The <i>FileControl</i> class acts as a buffer between a program and the
  * file system. The main purpose of this class is to ensure file access is
  * done using reasonable error checking and validation before assuming a file
  * is ready to use.
  *
  */

/** Enumerates unit commit constraints
 *  RIM supports a set of constraint types for unit commit optimization. The
 *  enumeration gives each constraint type a unique value and supplies the
 *  number of constraints for this optimization.
 */
enum UC_CONSTRAINT {
	POWER_BALANCE_CONSTRAINT_UC, /** Ensures the BA power generation matches the load */
	POWER_LIMIT_CONSTRAINT_UC  , /** Keeps each generator's power between the maximum and minimum generator ratings */
	START_STOP_CONSTRAINT_UC   , /** Determines whether a generator is started or stop on a given time step */
	RAMP_CONSTRAINT_UC         , /** Prevents generators from ramping faster than their rated rates */
	RESERVE_CONSTRAINT_UC      , /** Ensures the BA is able to maintain the required reserve power */
	UP_DOWN_CONSTRAINT_UC      , /** Keeps the generator on or off for the minimum required time */
	PIECEWISE_CONSTRAINT_UC    , /** Constrains power output to a point and segment of the cost curve */
	INTRAZONELINE_CONSTRAINT_UC, /** Ensures the power transmission between two zones within a BA does not exceed the capacity of the power line */
	INTRAZONEPTDF_CONSTRAINT_UC, /** Ensures power transmission from one generator is distributed according to the PTDF */
	TRANSPORT_CONSTRAINT_UC    ,
	ZONE_POWER_BALANCE_UC      , /** Ensures the zone power generation within a BA, power transmission from the zone and power reception from other zones matches the load */
	DERATING_UC                ,
	N_UC_CONSTRAINTS             /** The number of constraints available to the unit commit optimizer */
};

/** Enumerates load following constraints
 *  RIM supports a set of constraint types for load following optimization. The
 *  enumeration gives each constraint type a unique value and supplies the
 *  number of constraints for this optimization.
 */
enum LF_CONSTRAINT {
	POWER_BALANCE_CONSTRAINT_LF, /** Ensures the BA power generation matches the load */
	RAMP_CONSTRAINT_LF,          /** Prevents generators from ramping faster than their rated rates */
	PIECEWISE_CONSTRAINT_LF,     /** Constrains power output to a point and segment of the cost curve */
	INTRAZONELINE_CONSTRAINT_LF, /** Ensures the power transmission between two zones within a BA does not exceed the capacity of the power line */
	INTRAZONEPTDF_CONSTRAINT_LF, /** Ensures power transmission from one generator is distributed according to the PTDF */
	ZONE_POWER_BALANCE_LF,       /** Ensures the zone power generation within a BA, power transmission from the zone and power reception from other zones matches the load */
	INTERZONELINE_CONSTRAINT_LF, /** NOT IMPLEMENTED */
	INTERZONEPTDF_CONSTRAINT_LF, /** NOT IMPLEMENTED */
	N_LF_CONSTRAINTS             /** The number of constraints available to the unit commit optimizer */
};

/** Enumerates regulation constraints
 *  RIM supports a set of constraint types for regulation optimization. The
 *  enumeration gives each constraint type a unique value and supplies the
 *  number of constraints for this optimization.
 */
enum REG_CONSTRAINT {
	POWER_BALANCE_CONSTRAINT_REG, /** Ensures the BA power generation matches the load */
	RAMP_CONSTRAINT_REG,          /** Prevents generators from ramping faster than their rated rates */
	PIECEWISE_CONSTRAINT_REG,     /** Constrains power output to a point and segment of the cost curve */
	INTRAZONEPTDF_CONSTRAINT_REG, /** Ensures power transmission from one generator is distributed according to the PTDF */
	ZONE_POWER_BALANCE_REG,       /** Ensures the zone power generation within a BA, power transmission from the zone and power reception from other zones matches the load */
	INTERZONELINE_CONSTRAINT_REG, /** NOT IMPLEMENTED */
	INTERZONEPTDF_CONSTRAINT_REG, /** NOT IMPLEMENTED */
	N_REG_CONSTRAINTS
};

/** Enumerates model types supported by RIM
 *  RIM supports a set of predefined models. These include administrative,
 *  physical, time series and interconnection models. The enumeration gives
 *  each model type a unique value.
 *
 */
enum INFORMATION_TYPE {
	SIMULATION       , /** Refers to virtual time information and simulation configuration data */
	BALANCE_AUTHS    , /** Refers to BA composition and aggregate information */
	ZONE             , /** Refers to Zone composition and aggregate information */
	LOCATIONS        , /** */
	DERATING         , /** Air temperature derating */
	DERATINGEF       , /** Air temperature derating efficiency*/
	CONVENTIONAL     , /** Refers to properties and use of conventional generator (coal, gas, nuclear) */
	LOAD_TIMESERIES  , /** Refers to information related to load time series */
	WIND_TIMESERIES  , /** Refers to information related to wind generation time series */
	SOLAR_TIMESERIES , /** Refers to information related to solar  generation time series */
	ZONE_INTRACONNECT, /** Refers to information related to intrazone power transfer */
	ZONE_INTERCONNECT, /** Refers to information related to interzone power transfer  */
	ZONE_CONNECTIONS , /** Alternative to zone ZONE_INTRACONNECT */
	NUM_MODELS         /** Number of allowable RIM models */
};

/** Enumerates fuel types supported by RIM
 *  RIM allows the user to define a specific set of fuel types
 *
 */
enum FUEL_TYPE {
	NUCLEAR,
	GAS_CC,
	GAS_CT,
	COAL,
	DIESEL,
	NATURALGAS,
	NUM_FUEL_TYPES
};

/** Enumerates repeat types supported by RIM
 *  RIM allows the user to repeat a given simulation to support Monte Carlo
 *  simulation and sensitivity analysis.
 *
 */
enum REPEAT_TYPE {
	NO_REPEAT,                  /** No repeat type specified */
	MONTE_CARLO,                /** NOT IMPLEMENTED */
	SENSITIVITY_ANALYSIS        /** NOT IMPLEMENTED */
};

/** Enumerates the allowable generator states
 *  RIM generators must be in one of five states: INITIAL STATE, RAMPING UP,
 *  SUPPORTING, RAMPING_DOWN and OFF. This enumeration provides a unique value
 *  for each state.
 */
enum GENERATOR_STATE {
	GENERATOR_RAMPUP=42,
	GENERATOR_SUPPORTING,
	GENERATOR_RAMPDOWN,
	GENERATOR_OFF,
	GENERATOR_INIT,
	N_GENERATOR_STATES
};

/** Enumerates the allowable storage device states
 *  RIM storage device must be in one of five three: IDLE STATE, DISCHARGE, and
 *  CHARGING. This enumeration provides a unique value for each state.
 */
enum STORAGE_STATE {
	IDLE_STORAGE,
	DISCHARGING_STORAGE,
	CHARGING_STORAGE,
	N_STORAGE_STATES
};

/** Enumerates the set of storage devices
 *  NOT IMPLEMENTED.
 */
enum STORAGE_DEVICE {
	BATTERY,
	FLYWHEEL,
	CAES,                       /** Compressed Air Energy Storage */
	SUPER_CAPACITOR,
	PHES,                       /** Pumped Hydro Energy Storage */
	SMES,                       /** Superconducting Magnetic Energy Storage  */
	N_STORAGE_TYPES
};

/** Names the upper bound and lower bound indices
 */
enum LIMIT {
	LOWER_BOUND,
	UPPER_BOUND
};


/** Names direction of a rate change
 */
enum RATE  {
	DOWN,
	UP
};

enum STATE  {
	OFF_STATE,
	ON_STATE
};

enum LINE_COEFF  {
	POWER,
	COST
};

enum EMISSIONS_TYPE {
	SOX,
	NOX,
	CO2
};

enum SW_STATE
{
	SW_OFF_STATE,
	SW_ON_STATE,
	NO_SWITCH
};

enum TIMESERIES_TYPE {
	LOAD_TS,
	SOLAR_TS,
	WIND_TS,
	INTERCONNECT_TS,
	FUEL_TS,
	N_TIMESERIES_TYPES
};

/** \struct RampControl
 * \brief defines the changes in power output on each unit commit step.
 * RampControl defines how the generator should change on each unit commit
 * step.
 *
 */
struct RampControl
{
	FLOAT                  targetPower;  /** the power level determined by the optimizer for the current time step */
	GENERATOR_STATE        state;        /** the state of the system from the previous time step unless a state change happens on this step */
	INTEGER                timeInState;  /** the time the generator has been in the current state. */
	SW_STATE               genSwitch;    /** the time a change in generator state occurs determined by the optimizer for the current step */
};
// ----------------------------------------------------------------------

class SimulationInfo // TODO this class is entirely too large
{
public:
	SimulationInfo(void);                  // initialize values
	char                   scenarioName[MAX_NAME_LEN]; // INPUT:
	// Simulation time information
	Time                   start;          // INPUT: start date in MM/DD/YY format, the assumed start time is 00:00
	Time                   stop;           // INPUT: stop date in MM/DD/YY format, the assumed end time is 23:59
	INTEGER                baseTimeStep;   // INPUT: increment size of the simulation time loop in minutes
	// Simulation/Database interface information
	INTEGER                nDumpSteps;     // INPUT: number of time steps between data dumps
	// Optimization step information (all time in terms of base time step)
	INTEGER                daStep;         // INPUT: time in day at which an optimization is recomputed (default: 24 hrs)
	INTEGER                ucTrigger;      // INPUT: time in day at which an optimization is recomputed (default: 0 min)
	INTEGER                ucTimeStep;     // INPUT: time for a single UC step (default: 60 min)
	INTEGER                ucLength;       // INPUT: number of UC steps in UC forecast (default: 72 hrs)
	INTEGER                lfTimeStep;     // INPUT: time for a single LF step (default: 10 min)
	INTEGER                lfLength;       // INPUT: number of minutes in a LF forecast (default: 120 min)
	INTEGER                regTimeStep;    // INPUT: time for a single Reg step (default: 1 min)
	INTEGER                hydroTimeStep;  // INPUT: time for a single Hydro step (default: UNK)
	INTEGER                forecastDays;   // INPUT: number of days requested of forecast buffer. The value must be >= ucLength + 1 (default: 4 days)
	REPEAT_TYPE            repeatType;     // INPUT: number of forecasted days for day ahead
	INTEGER                nRepeats;       // INPUT: number of simulation repetitions for SA/MC
	// Derived time steps, used to normalize step information
	/*
	 * ASSUMPTIONS:
	 * 	UC steps are divisible by LF and Regulation steps
	 * 	LF steps are divisible by Regulation steps
	 * 	Day and hour steps are divisible by UC steps (=> also divisible by LF and Reg steps)
	 */
	INTEGER                  nInterfaceSteps;      // Derived:
	INTEGER                  nLoadRampSteps;       // Derived:
	INTEGER                  nBaseStepsInProjection;
	INTEGER                  nBaseStepsInUCStep;
	INTEGER                  nBaseStepsInLFStep;
	INTEGER                  nBaseStepsInRegStep;   // Derived:
	INTEGER                  nUCTimeStepsInDay;    // Derived:
	INTEGER                  nUCTimeStepsInHour;   // Derived:
	INTEGER                  nLFStepsInDay;    // Derived:
	INTEGER                  nLFStepsInHour;   // Derived:
	INTEGER                  nLFStepsInUCStep;
	INTEGER                  nRegStepsInDay;   // Derived:
	INTEGER                  nRegStepsInHour;  // Derived:
	INTEGER                  nRegStepsInUCStep;
	INTEGER                  nRegStepsInLFStep;
	INTEGER                  duration;       // simulation duration in minutes
	INTEGER                  dumpSize;       // size of the data dump nDumpSteps * sizeof(timeStep)
	// the size is the total size of a forecast buffer, i.e., the length times the forecast days
	// this must be longer than the ucLength
	INTEGER                  circularBuffer;       // size of the forecast buffer
	INTEGER                  nSimulations;
	size_t                   nBalancingAuths;
	INTEGER                  nSchedulers;          // if parallel run, nSchedulers > 1
	size_t                   nZones;
	size_t                   nConventionalGenerators;
	size_t                   nStorageGenerators;
	size_t                   nHydroGenerators;
	size_t                   nLines;
	size_t                   nDR;
	size_t                   nReplicates;
	size_t                   nCanCommit;
	size_t                   nCanRegulate;
	size_t                   nLoads;
	size_t                   nWindFarms;
	size_t                   nSolarFarms;
	size_t                   nInterchanges;
	size_t                   nGenerators;
	LOGICAL                  implementsLoadFollowing;
	LOGICAL                  implementsRegulation;
	FLOAT                    epGap;
};

struct TimeData {
	INTEGER                  day;
	INTEGER                  hour;
	INTEGER                  minute;

	INTEGER                  regStepInLfStep;
	INTEGER                  lfStepInHour;

	INTEGER                  sdiIndex;

	LOGICAL                  ucUpdate;
	LOGICAL                  lfUpdate;
	LOGICAL                  regUpdate;
};

struct BalanceAuthParams
{
	char                     name[MAX_NAME_LEN];
	FLOAT                    cps1;
	FLOAT                    cps2;
	FLOAT                    bias;
	FLOAT                    agcLower;
	FLOAT                    agcUpper;
	FLOAT                    reserveFactor;

	size_t                   nZones;

	INTEGER                  nGenerators;
	INTEGER                  nConventional;
	INTEGER                  nStorage;
	INTEGER                  nHydro;
	// initialization parameters
	FLOAT                    currentPower;
	FLOAT                    currentLoad;
	FLOAT                    outputPower;
	FLOAT                    outputLoad;
};

struct BalanceAuthState
{
	FLOAT                    currentPower;
	FLOAT                    currentLoad;
};

struct BalanceAuthOutput
{
	FLOAT                    outputPower;
	FLOAT                    outputLoad;
};

struct BalanceAuthCommit
{
};

struct BalanceAuthData
{
	BalanceAuthState         state;
	BalanceAuthOutput        output;
	BalanceAuthCommit        commit;
};

struct ZoneParams
{
	char                     name[MAX_NAME_LEN];
	FLOAT                    noxCost;
	FLOAT                    soxCost;
	FLOAT                    co2Cost;
	INTEGER                  balancingAuthIndex;  // index of BA to which zone belongs
	INTEGER                  zoneBAId;            // zone index within a BA
	INTEGER                  nGenerators;
	size_t                   nConventional;
	INTEGER                  nLoads;
	INTEGER                  nWindFarms;
	INTEGER                  nSolarFarms;
	char                     interchangeFile[MAX_NAME_LEN];
};

class AirTemperatureDeratingTable {
	public:
		AirTemperatureDeratingTable(std::vector<std::pair<double, double> > fahrenheitVsFactor);
		double deratingFactorFor(const ::Temperature & temperature);
	private:
		std::vector<std::pair<double, double> > fahrenheitVsFactor; // temperature, factor
};

class AirTemperatureefficientTable{
	public:
		AirTemperatureefficientTable(std::vector<std::pair<double, double> > fahrenh);
		double deratingFactorForef(const ::Temperature & temperature);
	private:
		std::vector<std::pair<double, double> > fahrenh; // temperature, factor
};


// NOTE: any changes to this structure have to be reflected in ModelConstruct::constructModels()
// ModelInterpreter::getGeneratorData()
struct GeneratorParams
{
	GeneratorParams();
	char                          name[MAX_NAME_LEN] ; // Generator name
	PiecewiseLinear               heatRateCurve      ; // This is computed from fuel time series and heat rate time series
	PiecewiseLinear               costCurve          ; // defines the output MW/input BTU
	FLOAT                         genPowerLimits[2]  ; // pMin, pMax
	FLOAT                         genRampRates[2]    ; // rampDownRate, rampUpRate
	FLOAT                         fuelCost           ; // TODO: this should be a time series
	INTEGER                       balancingAuth      ; // Area in which the generator resides
	INTEGER                       zone               ; // Zone in which the generator resides
	bool                          canCommit          ; //
	bool                          canLoadFollow      ; //
	bool                          canRegulate        ; //
	bool                          muston	         ; //
	FLOAT                         noxRate            ; // Rate of NOx emission
	FLOAT                         soxRate            ; // Rate of SOx emission
	FLOAT                         co2Rate            ; // Rate of CO2 emission
	FUEL_TYPE                     fuelType           ; //
	double                        operationsAndMaintenanceVariableCost; // $/MWh
	Location                    * location           ; // may be null if there is no associated location.  Can it be replaced with a boost::optional<Location&> ?
	AirTemperatureDeratingTable * airTemperatureDeratingTable; // likewise, may be null, and maybe replaceable by optional reference?
	AirTemperatureefficientTable * airTemperatureefficientTable; // likewise, may be null,
};

struct GeneratorData
{
	FLOAT                    currentPower; // Current power level of generator
	INTEGER                  timeInState ;
	GENERATOR_STATE          status      ;
	GeneratorData() :
		currentPower(0),
		timeInState(0),
		status(GENERATOR_OFF)
	{ }
};

struct ConventionalParams
{
	GeneratorParams          genParams           ;
	FLOAT                    genTransitionCost[2]; // start up, shut down states
	INTEGER                  minTimeInState[2]   ;    // up time, down time
	GENERATOR_STATE          status              ;
	ConventionalParams();
	double maxPowerAt(boost::posix_time::ptime);
	double deratf(boost::posix_time::ptime);
	double efficien(boost::posix_time::ptime);
};

struct ConventionalData
{
	GeneratorData              generatorData;
};

struct LoadParams
{
	char                     name[MAX_NAME_LEN];
	char                     fileName[MAX_NAME_LEN];
	INTEGER                  balancingAuth;
	INTEGER                  zone;

	// initialization parameters
	FLOAT                    currentLoad;
	FLOAT                    outputLoad;
};

struct WindParams
{
	char                     name[MAX_NAME_LEN];
	char                     fileName[MAX_NAME_LEN];
	INTEGER                  balancingAuth;
	INTEGER                  zone;
	// initialization parameters
	FLOAT                    currentWindPower;
	FLOAT                    outputWindPower;
};

struct SolarParams
{
	char                     name[MAX_NAME_LEN];
	char                     fileName[MAX_NAME_LEN];
	INTEGER                  balancingAuth;
	INTEGER                  zone;
	// initialization parameters
	FLOAT                    currentSolarPower;
	FLOAT                    outputSolarPower;
};

FUEL_TYPE setFuelType(std::string & fuel);
std::string getFuelType(FUEL_TYPE type);

INFORMATION_TYPE setModelType(std::string & model);
std::string getModelType(INFORMATION_TYPE type);

std::string getGeneratorStatus(GENERATOR_STATE status);
std::string getSwitchStatus(SW_STATE status);
std::string getBooleanValue(bool value);

void printPWCurve(PiecewiseLinear & cost, std::ostream & out);
INFORMATION_TYPE setModelType(std::string & model);
std::string getModelType(INFORMATION_TYPE type);

FUEL_TYPE setFuelType(const std::string & fuel);
std::string getFuelType(FUEL_TYPE type);

std::string getRepeatType(REPEAT_TYPE type);
REPEAT_TYPE getRepeatType(std::string & name);

