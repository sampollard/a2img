/* Anything to image
 * Author: Sam Pollard
 * Last Modified: July 22, 2014
 * Idea: Convert literally any file into an image.
 * Supported formats: PPM (default)
 */
#include "a2img.h"

/* Helper function(s) */
int getflags(int argc, char *argv[], int *flags);
int isDigit(char c); // replaces the horrible ctype isDigit function
long offset; // Global variable to store how many bits the program should skip

int main(int argc, char *argv[])
{
	FILE *infile;
	FILE *outimg;
	char imgname[256];
	int  outfilelen;

	/* Handle the extension */
	int  extsz = 4;	// Number of characters in the file extension
	char *extension;
	int retval = 0;
	extension = malloc(sizeof(char)*(extsz+1));
	strncpy(extension, ".ppm", extsz+1);

	/* Get the flags & skip past them for argv */
	int argoffs = 0; // Argument offset (to skip flags)
	int flags = 0;
	if (argc > 1) {
		if ((argoffs = getflags(argc, argv, &flags)) < 0)
			return 1;
	} else {
		printf(USAGE_STRING);
		return 1;
	}

	/* Check that the input file exists */
	if (argc - argoffs == 1) {
		printf(USAGE_STRING);
		free(extension);
		return 1;
	}
	if (access(argv[argoffs+1], F_OK) != -1) {
		infile = fopen(argv[argoffs+1], "r");
		if (infile == NULL) {
			perror("fopen");
			free(extension);
			return 1;
		}
	} else {
		perror("access");
		free(extension);
		return 1;
	}

	/* Get the name of the file to write to */
	switch (argc - argoffs) {
	case 2 : // Make the image name filename.extension
		outfilelen = strlen(argv[argoffs+1]) + extsz;
		if (outfilelen > MAX_FILENAME_LEN) {
			printf("%s: %s: file name too long\n", argv[0], argv[argoffs+1]);
			retval = 1;
			break;
		}
		strncpy(imgname, argv[argoffs+1], outfilelen);
		strncpy(&imgname[outfilelen-extsz], extension, extsz);
		imgname[outfilelen] = 0;
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
			printf("%s: %s: file name too long\n", argv[0], argv[argoffs+2]);
			retval = 1;
			break;
		}
		if (strcmp(argv[argoffs+1], argv[argoffs+2]) == 0) {
			printf("%s: input file and image name must be different\n",
					argv[0]);
			retval = 1;
			break;
		}
		outimg = fopen(argv[argoffs+2], "w");
		if (outimg == NULL) {
			printf("%s: could not open %s\n", argv[0], imgname);
			retval = 1;
		}
		break;
	default :
		printf(USAGE_STRING);
		retval = 1;
	}

	/* Infile and outfile are all set up by now. Start writing! */
	if (retval == 0)
		if (makeppm(infile, outimg, flags) < 0) {
			fprintf(stderr, "%s: unable to copy file. Deleting %s\n",
					argv[0], imgname);
			if (remove(imgname) < 0)
				perror("remove");
			retval = 1;
		}
	free(extension);
	return retval;
}

/* Given the command line arguments from main, this counts the number of flags
 * and sets them in the flags fiend. Returns a nonnegative number for normal
 * behavior, a negative number if an error occurs (e.g. can't find README file).
 */
