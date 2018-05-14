/*
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <gtest/gtest.h>
#include "src/robot.h"
#include "src/arena.h"
#include "src/arena_params.h"
#include "src/communication.h"

#ifdef INTEGRATION_TESTS

/******************************************************
* Integeration TestCase
*******************************************************/
class IntegrationTest : public ::testing::Test {
 protected:
  virtual void SetUp() {
    csci3081::arena_params aparams;
    arena = new csci3081::Arena(&aparams);
    robot = arena->robot();
  }

  csci3081::Arena * arena;
  csci3081::Robot * robot;
  csci3081::WheelVelocity wheelVelBefore;
  csci3081::WheelVelocity wheelVelAfter;
};

/*******************************************************************************
 * Test Cases
 ******************************************************************************/

TEST_F(IntegrationTest,TEST_ARROW_KEY_UP) {
  wheelVelBefore = robot->get_motion_handler().get_velocity();
  arena->AcceptCommand(csci3081::kIncreaseSpeed);
  wheelVelAfter = robot->get_motion_handler().get_velocity();
  EXPECT_LT(wheelVelBefore.left, wheelVelAfter.left) << "FAIL: TEST_ARROW_KEY_UP left wheel";
  EXPECT_LT(wheelVelBefore.right, wheelVelAfter.right) << "FAIL: TEST_ARROW_KEY_UP right wheel";
  EXPECT_LE(wheelVelAfter.left, ROBOT_MAX_SPEED) << "FAIL: TEST_ARROW_KEY_UP left wheel";
  EXPECT_LE(wheelVelAfter.right, ROBOT_MAX_SPEED) << "FAIL: TEST_ARROW_KEY_UP right wheel";
 }


TEST_F(IntegrationTest,TEST_ARROW_KEY_DOWN) {
  arena->AcceptCommand(csci3081::kIncreaseSpeed);
  wheelVelBefore = robot->get_motion_handler().get_velocity();
  arena->AcceptCommand(csci3081::kDecreaseSpeed);
  wheelVelAfter = robot->get_motion_handler().get_velocity();
  EXPECT_GT(wheelVelBefore.left, wheelVelAfter.left) << "FAIL: TEST_ARROW_KEY_DOWN left wheel";
  EXPECT_GT(wheelVelBefore.right, wheelVelAfter.right) << "FAIL: TEST_ARROW_KEY_DOWN right wheel";
  EXPECT_GE(wheelVelAfter.left, 0) << "FAIL: TEST_ARROW_KEY_DOWN left wheel";
  EXPECT_GE(wheelVelAfter.right, 0) << "FAIL: TEST_ARROW_KEY_DOWN right wheel";
 }


 TEST_F(IntegrationTest,TEST_LEFT_KEY) {
  arena->AcceptCommand(csci3081::kTurnLeft);
  wheelVelAfter = robot->get_motion_handler().get_velocity();
  EXPECT_GT(wheelVelAfter.right, wheelVelAfter.left) << "FAIL: TEST_TURN_LEFT_KEY";
  }


TEST_F(IntegrationTest,TEST_RIGHT_KEY) {
  arena->AcceptCommand(csci3081::kTurnRight);
  wheelVelAfter = robot->get_motion_handler().get_velocity();
  EXPECT_LT(wheelVelAfter.right, wheelVelAfter.left) << "FAIL: TEST_TURN_RIGHT_KEY";
  }
#endif /*INTEGERATION_TEST*/
