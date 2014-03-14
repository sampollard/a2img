/* Anything to image
 * Author: Sam Pollard
 * Last Modified:  March 14, 2014
 * Idea: Convert literally any file into an image.
 * Supported formats: PPM (default)
 */

#include <stdio.h>
#include <string.h>
#define MAX_FILENAME_LEN 255

int main(int argc, char *argv[])
{
	FILE *infile;
	FILE *outimg;
	char imgname[256];
	int  outfilelen;
	int  extsz = 3;	// The size of the file extension
	char extension[4] = "ppm";

	/* Get the name of the file to read and make the file to write to */
	switch (argc) {
	case 1 :
		printf("usage: a2img [options] filename imagename\n");
		return 1;
	case 2 : // Make the image name filename.extension
		outfilelen = strlen(argv[1]) + extsz;
		if (outfilelen > MAX_FILENAME_LEN) {
			printf("a2img: %s: file name too long\n", argv[1]);
			return 1;
		}
		strncpy(imgname, argv[1], outfilelen);
		strncpy(imgname, extension, extsz);
		outimg = fopen(imgname, "w");
		if (outimg == NULL) {
			printf("a2img: could not open %s\n", imgname);
			return 1;
		}
		break;
	case 3 : // Make the image name as it is given 
		outfilelen = strlen(argv[2]);
		if (outfilelen > MAX_FILENAME_LEN) {
			printf("a2img: %s: file name too long\n", argv[2]);
			return 1;
		}
		outimg = fopen(argv[2], "w");
		if (outimg == NULL) {
			printf("a2img: could not open %s\n", imgname);
			return 1;
		}
		break;
	default :
		printf("usage: a2img filename imagename");
		return 1;
	}

	infile = fopen(argv[1], "r");

	printf("Success!\n");
	return 0;
}