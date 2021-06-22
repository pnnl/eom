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

#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/stat.h>

#include <stdio.h>
#include <stdlib.h>

#define     MIN_IN_DAY   1440

void usage(int detailed)
{
	printf("USAGE: step <duration> <stripe>\n");
	if (detailed) {
		printf ("\t period is the peak-to-peak distance in days.\n");
		printf ("\t duration is number of simulated days.\n");
		printf ("\t stripe is the output frequency in minutes.\n");
	}
}

void generateWave(int argc, char * argv[])
{
	FILE                   * defFile        = NULL;
	FILE                   * textFile       = NULL;
	int                      binFile        = -1;
	int                      duration       = atoi(argv[1]);
	int                      stripe         = atoi(argv[2]);
	double                   value          = 0.0;
	int                    * endOfStep		= (int *)malloc(sizeof(int)*24);
	double                 * stepSize       = (double *)malloc(sizeof(double)*24);
	int                      i              = 0;
	int                      j              = 0;
	int                      k              = 0;
	int                      r              = 0;
	int                      lastValid      = 0;
	int                      index          = 0;
	char                     fileName[256];
	// collect input data
	endOfStep[0] = 0;
	printf("initial step size %d: ", i);
	scanf("%lf", &value);
	stepSize[0] = value;
	fflush(stdin);
	do {
	//		prompt for range in hours
		++i;
		printf("end of range %d: ", i);
		scanf("%d", &index);
		endOfStep[i] = index;
		fflush(stdin);

		printf("step size %d: ", i);
		scanf("%lf", &value);
		stepSize[i] = value;
		fflush(stdin);
	} while (endOfStep[i] < 24);
	lastValid = i;

	sprintf(fileName, "step_%d_%d.txt",(int)duration, (int)stripe);
	textFile = fopen("step.txt", "w");
	sprintf(fileName, "step_%d_%d.bin",(int)duration, (int)stripe);
	binFile  = open("step.bin", O_WRONLY | O_CREAT, S_IRWXU | S_IRWXG);
	sprintf(fileName, "step_%d_%d.def",(int)duration, (int)stripe);
	defFile  = fopen("step.def", "w");

	for (j = 0; j < i; j++) {
		fprintf(defFile, "[%d-%d, %lf)\n", endOfStep[j], endOfStep[j+1], stepSize[j]);
	}

	for (i = 0; i < duration; i++) {
		for (r = 0; r < lastValid; r++) {
			for (j = endOfStep[r]; j < endOfStep[r+1]; j++) {
				for (k = 0; k < 60; k+= stripe) {
					fprintf(textFile, "%lf\n", stepSize[r]);
					write(binFile, &stepSize[r], sizeof(double));
				}
			}
		}
	}

	fclose(defFile);
	fclose(textFile);
	close(binFile);
}

int main(int argc, char * argv[])
{
	if (argc != 3) {
		if (argc == 2) {
			if (!strcmp(argv[1], "--h") || !strcmp(argv[1], "--help")) {
				usage(1);
			} else {
				usage(0);
			}
		} else {
			usage(0);
		}
		exit(-1);
	}
	generateWave(argc, argv);
}
