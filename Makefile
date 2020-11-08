CXX= g++-10
LINK = $(CXX)
EXE = nbody_simulation
CXXFLAGS = -fopenmp -O3 -std=c++11 -lpthread
# CXXFLAGS = -O3 -std=c++11

#CXXFLAGS += -g -DVERBOSE

SRC = $(wildcard *.cpp)
OBJ = $(SRC:.cpp=.o)

OBJ_ROOT = nbody.o

default: $(EXE)

$(EXE): $(OBJ) $(OBJ_ROOT)
	$(LINK) $(OBJ) $(CXXFLAGS) -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $<

clean:
	rm -rf *.o $(EXE)

clean-o:
	rm -rf *.o
