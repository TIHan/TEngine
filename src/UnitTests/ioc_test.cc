#include <engine_network.h>
#include <gtest/gtest.h>

class IocTest : public ::testing::Test {
};

using namespace engine;
using namespace lib;

TEST_F(IocTest, Singleton) {
  Ioc::Register<network::ServerInterface, network::Server>(1331)->Singleton();
  auto server = Ioc::Resolve<network::ServerInterface>();
  auto server2 = Ioc::Resolve<network::ServerInterface>();
  EXPECT_EQ(1331, server->port());
  EXPECT_EQ(server, server2);
}