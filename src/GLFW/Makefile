UNAME := $(shell uname)
GFLW_PATH	= ../../lib/glfw-2.7.7

# OS Specifics
ifeq ($(UNAME), Linux)
CXX		= gcc -Wno-deprecated-declarations
SOURCES		:= $(wildcard $(GFLW_PATH)/lib/x11/*.c)
INCLUDES	:= -I$(GFLW_PATH)/lib/x11
endif
ifeq ($(UNAME), Darwin)
CXX		= clang
SOURCES		:= $(wildcard $(GFLW_PATH)/lib/cocoa/*.c)
INCLUDES	:= -I$(GFLW_PATH)/lib/cocoa
endif

# Global Settings
CXXFLAGS	= -Wall
SOURCES		:= $(wildcard $(GFLW_PATH)/lib/*.c) $(SOURCES)
OBJECTS		= $(SOURCES:%.c=%.o)
INCLUDES	:= -I$(GFLW_PATH)/lib $(INCLUDES)
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
