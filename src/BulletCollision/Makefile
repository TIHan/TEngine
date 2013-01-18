CXX		= clang++
CXXFLAGS	= -Wall -std=c++11 -stdlib=libc++
SOURCES		= $(wildcard ../../lib/bullet-2.81-rev2613/src/BulletCollision/BroadphaseCollision/*.cpp) $(wildcard ../../lib/bullet-2.81-rev2613/src/BulletCollision/CollisionDispatch/*.cpp) $(wildcard ../../lib/bullet-2.81-rev2613/src/BulletCollision/CollisionShapes/*.cpp) $(wildcard ../../lib/bullet-2.81-rev2613/src/BulletCollision/Gimpact/*.cpp) $(wildcard ../../lib/bullet-2.81-rev2613/src/BulletCollision/NarrowPhaseCollision/*.cpp)
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
