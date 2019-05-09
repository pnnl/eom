/* ***************************************************************************
 * Author      : Kevin Glass
 * Date        : Jan 11, 2011
 * File        : GenStartStopConstraintUC.hpp
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

#ifndef GENSTARTSTOPCONSTRAINTUC_HPP_
#define GENSTARTSTOPCONSTRAINTUC_HPP_

#include "structures.hpp"
#include "optimization_structs.hpp"
#include "optimizers/Constraint.hpp"

namespace model {

class GenStartStopConstraintUC : public Constraint
{
public:
	GenStartStopConstraintUC(UCData & data);
	virtual ~GenStartStopConstraintUC();
	virtual void load() {};
protected:
private:
	UCData              & data;

	GenStartStopConstraintUC(GenStartStopConstraintUC & gssc) : data(gssc.data){};
	GenStartStopConstraintUC & operator = (GenStartStopConstraintUC & gssc){return *this;};
};

} /* END OF NAMESPACE model */

#endif /* GENSTARTSTOPCONSTRAINTUC_HPP_ */
