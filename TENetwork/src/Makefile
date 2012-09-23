CXX		= g++
CXXFLAGS	= -Wall -pedantic-errors -g -Wno-variadic-macros
SOURCES		= test.cpp Network.cpp Socket.cpp ByteStream.cpp ByteIO.cpp
OBJECTS		= $(SOURCES:.cpp=.o)
EXECUTABLE	= te-network

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CXX) -o $@ $(OBJECTS) $(CXXFLAGS)

clean:
	rm -f $(OBJECTS) $(EXECUTABLE)

.c.o:
	$(CXX) -c -o $@ $< $(CXXFLAGS)
