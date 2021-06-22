/* ***************************************************************************
 * convert.c
 *
 *      Author: Cesar Silva Monroy
 *  Created on: Aug. 17, 2010
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
	printf("USAGE: zero <duration> <strip>\n");
	if (detailed) {
		printf ("\t inputFile contains input load data in text form (MW) in 10 min. steps.\n");
		printf ("\t file binaryLoad.bin containts the input data converter to binary form.\n");
		//printf ("\t period is the peak-to-peak distance in days.\n");
		//printf ("\t duration is number of simulated days.\n");
		//printf ("\t stripe is the output frequency in minutes.\n");
	}
}

void printOutputFile(int argc, char * argv[]) {
	int32_t                  binFile        = open("binary.bin", O_WRONLY | O_CREAT, S_IRWXU | S_IRWXG);
	int32_t                  duration       = atoi(argv[1]);
	int32_t                  stripe         = atoi(argv[2]);
	int32_t                  i              = 0;
	int32_t                  j              = 0;
	double                   value          = 0.0;

	for (i = 0; i < duration; i++) {
		for (j = 0; j < MIN_IN_DAY; j += stripe) {
			write(binFile, &value, sizeof(double));
		}
	}

	close(binFile);
}

int main(int argc, char * argv[])
{
	int32_t                  duration       = 0;
	if (argc != 3) {
		usage(0);
		exit(-1);
	}
	printOutputFile(argc, argv);
	return 0;
}
