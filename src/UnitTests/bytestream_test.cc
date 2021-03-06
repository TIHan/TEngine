#include <engine_lib.h>
#include <gtest/gtest.h>

class ByteStreamTest : public ::testing::Test {
};

TEST_F(ByteStreamTest, ReadAndWrite) {
  auto stream = std::make_shared<ByteStream>();
  stream->Write<int>(50);
  EXPECT_EQ(50, stream->Read<int>());
}