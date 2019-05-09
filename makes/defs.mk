##############################################################################
##############################################################################
###
### Makefile definitions.
###
### This file contains the definitions of all compilation, directory and 
### library macros. It is included as the first element of the main Makefile 
### as every part of the make structure relies on these data.
### 
###
##############################################################################
##############################################################################

CXX                          = g++
CPP                          = g++
CFLAGS                       = -Wall
CLINK                        = -L/opt/local/lib -lm -lboost_date_time
#CLINK                        = -L/opt/local/lib -L/usr/lib/x86_64-linux-gnu -lm -L/Users/d3p341/electric-fence-2.1.13/ -lefence
#CLINK                        = -lm

# This sets the RIM project macro to the current working directory if it is 
# not present in current environment
RIM_PROJECT ?= $(dir $(shell pwd))
HOSTNAME=$(shell hostname)
# This sets the macro indicating that RIM is being made on superdome.
ifeq ($(HOSTNAME),superdome.pnl.gov)
	COMPILE_ON_SUPERDOME?=yes
	CLINK                      = -L/opt/local/lib -lm -lboost_date_time
else
	COMPILE_ON_SUPERDOME?=no
endif

#-----------------------------------------------------------------------------
# The user must set an environment variable RIM_PROJECT to the root of the RIM
# directory.
#-----------------------------------------------------------------------------
RIM                          = ${RIM_PROJECT}

OBJS                         = $(RIM)objs
BIN                          = $(RIM)bin
LIB                          = $(RIM)lib

## +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
## +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
## Include file directories
## ---------------------------------------------------------------------------
## Exceptions and simulation are complete components, the interpreters, models
## and optimizers are specific to the users requests. The make descriptions
## are held in separate .mk files
INCLUDE                      = $(RIM)include

INCL_EXCEPTIONS              = $(INCLUDE)/exceptions
INCL_MODELS                  = $(INCLUDE)/models
INCL_OPTIMIZERS              = $(INCLUDE)/optimizers
INCL_SIMULATION              = $(INCLUDE)/simulation

INCL_TIMESERIES              = $(INCL_MODELS)/timeSeries
INCL_OPTIMIZATION            = $(INCL_OPTIMIZERS)/optimization

## +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


## +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
## +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
## Source code directories
## Exceptions and simulation are complete components, the interpreters, models
## and optimizers are specific to the users requests. The make descriptions
## are held in separate .mk files
SRC                          = $(RIM)src

MODELS                       = $(SRC)/models
OPTIMIZERS                   = $(SRC)/optimizers
SIMULATION                   = $(SRC)/simulation

TIME_SERIES                  = $(MODELS)/timeSeries
OPTIMIZATION                 = $(OPTIMIZERS)/optimization
## +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

SUPPORT                      = $(RIM)support
TESTING                      = $(RIM)testing
UNITS                        = $(RIM)units
UTILITIES                    = $(RIM)utilities

RESULTS                      = $(RIM)results

DATA                         = $(RIM)data
DOCS                         = $(RIM)docs

CFLAGS                      += -isystem /opt/local/include -I$(INCLUDE)

