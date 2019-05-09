#include "deratingefParser.hpp"
#include "universe.hpp"
#include "csv.hpp"
#include <fstream>
#include <algorithm>
#include <boost/lexical_cast.hpp>

using std::string;
using std::map;
using std::make_pair;
using std::pair;
using std::vector;
using std::ifstream;
using boost::lexical_cast;
using gov::pnnl::rim::csv::DictionaryReader;
using gov::pnnl::rim::csv::Dictionary;

namespace interpreter {

AirTemperatureDeratingefParser::AirTemperatureDeratingefParser(string & filename, map<string, AirTemperatureefficientTable> & tables) {
	DictionaryReader reader(new ifstream(filename.c_str()));
	vector<string> categories = reader.columns();
	categories.erase(std::find(categories.begin(), categories.end(), "Temperature (F)"));
	map<string,vector<pair<double, double> > > map;
	while (! reader.end()) {
		Dictionary row = reader.next();
		double fahrenheit = lexical_cast<double>(row["Temperature (F)"]);
		for (vector<string>::iterator it = categories.begin(); it != categories.end(); it++) {
			double factor = lexical_cast<double>(row[*it]);
			map[*it].push_back(make_pair(fahrenheit, factor));
		}
	}
	for (vector<string>::iterator it = categories.begin(); it != categories.end(); it++) {
		tables.insert(make_pair(*it, AirTemperatureefficientTable(map.at(*it))));
	}
}

} /* end of NAMESPACE */
