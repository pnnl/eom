##############################################################################
##############################################################################
###
### Definitions for general support
###
### 
###
##############################################################################
##############################################################################

## +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
## +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
## INTERPRETER SECTION
UTILITIES_INCL               = $(UTILITIES)/include
UTILITIES_SRC                = $(UTILITIES)/src
UTILITIES_OS_SRC             = $(UTILITIES)/src/linux
## ---------------------------------------------------------------------------
##  Interpreter selection flags
ifeq ($(operating_system),mac)
UTILITIES_OS_SRC             = $(UTILITIES)/src/mac
endif

ifeq ($(operating_system),windows)
UTILITIES_OS_SRC             = $(UTILITIES)/src/windows
endif
## ---------------------------------------------------------------------------

CFLAGS                      += -I$(UTILITIES_INCL)

utility_objs                 =     \
	$(OBJS)/FileControl.o          \


$(OBJS)/DirectoryControl.o : $(UTILITIES_OS_SRC)/DirectoryControl.cpp

$(OBJS)/FileControl.o : $(UTILITIES_OS_SRC)/FileControl.cpp
