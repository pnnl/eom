
PARSER_TESTS                 = $(UNIT_TESTS)/parserTests

all_parser_tests: parserTests

parserTests :                       \
			parserTest              \
			simParserTest           \
			baParserTest            \
			zoneParserTest          \
			generatorParserTest     \
			loadParserTest          \
			windParserTest          \
			solarParserTest         \
			rimParserBasicTest      \
			rimParserDataTest       \


$(OBJS)/parserTest.o : $(PARSER_TESTS)/parserTest.cpp
	$(CPP) -o $(OBJS)/parserTest.o -c $(PARSER_TESTS)/parserTest.cpp $(CFLAGS)

######################### Parser test
parserTest : 
	$(MAKE) parser_test debug=yes optimizer=no_opt time=compressed

parser_test : $(BIN)/parser_test

$(BIN)/parser_test : $(rim_objs) $(OBJS)/TestParser.o $(OBJS)/parserTest.o
	$(CPP) -o $(BIN)/parser_test $(rim_objs) $(OBJS)/TestParser.o $(OBJS)/parserTest.o $(CLINK)

$(OBJS)/TestParser.o : $(PARSER_TESTS)/TestParser.cpp
	$(CPP) -o $(OBJS)/TestParser.o -c $(PARSER_TESTS)/TestParser.cpp $(CFLAGS)
######################### Parser test CPLEX

parserTestC : 
	$(MAKE) parser_testC debug=yes optimizer=cplex

parser_testC : $(BIN)/parser_testC

$(BIN)/parser_testC : $(rim_objs) $(OBJS)/rim.o 
	$(CPP) -o $(BIN)/parser_testC $(rim_objs) $(OBJS)/rim.o $(CLINK)

######################### simulationParser test
simParserTest : 
	$(MAKE) simParser_test debug=yes optimizer=no_opt time=compressed

simParser_test : $(BIN)/simParser_test

$(BIN)/simParser_test : $(rim_objs) $(OBJS)/TestSimParser.o $(OBJS)/parserTest.o
	$(CPP) -o $(BIN)/simParser_test $(rim_objs) $(OBJS)/TestSimParser.o $(OBJS)/parserTest.o $(CLINK)

$(OBJS)/TestSimParser.o : $(PARSER_TESTS)/TestSimParser.cpp
	$(CPP) -o $(OBJS)/TestSimParser.o -c $(PARSER_TESTS)/TestSimParser.cpp $(CFLAGS)

######################### baParser test
baParserTest : 
	$(MAKE) baParser_test debug=yes optimizer=no_opt time=compressed

baParser_test : $(BIN)/baParser_test

$(BIN)/baParser_test : $(rim_objs) $(OBJS)/TestBAParser.o $(OBJS)/parserTest.o
	$(CPP) -o $(BIN)/baParser_test $(rim_objs) $(OBJS)/TestBAParser.o $(OBJS)/parserTest.o $(CLINK)

$(OBJS)/TestBAParser.o : $(PARSER_TESTS)/TestBAParser.cpp
	$(CPP) -o $(OBJS)/TestBAParser.o -c $(PARSER_TESTS)/TestBAParser.cpp $(CFLAGS)

$(OBJS)/BATest.o : $(PARSER_TESTS)/BATest.cpp
	$(CPP) -o $(OBJS)/BATest.o -c $(PARSER_TESTS)/BATest.cpp $(CFLAGS)

######################### zoneParser test
zoneParserTest : 
	$(MAKE) zoneParser_test debug=yes optimizer=no_opt time=compressed

zoneParser_test : $(BIN)/zoneParser_test

$(BIN)/zoneParser_test : $(rim_objs) $(OBJS)/TestZoneParser.o $(OBJS)/parserTest.o
	$(CPP) -o $(BIN)/zoneParser_test $(rim_objs) $(OBJS)/TestZoneParser.o $(OBJS)/parserTest.o $(CLINK)

$(OBJS)/TestZoneParser.o : $(PARSER_TESTS)/TestZoneParser.cpp
	$(CPP) -o $(OBJS)/TestZoneParser.o -c $(PARSER_TESTS)/TestZoneParser.cpp $(CFLAGS)

######################### generatorParser test
generatorParserTest : 
	$(MAKE) generatorParser_test debug=yes optimizer=no_opt time=compressed

generatorParser_test : $(BIN)/generatorParser_test

$(BIN)/generatorParser_test : $(rim_objs) $(OBJS)/TestGeneratorParser.o $(OBJS)/parserTest.o
	$(CPP) -o $(BIN)/generatorParser_test $(rim_objs) $(OBJS)/TestGeneratorParser.o $(OBJS)/parserTest.o $(CLINK)

$(OBJS)/TestGeneratorParser.o : $(PARSER_TESTS)/TestGeneratorParser.cpp
	$(CPP) -o $(OBJS)/TestGeneratorParser.o -c $(PARSER_TESTS)/TestGeneratorParser.cpp $(CFLAGS)

