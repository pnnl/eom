##############################################################################
##############################################################################
## Pre-compile defines for testing
##
## TEST_ALL               -
## RIM_DEBUG              -
## SIMULATION_TEST        -
##
## INCLUDE_HYDRO          -
## INCLUDE_STORAGE        -
## INCLUDE_WIND           -
## INCLUDE_SOLAR          -
## INCLUDE_DEMANDRESPONSE -
## INCLUDE_REPLICATE      -
## INCLUDE_CPLEX          -
##
##############################################################################
##############################################################################
all : TestHarness

TESTING_INCL                 = $(TESTING)/include
TESTING_SRC                  = $(TESTING)/src

UNIT_TESTS                   = $(TESTING_SRC)/unit_tests
UNIT_TESTS_INCL              = $(TESTING_INCL)/unit_tests

FILE_TESTS                   = $(UNIT_TESTS)/storage
FILE_TESTS_INCL              = $(UNIT_TESTS_INCL)/storage

INTERP_TESTS                 = $(UNIT_TESTS)/interpreterTests
INTERP_TESTS_INCL            = $(UNIT_TESTS_INCL)/interpreterTests

CONFIG_TESTS                 = $(UNIT_TESTS)/configurationTests
CONFIG_TESTS_INCL            = $(UNIT_TESTS_INCL)/configurationTests

CFLAGS                      += \
	-I$(TESTING_INCL)          \
	-I$(INTERP_TESTS_INCL)     \
	-I$(CONFIG_TESTS_INCL)

$(OBJS)/Validation.o : $(TESTING_SRC)/Validation.cpp
	$(CPP) -o $(OBJS)/Validation.o -c $(TESTING_SRC)/Validation.cpp    \
	$(CFLAGS) $(DEBUG)

##############################################################################
##############################################################################
##
## UNIT TESTS
##
##############################################################################
##############################################################################
## File tests

unit_tests: fileTest parserTest

test_src = $(OBJS)/Validation.o

## File Test ###################################################
fileTest :
	$(MAKE) file_test debug=yes

file_test : $(BIN)/file_test

$(BIN)/file_test : $(rim_objs) $(OBJS)/FileTest.o $(test_src)
	$(CPP) -o $(BIN)/file_test $(test_src) $(rim_objs) $(OBJS)/FileTest.o   \
	$(DEBUG)

$(OBJS)/FileTest.o : $(FILE_TESTS)/FileTest.cpp
	$(CPP) -o $(OBJS)/FileTest.o -c $(FILE_TESTS)/FileTest.cpp    \
	$(CFLAGS) $(DEBUG)

## Parser Test ###################################################

PARSER_TESTS                 = $(INTERP_TESTS)/parser_tests
PARSER_TESTS_INCL            = $(INTERP_TESTS_INCL)/parser_tests

parserTest :
	$(MAKE) parser_test optimizer=no_opt model=base debug=yes

parser_test : $(BIN)/parser_test

parser_test_objs   = \
	$(test_src)                              \
	$(OBJS)/ParserTest.o                     \
	$(OBJS)/BAParserTest.o                     \

$(BIN)/parser_test : $(rim_objs) $(parser_test_objs) 
	$(CPP) -o $(BIN)/parser_test             \
	$(rim_objs)                           \
	$(parser_test_objs)                      \
	$(DEBUG)

$(OBJS)/ParserTest.o : $(INTERP_TESTS)/ParserTest.cpp
	$(CPP) -o $(OBJS)/ParserTest.o -c $(INTERP_TESTS)/ParserTest.cpp    \
	$(CFLAGS) $(DEBUG)
## Parser Test ###################################################

clockTest :
	$(MAKE) clock_test optimizer=no_opt model=base debug=yes

clock_test : $(BIN)/clock_test

$(BIN)/clock_test : $(OBJS)/ClockTest.o $(rim_objs) $(test_src)
	$(CPP) -o $(BIN)/clock_test $(OBJS)/ClockTest.o                       \
	$(rim_objs) $(test_src)                \
	$(DEBUG)

$(OBJS)/ClockTest.o : $(CONFIG_TESTS)/ClockTest.cpp
	$(CPP) -o $(OBJS)/ClockTest.o -c $(CONFIG_TESTS)/ClockTest.cpp           \
	$(CFLAGS) $(DEBUG)

sdiTest :
	$(MAKE) sdi_test model=base debug=yes optimizer=no_opt

cplexTest :
	$(MAKE) sdi_test optimizer=cplex model=base objective=single debug=yes

sdi_test : $(BIN)/sdi_test

$(BIN)/sdi_test : $(OBJS)/SDITest.o $(rim_objs) $(test_src)
	$(CPP) -o $(BIN)/sdi_test $(OBJS)/SDITest.o               \
	$(rim_objs) $(test_src)                                         \
	$(DEBUG) $(CLINK)

$(OBJS)/SDITest.o : $(CONFIG_TESTS)/SDITest.cpp
	$(CPP) -o $(OBJS)/SDITest.o -c $(CONFIG_TESTS)/SDITest.cpp           \
	$(CFLAGS) $(DEBUG) -DCOMPRESSED_TIME


resetTests:
	rm -f $(RIM_PROJECT)/testing/data/fileTests/*  \
	rm -f $(RIM_PROJECT)/testing/data/parserTests/*  \
	rm -f $(RIM_PROJECT)/testing/results/parserTests/*  \
	rm -f $(RIM_PROJECT)/testing/results/results.log

