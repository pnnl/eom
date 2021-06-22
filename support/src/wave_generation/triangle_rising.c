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
	printf("USAGE: triangle_rising <peak> <valley> <period> <period-incr> <duration> <stripe>\n");
	if (detailed) {
		printf ("\t peak is the maximum height of the wave in MWatts.\n");
		printf ("\t valley is the minimum height of the wave in MWatts.\n");
		printf ("\t period is the peak-to-peak distance in days.\n");
		printf ("\t period-incr is the increase in peak and valley values after each period.\n");
		printf ("\t duration is number of simulated days.\n");
		printf ("\t stripe is the output frequency in minutes.\n");
	}
}

double computeAssentRate(int peak, int valley, int period)
{
	double                   d_peak         = (double)peak;
	double                   d_valley       = (double)valley;
	double                   d_period       = (double)period;
	double                   rate           = (double)period;

	return 2*(d_peak-d_valley)/period;
}

void generateWave(int argc, char * argv[])
{
	double                   peak           = atof(argv[1]);
	double                   valley         = atof(argv[2]);
	double                   period         = atof(argv[3]) * (double)MIN_IN_DAY;
	double                   period_incr    = atof(argv[4]);
	double                   rate           = 0.0;
	double                   output         = 0.0;
	FILE                   * textFile       = NULL;
	int                      binFile        = -1;
	int                      duration       = atoi(argv[5]) * MIN_IN_DAY;
	int                      stripe         = atoi(argv[6]);
	int                      repeat         = (int)period;
	int                      halfway        = repeat >> 1;
	int                      i              = 0;
	int                      j              = 0;
	int                      k              = 0;
	int                      incr           = 0;
	char                     fileName[256];

	sprintf(fileName, "triangle_rising_%d_%d_%d_%d_%d_%d.txt", (int)peak, (int)valley, (int)period, (int)period_incr, (int)duration, (int)stripe);
	textFile = fopen(fileName, "w");
	sprintf(fileName, "triangle_rising_%d_%d_%d_%d_%d_%d.bin", (int)peak, (int)valley, (int)period, (int)period_incr, (int)duration, (int)stripe);
	binFile  = open(fileName, O_WRONLY | O_CREAT, S_IRWXU | S_IRWXG);

	rate = computeAssentRate(peak, valley, period);

	while (i < duration) {
		output = valley + incr;
		for (j = 0; j < halfway; j += stripe) {
			fprintf(textFile, "%d %lf\n", i, output);
			write(binFile, &output, sizeof(double));
			output += rate*stripe;
			i += stripe;
		}
		for (j = halfway; j < repeat; j += stripe) {
			fprintf(textFile, "%d %lf\n", i, output);
			write(binFile, &output, sizeof(double));
			output -= rate*stripe;
			i += stripe;
		}
		incr += period_incr;
	}

	fclose(textFile);
	close(binFile);
}

int main(int argc, char * argv[])
{
	if (argc != 7) {
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
