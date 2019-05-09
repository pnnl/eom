/**
 * The format of the source file is:
 * <pre>
 * Simulation  Requirements  : path to simulation requirements file
 * %%%%
 * Model Parameters and Initialization
 * Model tag 1              : path to model 1 requirements file
 * #Model tag 4             : path to model 4 requirements file, this line is commented out, so it won't be read
 * %%%%
 *
 * Time Series File Set
 * Load                     : path to load data file (contains names of files and BA/Zone associations
 * Wind                     : path to wind data file (contains names of files and BA/Zone associations
 * Solar                    : path to solar data file (contains names of files and BA/Zone associations
 * %%%%
 * </pre>
 *
 * Each requirements file is associated with a parser that interprets the file's contents.
 * The parser API will provide a detailed description of the format of its associated
 * requirements file
 */
#include <stdexcept>
#include "universe.hpp"
#include "exceptions/Exception.hpp"
#include "exceptions/TerminationException.hpp"
#include "structures.hpp"
#include "ModelParser.hpp"
#include "FileControl.hpp"
#include "RIMParser.hpp"
#include "SimulationParser.hpp"
#include "BAParser.hpp"
#include "ZoneParser.hpp"
#include "GeneratorParser.hpp"
#include "StorageParser.hpp"
#include "HydroParser.hpp"
#include "LoadParser.hpp"
#include "WindParser.hpp"
#include "SolarParser.hpp"
#include "ZoneIntraConnectParser.hpp"
#include "ZoneConnectionParser.hpp"
#include "locationParser.hpp"
#include "deratingParser.hpp"
#include "deratingefParser.hpp"
using boost::shared_ptr;
using std::string;
using std::endl;
using std::cerr;
using std::clog;
using std::vector;

