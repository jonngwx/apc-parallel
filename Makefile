objects= heat_eq.o euler.o utils.o
objects_omp= heat_eq_omp.o euler_omp.o utils_omp.o
solvers= heat_serial heat_omp

CXX = g++
CXXFLAGS = -g  -Wall -O3 -ffast-math
OMPFLAGS = -fopenmp 
LIBS = -lm

all: $(solvers)

heat_serial: heat_serial.o $(objects)
	$(CXX) -o $@ $^ $(CXXFLAGS) $(LIBS)


heat_omp: heat_omp.o $(objects_omp)
	$(CXX) -o $@ $^ $(OMPFLAGS) $(CXXFLAGS) $(LIBS)

euler: euler.o
	$(CXX) -o $@ $^ $(CXXFLAGS) $(LIBS)

heat_eq: heat_eq.o
	$(CXX) -o $@ $^ $(CXXFLAGS) $(LIBS)

utils: utils.o
	$(CXX) -o $@ $^ $(CXXFLAGS) $(LIBS)

euler_omp.o: euler.cc
	$(CXX) -c -o euler_omp.o -c euler.cc $(CXXFLAGS) $(OMPFLAGS) $(LIBS)

heat_eq_omp.o: heat_eq.cc
	$(CXX) -c -o heat_eq_omp.o $^ $(CXXFLAGS) $(OMPFLAGS) $(LIBS)

utils_omp.o: utils.cc
	$(CXX) -c $^ $(CXXFLAGS) -o utils_omp.o  $(OMPFLAGS) $(LIBS)

clean:
	$(RM) *.o
