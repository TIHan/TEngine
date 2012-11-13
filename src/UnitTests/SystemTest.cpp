#include <TELib.hpp>
#include <Network.hpp>
#include <gtest/gtest.h>

using namespace TE;

class SystemTest : public ::testing::Test {
};

TEST_F(SystemTest, TicksAndDelay) {
  for (int i = 0; i < 100; i++) {
    TEuint64 ticks = System::GetTicks();
    System::Delay(10);
    TEuint64 time = System::GetTicks() - ticks;
    cout << "TICKS: " << time << endl;
    ASSERT_TRUE(time <= 11);
  }
}