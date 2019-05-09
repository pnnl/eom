/* ***************************************************************************
 * PredictionData.hpp
 * Author      : Kevin Glass
 * Date        : Feb 1, 2010
 * File        : TForecastSeries.hpp
 * Project     : RIM
 *
 * Purpose:
 * This is a time series class that holds a finite buffer.
 *
 * STATUS:
 *
 ************************************************************************** */

#ifndef PREDICTIONDATA_HPP_
#define PREDICTIONDATA_HPP_

#include "structures.hpp"

namespace model {

template <typename T>
class PredictionData
{
public:
	PredictionData(INTEGER r, INTEGER c) :
		data(NULL),
		row(NULL),
		nRows(r),
		nCols(c)
	{
		data            = new T[nRows * nCols];
		row             = new T*[nRows];
		row[0]          = data;

		for (INTEGER i = 1; i < nRows; i++) {
			row[i] = row[i-1] + nCols;
		}
	};
	virtual ~PredictionData(){};
	T ** getData(){return row;};
	T  * getArray(INTEGER i){return row[i];};
protected:
private:
	T                      * data;    // stored in a 2-D array
	T                     ** row;    //
	INTEGER                  nRows;
	INTEGER                  nCols;
	/* -----------------------------------------------------------------------
	 * Hidden automatically generated constructors and operators
	 * -------------------------------------------------------------------- */
	PredictionData<T> & operator = (const PredictionData<T> & model){return * this;};
};

} /* END OF NAMESPACE model */

#endif /* PREDICTIONDATA_HPP_ */
