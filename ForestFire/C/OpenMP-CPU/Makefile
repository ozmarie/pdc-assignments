# Makefile for ForestFire project
# Originally created by Joel Adams, Calvin University
# Modified by Steven McKelvey, Calvin University
# To build, enter: make

# name of the binary
PROGRAM   = Fire

# source files
SRCS      = firestarter.c X-graph.c display.c lcrng.c

# object files from source files
OBJS      = $(SRCS:.c=.o)

# which compiler to use
CC        = gcc

# flags for compilation and linking 
CFLAGS    = -I/usr/X11R6/include -Wall -fopenmp
LFLAGS    = -L/usr/X11R6/lib -lX11 -lm -fopenmp

# Rules
.PHONY: clean

# valid file suffixes 
.SUFFIXES: .c .o .cpp

# Link object files
$(PROGRAM): $(OBJS)
	@echo "Linking $@..."
	@$(CC) $^ $(LFLAGS) -o $@ \
	|| { echo "Linking failed for $@"; exit 1; }
	@echo "Finished building $@"
	@echo " "

# command to build .o files from .c files
%.o: %.c
	@echo "Compiling $< to $@..."
	@$(CC) $(CFLAGS) -c $< -o $@ \
	|| { echo "Compiliation failed for $<"; exit 1; }
	@echo "Finished compiling $<"
	@echo " "

# other dependencies (based on #includes)
X-graph.o: X-graph.h display.h
display.o: display.h
firestarter.o: X-graph.h
lcrng.o: lcrng.h

clean:
	/bin/rm -f $(OBJS) $(PROGRAM) *~ *#

