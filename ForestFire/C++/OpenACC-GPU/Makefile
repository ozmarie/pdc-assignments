# Makefile for ForestFire project
# Originally created by Joel Adams, Calvin University
# Modified by Steven McKelvey, Calvin University
# To build, enter: make

# name of the binary
PROGRAM   = Fire

# source files
SRCS      = firestarter.cpp X-graph.cpp display.c Tree.cpp Forest.cpp FireSimulator.cpp lcrng.c

# object files from source files
CONVERT_CPP = $(SRCS:.cpp=.o)
OBJS      =  $(CONVERT_CPP:.c=.o)

# which compiler to use
CC        = nvc++
# flags for compilation and linking 
# Uncomment -Minfo=accel for details about GPU offloading
FLAGS    = -I/usr/X11R6/include -L/usr/X11R6/lib -lX11 -fast -acc=gpu# -Minfo=accel

SHELL = /bin/bash
MODULE_LOAD = module load nvhpc-hpcx-cuda12/24.3

# Rules
.PHONY: clean

# valid file suffixes 
.SUFFIXES: .c .o .cpp

# Link object files
$(PROGRAM): $(SRCS)
	@echo "Linking $@..."
	@$(MODULE_LOAD) && $(CC) $(FLAGS) -o $@ $^ \
	|| { echo "Linking failed for $@"; exit 1; }
	@echo "Finished building $@"
	@echo " "

# other dependencies (based on #includes)
X-graph.o: X-graph.h display.h
display.o: display.h
firestarter.o: X-graph.h FireSimulator.h
Forest.o: Tree.h
FireSimulator.o: Forest.h
lcrng.o: lcrng.h

clean:
	/bin/rm -f $(OBJS) $(PROGRAM) *~ *#

