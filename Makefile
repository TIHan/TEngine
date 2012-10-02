CXX		= g++
CXXFLAGS	= -Wall -pedantic-errors -g -Wno-variadic-macros
SOURCES		= src/test.cpp src/network/Network.cpp src/network/Socket.cpp src/lib/ByteStream.cpp src/network/Packet.cpp
OBJECTS		= $(SOURCES:%.cpp=%.o)
INCLUDES	= -Isrc/lib -Isrc/network
EXECUTABLE	= te-test

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CXX) -o $@ $(OBJECTS) $(CXXFLAGS)

clean:
	rm -f $(OBJECTS) $(EXECUTABLE)

.cpp.o:
	$(CXX) $(INCLUDES) -c -o $@ $< $(CXXFLAGS)
