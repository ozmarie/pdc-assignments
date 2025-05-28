CC            = nvc++
CFLAGS	      = -Wall -DHAS_X11 -acc=gpu -Minfo=accel
LFLAGS          = -lm -lX11

SHELL = /bin/bash
MODULE_LOAD = module load nvhpc-hpcx-cuda12/24.3

PROGRAM	= galaxsee

SRCS 	= galaxsee.cpp nbody.cpp graphics.cpp display.cpp calcDerivs.cpp
		
# Rules
.PHONY: all clean

all: $(PROGRAM)

# Link object files
$(PROGRAM): $(SRCS)
	@echo "Linking $@..."
	$(MODULE_LOAD) && $(CC) $(CFLAGS) -o $@ $^ $(LFLAGS) \
	|| { echo "Linking failed for $@"; exit 1; }
	@echo "Finished building $@"
	@echo " "

clean:		
	rm -f $(PROGRAM) out*.png
