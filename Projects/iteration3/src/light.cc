/**
 * @file light.cc
 *
 * @copyright 2017 3081 Staff, All rights reserved.
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "src/light.h"
#include "src/params.h"

/*******************************************************************************
 * Namespaces
 ******************************************************************************/
NAMESPACE_BEGIN(csci3081);

/*******************************************************************************
 * Constructors/Destructor
 ******************************************************************************/
Light::Light() :
    motion_handler_(this),
    motion_behavior_(this) {
  set_color(LIGHT_COLOR);
  set_pose(LIGHT_POSITION);
  set_radius(LIGHT_RADIUS);
  set_type(kLight);
}
/*******************************************************************************
 * Member Functions
 ******************************************************************************/
void Light::TimestepUpdate(unsigned int dt) {
  if (timer_ != 0) {
    motion_handler_.set_velocity(-0.75, -1);  // move in an arc
    timer_--;
  } else {
    motion_handler_.set_velocity(0.25, 0.25);  // move forward
  }
  motion_behavior_.UpdatePose(dt, motion_handler_.get_velocity(), 0, 0);
  sensor_touch_->Reset();
} /* TimestepUpdate() */

void Light::HandleCollision(EntityType object_type,
                            __unused ArenaEntity * object) {
  // sensor_touch_-> HandleCollision(object)
  switch (object_type) {
    case(kFood): break;
    case(kLight):
      // motion_handler_.SwitchVelocity();
      if (timer_ == 0) {
        timer_ = 230;
      }
      break;
    case(kEntity): break;
    case(kUndefined): break;
    case(kTopWall): set_heading(270);
    timer_ = 230;
    break;
    case(kRightWall): set_heading(0);
    timer_ = 230;
    break;
    case(kBottomWall): set_heading(90);
    timer_ = 230;
    break;
    case(kLeftWall): set_heading(180);
    timer_ = 230;
    break;
    default:
      break;
  }
}

void Light::Reset() {
  set_pose(SetPoseRandomly());
  set_radius(SetRadiusRandomly());
  motion_handler_.set_max_speed(LIGHT_MAX_SPEED);
  motion_handler_.set_max_angle(LIGHT_MAX_ANGLE);
  sensor_touch_->Reset();
  // timer_ = 0;
} /* Reset() */

void Light::BounceOff() {
  // timer_ = 25;
  // isrecovering = true;
  motion_handler_.SwitchVelocity();
  // for now just flip around 135 deg
}

Pose Light::SetPoseRandomly() {
  // Dividing arena into 19x14 grid. Each grid square is 50x50
  return {static_cast<double>((30 + (random() % 19) * 50)),
        static_cast<double>((30 + (random() % 14) * 50))};
}

int Light::SetRadiusRandomly() {
  // Choose randomly between the max and min possible radius
  // (biased towards smaller but that's fine).
  return {static_cast<int>(random() % (LIGHT_MAX_RADIUS-
          LIGHT_MIN_RADIUS+1) + LIGHT_MIN_RADIUS)};
}

NAMESPACE_END(csci3081);
