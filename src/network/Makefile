CXX		= g++
CXXFLAGS	= -static -Wall -pedantic-errors -g -Wno-variadic-macros
SOURCES		= Network.cpp Packet.cpp Socket.cpp
OBJECTS		= $(SOURCES:.cpp=.o)
OUTPUT		= network.a
INCLUDES	= -I../lib/te/include/

all: $(SOURCES) $(OUTPUT)

$(OUTPUT): $(OBJECTS)
	ar rcs $(OUTPUT) $(OBJECTS)

clean:
	rm -f $(OBJECTS) $(OUTPUT)

.cpp.o:
	$(CXX) $(INCLUDES) -c -o $@ $< $(CXXFLAGS)
