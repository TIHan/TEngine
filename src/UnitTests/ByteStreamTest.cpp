#include <TELib.hpp>
#include <Network.hpp>
#include <gtest/gtest.h>

using namespace TE;

class ByteStreamTest : public ::testing::Test {
};

TEST_F(ByteStreamTest, ReadAndWrite) {
  auto stream = make_shared<ByteStream>(512);
  stream->Write<int>(50);
  EXPECT_EQ(50, stream->Read<int>());
}