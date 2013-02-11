UNAME := $(shell uname)
GLEW_PATH	= ../../lib/glew-1.9.0

# OS Specifics
ifeq ($(UNAME), Linux)
CXX		= gcc
endif
ifeq ($(UNAME), Darwin)
CXX		= clang
endif

# Global Settings
CXXFLAGS	= -Wall -DGLEW_STATIC
SOURCES		:= $(wildcard $(GLEW_PATH)/src/glew.c) $(SOURCES)
OBJECTS		= $(SOURCES:%.c=%.o)
INCLUDES	:= -I$(GLEW_PATH)/include
OUTPUT		= libGLEW.a

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

.m.o:
	$(CXX) $(INCLUDES) -c -o $@ $< $(CXXFLAGS)
