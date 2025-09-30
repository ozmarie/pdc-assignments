CC 				= nvcc
CFLAGS			= -DHAS_X11
NVIDIA_COMPUTE = -arch=compute_86 -code=sm_86
# NVIDIA_COMPUTE = -arch=compute_61 -code=sm_61
# add NVIDIA_COMPUTE flags based on your GPU's compute capability (https://developer.nvidia.com/cuda-gpus). 
# Ex. compute capability of 6.1 -> compute_61 and sm_61
LDFLAGS	      = -o 
LFLAGS          = -lm -lX11

PROGRAM	= galaxsee

SRCS 	= nbody.cu graphics.cu display.cu calcDerivs.cu galaxsee.cu stepModel.cu

OBJS 	= $(SRCS:.cu=.o)
		
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

# command to build .o files from .cu files
%.o: %.cu
	@echo "Compiling $< to $@..."
	@$(CC) $(CFLAGS) $(NVIDIA_COMPUTE) -c $< -o $@ \
	|| { echo "Compiliation failed for $<"; exit 1; }
	@echo "Finished compiling $<"
	@echo " "

# Dependencies
nbody.o: nbody.cuh calcDerivs.cu
graphics.o: graphics.cuh display.cuh nbody.cuh
display.o: display.cuh
calcDerivs.o: nbody.cuh
galaxsee.o: galaxsee.cu nbody.cuh graphics.cuh

clean:		
	rm -f $(OBJS) $(PROGRAM) out*.png

