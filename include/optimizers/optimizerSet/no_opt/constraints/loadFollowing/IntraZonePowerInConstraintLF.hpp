/* ***************************************************************************
 * Author      : Kevin Glass
 * Date        : Jan 11, 2011
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

#ifndef IZPCONSTRAINTLF_HPP_
#define IZPCONSTRAINTLF_HPP_

#include "structures.hpp"
#include "optimization_structs.hpp"
#include "optimizers/Constraint.hpp"

namespace model {

class IntraZonePowerInConstraintLF : public Constraint
{
public:
	IntraZonePowerInConstraintLF(LFData & data);
	virtual ~IntraZonePowerInConstraintLF();

	virtual void load()                          = 0;
	virtual void reload()                        = 0;
	virtual void printErrorState()               = 0;
protected:
private:
	LFData              & data;

	IntraZonePowerInConstraintLF(IntraZonePowerInConstraintLF & gssc) : data(gssc.data){};
	IntraZonePowerInConstraintLF & operator = (IntraZonePowerInConstraintLF & gssc){return *this;};
};

} /* END OF NAMESPACE model */

#endif /* IZPCONSTRAINTLF_HPP_ */
