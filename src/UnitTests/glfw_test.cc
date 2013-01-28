#include <engine_lib.h>
#include <GL/glfw.h>
#include <gtest/gtest.h>

class GLFWTest : public ::testing::Test {
};

TEST_F(GLFWTest, Initialize) {
    EXPECT_TRUE(glfwInit());
    glfwTerminate();
}

TEST_F(GLFWTest, OpenWindow) {
    EXPECT_TRUE(glfwInit());
    
    EXPECT_TRUE(glfwOpenWindow(640, 480, 0, 0, 0, 0, 32, 0, GLFW_WINDOW));
    
    glfwTerminate();
}
