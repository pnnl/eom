#include "universe.hpp"
#include "location.hpp"
#include "weather.hpp"
#include <boost/date_time/posix_time/ptime.hpp>
#include <algorithm>

using std::min;

double Universe::getMaxPower(boost::posix_time::ptime time, size_t balancingAuthorityIndex, size_t generatorBaIndex) {
	ConventionalParams & gen = conventionalParameters.at(generatorBaIndex);
	return gen.maxPowerAt(time);
}
double Universe::getderatfactor(boost::posix_time::ptime time, size_t balancingAuthorityIndex, size_t generatorBaIndex) {
	ConventionalParams & gen = conventionalParameters.at(generatorBaIndex);
	return gen.deratf(time);
}
double Universe::efficientyderat(boost::posix_time::ptime time, size_t balancingAuthorityIndex, size_t generatorBaIndex) {
	ConventionalParams & gen = conventionalParameters.at(generatorBaIndex);
	return gen.efficien(time);
}