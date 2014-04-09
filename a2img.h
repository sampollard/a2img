/* Anything to image
 * Author: Sam Pollard
 * Last Modified:  March 15, 2014
 * Header file for includes, constant definitions, prototypes
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#define MAX_FILENAME_LEN 255
#define VERBOSE 1 // If the "-v" flag is present
#define USAGE_STRING "usage: a2img [options] filename imagename\n"

int makeppm(FILE *infile, FILE *outppm, int mode);