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

//#define     MIN_IN_DAY   1440

void usage(int detailed)
{
	printf("USAGE: converter <inputFile> <time step> <duration>\n");
	if (detailed) {
		printf ("\t inputFile contains input load data in text form (MW) in 10 min. steps.\n");
		printf ("\t file binaryLoad.bin containts the input data converter to binary form.\n");
		//printf ("\t period is the peak-to-peak distance in days.\n");
		//printf ("\t duration is number of simulated days.\n");
		//printf ("\t stripe is the output frequency in minutes.\n");
	}
}

int32_t validateFile(const char * fileName)
{
	struct stat              status;
	int32_t                  error;
	error = stat(fileName, &status);
	if (error < 0) {
		printf("ERROR: could not access the file %s\n", fileName);
		exit(-1);
	}

	return status.st_size;
}

int32_t openFile(const char * fileName)
{
	int32_t                  handle         = open(fileName, O_RDONLY);
	if (handle < 0) {
		printf("ERROR: could not open file %s\n", fileName);
		exit(-1);
	}
	return handle;
}

char * loadFile(const char * fileName)
{
	int32_t                  buffSize       = validateFile(fileName);
	int32_t                  handle         = openFile(fileName);
	char                   * buff           = (char *)malloc(buffSize*sizeof(char));

	if (handle < 0) {
		printf("ERROR: could not open file %s\n", fileName);
	}

	int32_t readSize = read(handle, buff, buffSize);
	if (readSize != buffSize) {
		printf("ERROR: could not open file %s\n", fileName);
		exit(-1);
	}
	close(handle);
	return buff;
}

char * readInputFile(int argc, char * argv[])
{
	char                   * fileName       = argv[1];
	int32_t                  timeStep       = atoi(argv[2]);

	char                   * buffer         = NULL;
	int32_t                  bufferSize     = -1;

	buffer = loadFile(fileName);
}

void printOutputFile(char * dataSpace, int32_t duration) {
	int32_t                  binFile        = open("binary.bin", O_WRONLY | O_CREAT, S_IRWXU | S_IRWXG);
	int32_t                  i              = 0;
	int32_t                  index          = 0;
	double                   value          = 0.0;
	char                   * ptr            = NULL;

	for (i = 0; i < duration; i++) {
		ptr = &dataSpace[index];
		while (dataSpace[index] != '\n'  && dataSpace[index] != '\r' && dataSpace[index] != '\f') {
			++index;
		}
		dataSpace[index] = '\0';
		++index;
		while (dataSpace[index] == '\n' || dataSpace[index] == '\r' || dataSpace[index] == '\f') {
			++index;
		}
		value = atof(ptr);
    write(binFile, &value, sizeof(double));
	}

	close(binFile);
}

int main(int argc, char * argv[])
{
	char                   * buffer         = NULL;
	int32_t                  duration       = 0;
	if (argc != 4) {
		if (argc == 2) {
			if (!strcmp(argv[1], "--h") || !strcmp(argv[1], "--help")) {
				usage(1);
			} else {
				usage(1);
			}
		} else {
			usage(1);
		}
		exit(-1);
	}
	duration       = atoi(argv[3]);
	buffer = readInputFile(argc, argv);
	printOutputFile(buffer, duration);
	return 0;
}
