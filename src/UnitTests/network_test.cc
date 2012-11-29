#include <engine_network.h>
#include <gtest/gtest.h>

using namespace engine;

class NetworkTest : public ::testing::Test {
};

TEST_F(NetworkTest, ConnectTest) {
  network::Server server(1337);
  server.Start();
  server.RegisterMessageCallback(network::ClientMessage::kConnect,
      [] (std::shared_ptr<network::ReceiveMessage>) {
    std::cout << "Client connected.\n";
  });

  network::Client client;
  client.Connect("127.0.0.1", "1337");
  auto connectMessage = client.CreateMessage(network::ClientMessage::kConnect);
  connectMessage->Write<uint8_t>(238);
  connectMessage->Send();

  std::cout << "Press any key to connect...\n";
  std::cin.get();
  client.SendMessages();

  std::cout << "Press any key to process messages...\n";
  std::cin.get();
  try {
    server.ProcessMessages();
  } catch (const std::exception& e) {
    std::cout << "Something bad happened on message read.\n";
  }
}

                                                                               