#include <TELib.hpp>
#include <Network.hpp>
#include "../Engine/network/UdpSocket.hpp"
#include <gtest/gtest.h>

using namespace TE;

// [WS] Unit Tests
class SocketTest : public ::testing::Test {
};

TEST_F(SocketTest, SendAndReceive) {
  auto server = make_shared<UdpSocket>(SOCKET_IPV4);
  EXPECT_EQ(false, server->HasErrors());
  server->Bind(1337);

  auto client = make_shared<UdpSocket>(SOCKET_IPV4, "127.0.0.1", "1337");
  EXPECT_EQ(false, client->HasErrors());
  auto clientStream = make_shared<ByteStream>(512);
  clientStream->WriteString("Hello");
  client->Send(clientStream);

  auto tupleReceive = server->ReceiveFrom();
  auto receiveSequence = get<0>(tupleReceive);
  auto address = get<1>(tupleReceive);
  auto serverStream = make_shared<ByteStream>(512, receiveSequence);

  EXPECT_EQ("127.0.0.1", Socket::GetAddress(address));
  EXPECT_EQ("Hello", serverStream->ReadString());

  server->SendTo(serverStream, address);

  auto clientTupleReceive = client->ReceiveFrom();
  auto clientReceive = get<0>(clientTupleReceive);
  auto serverAddress = get<1>(clientTupleReceive);
  auto clientReceiveStream = make_shared<ByteStream>(512, clientReceive);

  EXPECT_EQ("127.0.0.1", Socket::GetAddress(serverAddress));
  EXPECT_EQ("Hello", clientReceiveStream->ReadString());

}

