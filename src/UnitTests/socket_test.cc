#include "../Engine/network/udp_socket.h"
#include <gtest/gtest.h>

using namespace engine;

// [WS] Unit Tests
class SocketTest : public ::testing::Test {
};

TEST_F(SocketTest, SendAndReceive) {
  network::UdpSocketOptions server_options;
  server_options.family = network::SocketFamily::kIpv4;
  auto server = std::make_shared<network::UdpSocket>(server_options); 
  server->Open();
  server->Bind(1337);

  // Test close and re-open.
  server->Close();
  server->Open();
  server->Bind(1337);

  auto client = std::make_shared<network::UdpSocket>();
  client->Open("127.0.0.1", "1337");
  auto clientStream = std::make_shared<ByteStream>();
  clientStream->WriteString("Hello");
  client->Send(*clientStream);

  auto tupleReceive = server->ReceiveFrom();
  auto serverStream = std::move(std::get<0>(tupleReceive));
  auto address = std::move(std::get<1>(tupleReceive));

  EXPECT_EQ("127.0.0.1", address->GetText());
  EXPECT_EQ("Hello", serverStream->ReadString());

  server->SendTo(*serverStream, *address);

  auto clientTupleReceive = client->ReceiveFrom();
  auto clientReceiveStream = std::move(std::get<0>(clientTupleReceive));
  auto serverAddress = std::move(std::get<1>(clientTupleReceive));

  EXPECT_EQ("127.0.0.1", address->GetText());
  EXPECT_EQ("Hello", clientReceiveStream->ReadString());

  server->Close();
  client->Close();
}

TEST_F(SocketTest, SendAndReceive2) {
  network::UdpSocketOptions server_options;
  server_options.family = network::SocketFamily::kIpv4;
  auto server = std::make_shared<network::UdpSocket>(server_options); 
  server->Open();
  server->Bind(1337);

  auto client = std::make_shared<network::UdpSocket>();
  client->Open("127.0.0.1", "1337");

  auto clientStream = std::make_shared<ByteStream>();
  clientStream->Write<int>(2147483647);
  int bytes = client->Send(*clientStream);
  EXPECT_EQ(4, bytes);
}

