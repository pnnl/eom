/* ***************************************************************************
 * Author      : Kevin Glass
 * Date        : Feb 11, 2010
 * File        : Generator.hpp
 * Project     : RIM
 *
 * PURPOSE:
 *
 *
 * STATUS:
 *     naming convention conformity
 *     doxygen comments
 *     private comments
 *     comment block conformity
 *     testing information
 *     ensure hidden methods
 *
 * TESTING:
 *
 * ---------------------------------------------------------------------------
 */

/** \class Generator
 * \brief defines the behavior of arbitrary generators.
 *
 * The Generator class defines how arbitrary generators respond to various
 * time steps by implementing virtual methods defined in the RIMModel
 * super-class. It also declares two pure virtual methods: <i>dailySchedule</i>
 * and <i>computeGeneration</i>. These methods are specific to the generator
 * type (sub-class).
 *
 * The virtual functions are:
 * <table>
 * <tr>
 * <th>Method</th><th>Purpose</th>
 * </tr>
 * <tr>
 * <td colspan='2'>Public (interface) methods</td>
 * </tr>
 * <tr>
 * <td>dailyTrigger() : void <br> virtual from RIMModel</td>
 * <td>The <i>dailyTrigger</i> method defines the model's response to the a
 * daily time step. The daily time step occurs when the <a href="dataDictionary.html#UnitCommit">Unit Commitment </a>
 * optimizer updates the 72-hour <a href="dataDictionary.html#Lookahead">lookahead</a>.</td>
 * </tr>
 * <tr>
 * <td>ucTrigger() : void <br> virtual from RIMModel</td>
 * <td>The <i>ucTrigger</i> method defines the model's response to the a
 * unit commitment step. The 72-hour <a href="dataDictionary.html#UnitCommit">lookahead</a> is subdivided into regular time
 * spans called the <a href="dataDictionary.html#UnitCommitTS">unit commitment time step</a>.
 * Generally, the steps are one hour increments starting at the daily time step). The unit
 * commitment time step marks the start of each subdivision.</td>
 * </tr>
 * <tr>
 * <td>lfTrigger() : void <br> virtual from RIMModel</td>
 * <td>The <i>lfTrigger</i> method defines the model's response to the a
 * <a href="dataDictionary.html#LoadFollowingTS">load following time step</a>.
 * Each unit commitment step is subdivided into <a href="dataDictionary.html#LoadFollowing">load following</a>
 * steps.</td>
 * </tr>
 * <tr>
 * <td>regTrigger() : void <br> virtual from RIMModel</td>
 * <td>The <i>regTrigger</i> method defines the model's response to the a
 * regulation step. <a href="dataDictionary.html#Regulation">regulation</a>.</td>
 * </tr>
 * <tr>
 * <td colspan='2'>Protected methods</td>
 * </tr>
 * <tr>
 * <td>dailySchedule(ostream & out) : void</td>
 * <td>This produces a formatted print each of the model's <a href="dataDictionary.html#ModelState">state</a> values. The
 * results are stored in the specified output stream, which may be <i>cerr</i></td>
 * </tr>
 * <tr>
 * <td>computeGeneration(GeneratorData * startStep,
			GeneratorData * endStep) : void</td>
 * <td>This produces a formatted print each of the model's <a href="dataDictionary.html#ModelParameter">parameter</a> values. The
 * results are stored in the specified output stream, which may be <i>cerr</i></td>
 * </tr>
 * <tr>
 * <td colspan='2'>Private methods</td>
 * </tr>
 * <tr>
 * <td>RIMModel(RIMModel & model)</td>
 * <td> </td>
 * </tr>
 * <tr>
 * <td>operator = (const RIMModel & model) : RIMModel &</td>
 * <td> </td>
 * </tr>
 * </table>
 *
 * Testing
 * <table>
 * <tr>
 * <th>Test name</th><th>Purpose</th>
 * </tr>
 * <tr>
 * <td colspan='2'>Public (interface) methods</td>
 * </tr>
 * <tr>
 * <td>dailyTrigger() : void <br> virtual from RIMModel</td>
 * <td>The <i>dailyTrigger</i> method defines the model's response to the a
 * daily time step. The daily time step occurs when the <a href="dataDictionary.html#UnitCommit">Unit Commitment </a>
 * optimizer updates the 72-hour <a href="dataDictionary.html#Lookahead">lookahead</a>.</td>
 * </tr>
 * <tr>
 * <td>ucTrigger() : void <br> virtual from RIMModel</td>
 * <td>The <i>ucTrigger</i> method defines the model's response to the a
 * unit commitment step. The 72-hour <a href="dataDictionary.html#UnitCommit">lookahead</a> is subdivided into regular time
 * spans called the <a href="dataDictionary.html#UnitCommitTS">unit commitment time step</a>.
 * Generally, the steps are one hour increments starting at the daily time step). The unit
 * commitment time step marks the start of each subdivision.</td>
 * </tr>
 * <tr>
 * <td>lfTrigger() : void <br> virtual from RIMModel</td>
 * <td>The <i>lfTrigger</i> method defines the model's response to the a
 * <a href="dataDictionary.html#LoadFollowingTS">load following time step</a>.
 * Each unit commitment step is subdivided into <a href="dataDictionary.html#LoadFollowing">load following</a>
 * steps.</td>
 * </tr>
 * <tr>
 * <td>regTrigger() : void <br> virtual from RIMModel</td>
 * <td>The <i>regTrigger</i> method defines the model's response to the a
 * regulation step. <a href="dataDictionary.html#Regulation">regulation</a>.</td>
 * </tr>
 * <tr>
 * <td colspan='2'>Protected methods</td>
 * </tr>
 * <tr>
 * <td>dailySchedule(ostream & out) : void</td>
 * <td>This produces a formatted print each of the model's <a href="dataDictionary.html#ModelState">state</a> values. The
 * results are stored in the specified output stream, which may be <i>cerr</i></td>
 * </tr>
 * <tr>
 * <td>computeGeneration(GeneratorData * startStep,
			GeneratorData * endStep) : void</td>
 * <td>This produces a formatted print each of the model's <a href="dataDictionary.html#ModelParameter">parameter</a> values. The
 * results are stored in the specified output stream, which may be <i>cerr</i></td>
 * </tr>
 * <tr>
 * <td colspan='2'>Private methods</td>
 * </tr>
 * <tr>
 * <td>RIMModel(RIMModel & model)</td>
 * <td> </td>
 * </tr>
 * <tr>
 * <td>operator = (const RIMModel & model) : RIMModel &</td>
 * <td> </td>
 * </tr>
 * </table>
 *
 * @ingroup generation
 */
#ifndef GENERATOR_HPP_
#define GENERATOR_HPP_

#include "structures.hpp"
#include "models/RIMModel.hpp"
#include "RIMParser.hpp"
#include "simulation/SDInterface.hpp"


namespace model {

class Generator : public RIMModel
{
public:
	Generator(void);
	virtual ~Generator(void);
protected:
	size_t                   generatorID;
	static size_t            GeneratorID;
	LOGICAL                  implementsLoadFollowing;
	LOGICAL                  implementsRegulation;

	virtual LOGICAL getCommit()             = 0;
	virtual LOGICAL getRegulate()           = 0;
	virtual char *  getName()               = 0;

	virtual LOGICAL getZone()               = 0;
	virtual LOGICAL getBA()                 = 0;
};

} /* END OF NAMESPACE model */

#endif /* GENERATOR_HPP_ */
