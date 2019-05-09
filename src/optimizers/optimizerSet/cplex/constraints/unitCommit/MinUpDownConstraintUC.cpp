#include "constraints/unitCommit/MinUpDownConstraintUC.hpp"

using std::max;
using std::min;

namespace model {

// NOTE: for now, we are only concerned with the onOff schedule's
// switching times which correspond to GENERATOR_SUPPORTING (on)
// and GENERATOR_RAMPDOWN (off)
void
MinUpDownConstraintUC::load()
{
	//std::cout<<"r6"<<std::endl;
	for (size_t gen = 0; gen < data.nGenerators; gen++) {
		//std::cout<<gen<<std::endl;

		int minDown = data.minDownTime[gen];
		int minUp = data.minUpTime[gen];
		int timeToSwitch = 0;
		int initialWasOn = GENERATOR_SUPPORTING == data.convGenPrev[gen].generatorData.status? 1 : 0;
		if (initialWasOn)
			timeToSwitch = minUp - data.convGenPrev[gen].generatorData.timeInState - 1;
		else
			timeToSwitch = minDown - data.convGenPrev[gen].generatorData.timeInState - 1;
		int gj = max(0,min((int)data.ucLength, timeToSwitch));
		IloNumExpr initialStepsOn(optData.control.iloEnv);
		if (gj>0) {
			if (gj>data.ucLength) {
				gj=data.ucLength;
			}
			for (int step = 0; step < gj; step++) {
				initialStepsOn += optData.iloSegmentIsOn[step][gen];
			}

			if (initialWasOn)
				if (minUp>1){
					addRangeConstraint(initialStepsOn - gj == 0,"initialTimeInState[%d]",gen);
				}
				else
					if (minDown>1){
						addRangeConstraint(initialStepsOn == 0,"initialTimeInState[%d]",gen);
					}
		}


		if (gj < data.ucLength-1) {
			if (gj ==0){
				IloBoolVar &isOn = optData.iloSegmentIsOn[0][gen];
				double wasOn = initialWasOn;
				IloNumExpr stepsOn(optData.control.iloEnv);
				IloNumExpr stepsOff(optData.control.iloEnv);
				if (minUp>1){
					if (minUp<= data.ucLength) {
						for (int offset = 0; offset < minUp; offset++) {
							stepsOn += optData.iloSegmentIsOn[offset][gen];
						}
						addRangeConstraint(stepsOn - minUp*(isOn-wasOn) >= 0,"minUp[%d][%d]",0,gen);
					} else {
						for (int offset = 0; offset < data.ucLength; offset++) {
							stepsOn += optData.iloSegmentIsOn[offset][gen];
						}
						addRangeConstraint(stepsOn - minUp*(isOn-wasOn) >= 0,"minUp[%d][%d]",0,gen);
					}
				}
				if (minDown>1){
					if (minDown<= data.ucLength) {

						for (int offset = 0; offset < minDown; offset++) {
							stepsOff += 1 - optData.iloSegmentIsOn[offset][gen];
						}
						addRangeConstraint(stepsOff + minDown*(isOn-wasOn) >= 0,"minDown[%d][%d]",0,gen);
					} else {
						for (int offset = 0; offset < data.ucLength; offset++) {
							stepsOff += 1 - optData.iloSegmentIsOn[offset][gen];
						}
						addRangeConstraint(stepsOff + minDown*(isOn-wasOn) >= 0,"minDown[%d][%d]",0,gen);
					}
				}
				gj=1;

			}
			//now gj is no longer 0, we can start the for cycle, but need be careful

			int decup=data.ucLength - minUp;
			int decdown=data.ucLength - minDown;
			if (minUp>1){
				if (gj <= data.ucLength - minUp) {

					for (int step = gj; step < decup; step++) {
						IloBoolVar &isOn = optData.iloSegmentIsOn[step][gen];
						IloBoolVar &wasOn = optData.iloSegmentIsOn[step-1][gen];
						IloNumExpr stepsOn(optData.control.iloEnv);
						for (int offset = 0; offset < minUp; offset++) {
							stepsOn += optData.iloSegmentIsOn[step+offset][gen];

						}
						addRangeConstraint(stepsOn - minUp*(isOn-wasOn) >= 0,"minUp[%d][%d]",step,gen);
					}
				} else if (gj > decup ) {
					for (int step = gj; step < data.ucLength; step++) {
						IloBoolVar &isOn = optData.iloSegmentIsOn[step][gen];
						IloBoolVar &wasOn = optData.iloSegmentIsOn[step-1][gen];
						IloNumExpr stepsOn(optData.control.iloEnv);
						for (int offset = 0; offset < data.ucLength-step; offset++) {
							stepsOn += optData.iloSegmentIsOn[step+offset][gen]- optData.iloSegmentIsOn[step][gen]+optData.iloSegmentIsOn[step-1][gen];

						}
						addRangeConstraint(stepsOn >= 0,"minUp[%d][%d]",step,gen);
					}
				}
			}
			if (minDown>1){
				if (gj <= decdown) {
					for (int step = gj; step < data.ucLength - minDown; step++) {
						IloBoolVar &isOn = optData.iloSegmentIsOn[step][gen];
						IloBoolVar &wasOn = optData.iloSegmentIsOn[step-1][gen];
						IloNumExpr stepsOff(optData.control.iloEnv);
						for (int offset = 0; offset < minDown; offset++) {
							stepsOff += 1 - optData.iloSegmentIsOn[step+offset][gen];
						}
						addRangeConstraint(stepsOff + minDown*(isOn-wasOn) >= 0,"minDown[%d][%d]",step,gen);
					}
				} else if (gj > decdown) {
					for (int step = gj; step < data.ucLength; step++) {
						IloBoolVar &isOn = optData.iloSegmentIsOn[step][gen];
						IloBoolVar &wasOn = optData.iloSegmentIsOn[step-1][gen];
						IloNumExpr stepsOff(optData.control.iloEnv);
						for (int offset = 0; offset < data.ucLength-step ; offset++) {
							stepsOff += 1 - optData.iloSegmentIsOn[step+offset][gen]+ optData.iloSegmentIsOn[step][gen]-optData.iloSegmentIsOn[step-1][gen];
						}
						addRangeConstraint(stepsOff >= 0,"minDown[%d][%d]",step,gen);
					}
				}
			}
			if (minUp>1){
				if (gj <= decup ) {
					for (int step = data.ucLength-minUp; step < data.ucLength; step++) {
						IloBoolVar &isOn = optData.iloSegmentIsOn[step][gen];
						IloBoolVar &wasOn = optData.iloSegmentIsOn[step-1][gen];
						IloNumExpr stepsOn(optData.control.iloEnv);
						for (int offset = 0; offset < data.ucLength-step; offset++) {
							stepsOn += optData.iloSegmentIsOn[step+offset][gen]- optData.iloSegmentIsOn[step][gen]+optData.iloSegmentIsOn[step-1][gen];

						}
						addRangeConstraint(stepsOn  >= 0,"minUp[%d][%d]",step,gen);
					}
				}
			}
			if (minDown>1){
				if (gj <= decdown) {
					for (int step = data.ucLength-minDown; step < data.ucLength; step++) {
						IloBoolVar &isOn = optData.iloSegmentIsOn[step][gen];
						IloBoolVar &wasOn = optData.iloSegmentIsOn[step-1][gen];
						IloNumExpr stepsOff(optData.control.iloEnv);
						for (int offset = 0; offset < data.ucLength-step; offset++) {
							stepsOff += 1 - optData.iloSegmentIsOn[step+offset][gen]+ optData.iloSegmentIsOn[step][gen]-optData.iloSegmentIsOn[step-1][gen];
						}
						addRangeConstraint(stepsOff >= 0,"minDown[%d][%d]",step,gen);
					}
				}
			}
		}

	}
	return;
}

} /* END OF NAMESPACE model */
