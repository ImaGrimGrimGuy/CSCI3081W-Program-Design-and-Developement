/**
 * @file motion_handler_coward.cc
 *
 * @copyright 2018 3081 Staff, All rights reserved.
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "src/motion_handler_coward.h"
#include "src/motion_behavior_differential.h"
#include "src/scaredyboi.h"
#include "src/rgb_color.h"

/*******************************************************************************
 * Namespaces
 ******************************************************************************/
NAMESPACE_BEGIN(csci3081);

/*******************************************************************************
 * Member Functions
 ******************************************************************************/
// @TODO add clamped

double MotionHandlerCoward::clamp_vel(double vel, char wheel) {
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


void MotionHandlerCoward::UpdateVelocity() {
  double vleft = 0;
  double vright = 0;
  int mag = dynamic_cast<ScaredyBoi*>(entity_)->get_sensitivity();

  // positive uncrossed inputs adjusted to the magnitude of velocity
  if (dynamic_cast<ScaredyBoi*>(entity_)->get_hunger() >= 1800) {
    vleft = mag * get_max_speed()/1000.0 * (dynamic_cast<ScaredyBoi*>(entity_)
                                            ->get_left_light()->get_reading());

    vright = mag * get_max_speed()/1000.0 * (dynamic_cast<ScaredyBoi*>(entity_)
                                           ->get_right_light()->get_reading());

    entity_->set_color(ROBOT_COLOR);
  } else if (dynamic_cast<ScaredyBoi*>(entity_)->get_hunger() < 1800
              && dynamic_cast<ScaredyBoi*>(entity_)->get_hunger() >= 0) {
    vleft = mag * (get_max_speed()/1000.0 * (dynamic_cast<ScaredyBoi*>(entity_)
                                           ->get_left_light()->get_reading()))
          + 100 * (get_max_speed()/1000.0 * (dynamic_cast<ScaredyBoi*>(entity_)
                                     ->get_right_food()->get_reading())) + 0.5;

    vright = mag * (get_max_speed()/1000.0*(dynamic_cast<ScaredyBoi*>(entity_)
                                           ->get_right_light()->get_reading()))
           + 100 * (get_max_speed()/1000.0 * (dynamic_cast<ScaredyBoi*>(entity_)
                                       ->get_left_food()->get_reading())) + 0.5;
    entity_->set_color({ 255, 255, 50 });
  } else {
    vleft = 500 * get_max_speed()/1000.0 * (dynamic_cast<ScaredyBoi*>(entity_)
                                      ->get_right_food()->get_reading()) + 0.5;

    vright = 500 * get_max_speed()/1000.0 * (dynamic_cast<ScaredyBoi*>(entity_)
                                       ->get_left_food()->get_reading()) + 0.5;

    entity_->set_color({ 255, 130, 0 });
  }
/*
  set_velocity(
    clamp_vel(vleft, 'l'),
    clamp_vel(vright, 'r'));
*/
  set_velocity(vleft, vright);
}

void MotionHandlerCoward::SwitchVelocity() {
  entity_->RelativeChangeHeading(90);
}

NAMESPACE_END(csci3081);
