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

TEST_F(ListTest, ModifyingWorks) {
  l1_.Add(5);
  EXPECT_EQ(1, l1_.GetSize());
  l1_.Remove(5);
  EXPECT_EQ(0, l1_.GetSize());
  l1_.Add(6);
  l1_.Add(6);
  EXPECT_EQ(2, l1_.GetSize());
  l1_.Remove(6);
  EXPECT_EQ(1, l1_.GetSize());
  l1_.Add(7);
  EXPECT_EQ(2, l1_.GetSize());
  l1_.Remove(8);
  EXPECT_EQ(2, l1_.GetSize());
  l1_.Remove(6);
  l1_.Remove(7);
  EXPECT_EQ(0, l1_.GetSize());
}

TEST_F(ListTest, QueryingWorks) {
  l1_.Add(6);
  l1_.Add(2);
  l1_.Add(3);
  l1_.Add(6);
  l1_.Add(5);
  l1_.Add(6);
  l1_.Add(8);
  l1_.Add(8);
  l1_.Add(10);
  l1_.Add(10);
  l1_.Add(10);
  l1_.Add(10);

  EXPECT_EQ(1, static_pointer_cast<List<TEint>>(l1_.Where([] (TEint i) -> bool { return i == 2; }))->GetSize());
  EXPECT_EQ(1, static_pointer_cast<List<TEint>>(l1_.Where([] (TEint i) -> bool { return i == 3; }))->GetSize());
  EXPECT_EQ(2, static_pointer_cast<List<TEint>>(l1_.Where([] (TEint i) -> bool { return i == 8; }))->GetSize());
  EXPECT_EQ(4, static_pointer_cast<List<TEint>>(l1_.Where([] (TEint i) -> bool { return i == 10; }))->GetSize());
  EXPECT_EQ(3, static_pointer_cast<List<TEint>>(l1_.Where([] (TEint i) -> bool { return i == 6; }))->GetSize());
  EXPECT_EQ(0, static_pointer_cast<List<TEint>>(l1_.Where([] (TEint i) -> bool { return i == 7; }))->GetSize());
}
