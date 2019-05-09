/* ***************************************************************************
 * Author      : Kevin Glass
 * Date        : May 8, 2011
 * File        : IntraZonePowerInConstraintLF.hpp
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

#ifndef INTRAZONEPOWERINCONSTRAINTLF_HPP_
#define INTRAZONEPOWERINCONSTRAINTLF_HPP_

#include "structures.hpp"
#include "optimization_structs.hpp"
#include "optimizer_structs.hpp"
#include "optimizers/Constraint.hpp"

namespace model {

class IntraZonePowerInConstraintLF : public Constraint
{
public:
	IntraZonePowerInConstraintLF(UCData & ucData, LFData & lfData, LFOptData & optData) :
		ucData(ucData),
		lfData(lfData),
		optData(optData){};

	virtual ~IntraZonePowerInConstraintLF(){};

	virtual void load();
	virtual void printErrorState();
protected:
private:
	UCData                 & ucData;
	LFData                 & lfData;
	LFOptData              & optData;

	IntraZonePowerInConstraintLF(IntraZonePowerInConstraintLF & izlc) :
		ucData(izlc.ucData),
		lfData(izlc.lfData),
		optData(izlc.optData){};
	IntraZonePowerInConstraintLF & operator = (IntraZonePowerInConstraintLF & izlc){return *this;};
};

} /* END OF NAMESPACE model */

#endif /* INTRAZONEPOWERINCONSTRAINTLF_HPP_ */
