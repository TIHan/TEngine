UNAME := $(shell uname)
GFLW_PATH	= lib/glfw-2.7.7
GMOCK_PATH	= lib/gmock-1.6.0
GTEST_PATH	= lib/gmock-1.6.0/gtest
BULLET_PATH	= lib/bullet-2.81-rev2613/src

# OS Specifics
ifeq ($(UNAME), Linux)
CXX		= g++
LIBS		:= -pthread -lrt -lX11 -lGLU -lGL
endif
ifeq ($(UNAME), Darwin)
CXX		= clang++ -stdlib=libc++
LIBS           := -framework Cocoa -framework OpenGL -framework IOKit
endif

# Sources
ENGINE_SOURCES	= $(wildcard src/Engine/lib/*.cc) $(wildcard src/Engine/network/*.cc)
UT_SOURCES	= $(wildcard src/UnitTests/*.cc)

# Includes
GFLW_INCLUDES	= -I$(GFLW_PATH)/include
GMOCK_INCLUDES	= -I$(GMOCK_PATH) -I$(GMOCK_PATH)/include
GTEST_INCLUDES 	= -I$(GTEST_PATH) -I$(GTEST_PATH)/include
BULLET_INCLUDES = -I$(BULLET_PATH)
ENGINE_INCLUDES	= -Isrc/Engine/lib/include -Isrc/Engine/network/include

# Libs
GFLW_LIBS	= src/GLFW/libGLFW.a
GTEST_LIBS	= src/GoogleTest/libGoogleTest.a
BULLET_LIBS	= src/BulletDynamics/libBulletDynamics.a src/BulletCollision/libBulletCollision.a src/BulletLinearMath/libBulletLinearMath.a

# Global Settings
CXXFLAGS	= -Wall -std=c++11 -DGTEST_USE_OWN_TR1_TUPLE=1
SOURCES		= $(ENGINE_SOURCES) $(UT_SOURCES)
OBJECTS		= $(SOURCES:%.cc=%.o)
INCLUDES	= $(ENGINE_INCLUDES) $(GMOCK_INCLUDES) $(GTEST_INCLUDES) $(BULLET_INCLUDES) $(GFLW_INCLUDES)
LIBS		:= $(GTEST_LIBS) $(BULLET_LIBS) $(GFLW_LIBS) $(LIBS)
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
