UNAME := $(shell uname)

ifeq ($(UNAME), Linux)
CXX		= gcc
endif
ifeq ($(UNAME), Darwin)
CXX		= clang
endif
CXXFLAGS	= -Wall -std=c++11 -fpermissive
SOURCES		= $(wildcard ../../lib/glfw-2.7.7/lib/*.c) $(wildcard ../../lib/glfw-2.7.7/lib/x11/*.c)
OBJECTS		= $(SOURCES:%.c=%.o)
INCLUDES	= -I../../lib/glfw-2.7.7/lib -I../../lib/glfw-2.7.7/lib/x11
OUTPUT		= libGLFW.a

release: CXXFLAGS += -O2
release: $(SOURCES) $(OUTPUT)

debug: CXXFLAGS += -DDEBUG -g3
debug: $(SOURCES) $(OUTPUT)

$(OUTPUT): $(OBJECTS)
	ar rcs $(OUTPUT) $(OBJECTS)

clean:
	rm -f $(OBJECTS) $(OUTPUT)

.c.o:
	$(CXX) $(INCLUDES) -c -o $@ $< $(CXXFLAGS)
