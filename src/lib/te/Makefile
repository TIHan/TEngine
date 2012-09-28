CXX		= g++
CXXFLAGS	= -static -Wall -pedantic-errors -g -Wno-variadic-macros
SOURCES		= ByteStream.cpp
OBJECTS		= $(SOURCES:.cpp=.o)
OUTPUT		= telib.a

all: $(SOURCES) $(OUTPUT)

$(OUTPUT): $(OBJECTS)
	ar rcs $(OUTPUT) $(OBJECTS)

clean:
	rm -f $(OBJECTS) $(OUTPUT)

.cpp.o:
	$(CXX) -c -o $@ $< $(CXXFLAGS)
