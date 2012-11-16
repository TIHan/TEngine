#include <TELib.hpp>
#include <Network.hpp>
#include <gtest/gtest.h>
#include <iostream>

using namespace TE;

class SystemTest : public ::testing::Test {
};

TEST_F(SystemTest, TicksAndDelay) {
}

TEST_F(SystemTest, Types) {
  EXPECT_EQ(1, sizeof(char));
  EXPECT_EQ(1, sizeof(signed char));
  EXPECT_EQ(1, sizeof(unsigned char));
  EXPECT_EQ(2, sizeof(short));
  EXPECT_EQ(2, sizeof(unsigned short));
  EXPECT_EQ(4, sizeof(int));
  EXPECT_EQ(4, sizeof(unsigned int));
  EXPECT_EQ(4, sizeof(long));
  EXPECT_EQ(4, sizeof(unsigned long));
  EXPECT_EQ(8, sizeof(long long));
  EXPECT_EQ(8, sizeof(unsigned long long));

  EXPECT_EQ(4, sizeof(float));
  EXPECT_EQ(8, sizeof(double));
  EXPECT_EQ(1, sizeof(bool));
}