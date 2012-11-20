#include <engine_lib.h>
#include <network.h>
#include "../Engine/network/udp_socket.h"
#include <gtest/gtest.h>

using namespace engine;

// [WS] Unit Tests
class SocketTest : public ::testing::Test {
};

TEST_F(SocketTest, SendAndReceive) {
  auto server = std::make_shared<network::UdpSocket>(network::SOCKET_IPV4); 
  server->OpenOrDie();
  server->Bind(1337);

  // Test close and re-open.
  server->Close();
  server->OpenOrDie();
  server->Bind(1337);

  auto client = std::make_shared<network::UdpSocket>();
  client->OpenOrDie("127.0.0.1", "1337");
  auto clientStream = std::make_shared<lib::ByteStream>(512);
  clientStream->WriteString("Hello");
  client->Send(*clientStream);

  auto tupleReceive = server->ReceiveFrom();
  auto receiveSequence = std::get<0>(tupleReceive);
  auto address = std::get<1>(tupleReceive);
  auto serverStream = std::make_shared<lib::ByteStream>(512);
  serverStream->WriteStream(*receiveSequence);

  EXPECT_EQ("127.0.0.1", network::GetSocketAddress(*address));
  EXPECT_EQ("Hello", serverStream->ReadString());

  server->SendTo(*serverStream, *address);

  auto clientTupleReceive = client->ReceiveFrom();
  auto clientReceive = std::get<0>(clientTupleReceive);
  auto serverAddress = std::get<1>(clientTupleReceive);
  auto clientReceiveStream = std::make_shared<lib::ByteStream>(512);
  clientReceiveStream->WriteStream(*clientReceive);

  EXPECT_EQ("127.0.0.1", network::GetSocketAddress(*serverAddress));
  EXPECT_EQ("Hello", clientReceiveStream->ReadString());

  server->Close();
  client->Close();
}

TEST_F(SocketTest, SendAndReceive2) {
  auto server = std::make_shared<network::UdpSocket>(network::SOCKET_IPV4); 
  server->OpenOrDie();
  server->Bind(1337);

  auto client = std::make_shared<network::UdpSocket>();
  client->OpenOrDie("127.0.0.1", "1337");

  auto clientStream = std::make_shared<lib::ByteStream>(512);
  clientStream->Write<int>(2147483647);
  int bytes = client->Send(*clientStream);
  EXPECT_EQ(4, bytes);
}

