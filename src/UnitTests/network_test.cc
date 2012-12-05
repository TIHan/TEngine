#include <engine_network.h>
#include <gtest/gtest.h>

using namespace engine;

class NetworkTest : public ::testing::Test {
};

TEST_F(NetworkTest, BasicTest) {
  static int count = 0;

  network::Server server(1337);
  server.Start();
  server.RegisterMessageCallback(network::ReservedClientMessage::kConnect,
      [] (std::shared_ptr<network::ReceiveMessage> message) {
    count++;
    for (int i = 0; i < 1023; ++i) {
      uint8_t hello = message->Read<uint8_t>();
    }
  });

  network::Client client;
  client.Connect("127.0.0.1", "1337");

  for (int i = 0; i < 8; ++i) {
    auto connectMessage = client.CreateMessage(
        network::ReservedClientMessage::kConnect);
    for (int i = 0; i < 1023; ++i) {
      connectMessage->Write<uint8_t>(255);
    }
    connectMessage->Send();
  }

  std::cout << "Press any key to send messages...\n";
  std::cin.get();


  client.SendMessages();

  std::chrono::milliseconds time(25);
  std::this_thread::sleep_for(time);

  server.ProcessMessages();

  EXPECT_EQ(count, 8);
}

                                                                               