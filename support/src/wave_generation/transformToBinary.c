/* ***************************************************************************
 * Author      : Kevin Glass
 * Date        : Mar 22, 2010
 * File        : transformToBinary.c
 * Project     : rim
 *
 *
 * Contents    :
 *
 * Assumptions :
 *
 * ---------------------------------------------------------------------------
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/stat.h>

char * getFile(const char * fileName, int32_t size);
void storeAsBinary(char * srcData, int32_t srcSize);
void nullCSV(char * buff, int32_t buffSize, int32_t * current);
void skipWhiteSpace(const char * buff, int32_t buffSize, int32_t * current);
void skipLine(const char * buff, int32_t buffSize, int32_t * current);
int32_t isLine(const char * buff, int32_t buffSize, int32_t * current);
void findChar(const char * buff, int32_t buffSize, char c, int32_t * current);
int32_t countLines(const char * buff, int32_t buffSize);
void analyze(double * windData, double * loadData, int32_t nLines);

int main (int32_t argc, char * argv[])
{
	int32_t                  size           = 0;
	int32_t                  handle         = 0;
	int32_t                  error          = 0;
	char                   * srcData        = NULL;
	char                   * loadFile       = "load.bin";
	char                   * windFile       = "wind.bin";
	struct stat              status;

	if (argc != 2) {
		printf("USAGE: include the source file.\nSource file is csv with two values per line: load,wind.\nGenerates separate binary files for each.\n");
		exit(-1);
	}

	error = stat(argv[1], &status);
	size = status.st_size;
	srcData = getFile(argv[1], size);

	storeAsBinary(srcData, size);

	free(srcData);
}

void nullCSV(char * buff, int32_t buffSize, int32_t * current)
{
	while(buff[*current] != ',' &&
			buff[*current] != '\n' && *current < buffSize) {
		++*current;
	}

	if (buff[*current] == ',' || buff[*current] == '\n') {
		buff[*current] = '\0';
		++*current;
	}
}

void skipWhiteSpace(const char * buff, int32_t buffSize, int32_t * current)
{
	while (isspace(buff[*current]) && *current < buffSize) {
		++*current;
	}
}

void skipLine(const char * buff, int32_t buffSize, int32_t * current)
{
	// WARNING: windows stuff ahead
	while (buff[*current] != '\n' && *current < buffSize) {
		++*current;
	}
	++*current; // start of second line
}

int32_t isLine(const char * buff, int32_t buffSize, int32_t * current)
{
	int32_t                     rtn            = 0;
	while (buff[*current] != '\n' && *current < buffSize) {
		if (isalnum(buff[*current])) {
			rtn = 1;
		}
		++*current;
	}
	return rtn;
}

void findChar(const char * buff, int32_t buffSize, char c, int32_t * current)
{
	while (buff[*current] != c) {
		++*current;
	}
}

int32_t countLines(const char * buff, int32_t buffSize)
{
	int32_t                  nLines         = 0;
	int32_t                  current        = 0;

	skipLine(buff, buffSize, &current); // skip header file
	while(current < buffSize) {
		if (isLine(buff, buffSize, &current)) {
			++nLines;
		}
		++current;
	}

	printf("N lines = %d\n", nLines);
	return nLines;
}

void storeAsBinary(char * srcData, int32_t srcSize)
{
	int32_t                  nLines         = countLines(srcData, srcSize);
	double                 * windData       = (double *)malloc(nLines*sizeof(double));
	double                 * loadData       = (double *)malloc(nLines*sizeof(double));
	char                   * value          = NULL;

	int32_t                  current        = 0;
	int32_t                  i              = 0;


	int32_t                  handle         = -1;

	skipLine(srcData, srcSize, &current);
	for (i = 0; i < nLines; i++){
		value = &srcData[current];
		nullCSV(srcData, srcSize, &current);
		loadData[i] = (double)atof(value);

		value = &srcData[current];
		nullCSV(srcData, srcSize, &current);
		windData[i] = (double)atof(value);
//		skipLine(srcData, srcSize, &current);
	}

	handle = open("load.bin", O_WRONLY | O_CREAT | O_TRUNC);
	write(handle, loadData, nLines*sizeof(double));
	close(handle);

	handle = open("wind.bin", O_WRONLY | O_CREAT | O_TRUNC);
	write(handle, windData, nLines*sizeof(double));
	close(handle);
	analyze(windData, loadData, nLines);
}

char * getFile(const char * fileName, int32_t size)
{
	char                   * rtn            = NULL;
	struct stat              status;
	int32_t                  readSize       = 0;
	int32_t                  handle         = 0;

	rtn = (char *)malloc(size*sizeof(char));

	handle = open(fileName, O_RDONLY);

	if (handle < 0) {
		printf("ERROR: could not access the source file in %d\n", fileName);
		exit(-1);
	}

	readSize = read(handle, rtn, size);
	/*
	if (readSize != size) {
		printf("ERROR: mismatch between file size (%d) and read size (%d) in %d\n", size, readSize, fileName);
		exit(-1);
	}
	*/
	close(handle);

	return rtn;
}

void analyze(double * windData, double * loadData, int32_t nLines)
{
	double                   maxLoad        = loadData[0];
	double                   minLoad        = loadData[0];
	double                   maxWind        = windData[0];
	double                   minWind        = windData[0];

	double                   maxLoadUp      = loadData[1] - loadData[0];
	double                   maxLoadDown    = maxLoadUp;
	double                   maxWindUp      = windData[1] - windData[0];
	double                   minWindDown    = maxWindUp;

	int32_t                   i             = 0;

	for (i = 1; i < nLines; i++) {
		if (loadData[i] < minLoad) {
			minLoad = loadData[i];
		}
		if (loadData[i] > maxLoad) {
			maxLoad = loadData[i];
		}
		if (windData[i] < minWind) {
			minWind = windData[i];
		}
		if (windData[i] > maxWind) {
			maxWind = windData[i];
		}
	}
	for (i = 2; i < nLines; i++) {
		if ((loadData[i] - loadData[i-1]) > maxLoadUp) {
			maxLoadUp = loadData[i] - loadData[i-1];
		}
		if ((loadData[i] - loadData[i-1]) < maxLoadDown) {
			maxLoadDown = loadData[i] - loadData[i-1];
		}
		if ((windData[i] - windData[i-1]) > maxWindUp) {
			maxWindUp = windData[i] - windData[i-1];
		}
		if ((windData[i] - windData[i-1]) < minWindDown) {
			minWindDown = windData[i] - windData[i-1];
		}
		printf("%d load %lf, wind %lf\n", i, loadData[i], windData[i]);
		/*
		if (-maxLoadDown > 3200){
			printf("i = %d\n", i);
			exit(-1);
		}
		*/
	}
	printf("MAX load = %lf, MIN load = %lf\n", maxLoad, minLoad);
	printf("MAX wind = %lf, MIN wind = %lf\n", maxWind, minWind);
	printf("MAX load ramp up = %lf, MAX load ramp down= %lf\n", maxLoadUp, -maxLoadDown);
	printf("MAX wind ramp up = %lf, MAX load wind down= %lf\n", maxWindUp, -minWindDown);
}
