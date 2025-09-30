# Makefile for ForestFire project
# Originally created by Joel Adams, Calvin University
# Modified by Steven McKelvey, Calvin University
# To build, enter: make

# name of the binary
PROGRAM   = Fire

# source files
SRCS      = firestarter.cu X-graph.cu display.cu lcrng.cu 

# object files from source files
OBJS      = $(SRCS:.cu=.o)

# which compiler to use
CC        = nvcc

# flags for compilation and linking 
# --device-c allows for separate compilation, -diag-suppress to allow string literals to be converted to char*
CFLAGS    = --device-c -I/usr/X11R6/include -diag-suppress 2464
LFLAGS    = -L/usr/X11R6/lib -lX11 -lgomp

# Rules
.PHONY: clean

# valid file suffixes 
.SUFFIXES: .cu .o

# Link object files
$(PROGRAM): $(OBJS)
	@echo "Linking $@..."
	@$(CC) $^ $(LFLAGS) -o $@ \
	|| { echo "Linking failed for $@"; exit 1; }
	@echo "Finished building $@"
	@echo " "

# command to build .o files from .cu files
%.o: %.cu
	@echo "Compiling $< to $@..."
	@$(CC) $(CFLAGS) -c $< -o $@ \
	|| { echo "Compiliation failed for $<"; exit 1; }
	@echo "Finished compiling $<"
	@echo " "

# other dependencies (based on #includes)
X-graph.o: X-graph.cuh display.cuh
display.o: display.cuh
firestarter.o: X-graph.cuh
lcrng.o: lcrng.cuh

clean:
	/bin/rm -f $(OBJS) $(PROGRAM) *~ *#

