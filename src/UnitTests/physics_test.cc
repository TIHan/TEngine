#include <engine_lib.h>
#include <btBulletDynamicsCommon.h>
#include <gtest/gtest.h>

class PhysicsTest : public ::testing::Test {
};

TEST_F(PhysicsTest, PhysicsTest1) {
  auto box = std::make_shared<btBoxShape>(btVector3(1,1,1));
}