#ifndef WEATHER_PARSER_HPP_
#define WEATHER_PARSER_HPP_

#include <map>
#include <string>
#include <fstream>
#include "boost/date_time/posix_time/posix_time.hpp"
#include "boost/lexical_cast.hpp"

namespace interpreter {

class Temperature {
	public:
		double celsius() const { return degreesCelsius; }
		static Temperature fromCelsius(double degreesCelsius) { return new Temperature(degreesCelsius); }
	private:
		// private so we can move to subclass later and have another subclass for Fahrenheit
		Temperature(double degreesCelsius) : degreesCelsius(degreesCelsius) {}
		const double degreesCelsius;
};

class Pressure {
	public:
		double pascals() const { return valueInPascals; }
		static Pressure fromPascals(double pascals) { return new Pressure(pascals); }
	private:
		// private so we can move to subclass later
		Pressure(double pascals) : valueInPascals(pascals) {}
		const double valueInPascals;
};

class Weather {
	public:
		Temperature getTemperature(void) const;
		Pressure getPressure(void) const;
};

using boost::posix_time::ptime;
using std::map;
using std::vector;
using std::string;
using std::ifstream;
using std::istream;
class WeatherSeries {
	public:
		WeatherSeries(map<ptime, Weather> data): data(data) {}
		Weather weatherAt(ptime & time) const;
	private:
		map<ptime, Weather> data;
};

class CsvReader {
	public:
		CsvReader(string csvData);
		vector<string> readRow();
		bool end() const;
	private:
		ifstream in;
};

class CsvDictionaryReader {
	public:
		CsvDictionaryReader(istream & csvData): reader(csvData), headers(reader.readRow()) {
		}
		map<string, string> readRow() {
			map<string, string> row;
			vector<string> values = reader.readRow();
			if (values.size() < headers.size()) {
				// too few values
			} else if (values.size() > headers.size()) {
				// too many values
			} else {
				// assign keys and values in row
			}
			return row;
		};
		bool end() const { return reader.end(); };
	private:
		vector<string> headers;
		CsvReader reader;
};

class WeatherCsvParser
{
	public:
		WeatherCsvParser();
		~WeatherCsvParser();
		WeatherSeries parseWeatherCsvFile(string weatherFileName) {

			std::ifstream weatherFileStream(weatherFileName);
			weatherFileStream.seekg(0, std::ios_base::end);
			int length = weatherFileStream.tellg();
			weatherFileStream.seekg(0, std::ios_base::beg);
			char * buffer = new char[length];
			weatherFileStream.read(buffer, length);
			weatherFileStream.close();
			string weatherFileContent(buffer, length);
			delete buffer;

			CsvDictionaryReader reader(weatherFileContent);
			map<ptime, Weather> weatherSeriesData;
			while (!reader.end()) {
				map<string,string> row = reader.readRow();
				ptime time(row["Time"]);
				Temperature temperature = Temperature::fromCelsius(boost::lexical_cast<double>(row["Temperature (�C)"])); // b0 (cp1252/latin-1) or c2b0 (utf-8) for degree?
				Pressure pressure = Pressure::fromPascals(boost::lexical_cast<double>(row["Pressure (Pa)"]));
				Weather weather(temperature, pressure);
				weatherSeriesData(time) = weather;
			}
			return WeatherSeries(weatherSeriesData);
		}
};

} /* END OF NAMESPACE */

#endif /* WEATHER_PARSER_HPP_ */
