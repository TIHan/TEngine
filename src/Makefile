CXX		= g++
CXXFLAGS	= -Wall -pedantic-errors -g
SOURCES		= test.cpp
OBJECTS		= $(SOURCES:.cpp=.o)
EXECUTABLE	= datagame-protocol-test

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CXX) -o $@ $(OBJECTS)

clean:
	rm -f $(OBJECTS) $(EXECUTABLE)

.cpp.o:
	$(CXX) $(CXXFLAGS) -c -o $@ $<
