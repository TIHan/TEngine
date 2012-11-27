#include <engine_network.h>
#include <gtest/gtest.h>

using namespace engine;

class NetworkTest : public ::testing::Test {
};

TEST_F(NetworkTest, ServerTest) {
  network::Server server(1337);
  server.Start();
  server.Stop();
  server.Start();
  std::cout << "Press any key to continue...\n";
  std::cin.get();
}

                                                                               