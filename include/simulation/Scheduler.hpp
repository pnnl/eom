/* ***************************************************************************
 * Author      : Kevin Glass
 * Date        : Feb 1, 2010
 * File        : Scheduler.hpp
 * Project     : RIM
 *
 *
 * Contents    :
 *
 * Assumptions :
 *
 * The scheduler is responsible for setting up and running the simulation. The
 * Therefore, the scheduler has two major tasks:
 *      1) allocate and configure the BA, zone, conventional, storage and hydro
 *         generators.
 *      2) initialize and run the simulation loop.
 *
 *============================================================================
 * ALLOCATING AND CONFIGURING THE MODELS:
 *
 * Each model type (BA, zone and generators) are dynamically allocated arrays
 * of the appropriate type. Each administrative model (BA and zone) require the
 * set of generators associated with that model decomposed by: Administrative
 * unit generator set, the Admin unit commit set (these are generators that
 * can participate in LF); the Admin Unit committed generator set (these are
 * generators that are selected by the optimizer to participate in LF), the
 * Admin Unit regulator set (these are generators that can be used as
 * regulating generators) and the regulating set (these are the generators
 * identified by the UC optimizer as regulating generator.
 *
 * The order of model creation is:
 *       1) BAs
 *       2) Zones
 *       3) Conventional, Storage, Hydro
 *
 * The generators are independent of one another so the order of generator
 * instantiation is set Conventional, Storage, Hydro for convention purposes.
 * The generators are stored by BA order. The generators within a BA are
 * ordered by zone.
 *
 * Model Configuration details
 *
 * Each BA has a set of zones, the number of zones in a baParam[i].nZones. When
 * After the Zones are set the starting location for BA[i]'s Zone[0] is set and
 * the BA can access its zone set using that pointer.
 *
 *           0_1_2_3_4_5_6_7_8_9_0_1_2_3_4_5_6_7_8_9
 * Zone set | | | | | | | | | | | | | | | | | | | | |
 *          |_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|
 *           | +---------+             |
 *           | | +---------------------+
 *           | | |
 *           |_|_|
 * BA set   | | | |
 *          |_|_|_|
 *           0 1 2
 *
 * In this example, there are 20 zones and 3 BAs. The zones are ordered by BA
 * in the zoneSet. In the BA set, BA[0].zone points at the address of zone[0].
 * A programmer can address the 3rd zone in BA[0] by BA[0].zone[2]. Since there
 * are 7 zones, in BA[0], BA[1].zone must start at the address of zone[7]. Again
 * to address the 2nd generator in BA[1], the programmer uses BA[1].zone[1].
 * For BA[i], the starting address is the address of
 *
 *                  index[j] = SUM(n=0->j-1)BA[n].nZones
 * -------------------------------------------------------------------------
 * Each BA also has a set of Generators (Conventional, Storage, Hydro). Since
 * all generator types have the Generator class as a superclass, the BA
 * generator set is an array of pointers to the generator's. For each BA, the
 * generatorSet is the complete set of Generators. The following three sections
 * describe how each BA-generator set is to be configured
 * -------------------------------------------------------------------------
 * BA[0]'s generator set consists of 6 conventional, 3 storage and 2 Hydro
 * generators. The BA[0].generator[0] is the address of conventional[0],
 * BA[0].generator[7] is the address of storage[0] and the BA[0].generator[10]
 * is the address of hydro[0]
 *
 *           Conv                  Storage     Hydro Array
 *           0_1_2_3_4_5_6_7_8_9    0_1_2_3_4     0_1_2
 * Gen set  | | | | | | | | | | |  | | | | | |   | | | |
 *          |_|_|_|_|_|_|_|_|_|_|  |_|_|_|_|_|   |_|_|_|
 *           |         |            |   |         | |
 *           +-+-------+            |   |         | |
 *             |   +----------------+---+         | |
 *             |   |   +--------------------------+-+
 *             |   |   |
 *            _+___+___+_
 * BA[0]     | C | S | H | NOTE: the distinction between generator type is for
 *           |___|___|___| clarity of the description. The BA only sees a
 * nPointers    6  3   2   set of generator pointers. The BA can find a
 *                         generator of a specific type using the appropriate
 *                         offset. For the generator i, the starting point
 *                         of the storage array is
 *                    BA[i].generator[BA[i].nConventional]
 *                         the starting point of the hydro array is
 *                    BA[i].generator[BA[i].nConventional + BA[i].nStorage]
 * -------------------------------------------------------------------------
 * BA[1]'s generator set consists of 3 conventional, 2 storage and 0 Hydro
 * generators. The BA[1].generator[0] is the address of conventional[6],
 * BA[1].generator[3] is the address of storage[3]. Since there are no hydro
 * generators, this enumeration completes the BA[1] generator set.
 *
 *           Conv                  Storage     Hydro Array
 *           0_1_2_3_4_5_6_7_8_9    0_1_2_3_4     0_1_2
 * Gen set  | | | | | | | | | | |  | | | | | |   | | | |
 *          |0|0|0|0|0|0|_|_|_|_|  |0|0|0|_|_|   |0|0|_|
 *                       |   |            | |
 *             +---------+---+            | |
 *             |   +----------------------+-+
 *             |   |
 *             |   |
 *            _+___+_
 * BA[1]     | C | S |     NOTE: the distinction between generator type is for
 *           |___|___|     clarity of the description. The BA only sees a
 * nPointers   3   2       set of generator pointers. The BA can find a
 *                         generator of a specific type using the appropriate
 *                         offset. For the generator i, the starting point
 *                         of the storage array is
 *                    BA[i].generator[BA[i].nConventional]
 *                         the starting point of the hydro array is
 *                    BA[i].generator[BA[i].nConventional + BA[i].nStorage]
 * -------------------------------------------------------------------------
 * BA[2]'s generator set consists of 1 conventional, 0 storage and 1 Hydro
 * generators. The BA[2].generator[0] is the address of conventional[9],
 * BA[2].generator[1] is the address of hydro[2]. Since there are no storage
 * generators, this enumeration completes the BA[2] generator set.
 *
 *           Conv                  Storage     Hydro Array
 *           0_1_2_3_4_5_6_7_8_9    0_1_2_3_4     0_1_2
 * Gen set  | | | | | | | | | | |  | | | | | |   | | | |
 *          |0|0|0|0|0|0|1|1|1|_|  |0|0|0|1|1|   |0|0|_|
 *                             |                      |
 *             +---------------+                      |
 *             |   +----------------------------------+
 *             |   |
 *             |   |
 *            _+___+_
 * BA[1]     | C | H |     NOTE: the distinction between generator type is for
 *           |___|___|     clarity of the description. The BA only sees a
 * nPointers   1   1       set of generator pointers. The BA can find a
 *                         generator of a specific type using the appropriate
 *                         offset. For the generator i, the starting point
 *                         of the storage array is
 *                    BA[i].generator[BA[i].nConventional]
 *                         the starting point of the hydro array is
 *                    BA[i].generator[BA[i].nConventional + BA[i].nStorage]
 * -------------------------------------------------------------------------
 * When the BAs are configured, the BA-generator set will look like:
 *
 *           Conv                  Storage     Hydro Array
 *           0_1_2_3_4_5_6_7_8_9    0_1_2_3_4     0_1_2
 * Gen set  | | | | | | | | | | |  | | | | | |   | | | |
 *          |0|0|0|0|0|0|1|1|1|2|  |0|0|0|1|1|   |0|0|2|
 *
 * Each BA will point at its set of generators as Generator pointers. The
 * number of pointers for the ith BA is BA[i].nGenerators, which must equal
 * BA[i].nConventional + BA[i].nStorage + BA[i].nHydro.
 *
 * The program should verify that the counts are correct and the pointers point
 * at the correct type.
 * ---------------------------------------------------------------------------
 * The zone, commit and regulation generator sets are configured in the same
 * way the BA set was configured.
 *
 * The committed and regulating generator sets are determined by the optimizer
 * so they must be reset during each UC step (default is hourly). The strong
 * assumption is the all generators may be required as some point, so the size
 * of the array must be the number of generators available to the BA/Zone. The
 * weak assumption is that we cannot know the number of generators required by
 * a given BA/zone can be known a priori, so we use the number of generators
 * owned by a BA/zone as the size of these arrays. In either case, the size of
 * the array for a give BA/zone is the number of arrays in that BA/zone.
 *
 *============================================================================
 * INITIALIZING AND RUNNING THE SIMULATION
 *
 * The simulator is responsible for ensuring the all models update their state
 * at the appropriate time and in the appropriate order. Initializing the
 * simulation requires two processes requires the initialization of the adjusted
 * load set (load - [solar + wind + hydro]), the SDI (which requires information
 * from the adjusted load set). The SDI initialization process must initialize
 * the model input (the initial values) at time 0. Once the SDI is configured,
 * the first optimization for UC,LF and reg are run. These will update the
 * commit, and output data for each model at time step 1.
 *
 * The model is run by:
 *   1) Loop through each time step from time = 1 to t = endOfSimulation.
 *          NOTE: the loop step is the regulation step.
 *       2) on each loop step, check whether or not the adjusted load needs to
 *          be updated AND the baSet needs to be updated the condition is:
 *               Clock::onUCProjectionStep
 *          The project step is the time for the daily lookahead schedule
 *          generation, which is controlled by the BA.
 *       3) on each loop step, update the SDI to ensure it is pointing at the
 *          correct time.
 *       4) on each loop step, if the step is a UC step (Clock::onUCStep()),
 *          trigger the BA to invoke its UC response (this is the hourly step
 *          and it sets the committed and regulating generator step for the next
 *          hour).
 *       5) on each loop step, if the step is an LF step (Clock::onLFStep()),
 *          trigger the BA to invoke its LF response (update the load following
 *          adjusted load data, run the BA's LF optimizer to select the power
 *          levels of the committed generator set, update the generator
 *          state/committed values in the SDI)
 *       6) on each loop step, trigger the BA to invoke its regulation response
 *          (update the regulation adjusted load data, run the BA's Reg
 *          optimizer, set the regulation commit values for each generator.
 *          If the optimizer converges, update the generators by setting the
 *          regulation power and actual power to the values determined by the
 *          optimizations. If the optimizer fails to converge, the regulation
 *          optimizer will ramp the generator (up or down as need) to the
 *          maximum possible |delta|. The sum of the generator outputs minus the
 *          required power (adjusted load) is converted to a frequency shift
 *          that must be made up on the next time step, if possible.
 *       7) on each loop step, if the step is a dump step (Clock::onDumpStep)
 *          dump the current set of completed steps (default one day). This
 *          will store the results using a unique file name based on the time
 *          of the dump.
 *   End of loop
 *
 *   After the loop is complete--termination because of error or because the
 *   end of the simulation is reached--dump the current set of completed time
 *   steps.
 * ---------------------------------------------------------------------------
 * There is a set of incomplete code used to append a "slow" ramp initialized
 * by starting from the lower bound of the generator with the smallest lower
 * bound and ending a the first simulation time step. This is a bit tricky and
 * is under development.
 *
 */

