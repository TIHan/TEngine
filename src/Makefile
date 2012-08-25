CXX		= gcc
CXXFLAGS	= `sdl-config --cflags --libs` -Wall -pedantic-errors -g -std=c99
SOURCES		= test.c
OBJECTS		= $(SOURCES:.c=.o)
EXECUTABLE	= datagame-protocol-test

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CXX) -o $@ $(OBJECTS) $(CXXFLAGS)

clean:
	rm -f $(OBJECTS) $(EXECUTABLE)

.c.o:
	$(CXX) -c -o $@ $< $(CXXFLAGS)
