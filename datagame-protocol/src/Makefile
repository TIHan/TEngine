CXX		= g++
CXXFLAGS	= -Wall -pedantic-errors -g
SOURCES		= test.cpp
OBJECTS		= $(SOURCES:.cpp=.o)
EXECUTABLE	= datagame-protocol-test

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CXX) -o $@ $(OBJECTS) $(CXXFLAGS)

clean:
	rm -f $(OBJECTS) $(EXECUTABLE)

.c.o:
	$(CXX) -c -o $@ $< $(CXXFLAGS)
