#ifndef UNIVERSE_HPP
#define UNIVERSE_HPP

#include "structures.hpp"
#include <map>

class Location;

class Universe {
	public:
		SimulationInfo simulationInfo;
		std::vector<BalanceAuthParams> balancingAuthorityParameters;
		std::vector<ZoneParams> zoneParameters;
		std::vector<ConventionalParams> conventionalParameters;
		std::vector<WindParams> windParameters;
		std::vector<SolarParams> solarParameters;
		std::vector<LoadParams> loadParameters;
		std::vector<ConventionalData> conventionalData;
		std::vector<Location> locations;
		std::map<std::string, AirTemperatureDeratingTable> airTemperatureDeratingTable;
		std::map<std::string, AirTemperatureefficientTable> airTemperatureefficientTable;
		double getMaxPower(boost::posix_time::ptime time, size_t balancingAuthorityIndex, size_t generatorBaIndex); // returns maximum power in MW of the specified generator at the given time
		double efficientyderat(boost::posix_time::ptime time, size_t balancingAuthorityIndex, size_t generatorBaIndex); // returns maximum power in MW of the specified generator at the given time
		double getderatfactor(boost::posix_time::ptime time, size_t balancingAuthorityIndex, size_t generatorBaIndex); // returns maximum power in MW of the specified generator at the given time
};

#endif
