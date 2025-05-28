CC            = g++
# CC            = mpic++
#CFLAGS	      = -Wall -ggdb
#CFLAGS	      = 
# CFLAGS	      = -Wall -DHAS_LIBGD -DHAS_X11 -DHAS_MPI
CFLAGS	      = -Wall -DHAS_X11 -DHAS_MPI -fopenmp
# CFLAGS	      = -Wall -DHAS_X11
#CFLAGS	      = -Wall -DHAS_MPI -g
LFLAGS          = -lm -lX11 -fopenmp
#LFLAGS          = -lm -ljpeg -lpng -lz -lgd -lmpich
#LFLAGS          = -lm 

PROGRAM	= galaxsee

SRCS 	= nbody.cpp graphics.cpp display.cpp calcDerivs.cpp galaxsee.cpp

OBJS 	= $(SRCS:.cpp=.o)
		
# Rules
.PHONY: all clean

all: $(PROGRAM)

# Link object files
$(PROGRAM): $(OBJS)
	@echo "Linking $@..."
	@$(CC) $^ $(LFLAGS) -o $@ \
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

# Dependencies
nbody.o: nbody.hpp calcDerivs.cpp
graphics.o: graphics.hpp display.hpp nbody.hpp
display.o: display.hpp
calcDerivs.o: nbody.hpp
galaxsee.o: galaxsee.cpp nbody.hpp graphics.hpp Simulator.hpp

clean:		
	rm -f $(OBJS) $(PROGRAM) out*.png
