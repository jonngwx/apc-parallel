integrators= euler.o
equations= heat_eq.o
objects= $(integrators) $(equations) utils.o
solvers= heat_serial heat_omp

CXX = CC
CXXFLAGS = -g  -O3 -ffast-math -Wall -fopenmp
PARALLELFLAGS = 
LIBS = -lm

all: $(solvers)

heat_serial: heat_serial.o $(objects)
	$(CXX) -o $@ $^ $(CXXFLAGS) $(LIBS)


heat_omp: heat_omp.o $(objects)
	$(CXX) -o $@ $^ $(PARALLELFLAGS) $(CXXFLAGS) $(LIBS)

clean:
	$(RM) *.o
	$(RM) .depend

depend: 
	$(CXX) -MM $(CXXFLAGS) $(PARALLELFLAGS) *.cc > .depend

-include .depend