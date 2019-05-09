#ifndef ZONE_CONNECTIONS_PARSER_HPP
#define ZONE_CONNECTIONS_PARSER_HPP

#include "ModelParser.hpp"
#include "RIMParser.hpp"

namespace interpreter {

class ZoneConnectionParser: public ModelParser
{
public:
	ZoneConnectionParser(
			const std::string & fileName,
			const std::vector<BalanceAuthParams> & baParams,
			const std::vector<ZoneParams> & zoneParams,
			std::vector<std::vector<std::vector <Connection> > > & zoneIntraBAConnect,
			SimulationInfo & simulationInfo
	);
	virtual ~ZoneConnectionParser();
	virtual void setModels(SimulationInfo & info);
private:
	void readZoneConnections(std::vector<Connection> &connections);
	const std::vector<BalanceAuthParams> & baParams;
	const std::vector<ZoneParams> & zoneParams;
	std::vector<std::vector<std::vector <Connection> > > & zoneIntraBAConnect;
};

} /* END OF NAMESPACE interpreter */

#endif /* ZONE_CONNECTIONS_PARSER_HPP */
