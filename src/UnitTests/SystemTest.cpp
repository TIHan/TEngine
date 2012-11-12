#include <TELib.hpp>
#include <Network.hpp>
#include <gtest/gtest.h>

using namespace TE;

class SystemTest : public ::testing::Test {
};

TEST_F(SystemTest, TicksAndDelay) {
  TEuint64 ticks = System::GetTicks();
  System::Delay(1000);
  cout << System::GetTicks() - ticks << endl;
}