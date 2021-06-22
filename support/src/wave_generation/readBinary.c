#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>



int main(int argc, char * argv[])
{
	FILE        * fh            = 0;
	double        data          = 0.0;
	int           size          = 0;
	int           byte          = 0;
	int           err           = 0;
	struct stat status;
	if (argc != 2){
		printf("USAGE: readBin <file>\n");
		exit(-1);
	}

	err = stat(argv[1], &status);
	if (err < 0) {
		printf("ERROR: file is not accessible\n");
		exit(-1);
	}
	size = status.st_size;
	fh = fopen(argv[1], "rb");	
	while(byte < size) {
		fread(&data, sizeof(double), 1, fh);
		printf("%f\n", data);
		byte += sizeof(double);
	}	
	return 0;
}
