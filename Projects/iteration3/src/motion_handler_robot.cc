/**
 * @file motion_handler_robot.cc
 *
 * @copyright 2018 3081 Staff, All rights reserved.
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "src/motion_handler_robot.h"
#include "src/motion_behavior_differential.h"

/*******************************************************************************
 * Namespaces
 ******************************************************************************/
NAMESPACE_BEGIN(csci3081);

/*******************************************************************************
 * Member Functions
 ******************************************************************************/
// @TODO add clamped

void MotionHandlerRobot::TurnLeft() {
  set_velocity(
    get_velocity().left  - get_angle_delta()/4,
    get_velocity().right + get_angle_delta()/4);
}

void MotionHandlerRobot::TurnRight() {
  set_velocity(
    get_velocity().left  + get_angle_delta()/4,
    get_velocity().right - get_angle_delta()/4);
}

void MotionHandlerRobot::IncreaseSpeed() {
  double vleft = get_velocity().left  + get_speed_delta();
  double vright = get_velocity().right + get_speed_delta();
  set_velocity(
    clamp_vel(vleft, 'l'),
    clamp_vel(vright, 'r'));
}

void MotionHandlerRobot::DecreaseSpeed() {
  double vleft = get_velocity().left  - get_speed_delta();
  double vright = get_velocity().right - get_speed_delta();
  set_velocity(
    clamp_vel(vleft, 'l'),
    clamp_vel(vright, 'r'));
}

void MotionHandlerRobot::UpdateVelocity() {
  if (entity_->get_touch_sensor()->get_output()) {
    entity_->RelativeChangeHeading(+180);
  }
}

double MotionHandlerRobot::clamp_vel(double vel, char wheel) {
  // @TODO: don't go backwards
  double clamped = 0.0;
  switch (wheel) {
    case 'l':
      if (vel > 0) {
        clamped = (vel > get_max_speed()) ?
                  get_max_speed():
                  vel;
      } else {
        clamped = (vel < -get_max_speed()) ?
                  -get_max_speed():
                  vel;
        clamped = (vel < -get_velocity().right) ?
                  -get_velocity().right:
                  vel;
      }
      break;
    case 'r':
      if (vel > 0) {
        clamped = (vel > get_max_speed()) ?
                  get_max_speed():
                  vel;
      } else {
        clamped = (vel < -get_max_speed()) ?
                  -get_max_speed():
                  vel;
        clamped = (vel < -get_velocity().right) ?
                  -get_velocity().right:
                  vel;
      }
      break;
    default: std::cout << "error, failure in clamp_vel";
      break;
  }
  return clamped;
} /* clamp_vel() */

NAMESPACE_END(csci3081);
