CXX			= g++
CXXFLAGS	= -Wall -pedantic-errors -Wno-variadic-macros -std=c++0x -pthread
SOURCES		= $(wildcard src/Engine/lib/*.cpp) $(wildcard src/Engine/network/*.cpp) $(wildcard src/UnitTests/*.cpp)
OBJECTS		= $(SOURCES:%.cpp=%.o)
INCLUDES	= -Isrc/Engine/lib/include -Isrc/Engine/network/include -Ilib/gtest-1.6.0 -Ilib/gtest-1.6.0/include
LIBS		= src/GoogleTest/libGoogleTest.a
EXECUTABLE	= te-test

release: CXXFLAGS += -O2
release: $(SOURCES) $(EXECUTABLE)

debug: CXXFLAGS += -DDEBUG -g3
debug: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CXX) -o $@ $(OBJECTS) $(CXXFLAGS) $(LIBS)

clean:
	rm -f $(OBJECTS) $(EXECUTABLE)

.cpp.o:
	$(CXX) $(INCLUDES) -c -o $@ $< $(CXXFLAGS)
