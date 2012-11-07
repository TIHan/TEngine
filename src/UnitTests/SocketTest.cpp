#include <TELib.hpp>
#include <Network.hpp>
#include "../Engine/network/UdpSocket.hpp"
#include <gtest/gtest.h>

using namespace TE;

// [WS] Unit Tests
class SocketTest : public ::testing::Test {
};

TEST_F(SocketTest, SendAndReceive) {
  auto s2 = make_shared<UdpSocket>(SOCKET_IPV4);
  s2->Bind(1337);

  auto s1 = make_shared<UdpSocket>(SOCKET_IPV4, "127.0.0.1", "1337");
  auto byteS = make_shared<ByteStream>(512);
  byteS->WriteString("Hello");
  TEint results = s1->Send(byteS);

  auto tupleReceive = s2->ReceiveFrom();
  auto sequence = get<0>(tupleReceive);
  auto byteS2 = make_shared<ByteStream>(512, sequence);

  EXPECT_EQ("Hello", byteS2->ReadString());
}

