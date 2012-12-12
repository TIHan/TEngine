#include <engine_network.h>
#include <gtest/gtest.h>

class IocTest : public ::testing::Test {
};

using namespace engine;
using namespace lib;

TEST_F(IocTest, Singleton) {
  Ioc::Register<network::ServerInterface>([] {
    return std::make_shared<network::Server>(1331);
  });
  auto server = Ioc::Resolve<network::ServerInterface>();
  auto server2 = Ioc::Resolve<network::ServerInterface>([] {
    return std::make_shared<network::Server>(1338);
  });
}