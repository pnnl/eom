#ifndef GENERIC_HPP_
#define GENERIC_HPP_

#if defined OS_WINDOWS
#  define int32_t int
#else
#  include <stdint.h>
#endif

#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cmath>
#include <errno.h>

namespace boost {
	namespace gregorian { class date; }
	namespace posix_time { class ptime; class time_duration; }
}

typedef int32_t TIME;
typedef int32_t FLAG;
typedef double  FLOAT;
typedef double  LONG_FLOAT;
typedef int32_t INTEGER;
typedef int64_t LONG_INTEGER;
typedef bool    LOGICAL;

typedef int     FILE_HANDLE;

#if defined COMPRESSED_TIME
#define MINUTES_IN_DAY          8
#define HOURS_IN_DAY            2
#define MINUTES_IN_HOUR         4
#else
#define MINUTES_IN_DAY       1440
#define HOURS_IN_DAY           24
#define MINUTES_IN_HOUR        60
#endif

#if _WIN32

#define EOL                "\r\n"
#define EOL_CHARACTERS          2

#else

#define EOL                  "\n"
#define EOL_CHARACTERS          1

#endif

#define EMPTY_BUFFER   NULL
#define END_OF_LINE(c)  (c == '\r' || c == '\n')
#define MAX(a,b) (a >= b ? a : b)

enum DATE_FORMAT
{
	YYYYMMDD_HYPEN, MMDDYYYY_HYPEN, YYYYMMDD_SLASH, MMDDYYYY_SLASH
};

enum TIME_FORMAT
{
	HHMMSS_HYPEN, HHMM_HYPEN, HHMMSS_COLON, HHMM_COLON
};

/**
 * All file input accesses should use the <i>FileData</i> structure
 * exclusively. File output is optional. Input files are handled in one of two
 * ways: copy the entire file into a single data space or copy file blocks into
 * the data space treating it as a circular buffer. The circular buffer is
 * composed of an integer number of blocks. When the incremental load places a
 * block into the last position of the buffer, it will place the next block in
 * first position of the buffer.
 *
 * The file control stores information regarding:
 *     1) The file
 *        a) fileName        name of the source file
 *        b) fileSize        size of the source file
 *        c) fileHandle      handle of the open file
 *     2) The file input buffer
 *        a) blockSize       size of a block
 *        b) nBlocks         number of blocks in the input file
 *        c) fileBufferSize  size of the input buffer
 *        d) fileBuffer      the input buffer (int bytes)
 *        e) readPoint       the next block from which a block should be read
 *        f) loadPoint       the next block to receive data from the file
 *     3) ordered buffer
 *        a) nOutputBlocks   number of blocks in the output buffer
 *        b) outputSize      size of the output buffer (in bytes)
 *        c) outputBuffer    the buffer used to hold contiguous data
 *
 * For data stored in a circular buffer, the data is transferred to a contiguous
 * output buffer. The data in this buffer is stored in order. For example,
 * consider a the fileBuffer holds five blocks and the output buffer holds
 * three blocks. A transfer of data from the file buffer to the output buffer
 * when the loadPoint is 4 should leave the output buffer with the following
 * block sequence 4,0,1.
 */
struct FileData
{
	// file information, fileSize is the actual size of the file
	char                     fileName[256];
	INTEGER                  fileSize;
	FILE_HANDLE              fileHandle;

	// file input buffer
	INTEGER                  blockSize;
	INTEGER                  fileBuffSize;
	char *                   fileBuffer;

	INTEGER                  readPoint;
	INTEGER                  loadPoint;

	// ordered file buffer
	INTEGER                  nOutputBlocks;
	char *                   outputBuffer;
};

struct Date
{
	INTEGER                  year;
	INTEGER                  month;
	INTEGER                  day;
	operator boost::gregorian::date() const;
};

struct TimeOfDay
{
	INTEGER                  hours;
	INTEGER                  minutes;
	INTEGER                  seconds;
	operator boost::posix_time::time_duration() const;
};

struct Time
{
	Date                     date;
	TimeOfDay                tod;
	LONG_INTEGER             minutesPastZero;
	operator boost::posix_time::ptime() const;
};

/* ****************************************************************************
 * Piecewise linear curve
 *************************************************************************** */
#define N_PW_SEGMENTS     5   // maximum number of points in a piecewise linear curve
#define PW_DIM            2

struct LineSegment
{
	FLOAT                    startPoint[PW_DIM];
	FLOAT                    endPoint[PW_DIM];
	FLOAT                    slope;
	FLOAT                    intercept;
	FLOAT                    delta[PW_DIM];
	LineSegment() {
		startPoint[0] = nan("");
		startPoint[1] = nan("");
		endPoint  [0] = nan("");
		endPoint  [1] = nan("");
		slope         = nan("");
		intercept     = nan("");
		delta     [0] = nan("");
		delta     [1] = nan("");
	};
	LineSegment(double x0, double y0, double x1, double y1)
	{
		startPoint[0] = x0;
		startPoint[1] = y0;
		endPoint  [0] = x1;
		endPoint  [1] = y1;
		slope         = (y1-y0)/(x1-x0);
		intercept     = y0 - slope * x0;
		delta     [0] = x1-x0;
		delta     [1] = y1-y0;
	}; // y = mx + b; b = y - mx
};
struct PiecewiseLinear
{
	size_t                   nSegments;
	LineSegment              segment[N_PW_SEGMENTS];
	PiecewiseLinear() : nSegments(0) {};
};

struct Connection {
	INTEGER                  connectTo;
	FLOAT                    lineCapacity;
	FLOAT                    ptdf;
	Connection() :
		connectTo(-5),
		lineCapacity(-5),
		ptdf(-5)
	{};
};

#endif /* GENERIC_HPP_ */
