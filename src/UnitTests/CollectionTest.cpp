#include <TELib.hpp>
#include <Network.hpp>
#include <gtest/gtest.h>

using namespace TE;

class CollectionTest : public ::testing::Test {
protected:
  shared_ptr<IList<int>> l1_;
  shared_ptr<ISequence<int>> s1_;
};

TEST_F(CollectionTest, IsEmptyInitially) {
  l1_ = make_shared<List<int>>();
  s1_ = make_shared<Sequence<int>>();
  EXPECT_EQ(0, l1_->GetSize());
  EXPECT_EQ(0, s1_->GetSize());
}

TEST_F(CollectionTest, TestListSpeed) {
  l1_ = make_shared<List<int>>();
  for (int i = 0; i < 50000; i++) {
    l1_->Add(i);
  }
}

TEST_F(CollectionTest, TestSequenceSpeed) {
  s1_ = make_shared<Sequence<int>>();
  for (int i = 0; i < 50000; i++) {
    s1_->Add(i);
  }
}