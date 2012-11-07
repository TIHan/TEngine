#include <TELib.hpp>
#include <Network.hpp>
#include "../Engine/network/UdpSocket.hpp"
#include <gtest/gtest.h>

using namespace TE;

// [WS] Unit Tests
class SocketTest : public ::testing::Test {
protected:
  shared_ptr<UdpSocket> s;
};

TEST_F(SocketTest, GetAddress) {
  s = make_shared<UdpSocket>(SOCKET_IPV4);
  string str = s->GetAddress();
  ASSERT_TRUE(!str.empty());
}

