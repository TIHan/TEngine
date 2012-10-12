CXX		= g++
CXXFLAGS	= -Wall -pedantic-errors -g -Wno-variadic-macros -std=c++0x
SOURCES		= src/test.cpp src/network/Network.cpp src/network/Socket.cpp src/lib/ByteStream.cpp src/lib/System.cpp src/network/Packet.cpp
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
