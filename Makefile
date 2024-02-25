CXX = g++ -std=c++20
CXXFLAGS = -Wall -g -O -MMD # use -MMD to generate dependencies
SRCDIRS = board pieces players views . 
SOURCES = $(foreach dir,$(SRCDIRS),$(wildcard $(dir)/*.cc))
OBJECTS = ${SOURCES:.cc=.o}  # .o files depend upon .cc files with same names
DEPENDS = ${OBJECTS:.o=.d}   # .d file is list of dependencies for corresponding .cc file
EXEC=chess


# First target in the makefile is the default target.
$(EXEC): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $(EXEC) -lX11

SUBDIRS := $(wildcard */.)

all: $(SUBDIRS)
$(SUBDIRS):
		$(MAKE) -C $@

%.o: %.cc 
	$(CXX) -c -o $@ $< $(CXXFLAGS) $(LIBFLAGS)

-include ${DEPENDS}

.PHONY: clean
clean:
	rm  -f $(OBJECTS) $(DEPENDS) $(EXEC)
