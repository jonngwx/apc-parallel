integrators= euler.o
equations= heat_eq.o
objects= $(integrators) $(equations) 

CXX = CC
CXXFLAGS = -g 
LIBS = -lm

all: solver

solver: solver.o $(objects)
	$(CXX) -o $@ $^ utils.h $(LIBS)

clean:
	$(RM) *.o
	$(RM) .depend

depend: 
	$(CXX) -MM $(CXXFLAGS) *.cc > .depend

-include .depend