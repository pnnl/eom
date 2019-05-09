#include "structures.hpp"
#include "simulation/Scheduler.hpp"
#include "exceptions/TerminationException.hpp"
#include "location.hpp"
#include "universe.hpp"
#include <sstream>

using interpreter::RIMParser;
using simulator::Scheduler;
using std::streambuf;
using std::ostream;
using std::ofstream;
using std::flush;
using std::string;
using std::stringstream;
using std::endl;
using std::cerr;
using std::clog;
using std::cout;

// redirect source to write to destination for the scope of this object
class Redirect {
private:
	streambuf * originalBuffer;
	ostream & source;
public:
	Redirect(ostream & source, ostream & destination) :
		originalBuffer(source.rdbuf(destination.rdbuf())),
		source(source)
	{
	}
	~Redirect() {
		source.rdbuf(originalBuffer);
	}
};

int
main(int argc, char * argv[])
{
	if (argc != 2) {
		cout << "USAGE: rim <source file>" << endl;
		exit(-1);
	}
	string                   sources(argv[1]);
	stringstream             message;
	ofstream                 logFile("rim-log");
	ofstream                 errFile("rim-errLog");
	try {
		Redirect clogRedirect(clog, logFile);
		Redirect cerrRedirect(cerr, errFile);
		Universe universe;
		RIMParser rimParser(sources, universe);
		Scheduler scheduler(universe);
		try {
			scheduler.simulate();
			message << "SIMULATION COMPLETE" << endl;
		} catch (TerminationException & e) {
			cerr << e << endl;
			message << e << endl;
			message << "FATAL: the simulation has failed due to a fatal error." << endl;
			message << "please see the rim-errorLog file for details." << endl;
		} catch (Exception & e) {
			cerr << "ERROR: unspecified RIM error." << endl;
			cerr << e << endl;
		//} catch (...) {
			//cerr << "ERROR: unspecified error in simulation" << endl;
		}
	} catch (Exception & e) {
		cerr << "ERROR: in interpreter" << endl << e << endl;
	}
	cout << message.str() << flush; // output message here because Redirect objects are out of scope so cout is normal again
	return 0;
}
