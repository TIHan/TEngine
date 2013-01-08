#include <engine_network.h>
#include <gtest/gtest.h>

using namespace engine;

class NetworkTest : public ::testing::Test {
};

TEST_F(NetworkTest, BasicTest) {
  std::chrono::milliseconds time(500);
  network::Server server(1337);
  network::Client client;
  network::Client client2;

  server.Start();
  client.Connect("127.0.0.1", "1337");
  client2.Connect("127.0.0.1", "1337");
  std::this_thread::sleep_for(time);
  server.SendMessages();
  std::this_thread::sleep_for(time);
  client.ProcessMessages();
  client2.ProcessMessages();

  // heartbeat test
  client.SendHeartbeat();
  client.SendMessages();
  std::this_thread::sleep_for(time);
  server.ProcessMessages();
  server.SendMessages();
  std::this_thread::sleep_for(time);
  client.ProcessMessages();
  client2.ProcessMessages();
}

                                                                               