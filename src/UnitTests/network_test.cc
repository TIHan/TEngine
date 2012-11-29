#include <engine_network.h>
#include <gtest/gtest.h>

using namespace engine;

class NetworkTest : public ::testing::Test {
};

TEST_F(NetworkTest, ConnectTest) {
  network::Server server(1337); // TODO: need to set udp socket type
  server.Start();
  server.RegisterMessageCallback(network::ClientMessage::kConnect,
      [] (std::shared_ptr<network::ReceiveMessage>) {
    std::cout << "Client connected.\n";
  });

  network::Client client;
  client.Connect("127.0.0.1", "1337"); // hack
  auto connectMessage = client.CreateMessage(network::ClientMessage::kConnect);
  connectMessage->Send();

  std::cout << "Press any key to connect...\n";
  std::cin.get();
  client.SendMessages();

  std::cout << "Press any key to process messages...\n";
  std::cin.get();
  server.ProcessMessages();
}

                                                                               