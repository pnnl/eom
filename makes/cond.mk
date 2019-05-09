##############################################################################
##############################################################################
###
### Makefile conditional compilations
###
###
##############################################################################
##############################################################################
##  Debug flags
ifeq ($(debug),yes)
CFLAGS                      += -g
endif

ifeq ($(verbose),yes)
CFLAGS                      += -DVERBOSE
CFLAGS                      += -DCHECK_OPT
endif

ifeq ($(time), compressed)
CFLAGS                       = -DCOMPRESSED_TIME
endif


## +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
## +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
## Link include directory set
## ---------------------------------------------------------------------------
## 
INCL_MODEL_SETS              = $(INCLUDE)/codeSets/modelSet
INCL_INTERP_SETS             = $(INCLUDE)/codeSets/interpreterSet
INCL_OPT_SETS                = $(INCL_OPTIMIZERS)/optimizerSet

INCL_CPLEX_OPTIMIZER         = $(INCL_OPT_SETS)/cplex
INCL_NOOPT_OPTIMIZER         = $(INCL_OPT_SETS)/no_opt

## +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
## +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
## Link source directory set
## ---------------------------------------------------------------------------
## 
MODEL_SETS                   = $(SRC)/codeSets/modelSet
INTERP_SETS                  = $(SRC)/codeSets/interpreterSet
OPT_SETS                     = $(OPTIMIZERS)/optimizerSet

CPLEX_OPTIMIZER              = $(OPT_SETS)/cplex
NOOPT_OPTIMIZER              = $(OPT_SETS)/no_opt

INCL_OBJECTIVES              = $(INCL_NOOPT_OPTIMIZER)/objectives
OBJECTIVES                   = $(NOOPT_OPTIMIZER)/objectives

## +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
## +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
## Optimizer selection
## +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
## 
INCL_OPTIMIZER               = $(INCL_NOOPT_OPTIMIZER)
OPTIMIZER                    = $(NOOPT_OPTIMIZER)

ifeq ($(optimizer),cplex)
include optimizers/cplex.mk
INCL_OPTIMIZER               = $(INCL_CPLEX_OPTIMIZER)
OPTIMIZER                    = $(CPLEX_OPTIMIZER)
endif 
## +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
## 
INCL_CONSTRAINT              = $(INCL_OPTIMIZER)/constraints

INCL_CONSTRAINT_UC           = $(INCL_CONSTRAINT)/unitCommit
INCL_CONSTRAINT_LF           = $(INCL_CONSTRAINT)/loadFollowing
INCL_CONSTRAINT_REG          = $(INCL_CONSTRAINT)/regulation

CONSTRAINT                   = $(OPTIMIZER)/constraints

CFLAGS                      += 
CONSTRAINT_UC                = $(CONSTRAINT)/unitCommit
CONSTRAINT_LF                = $(CONSTRAINT)/loadFollowing
CONSTRAINT_REG               = $(CONSTRAINT)/regulation

## +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
## Objective function selection
## 

ifeq ($(optimizer),cplex)
## Default is linear
	INCL_OBJECTIVES          = $(INCL_CPLEX_OPTIMIZER)/objectives
	OBJECTIVE_SET            = $(CPLEX_OPTIMIZER)/objectiveSet
	OBJECTIVES               = $(OBJECTIVE_SET)/linear
ifeq ($(obj_type),single)
		OBJECTIVES           = $(OBJECTIVE_SET)/single
endif
ifeq ($(obj_type),multiple)
		OBJECTIVES           = $(OBJECTIVE_SET)/multiple
endif
endif
## +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
## +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
## Model selection
## +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
## 
INCL_MODEL                   = $(INCL_MODEL_SETS)/base
MODEL                        = $(MODEL_SETS)/base

INCL_INTERPRETER             = $(INCL_INTERP_SETS)/base
INTERPRETER                  = $(INTERP_SETS)/base
## +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
INCL_MODEL_ABSTRACTION       = $(MODEL_INCL)/modelAbs
INCL_ADMINISTRATIVE          = $(MODEL_INCL)/administrative
INCL_GENERATION              = $(MODEL_INCL)/generation

MODEL_ABSTRACTION            = $(MODEL)/modelAbs
ADMINISTRATIVE               = $(MODEL)/administrative
GENERATION                   = $(MODEL)/generation

CFLAGS                      +=   \
	-I$(INCL_OPTIMIZER)          \
	-I$(INCL_OBJECTIVES)         \
	-I$(INCL_MODEL)              \
	-I$(INCL_INTERPRETER)        \

## +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# end of cplex question

