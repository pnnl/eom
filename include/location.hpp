#ifndef LOCATION_HPP
#define LOCATION_HPP

#include <boost/shared_ptr.hpp>
#include <string>

class Weather;
class WeatherSeries;
class Temperature;
namespace boost {
	namespace posix_time {
		class ptime;
	}
}

class Location {
	public:
		std::string getName() const;
		const Temperature temperatureAt(const boost::posix_time::ptime &);
		Location(const std::string & name, WeatherSeries * weatherSeries);
	private:
		std::string name;
		boost::shared_ptr<WeatherSeries> weatherSeries;
};

#endif
