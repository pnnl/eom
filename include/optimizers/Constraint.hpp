#ifndef CONSTRAINT_HPP_
#define CONSTRAINT_HPP_

namespace model {

class Constraint
{
public:
	virtual ~Constraint(){};
	virtual void load() = 0;
};

} /* END OF NAMESPACE model */

#endif /* CONSTRAINT_HPP_ */
