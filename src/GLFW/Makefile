UNAME := $(shell uname)
GFLW_PATH	= ../../lib/glfw-2.7.7/lib

# OS Specifics
ifeq ($(UNAME), Linux)
CXX		= gcc -Wno-deprecated-declarations
SOURCES		:= $(wildcard $(GFLW_PATH)/x11/*.c)
INCLUDES	:= -I$(GFLW_PATH)/x11
endif
ifeq ($(UNAME), Darwin)
CXX		= clang
SOURCES		:= $(wildcard $(GFLW_PATH)/cocoa/*.c)
INCLUDES	:= -I$(GFLW_PATH)/cocoa
endif

# Global Settings
CXXFLAGS	= -Wall
SOURCES		:= $(wildcard $(GFLW_PATH)/*.c) $(SOURCES)
OBJECTS		= $(SOURCES:%.c=%.o)
INCLUDES	:= -I$(GFLW_PATH) $(INCLUDES)
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
