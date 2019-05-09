#include "location.hpp"
#include "weather.hpp"

std::string Location::getName() const {
	return name;
}
const Temperature Location::temperatureAt(const boost::posix_time::ptime &time) {
	return weatherSeries->weatherAt(time).temperature();
}
Location::Location(const std::string & name, WeatherSeries * weatherSeries) :
	name(name),
	weatherSeries(boost::shared_ptr<WeatherSeries>(weatherSeries))
{
}
