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
    static int count;
    count++;
    std::cout << "Message " << count << std::endl;
    for (int i = 0; i < 511; ++i) {
      uint8_t hello = message->Read<uint8_t>();
    }
  });

  network::Client client;
  client.Connect("127.0.0.1", "1337");

  for (int i = 0; i < 512; ++i) {
  auto connectMessage = client.CreateMessage(
      network::ReservedClientMessage::kConnect);
  for (int i = 0; i < 511; ++i) {
    connectMessage->Write<uint8_t>(255);
  }
  connectMessage->Send();
  }

  std::cout << "Press any key to connect...\n";
  std::cin.get();
  client.SendMessages();

  std::cout << "Press any key to process messages...\n";
  std::cin.get();
  server.ProcessMessages();
}

                                                                               