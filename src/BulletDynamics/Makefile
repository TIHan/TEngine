UNAME := $(shell uname)

ifeq ($(UNAME), Linux)
CXX		= g++
endif
ifeq ($(UNAME), Darwin)
CXX		= clang++ -stdlib=libc++
endif
CXXFLAGS	= -Wall -std=c++11
SOURCES		= $(wildcard ../../lib/bullet-2.81-rev2613/src/BulletDynamics/Character/*.cpp) $(wildcard ../../lib/bullet-2.81-rev2613/src/BulletDynamics/ConstraintSolver/*.cpp) $(wildcard ../../lib/bullet-2.81-rev2613/src/BulletDynamics/Dynamics/*.cpp) $(wildcard ../../lib/bullet-2.81-rev2613/src/BulletDynamics/Vehicle/*.cpp)
OBJECTS		= $(SOURCES:%.cpp=%.o)
INCLUDES	= -I../../lib/bullet-2.81-rev2613/src
OUTPUT		= libBulletDynamics.a

release: CXXFLAGS += -O2
release: $(SOURCES) $(OUTPUT)

debug: CXXFLAGS += -DDEBUG -g3
debug: $(SOURCES) $(OUTPUT)

$(OUTPUT): $(OBJECTS)
	ar rcs $(OUTPUT) $(OBJECTS)

clean:
	rm -f $(OBJECTS) $(OUTPUT)

.cpp.o:
	$(CXX) $(INCLUDES) -c -o $@ $< $(CXXFLAGS)
