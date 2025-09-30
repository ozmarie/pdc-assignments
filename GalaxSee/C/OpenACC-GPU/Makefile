CC            = nvc
CFLAGS	      = -Wall -acc=gpu -Minfo=accel
LFLAGS          = -lm -lX11
#LFLAGS          = -lm -ljpeg -lpng -lz -lgd -lmpich

SHELL = /bin/bash
MODULE_LOAD = module load nvhpc-hpcx-cuda12/24.3

SRCS	= galaxsee.c nbody.c stepModel.c graphics.c display.c calcDerivs.c 

PROGRAM		= galaxsee
	
# Rules
.PHONY: all clean

all: $(PROGRAM)

# Link object files
$(PROGRAM): $(SRCS)
	@echo "Linking $@..."
	@$(MODULE_LOAD) && $(CC) $(CFLAGS) -o $@ $^ $(LFLAGS) \
	|| { echo "Linking failed for $@"; exit 1; }
	@echo "Finished building $@"
	@echo " "

clean:		
	rm -f $(PROGRAM) out*.png
