/* ***************************************************************************
 * Author      : Kevin Glass
 * Date        : Dec 16, 2010
 * File        : optimization-structs.hpp :  NO OPTIMIZATION
 * Project     : RIM
 *
 * PURPOSE:
 *
 *
 * STATUS:
 *     naming convention conformity
 *     doxygen comments
 *     private comments
 *     comment block conformity
 *     testing information
 *     ensure hidden methods
 *
 * TESTING:
 *
 * ---------------------------------------------------------------------------
 */

#ifndef NO_OPT_2D_OPTIMIZER_HPP_
#define NO_OPT_2D_OPTIMIZER_HPP_


#include "structures.hpp"

struct Control
{
	INTEGER                  test;
};

// ###########################################################################
/**
 * The UC, LF and Reg 2D 2D structures allow constraints and objective
 * functions to share the same 2D spaces
 */
// ###########################################################################
struct UCOptData
{
	Control             control;

	// ------------------------------------------------------------------------
	// Optimization variables
	// ------------------------------------------------------------------------
	// | noOptZCoeff3D                         |  [lfLength] x [nGenerators] x [breakPoints]

	// | noOptZCoeff2D                     |  [nGenerators] x [breakPoints]
	// | noOptGenPower2D                   |  [lfLength] x [nGenerators]
	// | noOptPowerLoadBalance2D           |  [lfLength] x [nGenerators]
	// | noOptOnOffError2D                 |  [lfLength] x [nGenerators]
	// | noOptStartStopConstraint2D        |  [lfLength] x [nGenerators]
	// | noOptMinDownTimeConstraint2D      |  [lfLength] x [nGenerators]
	// | noOptMinUpTimeConstraint2D        |  [lfLength] x [nGenerators]
	// | noOptGeneratorMustBeOn2D          |  [lfLength] x [nGenerators]
	// | noOptGeneratorMustBeOff2D         |  [lfLength] x [nGenerators]
	// | noOptGenIsOn2D                    |  [lfLength] x [nGenerators]
	// | noOptGenStarting2D                |  [lfLength] x [nGenerators]
	// | noOptGenStopping2D                |  [lfLength] x [nGenerators]
	// | noOptZCoeff                           |  [breakPoints]

	// | noOptGenPower                         |  [nGenerators]

	// | noOptPowerLoadBalance                 |  [nGenerators]
	// | noOptOnOffError                       |  [nGenerators]
	// | noOptStartStopConstraint              |  [nGenerators]
	// | noOptMinDownTimeConstraint            |  [nGenerators]
	// | noOptMinUpTimeConstraint              |  [nGenerators]
	// | noOptGeneratorMustBeOn                |  [nGenerators]
	// | noOptGeneratorMustBeOff               |  [nGenerators]
	// | noOptGenIsOn                          |  [nGenerators]
	// | noOptGenStarting                      |  [nGenerators]
	// | noOptGenStopping                      |  [nGenerators]

	// | noOptAvailablePower                   |  [nGenerators]
	// | noOptInitialMinUpTime                 |  [nGenerators]
	// | noOptInitialMinDownTime               |  [nGenerators]
	// | noOptReserve                          |  [nGenerators]
	// ------------------------------------------------------------------------
	//
	// 3D arrays -----------------------------------------------------
	FLOAT                  * noOptZCoeff3D;
	// 2D arrays from 3D arrays --------------------------------------
	FLOAT                 ** noOptZCoeff2D;
	// 1D arrays from 3D arrays --------------------------------------
	FLOAT                *** noOptZCoeff;

	// 2D arrays -----------------------------------------------------
	FLOAT                  * noOptGenPower2D;            // MinimizeCostUC, PowerBalanceUC
	FLOAT                  * noOptPowerLoadBalance2D;    // PowerBalanceUC
	FLOAT                  * noOptOnOffError2D;          // GenStartStop
	FLOAT                  * noOptStartStopConstraint2D; // GenStartStop

	INTEGER                * noOptMinDownTimeConstraint2D;  // MinUpDownTime
	INTEGER                * noOptMinUpTimeConstraint2D; // MinUpDownTime
	INTEGER                * noOptGeneratorMustBeOn2D;   // MinUpDownTime
	INTEGER                * noOptGeneratorMustBeOff2D;  // MinUpDownTime

