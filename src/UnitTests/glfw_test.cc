#include <engine_lib.h>
#include <GL/glew.h>
#include <GL/glfw.h>
#include <btBulletDynamicsCommon.h>
#include <gtest/gtest.h>

class GLFWTest : public ::testing::Test {
};

TEST_F(GLFWTest, Initialize) {
    EXPECT_TRUE(glfwInit());
    glfwTerminate();
}

TEST_F(GLFWTest, OpenWindow) {
  EXPECT_TRUE(glfwInit()); 

  glfwOpenWindowHint(GLFW_FSAA_SAMPLES, 4);
#ifdef _WIN32
  glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 3);
  glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, 3);
  glfwOpenWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#else
# ifdef __APPLE__
  glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 3);
  glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, 2);
  glfwOpenWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwOpenWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
# elif
  glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 3);
  glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, 0);
# endif
#endif

  EXPECT_TRUE(glfwOpenWindow(640, 360, 0, 0, 0, 0, 32, 0, GLFW_WINDOW));
  EXPECT_TRUE(glewInit() == GLEW_OK);


    
  glfwTerminate();
}
