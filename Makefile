UNAME := $(shell uname)

ifeq ($(UNAME), Linux)
CXX		= g++ -pthread
endif
ifeq ($(UNAME), Darwin)
CXX		= clang++ -stdlib=libc++
endif
CXXFLAGS	= -Wall -std=c++11 -DGTEST_USE_OWN_TR1_TUPLE=1
SOURCES		= $(wildcard src/Engine/lib/*.cc) $(wildcard src/Engine/network/*.cc) $(wildcard src/UnitTests/*.cc)
OBJECTS		= $(SOURCES:%.cc=%.o)
INCLUDES	= -Isrc/Engine/lib/include -Isrc/Engine/network/include -Ilib/gmock-1.6.0 -Ilib/gmock-1.6.0/include -Ilib/gmock-1.6.0/gtest -Ilib/gmock-1.6.0/gtest/include -Ilib/bullet-2.81-rev2613/src
LIBS		= src/GoogleTest/libGoogleTest.a src/BulletDynamics/libBulletDynamics.a src/BulletCollision/libBulletCollision.a src/BulletLinearMath/libBulletLinearMath.a
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