int getflags(int argc, char *argv[], int *flags)
{
	int argoffs = 0;
	while (argv[argoffs+1] != NULL && strncmp(argv[argoffs+1], "-", 1) == 0) {
		if (strncmp(argv[argoffs+1], "-h", 2) == 0) {
			if (system("less README.md") != 0) {
				printf("The README is available at ");
				printf(README_URL); printf("\n");
				return -1;
			}
		} else if (strncmp(argv[argoffs+1], "-v", 2) == 0) {
			*flags = *flags | VERBOSE;
		} else if (strncmp(argv[argoffs+1], "-r", 2) == 0) {
			*flags = *flags | RAWPPM;
		} else if (strncmp(argv[argoffs+1], "-o", 2) == 0) {
			// Found offset flag. Check that the next arg is a positive integer.
			if (argv[argoffs+2] == NULL) {
				printf("usage: a2img -o bit-offset filename [imagename]\n");
				return -1;
			} else if (!isDigit(*argv[argoffs+2])) {
				printf("Offset flag must be followed by a positive integer.\n");
				return -1;
			}
			// Offset is valid. Store the value and move on.
			// printf("offset = %d\n", atoi(argv[argoffs+2])); // TEST
			offset = (long) atoi(argv[argoffs+2]);
			argoffs++;
		*flags = *flags | OFFSET;
		}
		argoffs++;
	}
	return argoffs;
}

/* One pixel of is represented by 24 consecutive bits (RGB, respectively).
 * each is read as 1 byte, unsigned (range 0-255). Returns 0 on success,
 * -1 on error. */
int makeppm(FILE *infile, FILE *outppm, int mode)
{
	unsigned char pixel[3];	// [0] = Red, [1] = Green, [2] = Blue
	unsigned char tmp[4];   // Enough for offset
	long insize;			// To determine how large to make the image
	int width, height;		// Of the image, in pixels
	int offbits;			// offset modulo 8 (fseek adjusts by bytes)
	size_t sz;              // Read size

	/* Determine dimensions of the image */
	fseek(infile, 0L, SEEK_END);
	insize = ftell(infile);
	rewind(infile);
	width = (int) sqrt((insize - (offset+1)/8+1) / 3);
	height = width;

	/* Adjust file pointer to the offset */
	if (insize < offset) {
		printf("Offset is larger than file\n");
		return -1;
	} else if (insize == 0) {
		printf("Warning: input file has a size of 0\n");
	}
	fseek(infile, offset / 8, SEEK_CUR);
	offbits = offset % 8;

	/* Verbose */
	if (mode & VERBOSE) {
		printf("Size of input file: %ldB\nSize of image: %dx%d pixels\n",
				insize, height, width);
		if (mode & OFFSET)
			printf("Offset: %ldb\n", offset);
	}

	/* Make the ppm! */
	/* Magic Number (P6 is raw ppm, P3 is plain ppm), width, height, maxval */
	if (mode & RAWPPM)
		fprintf(outppm, "P6 %d %d 255\n", width, height);
	else
		fprintf(outppm, "P3 %d %d 255\n", width, height);
	int col, row;
	int mask = -1;
	mask = ~((unsigned char)mask >> offbits);
	for (row = 0; row < height; row++) {
		for (col = 0; col < width; col++) {
			// Read 4 bytes to account for offset
			sz = fread(tmp, 4, 1, infile);
			if (sz == 0) {
				if(!feof(infile)) {
					perror("fread");
					fclose(infile);
					fclose(outppm);
					return -1;
				}
			}
			fseek(infile, -1, SEEK_CUR);
			pixel[0] = tmp[0] << offbits;
			pixel[0] = pixel[0] | ((tmp[1] & mask) >> (8 - offbits));
			pixel[1] = tmp[1] << offbits;
			pixel[1] = pixel[1] | ((tmp[2] & mask) >> (8 - offbits));
			pixel[2] = tmp[2] << offbits;
			pixel[2] = pixel[2] | ((tmp[3] & mask) >> (8 - offbits));
			if (mode & RAWPPM)
				fwrite(pixel, 1, 3, outppm);
			else
				fprintf(outppm, "%hhu %hhu %hhu\n",
						pixel[0], pixel[1], pixel[2]);
		}
	}
	fclose(outppm);
	// printf("final row = %d, final col = %d\n", row, col); // TEST
	return 0;
}

int isDigit(char c) {
    switch (c) {
        case '0': ;
        case '1': ;
        case '2': ;
        case '3': ;
        case '4': ;
        case '5': ;
        case '6': ;
        case '7': ;
        case '8': ;
        case '9':
            return 1;
        default :
            return 0;
    }
}
