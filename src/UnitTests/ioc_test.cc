#include <engine_network.h>
#include <gtest/gtest.h>

class IocTest : public ::testing::Test {
};

using namespace engine;
using namespace lib;

TEST_F(IocTest, ReadAndWrite) {
  Ioc::Register<network::ServerInterface, network::Server>(1331);
  auto server = Ioc::Resolve<network::ServerInterface>();
}