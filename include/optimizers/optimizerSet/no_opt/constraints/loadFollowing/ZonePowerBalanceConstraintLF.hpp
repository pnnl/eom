/* ***************************************************************************
 * Author      : Kevin Glass
 * Date        : Jan 11, 2011
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

#ifndef ZPBLF_HPP_
#define ZPBLF_HPP_

#include "structures.hpp"
#include "optimization_structs.hpp"
#include "optimizers/Constraint.hpp"

namespace model {

class ZonePowerBalanceConstraintLF : public Constraint
{
public:
	ZonePowerBalanceConstraintLF(LFData & data);
	virtual ~ZonePowerBalanceConstraintLF();

	virtual void load()                          = 0;
	virtual void reload()                        = 0;
	virtual void printErrorState()               = 0;
protected:
private:
	LFData              & data;

	ZonePowerBalanceConstraintLF(ZonePowerBalanceConstraintLF & gssc) : data(gssc.data){};
	ZonePowerBalanceConstraintLF & operator = (ZonePowerBalanceConstraintLF & gssc){return *this;};
};

} /* END OF NAMESPACE model */

#endif /* ZPBLF_HPP_ */
