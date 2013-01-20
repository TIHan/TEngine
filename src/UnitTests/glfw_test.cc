#include <engine_lib.h>
#include <GL/glfw.h>
#include <gtest/gtest.h>

class GLFWTest : public ::testing::Test {
};

TEST_F(GLFWTest, GLFW1) {
  EXPECT_TRUE(glfwInit());
}