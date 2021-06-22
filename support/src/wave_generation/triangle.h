/*
 * triangle.h
 *
 *  Created on: Jul 25, 2011
 *      Author: kglass
 */

#ifndef TRIANGLE_H_
#define TRIANGLE_H_

#include <stdio.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

#define     MIN_IN_DAY   1440
#define     PI           3.1415926
#define     PI2          (2*PI)

typedef struct waveform_t
{
	double                   rampStart;
	double                   valley;
	double                   peak;
	double                   rise;
	int                      period;
	int                      duration;
	int                      stripe;
	int                      rampDays;
	int                      set[9];
	char                     functionType[32];
	char                     appendName[256];
	void                     (* waveFunction)(struct waveform_t * wave);
} WAVEFORM;

void openOutputFiles(WAVEFORM * wave, FILE ** text, int * binary);
void ramping(WAVEFORM * wave, FILE * text, int binary);
void appendWave(struct waveform_t * wave);
void sineWave(struct waveform_t * wave);
void triangleWave(WAVEFORM * wave);
void sawtoothWave(WAVEFORM * wave);
void usage(int argc, char * argv[]);
void printWave(WAVEFORM * wave);
void dumpWaveMetadata(WAVEFORM * wave);
void setFunction(WAVEFORM * wave, char * type);
WAVEFORM * readCmdLine(int argc, char * argv[]);

#endif /* TRIANGLE_H_ */
