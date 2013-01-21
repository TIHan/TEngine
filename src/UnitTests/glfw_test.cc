#include <engine_lib.h>
#include <GL/glfw.h>
#include <gtest/gtest.h>

class GLFWTest : public ::testing::Test {
};

TEST_F(GLFWTest, Initialize) {
    EXPECT_TRUE(glfwInit());
    glfwTerminate();
}

TEST_F(GLFWTest, Context3_2) {
    EXPECT_TRUE(glfwInit());
    
    glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 3);
    glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, 0);
    
    EXPECT_TRUE(glfwOpenWindow(640, 480, 0, 0, 0, 0, 32, 0, GLFW_WINDOW));
    
    glfwTerminate();
}
