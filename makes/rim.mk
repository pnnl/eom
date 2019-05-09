MODEL_READER_SRC             = $(SUPPORT)/src/model_reader
MODEL_READER_INCL            = $(SUPPORT)/include/model_reader

###############################################################################
###############################################################################
##
## Simulation base functions
##
###############################################################################
###############################################################################
base_objs                    = $(OBJS)/structures.o $(OBJS)/optimization_structs.o $(OBJS)/generic.o

$(OBJS)/structures.o : $(SRC)/structures.cpp
$(OBJS)/optimization_structs.o: $(SRC)/optimization_structs.cpp
$(OBJS)/generic.o: $(SRC)/generic.cpp

###############################################################################
###############################################################################
##
## CSV Interpreter
##
###############################################################################
###############################################################################
parser_objs =                       \
	$(OBJS)/Parser.o                  \
	$(OBJS)/RIMParser.o               \
	$(OBJS)/SimulationParser.o        \
	$(OBJS)/BAParser.o                \
	$(OBJS)/ZoneParser.o              \
	$(OBJS)/GeneratorParser.o         \
	$(OBJS)/LoadParser.o              \
	$(OBJS)/SolarParser.o             \
	$(OBJS)/WindParser.o              \
	$(OBJS)/ZoneIntraConnectParser.o  \
	$(OBJS)/ZoneConnectionParser.o    \
	$(OBJS)/locationParser.o          \
	$(OBJS)/deratingParser.o          \
	$(OBJS)/deratingefParser.o        \
	$(OBJS)/csv.o
	
$(OBJS)/Parser.o                 : $(SRC)/Parser.cpp
$(OBJS)/RIMParser.o              : $(INTERPRETER)/RIMParser.cpp
$(OBJS)/ModelParser.o            : $(INTERPRETER)/ModelParser.cpp
$(OBJS)/SimulationParser.o       : $(INTERPRETER)/SimulationParser.cpp
$(OBJS)/BAParser.o               : $(INTERPRETER)/BAParser.cpp
$(OBJS)/ZoneParser.o             : $(INTERPRETER)/ZoneParser.cpp
$(OBJS)/GeneratorParser.o        : $(INTERPRETER)/GeneratorParser.cpp
$(OBJS)/HydroParser.o            : $(INTERPRETER)/HydroParser.cpp
$(OBJS)/LoadParser.o             : $(INTERPRETER)/LoadParser.cpp
$(OBJS)/SolarParser.o            : $(INTERPRETER)/SolarParser.cpp
$(OBJS)/WindParser.o             : $(INTERPRETER)/WindParser.cpp
$(OBJS)/InterchangeParser.o      : $(INTERPRETER)/InterchangeParser.cpp
$(OBJS)/ZoneConnectionParser.o   : $(INTERPRETER)/ZoneConnectionParser.cpp
$(OBJS)/ZoneIntraConnectParser.o : $(INTERPRETER)/ZoneIntraConnectParser.cpp
$(OBJS)/locationParser.o         : $(INTERPRETER)/locationParser.cpp
$(OBJS)/deratingParser.o         : $(INTERPRETER)/deratingParser.cpp
$(OBJS)/deratingefParser.o       : $(INTERPRETER)/deratingefParser.cpp
$(OBJS)/csv.o: $(SRC)/csv.cpp

###############################################################################
###############################################################################
##
## Simulation Objects
##
###############################################################################
###############################################################################
sim_objs                   =         \
	$(OBJS)/Scheduler.o                \
	$(OBJS)/SDInterface.o              \
	$(OBJS)/Clock.o                    \

$(OBJS)/Scheduler.o : $(SIMULATION)/Scheduler.cpp
$(OBJS)/SDInterface.o : $(SIMULATION)/SDInterface.cpp
$(OBJS)/Clock.o : $(SIMULATION)/Clock.cpp

###############################################################################
###############################################################################
##
## General Modeling Objects
##
###############################################################################
###############################################################################
model_objs                 =        \
	$(OBJS)/RIMModel.o                \
	$(OBJS)/ScheduleData.o            \
	$(OBJS)/AdjLoadTS.o               \
	$(OBJS)/location.o                \
	$(OBJS)/weather.o                 \
	$(OBJS)/universe.o                \

