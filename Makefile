CXX		= g++
CXXFLAGS	= -Wall -pedantic-errors -g -Wno-variadic-macros -std=c++0x -O2 -pthread
SOURCES		= src/test.cpp src/network/ASocket.cpp src/network/UdpSocket.cpp src/lib/ByteStream.cpp src/lib/System.cpp src/lib/Thread.cpp
OBJECTS		= $(SOURCES:%.cpp=%.o)
INCLUDES	= -Isrc/lib/include -Isrc/network/include
EXECUTABLE	= te-test

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CXX) -o $@ $(OBJECTS) $(CXXFLAGS)

clean:
	rm -f $(OBJECTS) $(EXECUTABLE)

.cpp.o:
	$(CXX) $(INCLUDES) -c -o $@ $< $(CXXFLAGS)
