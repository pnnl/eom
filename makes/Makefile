##############################################################################
##############################################################################
###
### RIM Makefile
###		The make file 
###
##############################################################################
##############################################################################

include defs.mk
include cond.mk
include $(UTILITIES)/makes/utilities.mk
include rim.mk

## ---------------------------------------------------------------------------

## ---------------------------------------------------------------------------
# RIM Objects -- REQUIRED FOR ALL SIMULATIONS
rim_objs =                   \
	$(base_objs)             \
	$(parser_objs)           \
	$(sim_objs)              \
	$(model_objs)            \
	$(opt_objs)              \
	$(utility_objs) 

-include $(rim_objs:.o=.d)

$(OBJS)/%.d:

$(OBJS)/%.o:
	$(CXX) -o '$@' -c '$<' $(CFLAGS)
	$(CXX) -MP -MM -MT '$@' -MF '$(@:%.o=%.d)' '$<'  $(CFLAGS)

## ---------------------------------------------------------------------------
## ---------------------------------------------------------------------------
# the reader program is in $(RIM)/support/src/model_reader
## ---------------------------------------------------------------------------
include $(SUPPORT)/makes/support.mk

include $(UNITS)/makes/test.mk
ifeq ($(test),yes)
endif
###############################################################################

rim: $(BIN)/rim

$(BIN)/rim: $(rim_objs) $(OBJS)/rim.o
	$(CPP) -o $(BIN)/rim $(OBJS)/rim.o $(rim_objs) $(CLINK) $(CFLAGS)

$(OBJS)/rim.o : $(SRC)/rim.cpp

## ---------------------------------------------------------------------------
##  Clean up methods
## ---------------------------------------------------------------------------
clean:
	rm -f $(OBJS)/*
	rm -f $(SRC)/*~
	rm -f $(BIN)/rim $(BIN)/triangle $(BIN)/converter
	rm -f $(RIM_PROJECT)/*.csv

parser_check: $(OBJS)/RIMparser.o
