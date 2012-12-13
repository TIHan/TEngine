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
  Ioc::Register<IocObjectInterface>([] {
    return std::make_shared<IocObject>();
  })->Singleton();

  auto obj = Ioc::Resolve<IocObjectInterface>();
  auto obj2 = Ioc::Resolve<IocObjectInterface>();
  EXPECT_EQ(obj, obj2);

  auto obj3 = Ioc::Resolve<IocObjectInterface>([] {
    return std::make_shared<IocObject>();
  });
  for (int i = 0; i < 1000000; ++i) auto c = Ioc::Resolve<IocObjectInterface>([] {
    return std::make_shared<IocObject>();
  });
  EXPECT_FALSE(obj == obj3);
  EXPECT_EQ(obj, obj2);
}