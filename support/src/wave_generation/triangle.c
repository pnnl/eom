/* ***************************************************************************
 * triangle.c
 *
 *      Author: Kevin Glass
 *  Created on: May 12, 2010
 *
 * Inheritance
 *
 * Class members
 *
 * Class variables
 * 
 * Purpose
 *
 ************************************************************************** */

#include "triangle.h"
#include <math.h>
#include <string.h>

void openOutputFiles(WAVEFORM * wave, FILE ** text, int * binary)
{
	char                     outputStem[256];
	char                     textOutput[256];
	char                     binaryOutput[256];

	// get file names
	sprintf(outputStem, "%s_%d_%d_%d_%d_%d", wave->functionType, (int)wave->valley,
			(int)wave->peak, (int)wave->period, (int)wave->duration, (int)wave->stripe);
	if (wave->set[2]) {
		sprintf(outputStem, "%s_[%d]", outputStem, (int)wave->rise);
	}

	if (wave->set[7] && wave->set[8]) {
		sprintf(outputStem, "%s_[%d_%d]", outputStem, wave->rampDays, (int)wave->rampStart);
	}
	sprintf(textOutput, "%s.txt", outputStem);
	sprintf(binaryOutput, "%s.bin", outputStem);

	*text = fopen(textOutput, "w");
	*binary  = open(binaryOutput, O_WRONLY | O_CREAT, S_IRWXU | S_IRWXG);
}

void ramping(WAVEFORM * wave, FILE * text, int binary)
{
	double                   value          = wave->rampStart;
	double                   rate           = (wave->valley - value)*wave->stripe/wave->rampDays;
	int                      i              = 0;
	// from 0->rampDays (in minutes) output
	while (i < wave->rampDays) {
		fprintf(text, "%d %f\n", i, value);
		write(binary, &value, sizeof(double));
		value += rate;
		i += wave->stripe;
	}
}

void appendWave(WAVEFORM * wave)
{
	char                   * output         = NULL;
	struct stat              status;
	int                      error;
	int                      input;
	int                      outFile;
	char                     outputStem[256];
	char                     binaryOutput[256];
	double                   value          = wave->rampStart;
	double                   rate           = (wave->valley - value)*wave->stripe/wave->rampDays;
	int                      i              = 0;
	int                      len            = strlen(wave->appendName);
	char                   * fileName;

	i = len - 1;
	while (i > 0) {
		if (wave->appendName[i]== '/') {
			fileName = &wave->appendName[i+1];
			i = 0;
		}
		--i;
	}
	sprintf(binaryOutput, "%d_%d_%s", (int)wave->rampDays, (int)wave->rampStart, fileName);

	outFile  = open(binaryOutput, O_WRONLY | O_CREAT, S_IRWXU | S_IRWXG);

	if (wave->rampDays > 0) {
		while (i < wave->rampDays) {
			write(outFile, &value, sizeof(double));
			value += rate;
			i += wave->stripe;
		}
	}

	error = stat(wave->appendName, &status);
	if (error < 0) {
		printf("ERROR: could not read file %s", wave->appendName);
		exit(-1);
	}
	output = (char *)malloc(status.st_size);
	input  = open(wave->appendName, O_RDONLY);
	read(input, output, status.st_size);
	write(outFile, output, status.st_size);

	close(outFile);
	close(input);
}

void sineWave(WAVEFORM * wave)
{
	double                   output         = 0.0;
	double                   timeStep       = PI2/(double)MIN_IN_DAY;
	double                   stepInc        = 0.0;
	FILE                   * textFile       = NULL;
	int                      binFile        = -1;
	int                      i              = 0;
	int                      j              = 0;

	openOutputFiles(wave, &textFile, &binFile);

	if (wave->rampDays > 0) {
		ramping(wave, textFile, binFile);
	}

	while (i < wave->duration) {
		stepInc = 0.0;
		for (j = 0; j < MIN_IN_DAY; j += wave->stripe) {
			output = sin((PI2)*stepInc) + (wave->rise*(double)j);
			fprintf(textFile, "%d %f\n", i, output);
			write(binFile, &output, sizeof(double));
			stepInc += timeStep;
		}
	}

	fclose(textFile);
	close(binFile);
}

void triangleWave(WAVEFORM * wave)
{
	double                   rate           = 0.0;
	double                   output         = 0.0;
	FILE                   * textFile       = NULL;
	int                      binFile        = -1;
	int                      repeat         = (int)wave->period;
	int                      halfway        = repeat >> 1;
	int                      i              = 0;
	int                      j              = 0;
//	int                      k              = 0;

	double                   d_peak         = (double)wave->peak;
	double                   d_valley       = (double)wave->valley;
	double                   d_period       = (double)wave->period;
	double                   rise           = 0.0;
	int                      step           = 0;

	openOutputFiles(wave, &textFile, &binFile);

	rate                = 2.0*(d_peak-d_valley)/d_period;
	rise                = wave->rise * wave->stripe/(double)MIN_IN_DAY;
	if (wave->rampDays > 0) {
		ramping(wave, textFile, binFile);
	}

	output = wave->valley;
	while (i < wave->duration) {
		for (j = 0; j < halfway; j += wave->stripe) {
			fprintf(textFile, "%d %f\n", i, output);
			write(binFile, &output, sizeof(double));
			output += rate*wave->stripe;
			output += rise;
//			rise += wave->rise * wave->stripe;
			i += wave->stripe;
		}
		for (j = halfway; j < repeat; j += wave->stripe) {
			fprintf(textFile, "%d %lf\n", i, output);
			write(binFile, &output, sizeof(double));
			output -= rate*wave->stripe;
			output += rise;
//			rise += wave->rise * wave->stripe;
			i += wave->stripe;
		}
	}

	fclose(textFile);
	close(binFile);
}

