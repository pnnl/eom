
###############################################################################
###############################################################################
##
## Optimization objects, requires cplex
##
###############################################################################
###############################################################################

opt_objs   +=                       \
	$(OBJS)/UCOptConfigure.o        \
	$(OBJS)/LFOptConfigure.o        \
	$(OBJS)/RegOptConfigure.o       \

###############################################################################
## Optimization Objects
$(OBJS)/UCOptConfigure.o : $(OPT_TYPE)/UCOptConfigure.cpp
	g++ -o $(OBJS)/UCOptConfigure.o -c $(OPT_TYPE)/UCOptConfigure.cpp $(CFLAGS)

$(OBJS)/LFOptConfigure.o : $(OPT_TYPE)/LFOptConfigure.cpp
	g++ -o $(OBJS)/LFOptConfigure.o -c $(OPT_TYPE)/LFOptConfigure.cpp $(CFLAGS)

$(OBJS)/RegOptConfigure.o : $(OPT_TYPE)/RegOptConfigure.cpp
	g++ -o $(OBJS)/RegOptConfigure.o -c $(OPT_TYPE)/RegOptConfigure.cpp $(CFLAGS)

## ---------------------------------------------------------------------------
##  END OF OPTIMIZATION INCLUDE
## ---------------------------------------------------------------------------
## ---------------------------------------------------------------------------
## ---------------------------------------------------------------------------
