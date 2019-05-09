/* ***************************************************************************
 * Author      : Kevin Glass
 * Date        : Jan 11, 2011
 * File        : IntraZonePowerInConstraintUC.hpp
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

#ifndef IZPCONSTRAINTUC_HPP_
#define IZPCONSTRAINTUC_HPP_

#include "structures.hpp"
#include "optimization_structs.hpp"
#include "optimizers/Constraint.hpp"

namespace model {

class IntraZonePowerInConstraintUC : public Constraint
{
public:
	IntraZonePowerInConstraintUC(UCData & data);
	virtual ~IntraZonePowerInConstraintUC();
	virtual void load() {};
protected:
private:
	UCData              & data;

	IntraZonePowerInConstraintUC(IntraZonePowerInConstraintUC & gssc) : data(gssc.data){};
	IntraZonePowerInConstraintUC & operator = (IntraZonePowerInConstraintUC & gssc){return *this;};
};

} /* END OF NAMESPACE model */

#endif /* IZPCONSTRAINTUC_HPP_ */
