/* ***************************************************************************
 * Author      : Kevin Glass
 * Date        : May 8, 2011
 * File        : ZonePowerBalanceConstraintLF.hpp
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

#ifndef ZONEPOWERBALANCECONSTRAINTLF_HPP_
#define ZONEPOWERBALANCECONSTRAINTLF_HPP_

#include "structures.hpp"
#include "optimization_structs.hpp"
#include "optimizer_structs.hpp"
#include "optimizers/Constraint.hpp"

namespace model {

class ZonePowerBalanceConstraintLF : public Constraint
{
public:
	ZonePowerBalanceConstraintLF(UCData & ucData, LFData & lfData, LFOptData & optData) :
		ucData(ucData),
		lfData(lfData),
		optData(optData){};
	virtual ~ZonePowerBalanceConstraintLF(){};

	virtual void load();
	virtual void printErrorState();
protected:
private:
	UCData                 & ucData;
	LFData                 & lfData;
	LFOptData              & optData;

	ZonePowerBalanceConstraintLF(ZonePowerBalanceConstraintLF & izlc) :
		ucData(izlc.ucData),
		lfData(izlc.lfData),
		optData(izlc.optData){};
	ZonePowerBalanceConstraintLF & operator = (ZonePowerBalanceConstraintLF & izlc){return *this;};
};

} /* END OF NAMESPACE model */

#endif /* ZONEPOWERBALANCECONSTRAINTLF_HPP_ */
