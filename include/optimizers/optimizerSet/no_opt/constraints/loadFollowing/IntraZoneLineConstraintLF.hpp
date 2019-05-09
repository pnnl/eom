/* ***************************************************************************
 * Author      : Kevin Glass
 * Date        : Jan 11, 2011
 * File        : IntraZoneLineConstraintLF.hpp
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

#ifndef IZLCONSTRAINTLF_HPP_
#define IZLCONSTRAINTLF_HPP_

#include "structures.hpp"
#include "optimization_structs.hpp"
#include "optimizers/Constraint.hpp"

namespace model {

class IntraZoneLineConstraintLF : public Constraint
{
public:
	IntraZoneLineConstraintLF(LFData & data);
	virtual ~IntraZoneLineConstraintLF();

	virtual void load()                          = 0;
	virtual void reload()                        = 0;
	virtual void printErrorState()               = 0;
protected:
private:
	LFData              & data;

	IntraZoneLineConstraintLF(IntraZoneLineConstraintLF & gssc) : data(gssc.data){};
	IntraZoneLineConstraintLF & operator = (IntraZoneLineConstraintLF & gssc){return *this;};
};

} /* END OF NAMESPACE model */

#endif /* IZLCONSTRAINTLF_HPP_ */