namespace interpreter {
RIMParser   * RIMParser::rimParser = NULL;
/**
 * \brief Associate a tag name with unique id
 *
 * The tag name is used explicitly in the main source file to identify the names
 * of source files. The <i>ParserTag</i> associates the name of the
 * tag with a unique <i>INFORMATION_TYPE</i> enumeration (see structures.hpp).
 *
 * Using the MODEL_TYPE ensures unique id for each tag.
 */
const Parser::ParserTag tag[] =
{
		{"Simulation Requirements", SIMULATION},
		{"Balancing Authority",BALANCE_AUTHS},
		{"Zone",ZONE},
		{"Locations",LOCATIONS},
		{"AirTemperatureDerating",DERATING},
		{"AirTemperatureDerating Efficient",DERATINGEF},
		{"Conventional Generator",CONVENTIONAL},
		{"Load",LOAD_TIMESERIES},
		{"Wind",WIND_TIMESERIES},
		{"Solar",SOLAR_TIMESERIES},
		{"Zone IntraConnect",ZONE_INTRACONNECT},
		{"Zone InterConnect",ZONE_INTERCONNECT},
		{"Zone Connections",ZONE_CONNECTIONS}
};

/* ****************************************************************************
 * RIMParser(const char * fileName)
 *************************************************************************** */
/**
 * \details the RIMParser is constructed by:
 * <ul>
 * <li>load the source files from the initial source file</li>
 * <li>construct the interface and parameter data spaces</li>
 * <li>parse the source files into to data spaces</li>
 * </ul>
 */
RIMParser::RIMParser(
	const string & fileName,
	Universe & universe
) :
		interpreter::Parser(fileName),
		srcFileName(NUM_MODELS),
		universe(universe)
{
	try {
		if (RIMParser::rimParser != NULL)
			throw std::logic_error("RIMParser is singleton.  Constructor was called twice.");
		reduceFile(); // flattens file and makes it conform to linux EOL
		for (INTEGER i = 0; i < NUM_MODELS; i++) {
			string str = getTagValue(tag[i].modelTagName.c_str(), '<', '>', ':');
			if (str == "" && i == ZONE)
				universe.simulationInfo.nZones = universe.simulationInfo.nBalancingAuths;
			if (str != "") {
				srcFileName[i] = str;
				parseType((INFORMATION_TYPE)i);
			}
		}
		resolveDependenies(); // resolve dependencies that need multiple model types loaded, like linking zones to BAs.
	} catch (Exception & e) {
		clog << e << endl;
		clog << "ERROR (" <<  utility::FileControl::getFileName(__FILE__) << ":" << __LINE__ << "): Could not parse simulation files." << endl;
		exit(-1);
	}
	RIMParser::rimParser = this;
}

RIMParser::~RIMParser()
{
	RIMParser::rimParser = NULL;
}

RIMParser * RIMParser::getRIMParser()
{
	if (RIMParser::rimParser == NULL) {
		cerr << "FATAL: attempted to access unallocated parser" << endl;
		TerminationException(__FILE__, __LINE__, Exception::ACCESSED_NULL);
	}
	return RIMParser::rimParser;
}

BalanceAuthParams  * RIMParser::getBalanceAuthParams (size_t index) { return & universe.balancingAuthorityParameters[index]; };
ZoneParams         * RIMParser::getZoneParams        (size_t index) { return & universe.zoneParameters              [index]; };
ConventionalParams * RIMParser::getConventionalParams(size_t index) { return & universe.conventionalParameters      [index]; };
LoadParams         * RIMParser::getLoadParams        (size_t index) { return & universe.loadParameters              [index]; };
WindParams         * RIMParser::getWindParams        (size_t index) { return & universe.windParameters              [index]; };
SolarParams        * RIMParser::getSolarParams       (size_t index) { return & universe.solarParameters             [index]; };

size_t RIMParser::getNZoneGenerators (size_t zone) { return zoneConvGenSet [zone].size(); };
const vector<size_t> & RIMParser::getBAGenSet                             (size_t ba  ){return baConvGenSet      [ba  ];};
const vector<size_t> & RIMParser::getZoneLoadSet                          (size_t zone){return zoneLoadSet       [zone];};
const vector<size_t> & RIMParser::getZoneWindSet                          (size_t zone){return zoneWindSet       [zone];};
const vector<size_t> & RIMParser::getZoneSolarSet                         (size_t zone){return zoneSolarSet      [zone];};
const vector<vector<Connection> > & RIMParser::getZoneIntraBAConnectMatrix(size_t ba  ){return zoneIntraBAConnect[ba  ];};

/**
 * \details The <i>configureForParsing</i> method generates a parser object
 * for the requested model type, and sets the size associated with the model's
 * parameter and data structures.
 *
 * \param  modelType : INFORMATION_TYPE
 * \return parser  : ModelParser * sets the array
 */
void RIMParser::parseType(INFORMATION_TYPE modelType)
{
	// The limited scope of parser objects looks like poltergeist anti-pattern.
	// However, they don't have any reason to live longer right now.
	// I could reuse a parser on multiple files, say by injecting the model parsers into RIMParser.
	// We could pass in a vector<ParserInterface> and run
	//   if ("" != (filename = getTag(parsers[n]))) parsers[n].parse(filename, aggregateModel)
	// parsers don't need to meet any interface right now other than "the constructor does all the work"
	try {
	switch (modelType) {
		case SIMULATION       : SimulationParser            (srcFileName[SIMULATION       ], universe.simulationInfo); break;
		case BALANCE_AUTHS    : BAParser                    (srcFileName[BALANCE_AUTHS    ], universe.balancingAuthorityParameters, universe.simulationInfo); break;
		case ZONE             : ZoneParser                  (srcFileName[ZONE             ], universe.zoneParameters, universe.balancingAuthorityParameters, universe.simulationInfo); break;
		case LOCATIONS        : LocationParser              (srcFileName[LOCATIONS        ], universe.locations); break;
		case DERATING         : AirTemperatureDeratingParser(srcFileName[DERATING         ], universe.airTemperatureDeratingTable); break;
		case DERATINGEF       : AirTemperatureDeratingefParser(srcFileName[DERATINGEF     ], universe.airTemperatureefficientTable); break;
		case CONVENTIONAL     : GeneratorParser             (srcFileName[CONVENTIONAL     ], universe.conventionalParameters, universe.conventionalData, universe.locations, universe.airTemperatureDeratingTable, universe.airTemperatureefficientTable, universe.simulationInfo); break;
		case LOAD_TIMESERIES  : LoadParser                  (srcFileName[LOAD_TIMESERIES  ], universe.loadParameters, universe.simulationInfo); break;
		case WIND_TIMESERIES  : WindParser                  (srcFileName[WIND_TIMESERIES  ], universe.windParameters, universe.simulationInfo); break;
		case SOLAR_TIMESERIES : SolarParser                 (srcFileName[SOLAR_TIMESERIES ], universe.solarParameters, universe.simulationInfo); break;
		case ZONE_INTRACONNECT: ZoneIntraConnectParser      (srcFileName[ZONE_INTRACONNECT], universe.balancingAuthorityParameters, zoneIntraBAConnect, universe.simulationInfo); break;
		case ZONE_INTERCONNECT: break; // new InterchangeParser(srcFileName[ZONE_INTERCONNECT]);
		case ZONE_CONNECTIONS : ZoneConnectionParser        (srcFileName[ZONE_CONNECTIONS ], universe.balancingAuthorityParameters, universe.zoneParameters, zoneIntraBAConnect, universe.simulationInfo); break;
		default :
			cerr << "FATAL: attempted to use invalid model type" << endl;
			throw TerminationException(__FILE__, __LINE__, Exception::ACCESSED_NULL);
		}
	} catch (int) {
		cerr << "FATAL " << tag[modelType].modelTagName << " parser failed to open" << endl;
		throw;
	}
}

void
RIMParser::resolveDependenies()
{
	configureBAZoneSpace();
	configureZoneGeneratorSpace();
}

void
RIMParser::configureBAZoneSpace()
{
	// TODO parsing models occurs in order.  Should be able to rely on parsers to fill in these bits using information previously obtained.
	baConvGenSet.resize(universe.simulationInfo.nBalancingAuths);
	for (size_t gen = 0; gen < universe.simulationInfo.nConventionalGenerators; gen++) {
		baConvGenSet[universe.conventionalParameters[gen].genParams.balancingAuth].push_back(gen);
		++universe.balancingAuthorityParameters[universe.conventionalParameters[gen].genParams.balancingAuth].nConventional;
		++universe.balancingAuthorityParameters[universe.conventionalParameters[gen].genParams.balancingAuth].nGenerators;
		if (universe.conventionalParameters[gen].genParams.canCommit)
			++universe.simulationInfo.nCanCommit;
		if (universe.conventionalParameters[gen].genParams.canRegulate)
			++universe.simulationInfo.nCanRegulate;
	}
}

/*
 * Configure generator, load, etc. for each zone
 */
void
RIMParser::configureZoneGeneratorSpace()
{
	zoneConvGenSet.resize(universe.simulationInfo.nZones);
	for (size_t generatorIndex = 0; generatorIndex < universe.simulationInfo.nConventionalGenerators; generatorIndex++) {
		int zoneIndex = universe.conventionalParameters[generatorIndex].genParams.zone;
		zoneConvGenSet[zoneIndex].push_back(generatorIndex);
		++universe.zoneParameters[zoneIndex].nConventional;
		++universe.zoneParameters[zoneIndex].nGenerators;
	}
	zoneLoadSet.resize(universe.simulationInfo.nZones);
	for (size_t gen = 0; gen < universe.simulationInfo.nLoads; gen++) {
		zoneLoadSet[universe.loadParameters[gen].zone].push_back(gen);
		++universe.zoneParameters[universe.loadParameters[gen].zone].nLoads;
	}
	zoneWindSet.resize(universe.simulationInfo.nZones);
	for (size_t farm = 0; farm < universe.simulationInfo.nWindFarms; farm++) {
		zoneWindSet[universe.windParameters[farm].zone].push_back(farm);
		++universe.zoneParameters[universe.windParameters[farm].zone].nWindFarms;
	}
	zoneSolarSet.resize(universe.simulationInfo.nZones);
	for (size_t farm = 0; farm < universe.simulationInfo.nSolarFarms; farm++) {
		zoneSolarSet[universe.solarParameters[farm].zone].push_back(farm);
		++universe.zoneParameters[universe.solarParameters[farm].zone].nSolarFarms;
	}
}

} /* END OF NAMESPACE interpreter */
