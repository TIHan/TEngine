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

TEST_F(NetworkTest, RealTest) {
  int choice = 0;
  std::cout << "Enter 1 to host, 2 to connect.\n";
  std::cin >> choice;

  if (choice != 1 && choice != 2) {
    return;
  }

  if (choice == 1) {
    network::Server server(1338);
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
    std::cout << "Press any key to process messages.\n";
    std::cin.get();
    std::cin.get();
    server.ProcessMessages();
    return;
  }

  std::string address;
  std::string port;

  std::cout << "Enter address: ";
  std::cin >> address;

  std::cout << "Enter port: ";
  std::cin >> port;

  network::Client client;
  client.Connect(address, port);

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
}

                                                                               