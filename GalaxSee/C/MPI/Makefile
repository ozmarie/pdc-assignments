CC            = mpicc
CFLAGS	      = -Wall
LDFLAGS	      = -o 
LIBS          = -lm -lX11
#LIBS          = -lm -ljpeg -lpng -lz -lgd -lmpich

OBJS	      = nbody.o\
			stepModel.o\
			graphics.o\
			display.o\
			calcDerivs.o\
			galaxsee.o
		

PROGRAM		= galaxsee

all:		$(PROGRAM)

$(PROGRAM):	$(OBJS)
		$(CC) $(OBJS) $(LDFLAGS) $(PROGRAM) $(LIBS)


clean:;		rm -f $(OBJS) $(PROGRAM) out*.png
