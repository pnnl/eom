/* ***************************************************************************
 * Assumptions:
 * 1) The RIM parser assumes the user will want the current set of models and
 * the current input set of those models.
 *
 ************************************************************************** */

/** \class RIMParser
 * \brief Coordinates the parsing of the RIM input files.
 *
 * @ingroup interpreter
 */

#ifndef RIMPARSER_HPP_
#define RIMPARSER_HPP_

#include <vector>
#include <boost/shared_ptr.hpp>
#include "Parser.hpp"
#include "location.hpp"

class Universe;

namespace interpreter {

using boost::shared_ptr;

class RIMParser : private Parser
{
public:
	RIMParser(const std::string & fileName, Universe & universe);
	static RIMParser * getRIMParser(); // return a pointer to the RIMParser singleton.  Make sure someone created it first.
	virtual ~RIMParser();

	BalanceAuthParams  * getBalanceAuthParams (size_t ba                        );
	ZoneParams         * getZoneParams        (size_t zone                      );
	ConventionalParams * getConventionalParams(size_t conventionalGeneratorIndex);
	LoadParams         * getLoadParams        (size_t load                      );
	WindParams         * getWindParams        (size_t wind                      );
	SolarParams        * getSolarParams       (size_t solar                     );

	size_t getNZoneGenerators(size_t zone);
	const std::vector<size_t                  > & getBAGenSet                (size_t ba  );
	const std::vector<size_t                  > & getZoneLoadSet             (size_t zone);
	const std::vector<size_t                  > & getZoneWindSet             (size_t zone);
	const std::vector<size_t                  > & getZoneSolarSet            (size_t zone);
	const std::vector<std::vector<Connection> > & getZoneIntraBAConnectMatrix(size_t ba  );
private:
	void parseType(INFORMATION_TYPE modelType); // Computes the size of model classes used in the simulation
	void configureBAZoneSpace       (); // configure BA index tables to other models
	void configureZoneGeneratorSpace(); // configure zone index tables to other models
	void resolveDependenies         (); // configure indexing/counts between models after they've been loaded
	std::vector<std::string> srcFileName;
	Universe & universe;
	/*
	 * These double vectors are index tables.
	 * Use them like loadParameters[zoneLoadSet[n]] to get the load parameters for the nth load in the zone.
	 * Indices are zero-based.
	 */
	std::vector<std::vector<size_t> > baConvGenSet;
	std::vector<std::vector<size_t> > zoneConvGenSet   ;
	std::vector<std::vector<size_t> > zoneCommitSet    ;
	std::vector<std::vector<size_t> > zoneRegulationSet;
	std::vector<std::vector<size_t> > zoneLoadSet      ;
	std::vector<std::vector<size_t> > zoneWindSet      ;
	std::vector<std::vector<size_t> > zoneSolarSet     ;

	// Intraconnect configuration, the Matrix is the BA, zone matrix, the map is the index of a generator's connections
	std::vector<std::vector<std::vector <Connection> > > zoneIntraBAConnect;

	static RIMParser       * rimParser;
};

} /* END OF NAMESPACE interpreter */

#endif /* RIMPARSER_HPP_ */
