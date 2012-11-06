#include <TELib.hpp>
#include <Network.hpp>
#include <gtest/gtest.h>

using namespace TE;

// [WS] Unit Tests
class ListTest : public ::testing::Test {
protected:
  List<TEint> l1_;
};

TEST_F(ListTest, IsEmptyInitially) {
  EXPECT_EQ(0, l1_.GetSize());
}

TEST_F(ListTest, AddingWorks) {
  l1_.Add(5);
  EXPECT_EQ(1, l1_.GetSize());
}
