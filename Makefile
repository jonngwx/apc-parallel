integrators= euler.o
equations= heat_eq.o
objects= $(integrators) $(equations) utils.o
solvers= heat_serial

CXX = CC
CXXFLAGS = -g  -O3 -ffast-math -Wall
LIBS = -lm

all: $(solvers)

heat_serial: heat_serial.o $(objects)
	$(CXX) -o $@ $^ $(CXXFLAGS) $(LIBS)

clean:
	$(RM) *.o
	$(RM) .depend

depend: 
	$(CXX) -MM $(CXXFLAGS) *.cc > .depend

-include .depend