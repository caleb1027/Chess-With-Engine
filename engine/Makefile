CXX = g++ 
CXXFLAGS = -std=c++20 -Wall -g -O -MMD
INC=-I/D:/Eigen3
SOURCES = $(wildcard *.cc)
OBJECTS = ${SOURCES:.cc=.o}
DEPENDS = ${OBJECTS:.o=.d}
EXEC=layer

$(EXEC): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(INC) $(OBJECTS) -o $(EXEC)

%.o: %.cc 
	$(CXX) -c $(CXXFLAGS) -o $@ $< $(LIBFLAGS)

-include ${DEPENDS}

.PHONY: clean
clean:
	rm -f $(OBJECTS) $(DEPENDS)
