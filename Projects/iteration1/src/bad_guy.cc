/**
 * @file bad_guy.cc
 *
 * @copyright Daniel Hartmann, All rights reserved.
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "src/bad_guy.h"
#include "src/params.h"

/*******************************************************************************
 * Namespaces
 ******************************************************************************/
NAMESPACE_BEGIN(csci3081);

/*******************************************************************************
 * Constructors/Destructor
 ******************************************************************************/
Bad_Guy::Bad_Guy() :
    motion_handler_(this),
    motion_behavior_(this) {
  set_color(BAD_GUY_COLOR);
  set_pose(BAD_GUY_POSITION);
  set_radius(BAD_GUY_RADIUS);
  set_type(kBad_Guy);
}
/*******************************************************************************
 * Member Functions
 ******************************************************************************/
void Bad_Guy::TimestepUpdate(unsigned int dt) {
  if (timer_ != 0) {
    motion_handler_.set_velocity(0.25, 0.10);  // move in an arc
    timer_--;
  } else {
    motion_handler_.set_velocity(0.25, 0.25);  // move forward
  }
  motion_behavior_.UpdatePose(dt, motion_handler_.get_velocity());
  sensor_touch_->Reset();
} /* TimestepUpdate() */

void Bad_Guy::HandleCollision(EntityType object_type, ArenaEntity * object) {
  sensor_touch_->HandleCollision(object_type, object);
  if (sensor_touch_->get_output()) {
    motion_handler_.SwitchVelocity();
    timer_ = 25;
  }
}

void Bad_Guy::Reset() {
  set_pose(SetPoseRandomly());
  set_radius(SetRadiusRandomly());
  motion_handler_.set_max_speed(BAD_GUY_MAX_SPEED);
  motion_handler_.set_max_angle(BAD_GUY_MAX_ANGLE);
  sensor_touch_->Reset();
  // timer_ = 0;
} /* Reset() */

void Bad_Guy::BounceOff() {
  // timer_ = 25;
  // isrecovering = true;
  motion_handler_.SwitchVelocity();
  // for now just flip around 135 deg
}

Pose Bad_Guy::SetPoseRandomly() {
  // Dividing arena into 19x14 grid. Each grid square is 50x50
  return {static_cast<double>((30 + (random() % 19) * 50)),
        static_cast<double>((30 + (random() % 14) * 50))};
}

int Bad_Guy::SetRadiusRandomly() {
  // Choose randomly between the max and min possible radius
  // (biased towards smaller but that's fine).
  return {static_cast<int>(random() % (BAD_GUY_MAX_RADIUS-
          BAD_GUY_MIN_RADIUS+1) + BAD_GUY_MIN_RADIUS)};
}

NAMESPACE_END(csci3081);
