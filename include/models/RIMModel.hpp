/* ***************************************************************************
 * RIMModel.hpp
 * Author      : Kevin Glass
 * Date        : Feb 1, 2010
 * File        : RIMModel.hpp
 * Project     : RIM
 *
 * RIMModel is an Abstract Class
 *     Implemented methods
 *         RIMModel -- assigns a unique random id to the model
 *     Implemented virtual methods
 *         ~RIMModel -- does nothing
 *     Pure virtual methods
 *         configure(RIMInterpreter &) : void  --
 *              used to initialize model
 *         dailyTrigger() : void  --
 *              invokes model's response to a daily update time step
 *         ucTrigger : void  --
 *              invokes model's response to a unit commitment time step
 *         lfTrigger : void  --
 *              invokes model's response to a load following time step
 *         regTrigger : void  --
 *              invokes model's response to a regulation time step
 *         HIDDEN PURE VIRTUALS
 *         printState(ostream &) : void  --
 *              prints model state values, this is used for debugging and
 *              error handling
 *         printParameters(ostream &) : void  --
 *              prints model parameter values, this is used for debugging and
 *              error handling
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
 ************************************************************************** */
#ifndef RIMMODEL_HPP_
#define RIMMODEL_HPP_

#include "structures.hpp"
#include "RIMParser.hpp"
#include "exceptions/Exception.hpp"

namespace model {

class RIMModel
{
public:
	RIMModel() {};
	virtual ~RIMModel(){};

	virtual void dailyTrigger()                              = 0;
	virtual void setupTrigger()                               {};
	virtual void ucTrigger()                                 = 0;
	virtual void lfTrigger()                                 = 0;
	virtual void regTrigger()                                = 0;
};

} /* END OF NAMESPACE model */

#endif /* RIMMODEL_HPP_ */
