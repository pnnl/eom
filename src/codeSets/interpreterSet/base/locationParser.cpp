#include "location.hpp"
#include "locationParser.hpp"
#include "csv.hpp"
#include "structures.hpp"
#include <stdexcept>

using ::std::string;
using ::std::vector;
using ::gov::pnnl::rim::csv::Dictionary;
using ::gov::pnnl::rim::csv::DictionaryReader;

#include "weather.hpp"
#include <boost/date_time/posix_time/posix_time_types.hpp>
#include <boost/date_time/posix_time/time_parsers.hpp>
#include <boost/lexical_cast.hpp>
#include <deque>
#include <fstream>
using boost::posix_time::ptime;
using boost::posix_time::time_from_string;
using boost::lexical_cast;
using std::ifstream;
using std::deque;

class Forecast {
	public:
		ptime time;
		Weather weather;
};

class NaiveWeatherSeries : public WeatherSeries {
	private:
		string filename;
		size_t bufferSize;
		deque<Forecast> forecasts; // TODO convert to list for efficient removal
		ifstream::pos_type position;
		Forecast next(DictionaryReader & reader) {
			Dictionary row = reader.next();
			double fahrenheit = lexical_cast<double>(row["Temperature (F)"]);
			double pascals = lexical_cast<double>(row["Pressure (Pa)"]);
			ptime time = time_from_string(row["Time"]);
			Temperature temperature = Temperature::Fahrenheit(fahrenheit);
			Pressure pressure = Pressure::Pascals(pascals);
			Weather weather(temperature, pressure);
			Forecast forecast = {time, weather};
			return forecast;
		};
	public:
		NaiveWeatherSeries(const string & filename) :
			filename(filename),
			bufferSize(144)
		{
			ifstream * stream = new ifstream(filename.c_str());
			DictionaryReader reader(stream);
			forecasts.push_back(next(reader));
			position = stream->tellg();
		}
		Weather weatherAt(const ptime & time) {
			if (time > forecasts.at(forecasts.size()-1).time) {
				ifstream * stream = new ifstream(filename.c_str());
				DictionaryReader reader(stream);
				stream->seekg(position); // TODO makes assumptions about DictionaryReader internals
				while (time > forecasts.at(forecasts.size()-1).time && ! reader.end()) {
					forecasts.erase(forecasts.begin(), forecasts.begin()+(forecasts.end()-forecasts.begin())/2);
					while (! reader.end() && forecasts.size() < bufferSize)
						forecasts.push_back(next(reader)); // load more forecasts
					position = stream->tellg();
				}
			}
			if (time > forecasts.at(forecasts.size()-1).time)
				throw std::range_error("Requested weather for time after final forecast in file.");
			if (time < forecasts.at(0).time)
				throw std::range_error("Requested weather for time before beginning of buffered time."); // after end check in case loading more causes problems
			size_t index = 0;
			while (time > forecasts.at(index+1).time) // guaranteed to not overflow based on checks above, could use something like std::lower_bound if becomes a bottleneck
				index++;
			// now forecasts.at(index).time < time <= forecasts.at(index+1).time
			if (time == forecasts.at(index+1).time)
				return forecasts.at(index+1).weather;
			// linear interpolate if we don't have an exact match
			// could take closest match instead; picked this to smooth larger weather intervals than step intervals
			// cubic splines seemed like a bit of overkill compared to linear interpolation
			long forecastInterval = (forecasts.at(index+1).time - forecasts.at(index).time).total_seconds();
			long desiredInterval  = (                   time - forecasts.at(index).time).total_seconds();
			double percentageToNextIndex = desiredInterval / forecastInterval;
			double fahrenheit =
				(1-percentageToNextIndex) * forecasts.at(index+1).weather.temperature().fahrenheit() +
				   percentageToNextIndex  * forecasts.at(index  ).weather.temperature().fahrenheit();
			double pascals =
				(1-percentageToNextIndex) * forecasts.at(index+1).weather.pressure().pascals() +
				   percentageToNextIndex  * forecasts.at(index  ).weather.pressure().pascals();
			Temperature temperature = Temperature::Fahrenheit(fahrenheit);
			Pressure pressure = Pressure::Pascals(pascals);
			return Weather(temperature, pressure);
		}
};

namespace interpreter {

LocationParser::LocationParser(
	const string & fileName,
	vector<Location> & locations
)
{
	DictionaryReader reader(new std::ifstream(fileName.c_str()));
	while (! reader.end()) {
		Dictionary row = reader.next();
		string name = row["name"];
		string weatherFileName = row["weather file"];
		WeatherSeries * weatherSeries;
		try {
			weatherSeries = new NaiveWeatherSeries(weatherFileName);
		} catch(std::runtime_error &e) {
			std::cerr << "error while reading location #" << 1+locations.size() << std::endl;
			throw;
		}
		Location location(name, weatherSeries);
		locations.push_back(location);
	}
}

} /* end of NAMESPACE */


