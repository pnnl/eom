#include "ZoneIntraConnectParser.hpp"
#include "exceptions/Exception.hpp"
#include "FileControl.hpp"

using std::string;
using std::vector;
using std::cerr;
using std::endl;

namespace interpreter {

ZoneIntraConnectParser::ZoneIntraConnectParser(
		const string & fileName,
		vector<BalanceAuthParams> & baParam,
		vector<vector<vector <Connection> > > & zoneIntraBAConnect,
		SimulationInfo & simulationInfo
) :
	ModelParser(fileName),
	baParam(baParam),
	zoneIntraBAConnect(zoneIntraBAConnect)
{
	// account for zoneIntraConnect.csv header (3 lines);
	nLines             -= 3;
	setModels(simulationInfo);
};

ZoneIntraConnectParser::~ZoneIntraConnectParser() {
}

void ZoneIntraConnectParser::readZoneConnections(vector<Connection> &connections) {
	current             = skipCharacter(current, ','); // skips zone number

	// get capacity
	for (size_t zoneToIndex = 0; zoneToIndex < connections.size(); zoneToIndex++) {
		FLOAT     capacity = getFloat(',');
		connections[zoneToIndex].connectTo       = zoneToIndex;
		if (capacity > POWER_EPS){ // only need lines for connected zones
			connections[zoneToIndex].lineCapacity = capacity;
		} else {
			connections[zoneToIndex].lineCapacity    = 0.0;
		}
	}

	// get ptdf
	current             = skipCharacter(current, ','); // skip blank
	current             = skipCharacter(current, ','); // skip zone number
	for (size_t zoneToIndex = 0; zoneToIndex < connections.size(); zoneToIndex++) {
		FLOAT     ptdf = getFloat(",\n",zoneToIndex+1 < connections.size());
		if (POWER_EPS < ptdf) { // only need lines for connected zones
			connections[zoneToIndex].ptdf = ptdf;
		} else {
			connections[zoneToIndex].ptdf = 0.0;
		}
	}
}

void ZoneIntraConnectParser::setModels(SimulationInfo & info) {
	resetParser();

	try {
		for (size_t baIndex = 0; baIndex < baParam.size(); baIndex++) {
			BalanceAuthParams &ba = baParam[baIndex];
			current             = skipLine(current); // BA header
			current             = skipLine(current); // Data header
			current             = skipLine(current); // Zone header

			vector<vector<Connection> > connectionTableForBA(ba.nZones, vector<Connection>(ba.nZones));
			for (size_t zoneFromIndex = 0; zoneFromIndex < ba.nZones; zoneFromIndex++) {
				vector<Connection> & connectionListForZone = connectionTableForBA[zoneFromIndex];
				readZoneConnections(connectionListForZone);
				current             = skipLine(current); // BA header
			}
			zoneIntraBAConnect.push_back(connectionTableForBA);
		}
	} catch (Exception &) {
		cerr << "ERROR (" <<  utility::FileControl::getFileName(__FILE__) << ":" << __LINE__ << "): failed to parse BA model" << endl;
		throw;
	}
}

} /* end of NAMESPACE */
