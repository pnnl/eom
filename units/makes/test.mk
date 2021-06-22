
UNIT_TESTS                   = $(UNITS)/src

include $(UNITS)/makes/parserTest.mk
include $(UNITS)/makes/modelsTest.mk
include $(UNITS)/makes/rimTest.mk

all_tests: parserTests modelsTests
