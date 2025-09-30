# Makefile for ForestFire project
# Originally created by Joel Adams, Calvin University
# Modified by Steven McKelvey, Calvin University
# To build, enter: make

# name of the binary
PROGRAM   = Fire

# source files
SRCS      = firestarter.cpp X-graph.cpp display.c Tree.cpp Forest.cpp FireSimulator.cpp

# object files from source files
CONVERT_CPP = $(SRCS:.cpp=.o)
OBJS      =  $(CONVERT_CPP:.c=.o)

TRNG = ../../../../trng4/trng/trng/libtrng4.a

# which compiler to use
CC        = mpic++

# flags for compilation and linking 
CFLAGS    = -I/usr/X11R6/include \
			-I../../../../trng4 \
			-I../../../../trng4/trng \
			-I../../../../trng4/trng/trng/CMakeFiles/trng4.dir \
			-Wall -fopenmp \
			-Wno-narrowing # ignores narrowing warnings that come from the trng library

LFLAGS    = -L/usr/X11R6/lib -lX11 -lm -fopenmp

# Rules
.PHONY: clean

# valid file suffixes 
.SUFFIXES: .c .o .cpp

# Link object filese
$(PROGRAM): $(OBJS)
	@echo "Linking $@..."
	@$(CC) $^ $(TRNG) $(LFLAGS) -o $@ \
	|| { echo "Linking failed for $@"; exit 1; }
	@echo "Finished building $@"
	@echo " "

# command to build .o files from .cpp files
%.o: %.cpp
	@echo "Compiling $< to $@..."
	@$(CC) $(CFLAGS) -c $< -o $@ \
	|| { echo "Compiliation failed for $<"; exit 1; }
	@echo "Finished compiling $<"
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
firestarter.o: X-graph.h FireSimulator.h
Forest.o: Tree.h
FireSimulator.o: Forest.h

clean:
	/bin/rm -f $(OBJS) $(PROGRAM) *~ *#

