#ifndef WEATHER_HPP
#define WEATHER_HPP

namespace boost {
	namespace posix_time {
		class ptime;
	}
}

class Temperature {
	public:
		double fahrenheit() const;
		double celsius() const;
		static Temperature Fahrenheit(double fahrenheit);
		static Temperature Celsius(double celsius);
	private:
		double celsius_;
		double fahrenheit_;
		Temperature(double celsius, double fahrenheit);
};
class Pressure {
	public:
		double pascals() const;
		static Pressure Pascals(double pascals);
	private:
		double pascals_;
		Pressure(double pascals);
};
class Weather {
	public:
		const Temperature & temperature(void) const;
		const Pressure & pressure(void) const;
		Weather(Temperature, Pressure);
	private:
		Temperature temperature_;
		Pressure pressure_;
};
class WeatherSeries {
  public:
	virtual Weather weatherAt(const boost::posix_time::ptime &) = 0;
	virtual ~WeatherSeries();
};

#endif
