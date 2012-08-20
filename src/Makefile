CXX		= gcc
CXXFLAGS	= -Wall -pedantic-errors -g -std=c99
SOURCES		= test.c
OBJECTS		= $(SOURCES:.c=.o)
EXECUTABLE	= datagame-protocol-test

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CXX) -o $@ $(OBJECTS)

clean:
	rm -f $(OBJECTS) $(EXECUTABLE)

.cpp.o:
	$(CXX) $(CXXFLAGS) -c -o $@ $<
