CXX		= g++
CXXFLAGS	= -Wall -pedantic-errors -Wno-variadic-macros -std=c++0x -pthread
SOURCES		= $(wildcard src/Engine/lib/*.cc) $(wildcard src/Engine/network/*.cc) $(wildcard src/UnitTests/*.cc)
OBJECTS		= $(SOURCES:%.cc=%.o)
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

.cc.o:
	$(CXX) $(INCLUDES) -c -o $@ $< $(CXXFLAGS)
