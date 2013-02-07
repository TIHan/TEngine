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
    
    EXPECT_TRUE(glfwOpenWindow(640, 480, 0, 0, 0, 0, 32, 0, GLFW_WINDOW));

    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);


    // Ensure we can capture the escape key being pressed below
    glfwEnable(GLFW_STICKY_KEYS);
 
    do{
        // Draw nothing, see you in tutorial 2 !
 
        // Swap buffers
        glfwSwapBuffers();
 
    } // Check if the ESC key was pressed or the window was closed
    while(glfwGetKey(GLFW_KEY_ESC) != GLFW_PRESS &&
    glfwGetWindowParam(GLFW_OPENED));
    
    glfwTerminate();
}
