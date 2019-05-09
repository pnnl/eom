/* ***************************************************************************
 * Author      : Kevin Glass
 * Date        : Jan 11, 2011
 * File        : IntraZoneLineConstraintUC.hpp
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

#ifndef IZLCONSTRAINTUC_HPP_
#define IZLCONSTRAINTUC_HPP_

#include "structures.hpp"
#include "optimization_structs.hpp"
#include "optimizers/Constraint.hpp"

namespace model {

class IntraZoneLineConstraintUC : public Constraint
{
public:
	IntraZoneLineConstraintUC(UCData & data);
	virtual ~IntraZoneLineConstraintUC();
	virtual void load() {};
protected:
private:
	UCData              & data;

	IntraZoneLineConstraintUC(IntraZoneLineConstraintUC & gssc) : data(gssc.data){};
	IntraZoneLineConstraintUC & operator = (IntraZoneLineConstraintUC & gssc){return *this;};
};

} /* END OF NAMESPACE model */

#endif /* IZLCONSTRAINTUC_HPP_ */
