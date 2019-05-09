#ifndef ZONEINTRACONNECTPARSER_HPP_
#define ZONEINTRACONNECTPARSER_HPP_

#include "ModelParser.hpp"
#include "RIMParser.hpp"

namespace interpreter {

/**
 * \class ZoneIntraConnectParser
 *
 * \brief The ZoneIntraConnectParser reads the BA parameter set and derives initial
 * conditions for the BA models.
 * @ingroup interpreter
 */
class ZoneIntraConnectParser: public ModelParser
{
public:
	/**
	 * \brief Constructs a BA parser to parse the file <i>fileName</i>
	 * @param fileName name of the source file containing the BA parameter set
	 */
	ZoneIntraConnectParser(
			const std::string & fileName,
			std::vector<BalanceAuthParams> & baParam,
			std::vector<std::vector<std::vector <Connection> > > & zoneIntraBAConnect,
			SimulationInfo & simulationInfo
	);
	virtual ~ZoneIntraConnectParser();
	/**
	 * \brief Reads the parameter set for each BA model
	 * @param info : SimulationInfo is the initialized simulation data structure
	 */
	virtual void setModels(SimulationInfo & info);

private:
	void readZoneConnections(std::vector<Connection> &connectionListForZone);
	std::vector<BalanceAuthParams> & baParam;
	std::vector<std::vector<std::vector <Connection> > > & zoneIntraBAConnect;
};

} /* END OF NAMESPACE interpreter */

#endif /* ZONEINTRACONNECTPARSER_HPP_ */
