#include <engine_lib.h>
#include <gtest/gtest.h>

class IocTest : public ::testing::Test {
};

using namespace engine;
using namespace lib;

class IocObjectInterface {
public:
  virtual ~IocObjectInterface() {}
};

class IocObject : public virtual IocObjectInterface {
};

TEST_F(IocTest, Singleton) {
}