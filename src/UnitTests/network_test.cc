#include <engine_network.h>
#include <gtest/gtest.h>

using namespace engine;

class NetworkTest : public ::testing::Test {
};

TEST_F(NetworkTest, ConnectTest) {
  network::Server server(1337);
  server.Start();
  server.RegisterMessageCallback(network::ReservedClientMessage::kConnect,
      [] (std::shared_ptr<network::ReceiveMessage> message) {
    std::cout << "Client connected.\n";
    uint8_t hello = message->Read<uint8_t>();
  });

  network::Client client;
  client.Connect("127.0.0.1", "1337");
  auto connectMessage = client.CreateMessage(
      network::ReservedClientMessage::kConnect);
  connectMessage->Write<uint8_t>(238);
  connectMessage->Send();

  std::cout << "Press any key to connect...\n";
  std::cin.get();
  client.SendMessages();

  std::cout << "Press any key to process messages...\n";
  std::cin.get();
  server.ProcessMessages();
}

                                                                               