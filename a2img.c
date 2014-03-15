/* Anything to image
 * Author: Sam Pollard
 * Last Modified:  March 15, 2014
 * Idea: Convert literally any file into an image.
 * Supported formats: PPM (default)
 */
#include "a2img.h"

int main(int argc, char *argv[])
{
	FILE *infile;
	FILE *outimg;
	char imgname[256];
	int  outfilelen;
	int  extsz = 4;	// Number of characters in the file extension
	char *extension;
	int retval = 0;
	extension = malloc(sizeof(char)*(extsz+1));
	strncpy(extension, ".ppm", extsz+1);
	/* Get the name of the file to write to */
	switch (argc) {
	case 1 :
		printf("usage: a2img [options] filename imagename\n");
		return 1;
	case 2 : // Make the image name filename.extension
		outfilelen = strlen(argv[1]) + extsz;
		if (outfilelen > MAX_FILENAME_LEN) {
			printf("%s: %s: file name too long\n", argv[0], argv[1]);
			retval = 1;
			break;
		}
		strncpy(imgname, argv[1], outfilelen);
		strncpy(&imgname[outfilelen-extsz], extension, extsz);
		imgname[outfilelen] = 0;
		printf("%s\n", imgname); // TEST
		outimg = fopen(imgname, "w");
		if (outimg == NULL) {
			printf("%s: could not open %s\n", argv[0], imgname);
			retval = 1;
			break;
		}
		break;
	case 3 : // Make the image name as it is given 
		outfilelen = strlen(argv[2]);
		if (outfilelen > MAX_FILENAME_LEN) {
			printf("%s: %s: file name too long\n", argv[0], argv[2]);
			retval = 1;
			break;
		}
		if (strcmp(argv[1], argv[2]) == 0) {
			printf("%s: input file and image name must be different\n",
					argv[0]);
			retval = 1;
			break;
		}
		outimg = fopen(argv[2], "w");
		if (outimg == NULL) {
			printf("%s: could not open %s\n", argv[0], imgname);
			retval = 1;
			break;
		}
		break;
	default :
		printf("usage: a2img filename imagename");
		retval = 1;
	}
	/* Get the name of the file to read from */
	if (retval == 0) {
		infile = fopen(argv[1], "r");
		if (infile == NULL) {
			free(extension);
			return 1;
		} else {
			printf("Success!\n"); // TEST
		}
	}

	/* Infile and outfile are all set up by now. Start writing! */
	if (retval == 0)
		if (makeppm(infile, outimg) < 0) {
			fprintf(stderr, "%s: unable to copy file. Deleting %s\n",
					argv[0], imgname);
			if (remove(imgname) < 0)
				perror("remove");
			retval = 1;
		}
	free(extension);
	return retval;
}

/* TODO: Add a mode argument */
/* One pixel of is represented by 24 consecutive bits (RGB, respectively).
 * each is read as 1 byte, unsigned (range 0-255). Returns 0 on success,
 * -1 on error. */
int makeppm(FILE *infile, FILE *outppm)
{
	unsigned char pixel[3];	// [0] = Red, [1] = Green, [2] = Blue
	long insize;			// To determine how large to make the image
	int width, height;		// Of the image, in pixels
	long thirdpixcnt;		// The number of bytes to read (<= insize)

	/* Determine dimensions of the image */
	fseek(infile, 0L, SEEK_END);
	insize = ftell(infile);
	rewind(infile);
	width = (int) sqrt(insize/3);
	height = width;
	thirdpixcnt = width * height * 3;

	/* TEST */
	printf("size of infile: %ld\nSize of image: %dx%d\n",
			insize, height, width); // TEST
	printf("%ld < %ld\n", thirdpixcnt, insize);
	/* TSET */

	/* Make the ppm! */
	/* Magic Number (P6 is raw ppm, P3 is plain ppm), width, height, maxval */
	fprintf(outppm, "P3 %d %d 255\n", width, height);
	int col;
	int row;
	for (row = 0; row < height; row++) {
		for (col = 0; col < width; col++) {
			if (fread(pixel, 3, 1, infile) == 0) { // Read 3 bytes per fread
				if(!feof(infile)) {
					perror("fread");
					fclose(infile);
					fclose(outppm);
					return -1;
				}
			}
			fprintf(outppm, "%hhu %hhu %hhu\n", pixel[0], pixel[1], pixel[2]);
		}
	}
	fclose(outppm);
	printf("final row = %d, final col = %d\n", row, col);
	return 0;
}