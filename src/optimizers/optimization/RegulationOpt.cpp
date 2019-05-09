#include "optimizers/optimization/RegulationOpt.hpp"
#include "RIMParser.hpp"
#include "administrative/BalancingAuthority.hpp"
#include "simulation/Clock.hpp"
#include "simulation/SDInterface.hpp"
#include "models/timeSeries/ScheduleData.hpp"

using std::ostream;
using std::endl;

namespace model {

RegulationOpt::RegulationOpt(UCData & ucData, LFData & lfData, RegData & regData, size_t baID) :
	ucData(ucData),
	lfData(lfData),
	regData(regData),
	nGenerators(-1),
	baID(baID),
	scheduleData(NULL),
	adjLoad(NULL)
{
}


RegulationOpt::~RegulationOpt()
{
}


void
RegulationOpt::regTrigger()
{
}


// ############################################################################
// ############################################################################
// ############################################################################
// ############################################################################
void
RegulationOpt::cplexExceptionResponse(ostream & out)
{
	out << "CPLEX failed to find a solution in REGULATION. The state of the \n";
	out << "system when the exception was thrown is:" << endl;
	out << "#################################################################\n";
	out << "#################################################################\n";
	out << "###                                                           ###\n";
	out << "###  Model state                                              ###\n";
	out << "###                                                           ###\n";
	out << "#################################################################\n";
	out << "#################################################################\n";
	printState(out);
	out << "#################################################################\n";
	out << "#################################################################\n";
	out << "###                                                           ###\n";
	out << "###  Optimizer parameters                                     ###\n";
	out << "###                                                           ###\n";
	out << "#################################################################\n";
	out << "#################################################################\n";
	printParameters(out);
	out << "#################################################################\n";
	out << "#################################################################\n";
	out << "###                                                           ###\n";
	out << "###  Optimizer variables state                                ###\n";
	out << "###                                                           ###\n";
	out << "#################################################################\n";
	out << "#################################################################\n";
	printOptimzationVariables(out);
	out << "#################################################################\n";
	out << "#################################################################\n";

}
void
RegulationOpt::printState(ostream & out)
{
	/*
	cerr << "BA #" << baID << " : " << baParams.name << endl;
	// Parameter data
	cerr << "BA Parameter data " << endl;
	cerr << "\tcps1                           : " << baParams.cps1 << endl;
	cerr << "\tcps2                           : " << baParams.cps2 << endl;
	cerr << "\tbias                           : " << baParams.bias << endl;
	cerr << "\tagcLower                       : " << baParams.agcLower << endl;
	cerr << "\tagcUpper                       : " << baParams.agcUpper << endl;
	cerr << "\treserveFactor                  : " << baParams.reserveFactor << endl;

	cerr << "\tn zones                        : " << baParams.nZones << endl;
	cerr << "\tn generators                   : " << baParams.nGenerators << endl;
	cerr << "\tn conventional generators      : " << baParams.nConventional << endl;
	cerr << "\tn storage generators           : " << baParams.nStorage << endl;
	cerr << "\tn hydro generators             : " << baParams.nHydro << endl;

	cerr << "\tn loads                        : " << baParams.nLoads << endl;
	cerr << "\tn wind farms                   : " << baParams.nWindFarms << endl;
	cerr << "\tn solar farms                : " << baParams.nSolarFarms << endl;
	cerr << "\tcurrent power                  : " << baParams.currentPower << endl;
	cerr << "\tcurrent load                   : " << baParams.currentLoad << endl;
	cerr << "\toutput power                   : " << baParams.outputPower << endl;
	cerr << "\toutput load                    : " << baParams.outputLoad << endl;

	cerr << "Zone  Parameter data " << endl;
	for (INTEGER zone = 0; zone < baParams.nZones; zone++) {
		cerr << "Zone #" << zoneParams[zone].zoneBAId << " : " << zoneParams[zone].name << endl;
		cerr << "\tnoxCost                        : " << zoneParams[zone].noxCost << endl;
		cerr << "\tsoxCost                        : " << zoneParams[zone].soxCost << endl;
		cerr << "\tco2Cost                        : " << zoneParams[zone].co2Cost << endl;

		cerr << "\tbalancingAuthIndex             : " << zoneParams[zone].balancingAuthIndex << endl;

		cerr << "\tnGenerators                    : " << zoneParams[zone].nGenerators << endl;
		cerr << "\tnConventional                  : " << zoneParams[zone].nConventional << endl;
		cerr << "\tnnHydro                        : " << zoneParams[zone].nHydro << endl;
		cerr << "\tnLoads                         : " << zoneParams[zone].nLoads << endl;
		cerr << "\tnWindFarms                     : " << zoneParams[zone].nWindFarms << endl;
		cerr << "\tnSolarFarms                    : " << zoneParams[zone].nSolarFarms << endl;
		cerr << "\tninterchangeFile               : " << zoneParams[zone].interchangeFile << endl;
	}
	cerr << "Current time                = " << currentTime;
	cerr << "Conventional  state data " << endl;
	for (INTEGER gen = 0; gen < baParams.nConventional; gen++) {
		cerr << "Generator #" << gen << endl;
		cerr << "\tcurrent power                  : " << convGen[gen].generatorData.state.currentPower << endl;
		cerr << "\tcurrent time in state          : " << convGen[gen].generatorData.state.timeInState << endl;
		cerr << "\ton/off status                  : " << convGen[gen].generatorData.state.onStatus << endl;

		cerr << "\toutput power                   : " << convGen[gen].generatorData.output.outputPower<< endl;
		cerr << "\tpower cost on current step     : " << convGen[gen].generatorData.output.powerCostOnStep << endl;
		cerr << "\taccumulated power cost         : " << convGen[gen].generatorData.output.accumulatedPowerCost << endl;

		cerr << "\tNOx emitted                    : " << convGen[gen].generatorData.output.noxEmitted << endl;
		cerr << "\tSOx emitted                    : " << convGen[gen].generatorData.output.soxEmitted << endl;
		cerr << "\tCO2 emitted                    : " << convGen[gen].generatorData.output.co2Emitted << endl;

		cerr << "\toutput time in state           : " << convGen[gen].generatorData.output.timeInState << endl;

		cerr << "\trequired power on this step    : " << convGen[gen].generatorData.commit.requiredPower << endl;
	}
	*/
}

void
RegulationOpt::printParameters(ostream & out)
{
	/*
	cerr << "Current time                = " << currentTime;
	cerr << "\treserve factor               : " << data.reserveFactor << endl;
	cerr << "\tramp schedule                : " << data.rampSchedule << endl;
	cerr << "\tn conventional generators    : " << data.nConventionalGenerators << endl;
	cerr << "\tn storage generators         : " << data.nStorageGenerators << endl;
	cerr << "\tn hydro generators           : " << data.nHydroGenerators << endl;
	cerr << "\tn generators                 : " << data.nGenerators << endl;

	cerr << "\tn zones                      : " << data.nZones << endl;
	cerr << "\tUC projection length         : " << data.ucLength << endl;
	cerr << "\tramp length                  : " << data.rampLength << endl;


	for (INTEGER gen = 0; gen < data.nGenerators; gen++) {
		cerr << "\tGenerator cost curve: "<< endl;
		printPWCurve(data.costCurve[gen], cerr);
		cerr << "\tstart up cost                : " << data.genTurnOffCost[gen] << endl;
		cerr << "\tshut down cost               : " << data.genTurnOnCost[gen] << endl;
		cerr << "\tminimum power limit          : " << data.genPowerMinLimit[gen] << endl;
		cerr << "\tmaximum power limit          : " << data.genPowerMaxLimit[gen] << endl;
		cerr << "\tramp down rate               : " << data.genRampDownRate[gen] << endl;
		cerr << "\tramp up rate                 : " << data.genRampUpRate[gen] << endl;

		cerr << "\tNOx emission rate            : " << data.noxRate[gen] << endl;
		cerr << "\tSOx emission rate            : " << data.soxRate[gen] << endl;
		cerr << "\tCO2 emission rate            : " << data.co2Rate[gen] << endl;

		cerr << "\tMinimum off time             : " << data.minDownTime[gen] << endl;
		cerr << "\tMinimum on time              : " << data.minUpTime[gen] << endl;

		cerr << "\tEffective minimum power      : " << data.actualPowerDownLimit[gen] << endl;
		cerr << "\tEffective maximum power      : " << data.actualPowerUpLimit[gen] << endl;
	}
		*/
}

} /* END OF NAMESPACE model */
