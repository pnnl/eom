SUPPORT_SRC                  = $(SUPPORT)/src
WAVE_GEN                     = $(SUPPORT_SRC)/wave_generation
#-----------------------------------------------------------------------------
# Wave generators
#-----------------------------------------------------------------------------
triangle : $(WAVE_GEN)/triangle.c
	gcc -o $(BIN)/triangle $(WAVE_GEN)/triangle.c -lm -g

triangle_rising : $(WAVE_GEN)/triangle_rising.c
	gcc -o $(BIN)/triangle_rising $(WAVE_GEN)/triangle_rising.c

converter : $(WAVE_GEN)/converter.c
	gcc -o $(BIN)/converter $(WAVE_GEN)/converter.c -g

step : $(WAVE_GEN)/step.c
	gcc -o $(BIN)/step $(WAVE_GEN)/step.c -g

transformToBinary : $(WAVE_GEN)/transformToBinary.c
	gcc -o $(BIN)/transformToBinary $(WAVE_GEN)/transformToBinary.c -g

readBinary : $(WAVE_GEN)/readBinary.c
	gcc -o $(BIN)/readBinary $(WAVE_GEN)/readBinary.c -g