void sawtoothWave(WAVEFORM * wave)
{
	double                   rate           = 0.0;
	double                   output         = 0.0;
	FILE                   * textFile       = NULL;
	int                      binFile        = -1;
	int                      i              = 0;
	int                      j              = 0;

	double                   d_peak         = (double)wave->peak;
	double                   d_valley       = (double)wave->valley;
	double                   d_period       = (double)wave->period;

	openOutputFiles(wave, &textFile, &binFile);
	rate                = (d_peak-d_valley)/d_period;

	if (wave->rampDays > 0) {
		ramping(wave, textFile, binFile);
	}

	while (i < wave->duration) {
		output = wave->valley;
		for (j = 0; j < MIN_IN_DAY; j += wave->stripe) {
			fprintf(textFile, "%d %f\n", i, output);
			write(binFile, &output, sizeof(double));
			output += (rate + wave->rise)*wave->stripe;
			i += wave->stripe;
		}
	}

	fclose(textFile);
	close(binFile);
}

void usage(int argc, char * argv[])
{
	printf("USAGE: %s -peak <peak> -valley <valley> -period <period> \n", argv[0]);
	printf("-duration <duration> -stripe <stripe> [-function <file stem>] \n");
	printf("[-rise <rise>] [-rampDays <ramp days> -rampStart <rampStart>]\n");
	printf("where:\n");

	printf ("\t peak: is the maximum height of the wave in MWatts.\n");
	printf ("\t valley: is the minimum height of the wave in MWatts.\n");
	printf ("\t period: is the peak-to-peak distance in days.\n");
	printf ("\t duration: is number of simulated days.\n");
	printf ("\t stripe: is the output frequency in minutes.\n");
	printf ("\t function: [sin | triangle | sawtooth | append]\n");
	printf ("\t rise: is an optional value corresponding to a linear rise in \n");
	printf ("\t\t the triangle wave over a day default is 0\n");
	printf ("\t ramping is the period prior to start of the modeled wave and is\n");
	printf ("\t\t used to ensure the model is allowed to reach a valid state\n");
	printf ("\t\t rampDays: is the number of days for ramping\n");
	printf ("\t\t rampStart: is the initial starting point\n");
}

void printWave(WAVEFORM * wave)
{
	printf("WAVEFORM :\n");
	printf("\t peak                 = %f\n", wave->peak);
	printf("\t valley               = %f\n", wave->valley);
	printf("\t rise                 = %f\n", wave->rise);
	printf("\t rampStart            = %f\n", wave->rampStart);
	printf("\t period               = %d\n", wave->period);
	printf("\t duration             = %d\n", wave->duration);
	printf("\t stripe               = %d\n", wave->stripe);
	printf("\t rampDays             = %d\n", wave->rampDays);
	printf("\t functionType         = %s\n", wave->functionType);
}

void dumpWaveMetadata(WAVEFORM * wave)
{
	FILE                   * metaFile  = fopen(wave->functionType, "w");

	fprintf(metaFile, "WAVEFORM :\n");
	fprintf(metaFile, "\t peak                 = %f\n", wave->peak);
	fprintf(metaFile, "\t valley               = %f\n", wave->valley);
	fprintf(metaFile, "\t rise                 = %f\n", wave->rise);
	fprintf(metaFile, "\t rampStart            = %f\n", wave->rampStart);
	fprintf(metaFile, "\t period               = %d\n", wave->period);
	fprintf(metaFile, "\t duration             = %d\n", wave->duration);
	fprintf(metaFile, "\t stripe               = %d\n", wave->stripe);
	fprintf(metaFile, "\t rampDays             = %d\n", wave->rampDays);
	fprintf(metaFile, "\t functionType         = %s\n", wave->functionType);
	fclose(metaFile);
}

void setFunction(WAVEFORM * wave, char * type)
{
	if (!strcmp(type, "sin")) {
		strncpy(wave->functionType, "sin", 31);
		wave->waveFunction = sineWave;
	} else 	if (!strcmp(type, "triangle")) {
		strncpy(wave->functionType, "triangle", 31);
		wave->waveFunction = triangleWave;
	} else 	if (!strcmp(type, "sawtooth")) {
		strncpy(wave->functionType, "sawtooth", 31);
		wave->waveFunction = sawtoothWave;
	} else 	if (!strcmp(type, "append")) {
		strncpy(wave->functionType, "append", 31);
		wave->waveFunction = appendWave;
	} else {
		printf("ERROR: invalid function type");
		exit(-1);
	}

}

