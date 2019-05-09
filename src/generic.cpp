#include "generic.hpp"
#include <boost/date_time/posix_time/ptime.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>

using ::boost::posix_time::ptime;
using ::boost::posix_time::ptime;
using ::boost::gregorian::date;
using ::boost::posix_time::time_duration;

Date::operator boost::gregorian::date() const {
	return date(year, month, day);
}

TimeOfDay::operator boost::posix_time::time_duration() const {
	return time_duration(hours, minutes, seconds);
}

Time::operator boost::posix_time::ptime() const {
	return ptime(date, tod);
}
