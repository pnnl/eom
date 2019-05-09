#ifndef CPLEX_DATA_HPP_
#define CPLEX_DATA_HPP_

#include "cplex_data.hpp"
#include <vector>

struct Control
{
	Control() {
		iloEnv.end(); // TODO why?
	}
    IloCplex                 iloCplex;
    IloEnv                   iloEnv;
    IloModel                 iloModel;
    IloExpr                  iloObj;
};
class UCOptData
{
public:
	~UCOptData() {}
	Control             control;

	NumVar3D                 iloSegmentPower;        // [step][gen][segment]
	NumExprMatrix            iloGenPower;            // [step][gen]
	BoolVarMatrix            iloSegmentIsOn;         // [step][gen][segment]
	NumExprMatrix            iloGenIsOn;             // [step][gen]
	NumVarMatrix             iloGenStartSupport;     // [step][gen]
	//NumVarMatrix             iloGenStopSupport;      // [step][gen]
	NumVarMatrix             iloStepsToSwitch; // approximate number of steps until a generator can be switched on or off
	NumVarMatrix             iloZonePowerTransferOut;// [step][zone]
	NumExprMatrix            iloZonePowerTransferIn; // [step][zone]
	NumVar3D                 iloZonePowerTransferInPerZone; // [step][zone_to][zone_from]

	NumExprMatrix            iloMMBtuFuelInput;
	NumExprMatrix            iloFuelCost;

	// emissions
	NumExprMatrix            iloPoundsCo2Emissions;
	NumExprMatrix            iloCo2EmissionsCost;
	IloRangeArray            rangeConstraints;

};
// ###########################################################################
/**
 *
 */
// ###########################################################################
struct LFOptData
{
	// ------------------------------------------------------------------------
	// CPLEX variables and constants
	Control             control;

	// used in MinimizeCostLF, PowerBalanceConstraintLF
	SemiContVarMatrix        iloGenPower;
	IloExprArray             iloAvailablePower;

	// used in zone constraints
	BoolVarMatrix            iloZonePowerOutNonnegativeConstraint;
	NumExprArgMatrix         iloZonePowerConstraint;
	NumExprArgMatrix         iloZoneInConstraint;
	NumVar3D                 iloZoneIntraLineConstraint;
	NumVarMatrix             iloZonePowerTransferOut;
	NumVarMatrix             iloZonePowerTransferIn;
	NumExprMatrix            iloZonePowerInPerConnection;


	// used in RampConstraintLF
	IfThenMatrix             iloRampUpConstraint;
	IfThenMatrix             iloRampDownConstraint;

};
// ###########################################################################
/**
 *
 */
// ###########################################################################
struct RegOptData
{
	// ------------------------------------------------------------------------
	// CPLEX variables and constants
	Control                  control;

	// used in MinimizeCostLF, PowerBalanceConstraintLF
	IloSemiContVarArray      iloGenPower;
	IloExpr                  iloAvailablePower;


	// used in PowerBalanceConstraintLF
	IloNumExprArg            iloPowerLoadBalance;    // PowerBalanceUC

	// used in RampConstraintLF
	IfThenArray              iloRampUpConstraint;
	IfThenArray              iloRampDownConstraint;
	NumExprArgArray          iloRampStatus;

	// used in zone constraints
	BoolVarMatrix            iloZonePowerOutNonnegativeConstraint;
	NumExprArgMatrix         iloZonePowerConstraint;
	NumExprArgMatrix         iloZoneInConstraint;
	NumVar3D                 iloZoneIntraLineConstraint;
	NumVarMatrix             iloZonePowerTransferOut;
	NumVarMatrix             iloZonePowerTransferIn;
	NumExprMatrix            iloZonePowerInPerConnection;
};

#endif /* CPLEX_DATA_HPP_ */
