#include <gtest/gtest.h>

class SystemTest : public ::testing::Test {
};

TEST_F(SystemTest, TicksAndDelay) {
}

TEST_F(SystemTest, Types) {
  EXPECT_EQ(1, static_cast<int>(sizeof(char)));
  EXPECT_EQ(1, static_cast<int>(sizeof(signed char)));
  EXPECT_EQ(1, static_cast<int>(sizeof(unsigned char)));
  EXPECT_EQ(2, static_cast<int>(sizeof(short)));
  EXPECT_EQ(2, static_cast<int>(sizeof(unsigned short)));
  EXPECT_EQ(4, static_cast<int>(sizeof(int)));
  EXPECT_EQ(4, static_cast<int>(sizeof(unsigned int)));
// long has specific sizes for each OS
//  EXPECT_EQ(4, static_cast<int>(sizeof(long)));
//  EXPECT_EQ(4, static_cast<int>(sizeof(unsigned long)));
  EXPECT_EQ(8, static_cast<int>(sizeof(long long)));
  EXPECT_EQ(8, static_cast<int>(sizeof(unsigned long long)));

  EXPECT_EQ(4, static_cast<int>(sizeof(float)));
  EXPECT_EQ(8, static_cast<int>(sizeof(double)));
  EXPECT_EQ(1, static_cast<int>(sizeof(bool)));
// void* has specific sizes for each OS
//  EXPECT_EQ(4, static_cast<int>(sizeof(void*)));
}