$(OBJS)/RIMModel.o : $(MODELS)/RIMModel.cpp

$(OBJS)/ScheduleData.o : $(TIME_SERIES)/ScheduleData.cpp

$(OBJS)/AdjLoadTS.o : $(TIME_SERIES)/AdjLoadTS.cpp

$(OBJS)/location.o : $(SRC)/location.cpp
$(OBJS)/weather.o: $(SRC)/weather.cpp
$(OBJS)/universe.o: $(SRC)/universe.cpp

###############################################################################
###############################################################################
##
## Model Objects
##
###############################################################################
###############################################################################
model_objs +=                        \
	$(OBJS)/BalancingAuthority.o       \
	$(OBJS)/Zone.o                     \
	$(OBJS)/Generator.o                \
	$(OBJS)/ConventionalGenerator.o    \

$(OBJS)/BalancingAuthority.o : $(ADMINISTRATIVE)/BalancingAuthority.cpp

$(OBJS)/Zone.o : $(ADMINISTRATIVE)/Zone.cpp

$(OBJS)/Generator.o : $(MODEL_ABSTRACTION)/Generator.cpp

$(OBJS)/ConventionalGenerator.o : $(GENERATION)/ConventionalGenerator.cpp

$(OBJS)/HydroGenerator.o : $(GENERATION)/HydroGenerator.cpp

###############################################################################
###############################################################################
##
## Optimization Objects, general optimization
##
###############################################################################
###############################################################################

optimizers =                      \
	$(OBJS)/UnitCommitOptimizer.o   \
	$(OBJS)/UCOptConfigure.o        \
	$(OBJS)/LoadFollowingOpt.o      \
	$(OBJS)/LFOptConfigure.o        \
	$(OBJS)/RegulationOpt.o         \
	$(OBJS)/RegOptConfigure.o       \

obj_func  =                       \
	$(OBJS)/MinimizeCostUC.o        \
	$(OBJS)/MinimizeCostLF.o        \
#	$(OBJS)/MinimizeCostReg.o       \

unit_commit_constraints =                  \
	$(OBJS)/PowerBalanceConstraintUC.o       \
	$(OBJS)/GenStartStopConstraintUC.o       \
	$(OBJS)/MinUpDownConstraintUC.o          \
	$(OBJS)/PowerLimitConstraintUC.o         \
	$(OBJS)/RampConstraintUC.o               \
	$(OBJS)/ReserveConstraintUC.o            \
	$(OBJS)/InterzoneMaxLinePowerUC.o        \
	$(OBJS)/InterzonePowerTransferDistributionFactorUC.o \
	$(OBJS)/InterzoneTransportConstraintUC.o \
	$(OBJS)/ZonePowerBalanceConstraintUC.o   \
	$(OBJS)/DeratingConstraintUC.o           \
#	$(OBJS)/InterZoneLineConstraintUC.o  \
#	$(OBJS)/InterZonePTDFConstraintUC.o  \


load_following_constraints =             \
	$(OBJS)/PowerBalanceConstraintLF.o     \
	$(OBJS)/RampConstraintLF.o             \
	$(OBJS)/IntraZoneLineConstraintLF.o    \
	$(OBJS)/IntraZonePowerInConstraintLF.o \
	$(OBJS)/ZonePowerBalanceConstraintLF.o \

regulation_constraints =               \
#	$(OBJS)/PowerBalanceConstraintReg.o  \
	$(OBJS)/RampConstraintReg.o          \

opt_objs =                             \
	$(OBJS)/UnitCommitmentConstraint.o   \
	$(optimizers)                        \
	$(obj_func)                          \
	$(unit_commit_constraints)           \
	$(load_following_constraints)        \
#	$(regulation_constraints)            \

###############################################################################
## OPTIMIZERS

$(OBJS)/UnitCommitOptimizer.o : $(OPTIMIZATION)/UnitCommitOptimizer.cpp

$(OBJS)/LoadFollowingOpt.o : $(OPTIMIZATION)/LoadFollowingOpt.cpp

$(OBJS)/RegulationOpt.o : $(OPTIMIZATION)/RegulationOpt.cpp

$(OBJS)/UCOptConfigure.o : $(OPTIMIZER)/UCOptConfigure.cpp

