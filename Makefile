CXX			= g++
CXXFLAGS	= -Wall -pedantic-errors -Wno-variadic-macros -std=c++0x -pthread
SOURCES		= $(wildcard src/*.cpp) $(wildcard src/lib/*.cpp) $(wildcard src/network/*.cpp)
OBJECTS		= $(SOURCES:%.cpp=%.o)
INCLUDES	= -Isrc/lib/include -Isrc/network/include
EXECUTABLE	= te-test

release: CXXFLAGS += -O2
release: $(SOURCES) $(EXECUTABLE)

debug: CXXFLAGS += -DDEBUG -g3
debug: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CXX) -o $@ $(OBJECTS) $(CXXFLAGS)

clean:
	rm -f $(OBJECTS) $(EXECUTABLE)

.cpp.o:
	$(CXX) $(INCLUDES) -c -o $@ $< $(CXXFLAGS)
