/*
 * ObjectiveFunction.hpp
 *
 *  Created on: Dec 18, 2010
 *      Author: kevin
 */

#ifndef OBJECTIVEFUNCTION_HPP_
#define OBJECTIVEFUNCTION_HPP_

namespace model {

class ObjectiveFunction {
public:
	ObjectiveFunction(){};
	virtual ~ObjectiveFunction(){};

	virtual void load()                          = 0;
	virtual void printErrorState()               = 0;
protected:

private:
};

} /* END OF NAMESPACE model */

#endif /* OBJECTIVEFUNCTION_HPP_ */