	LOGICAL                * noOptGenIsOn2D;             // MinimizeCostUC, GenStartStop
	LOGICAL                * noOptGenStarting2D;         // MinimizeCostUC, GenStartStop
	LOGICAL                * noOptGenStopping2D;         // MinimizeCostUC, GenStartStop

	// 1D arrays from 2D arrays --------------------------------------
	FLOAT                 ** noOptGenPower;            // MinimizeCostUC, PowerBalanceUC
	FLOAT                 ** noOptPowerLoadBalance;    // PowerBalanceUC
	FLOAT                 ** noOptOnOffError;          // GenStartStop
	FLOAT                 ** noOptStartStopConstraint; // GenStartStop

	INTEGER               ** noOptMinDownTimeConstraint;  // MinUpDownTime
	INTEGER               ** noOptMinUpTimeConstraint; // MinUpDownTime
	INTEGER               ** noOptGeneratorMustBeOn;   // MinUpDownTime
	INTEGER               ** noOptGeneratorMustBeOff;  // MinUpDownTime

	LOGICAL               ** noOptGenIsOn;             // MinimizeCostUC, GenStartStop
	LOGICAL               ** noOptGenStarting;         // MinimizeCostUC, GenStartStop
	LOGICAL               ** noOptGenStopping;         // MinimizeCostUC, GenStartStop

	// simple arrays -----------------------------------------------------
	FLOAT                  * noOptAvailablePower;
	INTEGER                * noOptInitialMinUpTime;    // MinUpDownTime
	INTEGER                * noOptInitialMinDownTime;  // MinUpDownTime
	FLOAT                  * noOptReserve;

};
// ###########################################################################
/**
 *
 */
// ###########################################################################
struct LFOptData
{
	Control             control;

	// ------------------------------------------------------------------------
	// NO OPTIMIZATION 3D, MATICES and ARRAYS
	// ------------------------------------------------------------------------
	// | noOptZCoeff3D                 |  [lfLength] x [nGenerators] x [breakPoints]
	//
	// | noOptGenPower2D           |  [lfLength] x [nGenerators]
	// | noOptAvailablePower2D     |  [lfLength] x [nGenerators]
	// | noOptPowerLoadBalance2D   |  [lfLength] x [nGenerators]

	// | noOptZCoeff2D             |  [nGenerators] x [breakPoints]
	// | noOptGenPower                 |  [nGenerators]
	// | noOptAvailablePower           |  [nGenerators]
	// | noOptPowerLoadBalance         |  [nGenerators]

	// | noOptZCoeff                   |  [breakPoints]
	//
	// | noOptGenIsMoving2D        |  [lfLength] x [nGenerators]
	// | noOptGenIsOn2D            |  [lfLength] x [nGenerators]
	// | noOptGenStarting2D        |  [lfLength] x [nGenerators]
	// | noOptGenStopping2D        |  [lfLength] x [nGenerators]
	//
	// | noOptGenIsMoving              |  [nGenerators]
	// | noOptGenIsOn                  |  [nGenerators]
	// | noOptGenStarting              |  [nGenerators]
	// | noOptGenStopping              |  [nGenerators]
	// ------------------------------------------------------------------------

	FLOAT                  * noOptZCoeff3D;
	FLOAT                 ** noOptZCoeff2D;
	FLOAT                *** noOptZCoeff;

	FLOAT                  * noOptGenPower2D;
	FLOAT                  * noOptPowerLoadBalance2D;
	FLOAT                  * noOptAvailablePower2D;

	FLOAT                 ** noOptGenPower;
	FLOAT                 ** noOptAvailablePower;
	FLOAT                 ** noOptPowerLoadBalance;
};
// ###########################################################################
/**
 *
 */
// ###########################################################################
struct RegOptData
{
	Control             control;

	FLOAT                  * noOptZCoeff2D;
	FLOAT                 ** noOptZCoeff;

	FLOAT                  * noOptGenPower;
	FLOAT                  * noOptPowerLoadBalance;
	FLOAT                  * noOptAvailablePower;
};

#endif /* NO_OPT_2D_OPTIMIZER_HPP_ */