######################### loadParser test
loadParserTest : 
	$(MAKE) loadParser_test debug=yes optimizer=no_opt time=compressed

loadParser_test : $(BIN)/loadParser_test

$(BIN)/loadParser_test : $(rim_objs) $(OBJS)/TestLoadParser.o $(OBJS)/parserTest.o
	$(CPP) -o $(BIN)/loadParser_test $(rim_objs) $(OBJS)/TestLoadParser.o $(OBJS)/parserTest.o $(CLINK)

$(OBJS)/TestLoadParser.o : $(PARSER_TESTS)/TestLoadParser.cpp
	$(CPP) -o $(OBJS)/TestLoadParser.o -c $(PARSER_TESTS)/TestLoadParser.cpp $(CFLAGS)

######################### windParser test
windParserTest : 
	$(MAKE) windParser_test debug=yes optimizer=no_opt time=compressed

windParser_test : $(BIN)/windParser_test

$(BIN)/windParser_test : $(rim_objs) $(OBJS)/TestWindParser.o $(OBJS)/parserTest.o
	$(CPP) -o $(BIN)/windParser_test $(rim_objs) $(OBJS)/TestWindParser.o $(OBJS)/parserTest.o $(CLINK)

$(OBJS)/TestWindParser.o : $(PARSER_TESTS)/TestWindParser.cpp
	$(CPP) -o $(OBJS)/TestWindParser.o -c $(PARSER_TESTS)/TestWindParser.cpp $(CFLAGS)

######################### solarParser
solarParserTest : 
	$(MAKE) solarParser_test debug=yes optimizer=no_opt time=compressed

solarParser_test : $(BIN)/solarParser_test

$(BIN)/solarParser_test : $(rim_objs) $(OBJS)/TestSolarParser.o $(OBJS)/parserTest.o
	$(CPP) -o $(BIN)/solarParser_test $(rim_objs) $(OBJS)/TestSolarParser.o $(OBJS)/parserTest.o $(CLINK)

$(OBJS)/TestSolarParser.o : $(PARSER_TESTS)/TestSolarParser.cpp
	$(CPP) -o $(OBJS)/TestSolarParser.o -c $(PARSER_TESTS)/TestSolarParser.cpp $(CFLAGS)

######################### RIMParser tests
#rimParserBasicTest 
rimParserBasicTest : 
	$(MAKE) rimParserBasic_test debug=yes optimizer=no_opt time=compressed

rimParserBasic_test : $(BIN)/rimParserBasic_test

$(BIN)/rimParserBasic_test : $(rim_objs) $(OBJS)/BasicTestRIMParser.o $(OBJS)/parserTest.o
	$(CPP) -o $(BIN)/rimParserBasic_test $(rim_objs) $(OBJS)/BasicTestRIMParser.o $(OBJS)/parserTest.o $(CLINK)

$(OBJS)/BasicTestRIMParser.o : $(PARSER_TESTS)/BasicTestRIMParser.cpp
	$(CPP) -o $(OBJS)/BasicTestRIMParser.o -c $(PARSER_TESTS)/BasicTestRIMParser.cpp $(CFLAGS)

#rimParserDataTest 
rimParserDataTest : 
	$(MAKE) rimParserData_test debug=yes optimizer=no_opt time=compressed

rimParserData_test : $(BIN)/rimParserData_test

$(BIN)/rimParserData_test : $(rim_objs) \
	$(OBJS)/DataTestRIMParser.o         \
	$(OBJS)/parserTest.o                
	$(CPP) -o $(BIN)/rimParserData_test $(rim_objs) \
	$(OBJS)/DataTestRIMParser.o         \
	$(OBJS)/parserTest.o                \
	$(CLINK)

$(OBJS)/DataTestRIMParser.o : $(PARSER_TESTS)/DataTestRIMParser.cpp
	$(CPP) -o $(OBJS)/DataTestRIMParser.o -c $(PARSER_TESTS)/DataTestRIMParser.cpp $(CFLAGS)

#rimParserConfigTest 
rimParserConfigTest : 
	$(MAKE) rimParserConfig_test debug=yes optimizer=no_opt time=compressed

rimParserConfig_test : $(BIN)/rimParserConfig_test

$(BIN)/rimParserConfig_test : $(rim_objs) \
	$(OBJS)/ConfigTestRIMParser.o         \
	$(OBJS)/parserTest.o                
	$(CPP) -o $(BIN)/rimParserConfig_test $(rim_objs) \
	$(OBJS)/ConfigTestRIMParser.o         \
	$(OBJS)/parserTest.o                \
	$(CLINK)

$(OBJS)/ConfigTestRIMParser.o : $(PARSER_TESTS)/ConfigTestRIMParser.cpp
	$(CPP) -o $(OBJS)/ConfigTestRIMParser.o -c $(PARSER_TESTS)/ConfigTestRIMParser.cpp $(CFLAGS)
