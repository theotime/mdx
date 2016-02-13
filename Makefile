#
# Makefile
#

# compiler to use
CC = g++

# flags to pass compiler
CFLAGS = -g

# name for executable
EXE = mdx

# space-separated list of header files
HDRS = mdx.h geosets.h geoset.h

# space-separated list of libraries, if any,
# each of which should be prefixed with -l
LIBS = -lSDL2 -lGLEW -lGL -lSDL2_image

# space-separated list of source files
SRCS = main.cpp mdx.cpp geosets.cpp geoset.cpp

# automatically generated list of object files
OBJS = $(SRCS:.cpp=.o)


# default target
$(EXE): $(OBJS) $(HDRS) Makefile
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LIBS)

# dependencies 
$(OBJS): $(HDRS) Makefile

# housekeeping
clean:
	rm -f core $(EXE) *.o
