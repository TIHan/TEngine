#include <TELib.hpp>
#include <Network.hpp>
#include <gtest/gtest.h>

using namespace TE;

class SharedPointerTest : public ::testing::Test {
};

TEST_F(SharedPointerTest, Testy) {
  SharedPointer<ByteStream> byteStream(new ByteStream(512));
}