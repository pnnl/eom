/* ***************************************************************************
 * Author      : Kevin Glass
 * Date        : May 8, 2012
 * File        : IntraZoneLineConstraintLF.hpp
 * Project     : RIM
 *
 * PURPOSE:
 * This implements the line constraint between two zones in a single BA. The
 * constraint is:
 *
 * 	Load[z] =(SUM{0->nZones}P[z]) + Pin[z] - Pout[z]
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

#ifndef INTRAZONELINECONSTRAINTLF_HPP_
#define INTRAZONELINECONSTRAINTLF_HPP_

#include "structures.hpp"
#include "optimization_structs.hpp"
#include "optimizer_structs.hpp"
#include "optimizers/Constraint.hpp"

namespace model {

class IntraZoneLineConstraintLF : public Constraint
{
public:
	IntraZoneLineConstraintLF(UCData & ucData, LFData & lfData, LFOptData & optData) :
		ucData(ucData),
		lfData(lfData),
		optData(optData){};
	virtual ~IntraZoneLineConstraintLF(){};

	virtual void load();
	virtual void printErrorState();
protected:
private:
	UCData                 & ucData;
	LFData                 & lfData;
	LFOptData              & optData;

	IntraZoneLineConstraintLF(IntraZoneLineConstraintLF & izlc) :
		ucData(izlc.ucData),
		lfData(izlc.lfData),
		optData(izlc.optData){};
	IntraZoneLineConstraintLF & operator = (IntraZoneLineConstraintLF & izlc){return *this;};
};

} /* END OF NAMESPACE model */

#endif /* INTRAZONELINECONSTRAINTLF_HPP_ */