$(OBJS)/LFOptConfigure.o : $(OPTIMIZER)/LFOptConfigure.cpp

$(OBJS)/RegOptConfigure.o : $(OPTIMIZER)/RegOptConfigure.cpp

$(OBJS)/UnitCommitmentConstraint.o : $(SRC)/optimizers/UnitCommitmentConstraint.cpp

## OBJECTIVE FUNCTIONS

$(OBJS)/MinimizeCostUC.o : $(OBJECTIVES)/MinimizeCostUC.cpp

$(OBJS)/MinimizeCostLF.o : $(OBJECTIVES)/MinimizeCostLF.cpp

$(OBJS)/MinimizeCostReg.o : $(OBJECTIVES)/MinimizeCostReg.cpp

## CONSTRAINTS FUNCTIONS -- UNIT COMMIT

$(OBJS)/MinUpDownConstraintUC.o : $(CONSTRAINT_UC)/MinUpDownConstraintUC.cpp

$(OBJS)/PowerBalanceConstraintUC.o : $(CONSTRAINT_UC)/PowerBalanceConstraintUC.cpp

$(OBJS)/PowerLimitConstraintUC.o : $(CONSTRAINT_UC)/PowerLimitConstraintUC.cpp

$(OBJS)/RampConstraintUC.o : $(CONSTRAINT_UC)/RampConstraintUC.cpp

$(OBJS)/ReserveConstraintUC.o : $(CONSTRAINT_UC)/ReserveConstraintUC.cpp

$(OBJS)/GenStartStopConstraintUC.o : $(CONSTRAINT_UC)/GenStartStopConstraintUC.cpp

$(OBJS)/InterzoneMaxLinePowerUC.o : $(CONSTRAINT_UC)/InterzoneMaxLinePowerUC.cpp

$(OBJS)/InterzoneTransportConstraintUC.o : $(CONSTRAINT_UC)/InterzoneTransportConstraintUC.cpp

$(OBJS)/InterzonePowerTransferDistributionFactorUC.o : $(CONSTRAINT_UC)/InterzonePowerTransferDistributionFactorUC.cpp

$(OBJS)/ZonePowerBalanceConstraintUC.o : $(CONSTRAINT_UC)/ZonePowerBalanceConstraintUC.cpp

$(OBJS)/InterZoneLineConstraintUC.o : $(CONSTRAINT_UC)/InterZoneLineConstraintUC.cpp
	
$(OBJS)/InterZonePTDFConstraintUC.o : $(CONSTRAINT_UC)/InterZonePTDFConstraintUC.cpp
	
$(OBJS)/DeratingConstraintUC.o : $(CONSTRAINT_UC)/DeratingConstraintUC.cpp

## CONSTRAINTS FUNCTIONS -- LOAD FOLLOWING

$(OBJS)/PowerBalanceConstraintLF.o : $(CONSTRAINT_LF)/PowerBalanceConstraintLF.cpp

$(OBJS)/RampConstraintLF.o : $(CONSTRAINT_LF)/RampConstraintLF.cpp

$(OBJS)/IntraZoneLineConstraintLF.o : $(CONSTRAINT_LF)/IntraZoneLineConstraintLF.cpp

$(OBJS)/IntraZonePowerInConstraintLF.o : $(CONSTRAINT_LF)/IntraZonePowerInConstraintLF.cpp

$(OBJS)/ZonePowerBalanceConstraintLF.o : $(CONSTRAINT_LF)/ZonePowerBalanceConstraintLF.cpp

## CONSTRAINTS FUNCTIONS -- REGULATION

$(OBJS)/PowerBalanceConstraintReg.o : $(CONSTRAINT_REG)/PowerBalanceConstraintReg.cpp

$(OBJS)/RampConstraintReg.o : $(CONSTRAINT_REG)/RampConstraintReg.cpp

###############################################################################
###############################################################################
##
## Results interpreter
##
###############################################################################
###############################################################################
READER                       = $(SUPPORT)/src/model_reader
READER_INCL                  = $(SUPPORT)/include/model_reader

result_interpreter : $(rim_objs) $(INTERPRETER)/ResultInterpreter.cpp
	$(CXX) -o $(BIN)/resultInterpreter $(INTERPRETER)/ResultInterpreter.cpp $(rim_objs) $(CLINK) $(CFLAGS)

