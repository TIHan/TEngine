CXX		= g++
CXXFLAGS	= -Wall -pedantic-errors -g -Wno-variadic-macros
SOURCES		= test.cpp
OBJECTS		= $(SOURCES:.cpp=.o)
OUTPUT		= te-test
INCLUDES	= -I../lib/te/include/ -I../network/include/
LIBS		= ../lib/te/telib.a ../network/network.a

all: $(SOURCES) $(OUTPUT)

$(OUTPUT): $(OBJECTS)
	cd ../lib/te/; make
	cd ../network/; make
	$(CXX) $(OBJECTS) $(LIBS) -o $@

clean:
	cd ../lib/te/; make clean
	cd ../network/; make clean
	rm -f $(OBJECTS) $(OUTPUT)

.cpp.o:
	$(CXX) $(INCLUDES) -c -o $@ $< $(CXXFLAGS)
