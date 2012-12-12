CXX		= g++
CXXFLAGS	= -Wall -Wno-variadic-macros -std=c++11 -pthread
SOURCES		= ../../lib/gmock-1.6.0/src/gmock-all.cc ../../lib/gmock-1.6.0/gtest/src/gtest-all.cc
OBJECTS		= $(SOURCES:%.cc=%.o)
INCLUDES	= -I../../lib/gmock-1.6.0 -I../../lib/gmock-1.6.0/include -I../../lib/gmock-1.6.0/gtest -I../../lib/gmock-1.6.0/gtest/include
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
