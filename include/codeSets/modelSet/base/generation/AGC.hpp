/*
 * AGC.hpp
 *
 *  Created on: Jan 15, 2011
 *      Author: kevin
 */

#ifndef AGC_HPP_
#define AGC_HPP_

#include "structures.hpp"
class AGC {
public:
	AGC(){};
	virtual ~AGC(){};
	FLOAT getAGCValue(FLOAT value) {
		computeAGC(value);
		return agcValue;
	};
protected:
	virtual FLOAT computeAGC(FLOAT value)   = 0;
	FLOAT                    agcValue;
private:
};

#endif /* AGC_HPP_ */
