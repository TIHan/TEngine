CXX		= g++
CXXFLAGS	= -Wall -pedantic-errors -g -Wno-variadic-macros
SOURCES		= test.cpp
OBJECTS		= $(SOURCES:.cpp=.o)
OUTPUT		= te-test
INCLUDES	= -I../lib/te/include/ -I../network/include/
LIBS		= ../lib/te/telib.a ../network/network.a

all: $(SOURCES) $(OUTPUT)

$(OUTPUT): $(OBJECTS)
	$(CXX) $(OBJECTS) $(LIBS) -o $@

clean:
	rm -f $(OBJECTS) $(OUTPUT)

.cpp.o:
	$(CXX) $(INCLUDES) -c -o $@ $< $(CXXFLAGS)
