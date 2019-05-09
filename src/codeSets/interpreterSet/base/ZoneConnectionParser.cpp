#include "ZoneConnectionParser.hpp"
#include "exceptions/Exception.hpp"
#include "FileControl.hpp"
#include <stdexcept>

using std::string;
using std::vector;
using std::ostream;
using std::endl;
using std::cerr;
using std::cout;

namespace interpreter {

ZoneConnectionParser::ZoneConnectionParser(
		const string & fileName,
		const vector<BalanceAuthParams> & baParams,
		const vector<ZoneParams> & zoneParams,
		vector<vector<vector <Connection> > > & zoneIntraBAConnect,
		SimulationInfo & simulationInfo
) :
	ModelParser(fileName),
	baParams(baParams),
	zoneParams(zoneParams),
	zoneIntraBAConnect(zoneIntraBAConnect)
{
	// account for one-line header
	nLines--;
	setModels(simulationInfo);
};

ZoneConnectionParser::~ZoneConnectionParser() {}

void ZoneConnectionParser::readZoneConnections(vector<Connection> &connections) {
	for (size_t zoneToIndex = 0; zoneToIndex < connections.size(); zoneToIndex++) {
		connections[zoneToIndex].connectTo = zoneToIndex;
		connections[zoneToIndex].lineCapacity = 0.0;
		connections[zoneToIndex].ptdf = 0.0;
	}
}

static ostream & operator << (ostream & out, const Connection &c) {
	out << "Connection(capacity=" << c.lineCapacity << ",ptdf=" << c.ptdf << ")";
	return out;
}

template<class T>
static ostream & operator << (ostream & out, const vector<T> & v) {
	out << "[";
	typename vector<T>::const_iterator it = v.begin();
	if (it != v.end())
		out << *(it++);
	for (; it != v.end(); it++)
		out << "," << *it;
	out << "]";
	return out;
}
template<class T>
static ostream & operator << (ostream & out, const vector<vector<T> > & v) {
	out << "[";
	typename vector<vector<T> >::const_iterator it = v.begin();
	if (it != v.end())
		out << "\n  " << *(it++);
	for (; it != v.end(); it++)
		out << ",\n  " << *it;
	out << "\n]";
	return out;
}

void ZoneConnectionParser::setModels(SimulationInfo & info) {
	resetParser();
	current = skipLine(current); // skip header

	try {
		// initialize values
		zoneIntraBAConnect.resize(baParams.size());
		for (size_t baIndex = 0; baIndex < baParams.size(); baIndex++) {
			const BalanceAuthParams &ba = baParams[baIndex];
			zoneIntraBAConnect[baIndex].resize(ba.nZones);
			for (size_t zoneFromIndex = 0; zoneFromIndex < ba.nZones; zoneFromIndex++) {
				zoneIntraBAConnect[baIndex][zoneFromIndex].resize(ba.nZones);
				for (size_t zoneToIndex = 0; zoneToIndex < ba.nZones; zoneToIndex++) {
					Connection &connection = zoneIntraBAConnect[baIndex][zoneFromIndex][zoneToIndex];
					connection.connectTo = zoneToIndex;
					connection.lineCapacity = 0.0;
					connection.ptdf = 0.0;
				}
			}
		}

		for (int i = 0; i < nLines; i++) {
			const int zoneFrom = getInteger(',');
			const int zoneTo = getInteger(',');
			const FLOAT limit = getFloat();
			current = skipLine(current);

			int baIndex = zoneParams[zoneFrom].balancingAuthIndex;
			if (baIndex != zoneParams[zoneTo].balancingAuthIndex)
				throw std::runtime_error("Zone connections cannot cross BA boundaries.");
			int baZoneFrom = zoneParams[zoneFrom].zoneBAId;
			int baZoneTo = zoneParams[zoneTo].zoneBAId;
			Connection &connection = zoneIntraBAConnect[baIndex][baZoneFrom][baZoneTo];
			connection.lineCapacity = limit;
			//cout << i << ": " << limit << "\t" << connection <<  endl;
		}
		//cout << zoneIntraBAConnect << endl;
	} catch (Exception & e) {
		cerr << "ERROR (" <<  utility::FileControl::getFileName(__FILE__) << ":" << __LINE__ << "): failed to parse BA model" << endl;
		throw;
	}
}

} /* end of NAMESPACE */
