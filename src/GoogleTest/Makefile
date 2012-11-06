CXX			= g++
CXXFLAGS	= -Wall -pedantic-errors -Wno-variadic-macros -std=c++0x -pthread
SOURCES		= ../../lib/gtest-1.6.0/src/gtest-all.cc
OBJECTS		= $(SOURCES:%.cc=%.o)
INCLUDES	= -I../../lib/gtest-1.6.0 -I../../lib/gtest-1.6.0/include
OUTPUT		= libGoogleTest.a

release: CXXFLAGS += -O2
release: $(SOURCES) $(OUTPUT)

debug: CXXFLAGS += -DDEBUG -g3
debug: $(SOURCES) $(OUTPUT)

$(OUTPUT): $(OBJECTS)
	ar rcs $(OUTPUT) $(OBJECTS)

clean:
	rm -f $(OBJECTS) $(OUTPUT)

.cc.o:
	$(CXX) $(INCLUDES) -c -o $@ $< $(CXXFLAGS)