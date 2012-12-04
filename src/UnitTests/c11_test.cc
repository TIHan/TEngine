#include <engine_lib.h>
#include <gtest/gtest.h>

class C11Test : public ::testing::Test {
};

TEST_F(C11Test, MoveSemantics) {
  std::vector<uint8_t> v;
  auto i = std::move(1);
  v.push_back(i);             // lvalue
  v.push_back(std::move(i));  // rvalue
  v.push_back(1);             // rvalue
  v.push_back(std::move(1));  // rvalue
}