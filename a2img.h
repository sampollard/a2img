/* Anything to image
 * Author: Sam Pollard
 * Last Modified:  April 8, 2014
 * Header file for includes, constant definitions, prototypes
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>

#define MAX_FILENAME_LEN 255
#define VERBOSE 1 // If the "-v" flag is present
#define RAWPPM 2  // If the "-r" flag is present
#define OFFSET 4  // if the "-o" flag is present and followed by an integer > 0
#define USAGE_STRING "usage: a2img [options] filename [imagename]\n"
#define README_URL "https://github.com/sampollard/a2img/blob/master/README.md"

int makeppm(FILE *infile, FILE *outppm, int mode);