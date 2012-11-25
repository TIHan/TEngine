#include <engine_network.h>
#include <gtest/gtest.h>
#include <gmock/gmock.h>

using namespace engine;

class MockMessage : public network::MessageInterface {
public:
};

class MessageTest : public ::testing::Test {
};

TEST_F(MessageTest, TestAMessage) {

}
