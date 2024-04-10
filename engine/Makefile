CXX = g++ 
LIBTORCH=/d/libtorch
CXXFLAGS = -std=c++17 -O
SOURCES = $(wildcard *.cc)
OBJECTS = ${SOURCES:.cc=.o}
DEPENDS = ${OBJECTS:.o=.d}
EXEC=layer
INCLUDE=-I"$(LIBTORCH)/include" -I"$(LIBTORCH)/include/torch/csrc/api/include" -lc10 -lcaffe2 -ltorch
RPATH=-L/libtorch/lib/ -Wl,-rpath "$(LIBTORCH)/lib" 



$(EXEC): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(INC) $(OBJECTS) $(INCLUDE) -o $(EXEC) $(RPATH) -D_GLIBCXX_USE_CXX11_ABI=0

%.o: %.cc 
	$(CXX) -c  $(CXXFLAGS) -o $@ $< $(INCLUDE) $(LIBFLAGS) $(RPATH)  -D_GLIBCXX_USE_CXX11_ABI=0

-include ${DEPENDS}

.PHONY: clean
clean:
	rm -f $(OBJECTS) $(DEPENDS)

