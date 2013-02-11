#include <engine_lib.h>
#include <engine_core.h>
#include <GL/glew.h>
#include <GL/glfw.h>
#include <btBulletDynamicsCommon.h>
#include <gtest/gtest.h>

#include "GLTools/include/GLTools.h"
#include "GLTools/include/GLShaderManager.h"

class GameTest : public ::testing::Test {
};

TEST_F(GameTest, RenderLoop) {
  engine::core::GameLoop game_loop(120);

  EXPECT_TRUE(glfwInit()); 

  glfwOpenWindowHint(GLFW_FSAA_SAMPLES, 4);

#ifdef __APPLE__
  glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 3);
  glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, 2);
  glfwOpenWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwOpenWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#else
  //glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 3);
  //glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, 3);
  //glfwOpenWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif

  EXPECT_TRUE(glfwOpenWindow(1280, 720, 0, 0, 0, 0, 32, 0, GLFW_WINDOW));
  EXPECT_TRUE(glewInit() == GLEW_OK);

  GLBatch triangleBatch;
  GLShaderManager shaderManager;

  shaderManager.InitializeStockShaders();

  GLfloat vVerts[] = { -0.5f, 0.0f, 0.0f,
                        0.5f, 0.0f, 0.0f,
                        0.0f, 0.5f, 0.0f };

  triangleBatch.Begin(GL_TRIANGLES, 3);
  triangleBatch.CopyVertexData3f(vVerts);
  triangleBatch.End();

  game_loop.Run([&] {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    // MOVE

    GLfloat copyVerts[9];

    for(int i = 0; i < 9; ++i)
      copyVerts[i] = vVerts[i];

    if (glfwGetKey(GLFW_KEY_UP)) {
      for (int i = 1; i < 9; i += 3)
        copyVerts[i] += 0.025f;
    }

    if (glfwGetKey(GLFW_KEY_DOWN)) {
      for (int i = 1; i < 9; i += 3)
        copyVerts[i] -= 0.025f;
    }

    if (glfwGetKey(GLFW_KEY_RIGHT)) {
      for (int i = 0; i < 9; i += 3)
        copyVerts[i] += 0.025f;
    }

    if (glfwGetKey(GLFW_KEY_LEFT)) {
      for (int i = 0; i < 9; i += 3)
        copyVerts[i] -= 0.025f;
    }

    // END MOVE

    // COLLISION

    // y
    if (copyVerts[7] <= 1.0f && copyVerts[1] >= -1.0f && copyVerts[4] >= -1.0f) {
      for (int i = 1; i < 9; i += 3)
        vVerts[i] = copyVerts[i];
    }

    // x
    if (copyVerts[0] >= -1.0f && copyVerts[3] <= 1.0f) {
      for (int i = 0; i < 9; i += 3)
        vVerts[i] = copyVerts[i];
    }

    // END COLLISION


    triangleBatch.CopyVertexData3f(vVerts);

    GLfloat vRed[] = { 1.0f, 0.0f, 0.0f, 1.0f };
    shaderManager.UseStockShader(GLT_SHADER_IDENTITY, vRed);
    triangleBatch.Draw();

    glfwSwapBuffers();

    if (glfwGetKey(GLFW_KEY_ESC) == GLFW_PRESS &&
        glfwGetWindowParam(GLFW_OPENED))
      game_loop.Stop();
  });
    
  glfwTerminate();
}
