#include "constraints/unitCommit/GenStartStopConstraintUC.hpp"

namespace model {

void
GenStartStopConstraintUC::load()
{
	//std::cout<<"r2"<<std::endl;
	for (size_t gen = 0; gen < data.nConventionalGenerators; gen++){
		const int step = 0;
		size_t wasOn = GENERATOR_SUPPORTING == data.convGenPrev[gen].generatorData.status? 1 : 0;
		IloBoolVar &isOn = optData.iloSegmentIsOn[step][gen];
		IloNumVar &start = optData.iloGenStartSupport[step][gen];
		//IloNumVar &stop = optData.iloGenStopSupport[step][gen];

		addRangeConstraint(start >= 0, "StartNonnegative[%d][%d]",step,gen);
		addRangeConstraint(start + wasOn - isOn >= 0, "Start[%d][%d]",step,gen);
		//addRangeConstraint(start + wasOn - 1    <= 0, "StartWasOn[%d][%d]",step,gen);
		//addRangeConstraint(start - wasOn - isOn <= 0, "StayingOff[%d][%d]",step,gen);

		//addRangeConstraint(stop >= 0, "StopNonnegative[%d][%d]",step,gen);
		addRangeConstraint(start + isOn - wasOn >= 0, "Stop[%d][%d]",step,gen);
		//addRangeConstraint(stop + isOn - 1     <= 0, "StartIsOn[%d][%d]",step,gen);
		//addRangeConstraint(stop - isOn - wasOn <= 0, "StartStayingOff[%d][%d]",step,gen);
	}
	for (size_t step = 1; step < data.ucLength; step++){
		for (size_t gen = 0; gen < data.nGenerators; gen++){
			IloBoolVar &wasOn = optData.iloSegmentIsOn[step-1][gen];
			IloBoolVar &isOn  = optData.iloSegmentIsOn[step][gen];
			IloNumVar &start = optData.iloGenStartSupport[step][gen];
			//IloNumVar &stop  = optData.iloGenStopSupport[step][gen];

			addRangeConstraint(start >= 0, "StartNonnegative[%d][%d]",step,gen);
			addRangeConstraint(start + wasOn - isOn >= 0, "Start[%d][%d]",step,gen);
			//addRangeConstraint(start + wasOn - 1    <= 0, "StartWasOn[%d][%d]",step,gen);
			//addRangeConstraint(start - wasOn - isOn <= 0, "StayingOff[%d][%d]",step,gen);

			//addRangeConstraint(stop >= 0, "StopNonnegative[%d][%d]",step,gen);
			addRangeConstraint(start + isOn - wasOn >= 0, "Stop[%d][%d]",step,gen);
			//addRangeConstraint(stop + isOn - 1     <= 0, "StartIsOn[%d][%d]",step,gen);
			//addRangeConstraint(stop - isOn - wasOn <= 0, "StartStayingOff[%d][%d]",step,gen);
		}
	}
}

} /* END OF NAMESPACE model */
