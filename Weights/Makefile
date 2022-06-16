FF	= gfortran
LD	= gfortran
CC	= g++

LDFLAGS	= `root-config --libs`  -lgfortran
FFFLAGS = -g -c
CCFLAGS = `root-config --cflags` -g -c

all: calcpapcf

calcpapcf: calcpapcf.o  FsiTools.o FsiWeightLednicky5.o
	$(CC) $^ -o $@ $(LDFLAGS)



%.o: %.F
	$(FF) $^ -o $@ $(FFFLAGS)

%.o: %.cxx
	$(CC) $^ -o $@ $(CCFLAGS)

clean:
	rm -f *.o calcpapcf *.d