#ifndef SCHEDULER_HPP_
#define SCHEDULER_HPP_

#include "optimization_structs.hpp"

#include "RIMParser.hpp"

#include "simulation/SDInterface.hpp"

#include "models/RIMModel.hpp"
#include "administrative/BalancingAuthority.hpp"
#include "administrative/Zone.hpp"
#include "generation/ConventionalGenerator.hpp"

#include "models/timeSeries/AdjLoadTS.hpp"
#include "models/timeSeries/ScheduleData.hpp"

#include <boost/shared_ptr.hpp>

class Universe;

namespace simulator {

class Scheduler
{
public:
	Scheduler(Universe & universe);
	static Scheduler * getScheduler();
	virtual ~Scheduler();
	void simulate(void);

	FLOAT                      * getUCAdjSeries(int zone){return adjLoadTS[zone]->getUCAdjSeries();};
	FLOAT                      * getLFAdjSeries(int zone){return adjLoadTS[zone]->getLFAdjSeries();};
	FLOAT                      * getRegAdjSeries(int zone){return adjLoadTS[zone]->getRegAdjSeries();};

private:
	static Scheduler         * scheduler     ;
	const SimulationInfo     & simulationInfo;
	SDInterface                sdInterface   ;
	model::ScheduleData        scheduleData  ;

	// dynamically allocated generator sets for the each model type in the
	// simulation.
	std::vector<UCData>                       ucData    ;
	std::vector<model::BalancingAuthority>    baSet     ;
	std::vector<model::Zone>                  zoneSet   ;
	std::vector<model::ConventionalGenerator> convGenSet;

	// a list of generator pointers for each BA generator set type
	model::Generator               ** baGeneratorData       ;

	// a list of generator pointers for each Zone generator set type
	model::Generator              *** zoneGenerator    ;


	// ========================================================================

	/*
	 * The Scheduler (simulator) allocates the adjusted load for each zone.
	 * The AdjLoadTS consists of UC, LF and Regulation steps for the zone.
	 * The Scheduler will get pointers the the adjusted loads which it
	 * partitions based on zone. NOTE: the adjusted load will use one and only
	 * one load. The number of solar, wind and hydro time series depends on the
	 * number specified by the user in the RIM configuration files.
	 */
	std::vector<boost::shared_ptr<model::AdjLoadTS> > adjLoadTS;

	Scheduler & operator = (Scheduler & scheduler){return *this;};
};

} /* END OF NAMESPACE simulator */

#endif /* SCHEDULER_HPP_ */
