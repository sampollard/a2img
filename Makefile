# Anything to image
# Author: Sam Pollard
# Last Modified:  March 15, 2014

CC		= gcc
CFLAGS	= -g -Wall
SOURCES	= a2img.c
OBJECTS	= ${SOURCES:.c=.o}
LIBS	= m

a2img: $(OBJECTS)
	$(CC) $(CFLAGS) -o a2img $(OBJECTS) -l$(LIBS)

clean:
	rm -rf $(OBJECTS)

$(SOURCES): a2img.h