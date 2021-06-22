
MODEL_TESTS                 = $(UNIT_TESTS)/modelTests

all_model_tests: modelTest modelTest1 modelTest2 modelTest3 modelTest4

modelTests :                 
	modelTest              \
	modelTest1             \
	modelTest2             \
	modelTest3             \
	modelTest4             \
	modelTest5             \


$(OBJS)/modelTest.o : $(MODEL_TESTS)/modelTest.cpp
	$(CPP) -o $(OBJS)/modelTest.o -c $(MODEL_TESTS)/modelTest.cpp $(CFLAGS)
###########################################################################
######################### Model test
###########################################################################
modelTest : 
	$(MAKE) model_test debug=yes

model_test :                              \
	$(BIN)/model_test

$(BIN)/model_test : $(rim_objs) $(OBJS)/TestModel.o $(OBJS)/modelTest.o
	$(CPP) -o $(BIN)/model_test $(rim_objs) $(OBJS)/TestModel.o $(OBJS)/modelTest.o $(CLINK)

$(OBJS)/TestModel.o : $(MODEL_TESTS)/TestModel.cpp
	$(CPP) -o $(OBJS)/TestModel.o -c $(MODEL_TESTS)/TestModel.cpp $(CFLAGS)

######################### Model test
modelTest1 : 
	$(MAKE) model_test1 debug=yes optimizer=no_opt time=compressed test=yes

model_test1 : $(BIN)/model_test1

$(BIN)/model_test1 : $(rim_objs) $(OBJS)/TestModel1.o $(OBJS)/modelTest.o
	$(CPP) -o $(BIN)/model_test1 $(rim_objs) $(OBJS)/TestModel1.o $(OBJS)/modelTest.o $(CLINK)

$(OBJS)/TestModel1.o : $(MODEL_TESTS)/TestModel1.cpp
	$(CPP) -o $(OBJS)/TestModel1.o -c $(MODEL_TESTS)/TestModel1.cpp $(CFLAGS)

######################### Model test 2
modelTest2 : 
	$(MAKE) model_test2 debug=yes optimizer=no_opt time=compressed test=yes

model_test2 : $(BIN)/model_test2

$(BIN)/model_test2 : $(rim_objs) $(OBJS)/TestModel2.o $(OBJS)/modelTest.o
	$(CPP) -o $(BIN)/model_test2 $(rim_objs) $(OBJS)/TestModel2.o $(OBJS)/modelTest.o $(CLINK)

$(OBJS)/TestModel2.o : $(MODEL_TESTS)/TestModel2.cpp
	$(CPP) -o $(OBJS)/TestModel2.o -c $(MODEL_TESTS)/TestModel2.cpp $(CFLAGS)

######################### Model test 3
modelTest3 : 
	$(MAKE) model_test3 debug=yes optimizer=no_opt time=compressed test=yes

model_test3 : $(BIN)/model_test3

$(BIN)/model_test3 : $(rim_objs) $(OBJS)/TestModel3.o $(OBJS)/modelTest.o
	$(CPP) -o $(BIN)/model_test3 $(rim_objs) $(OBJS)/TestModel3.o $(OBJS)/modelTest.o $(CLINK)

$(OBJS)/TestModel3.o : $(MODEL_TESTS)/TestModel3.cpp
	$(CPP) -o $(OBJS)/TestModel3.o -c $(MODEL_TESTS)/TestModel3.cpp $(CFLAGS)

######################### Model test 4
modelTest4 : 
	$(MAKE) model_test4 debug=yes optimizer=no_opt time=compressed test=yes

model_test4 : $(BIN)/model_test4

$(BIN)/model_test4 : $(rim_objs) $(OBJS)/TestModel4.o $(OBJS)/modelTest.o
	$(CPP) -o $(BIN)/model_test4 $(rim_objs) $(OBJS)/TestModel4.o $(OBJS)/modelTest.o $(CLINK)

$(OBJS)/TestModel4.o : $(MODEL_TESTS)/TestModel4.cpp
	$(CPP) -o $(OBJS)/TestModel4.o -c $(MODEL_TESTS)/TestModel4.cpp $(CFLAGS)

######################### Model test 5
modelTest5 : 
	$(MAKE) model_test5 debug=yes optimizer=no_opt time=compressed test=yes

model_test5 : $(BIN)/model_test5

$(BIN)/model_test5 : $(rim_objs) $(OBJS)/TestModel5.o $(OBJS)/modelTest.o
	$(CPP) -o $(BIN)/model_test5 $(rim_objs) $(OBJS)/TestModel5.o $(OBJS)/modelTest.o $(CLINK)

$(OBJS)/TestModel5.o : $(MODEL_TESTS)/TestModel5.cpp
	$(CPP) -o $(OBJS)/TestModel5.o -c $(MODEL_TESTS)/TestModel5.cpp $(CFLAGS)
	