WAVEFORM * readCmdLine(int argc, char * argv[])
{
	WAVEFORM               * wave = (WAVEFORM *)malloc(sizeof(WAVEFORM));
	int                      i    = 0;

	for (i = 0; i < 9; i++) {
		wave->set[i]           = 0;
	}
	wave->peak              = 0.0;
	wave->valley            = 0.0;
	wave->rise              = 0.0;
	wave->rampStart         = 0.0;
	wave->period            = 0;
	wave->duration          = 0;
	wave->stripe            = 0;
	wave->rampDays          = 0;
	wave->functionType[0]   = '\0';
	wave->appendName[0]     = '\0';

	// arg0 is the file name, arg(2n-1) is the name of input parameter n+1,
	// arg(2n) is the value of parameter 2n-1, where n>=1
	for (i = 1; i < argc; i+=2) {
		if (!strcmp(argv[i], "-peak")) {
			if (wave->set[0]) {
				printf("ERROR: peak was requested twice\n");
				exit(-1);
			}
			wave->set[0] = 1;
			wave->peak       = atof(argv[i+1]);
		} else if (!strcmp(argv[i], "-valley")) {
			if (wave->set[1]) {
				printf("ERROR: valley was requested twice\n");
				exit(-1);
			}
			wave->set[1] = 1;
			wave->valley     = atof(argv[i+1]);
		} else if (!strcmp(argv[i], "-rise")) {
			if (wave->set[2]) {
				printf("ERROR: rise was requested twice\n");
				exit(-1);
			}
			wave->set[2] = 1;
			wave->rise       = atof(argv[i+1]);
		} else if (!strcmp(argv[i], "-rampStart")) {
			if (wave->set[3]) {
				printf("ERROR: rampStart was requested twice\n");
				exit(-1);
			}
			wave->set[3] = 1;
			wave->rampStart  = atof(argv[i+1]);
		} else if (!strcmp(argv[i], "-period")) {
			if (wave->set[4]) {
				printf("ERROR: period was requested twice\n");
				exit(-1);
			}
			wave->set[4] = 1;
			wave->period     = atoi(argv[i+1])*MIN_IN_DAY;
		} else if (!strcmp(argv[i], "-duration")) {
			if (wave->set[5]) {
				printf("ERROR: duration was requested twice\n");
				exit(-1);
			}
			wave->set[5] = 1;
			wave->duration   = atoi(argv[i+1])*MIN_IN_DAY;
		} else if (!strcmp(argv[i], "-stripe")) {
			if (wave->set[6]) {
				printf("ERROR: stripe was requested twice\n");
				usage(argc, argv);
			}
			wave->set[6] = 1;
			wave->stripe     = atoi(argv[i+1]);
		} else if (!strcmp(argv[i], "-rampDays")) {
			if (wave->set[7]) {
				printf("ERROR: rampDays was requested twice\n");
				exit(-1);
			}
			wave->set[7] = 1;
			wave->rampDays   = atoi(argv[i+1])*MIN_IN_DAY;
		} else if (!strcmp(argv[i], "-function")) {
			if (wave->set[8]) {
				printf("ERROR: function was requested twice\n");
				exit(-1);
			}
			wave->set[8] = 1;
			setFunction(wave, argv[i+1]);
			strncpy(wave->appendName, argv[i+2], 255);
			++i;
		} else {
			printf("ERROR: %s is an unknown argument\n", argv[i]);
			usage(argc, argv);
			free(wave);
			wave = NULL;
			exit(-1);
		}
	}
	if (!wave->set[0]) {
		printf("ERROR: peak was not set\n");
		usage(argc, argv);
		exit(-1);
	} else if (!wave->set[1]) {
		printf("ERROR: valley was not set\n");
		usage(argc, argv);
		exit(-1);
	} else if (!wave->set[4]) {
		printf("ERROR: period was not set\n");
		usage(argc, argv);
		exit(-1);
	} else if (!wave->set[5]) {
		printf("ERROR: duration was not set\n");
		usage(argc, argv);
		exit(-1);
	} else if (!wave->set[6]) {
		printf("ERROR: stripe was not set\n");
		usage(argc, argv);
		exit(-1);
	}

	return wave;
}

int main(int argc, char * argv[])
{
	WAVEFORM               * wave           = NULL;
	if (argc == 1) {
		usage(argc, argv);
		exit(0);
	} else if (!strcmp(argv[1], "-h") || !strcmp(argv[1], "-help")) {
		usage(argc, argv);
		exit(0);
	} else {
		wave = readCmdLine(argc, argv);
		if (wave == NULL) {
			exit(-1);
		}
	}
#if defined TEST
	printWave(wave);
#endif

	wave->waveFunction(wave);

	return 0;
}
