## ---------------------------------------------------------------------------
## CPLEX Conditional macros
## ---------------------------------------------------------------------------


CFLAGS                      += -DCPLEX_SET

#------------------------------------------------------------
# CPLEX
#------------------------------------------------------------
ifeq ($(COMPILE_ON_SUPERDOME),yes)
	SYSTEM                   = ia64_sles10.0_4.1
#	CLINK                   += -lefence
else
	SYSTEM                   = x86-64_linux
endif

LIBFORMAT                    = static_pic

#------------------------------------------------------------
#
# When you adapt this makefile to compile your CPLEX programs
# please copy this makefile and set CPLEXDIR and CONCERTDIR to
# the directories where CPLEX and CONCERT are installed.
#
#------------------------------------------------------------

CPLEXDIR                     = /opt/cplex
CONCERTDIR                   = /opt/concert
PATHWIDTH                    = 64

ifeq ($(OS),macos)
SYSTEM                       = x86_darwin8_gcc4.0
CPLEXDIR                     = /Users/kevin/Desktop/cplex/cplex121_0.darwin/cplex121
CONCERTDIR                   = /Users/kevin/Desktop/cplex/cplex121_0.darwin/concert29
endif
ifeq ($(HOSTNAME),lcarlsen-mac8)
SYSTEM     = x86-64_darwin8_gcc4.0
CPLEXDIR   = /usr/ilog/cplex121
CONCERTDIR = /usr/ilog/concert29
CLINK     += -framework CoreFoundation -framework IOKit -Wl,-no_compact_unwind
PATHWIDTH  = 64
endif

# ---------------------------------------------------------------------
# Compiler options 
# ---------------------------------------------------------------------
CPLEXBINDIR                  = $(CPLEXDIR)/bin/$(BINDIST)
CPLEXJARDIR                  = $(CPLEXDIR)/lib/cplex.jar
CPLEXLIBDIR                  = $(CPLEXDIR)/lib/$(SYSTEM)/$(LIBFORMAT)
CONCERTLIBDIR                = $(CONCERTDIR)/lib/$(SYSTEM)/$(LIBFORMAT)

CCOPT                        = -DIL_STD
COPT                         = -DIL_STD
CCLNFLAGS                    = -L$(CPLEXLIBDIR) -lilocplex -lcplex -L$(CONCERTLIBDIR) -lconcert -lm -pthread
CLNFLAGS                     = -L$(CPLEXLIBDIR) -lcplex -lm -pthread

ifeq ($(COMPILE_ON_SUPERDOME),yes)
else
ifeq ($(PATHWIDTH),64)
	CCOPT                += -m64
	COPT                 += -m64
	CCLNFLAGS            += -m64
	CLNFLAGS             += -m64
else
## default is a 32 bit path. It is not clear that the library does not 
## automatically use anywhere but superdome so the 64 bit version is 
## commented out
	CCOPT                += -m32
	COPT                 += -m32
	CCLNFLAGS            += -m32
	CLNFLAGS             += -m32
endif
endif

JOPT                         = -classpath $(CPLEXDIR)/lib/cplex.jar -O

# ---------------------------------------------------------------------
# Link options and libraries
# ---------------------------------------------------------------------

JAVA                         = java -Djava.library.path=$(CPLEXDIR)/bin/x86_debian4.0_4.1 -classpath $(CPLEXJARDIR):

CONCERTINCDIR                = $(CONCERTDIR)/include
CPLEXINCDIR                  = $(CPLEXDIR)/include

#------------------------------------------------------------
# CPLEX DONE
#------------------------------------------------------------
## cplex uses strstream.h and other deprecated headers so
## we have to include backward compatibility
CFLAGS                      += -isystem $(CPLEXINCDIR) -isystem $(CONCERTINCDIR) $(CCOPT)
CLINK                       += $(CCOPT) $(CCLNFLAGS)
