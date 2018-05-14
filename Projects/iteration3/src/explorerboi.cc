/**
 * @file robot.cc
 *
 * @copyright 2017 3081 Staff, All rights reserved.
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "src/explorerboi.h"
#include "src/params.h"

/*******************************************************************************
 * Namespaces
 ******************************************************************************/
NAMESPACE_BEGIN(csci3081);

/*******************************************************************************
 * Constructors/Destructor
 ******************************************************************************/
ExplorerBoi::ExplorerBoi() :
    motion_handler_(this),
    motion_behavior_(this),
    lives_(9) {
  Pose p = SetPoseRandomly();
  double r = SetRadiusRandomly();
  set_type(kExplorer);
  set_color(ROBOT_COLOR);
  set_pose(p);
  set_radius(r);
  left_light_ = new LightSensor(p.x - r, p.y);
  right_light_ = new LightSensor(p.x + r, p.y);
  left_food_ = new FoodSensor(p.x - r, p.y);
  right_food_ = new FoodSensor(p.x + r, p.y);
}

/*******************************************************************************
 * Member Functions
 ******************************************************************************/
void ExplorerBoi::TimestepUpdate(unsigned int dt) {
  if (timer_ != 0) {
    motion_handler_.set_velocity(-1, -0.75);  // move in an arc
    timer_--;
  } else {
    // Update heading as indicated by touch sensor
    motion_handler_.UpdateVelocity();
  }

  // Use velocity and position to update position
  motion_behavior_.UpdatePose(dt, motion_handler_.get_velocity(), 0, 0);

  double pi = 3.14159265359;

  left_light_->set_position(
    get_pose().x + get_radius()*cos(get_pose().theta*pi/180 - pi/2),
    get_pose().y + get_radius()*sin(get_pose().theta*pi/180 - pi/2));

  right_light_->set_position(
    get_pose().x + get_radius()*cos(get_pose().theta * pi/180 + pi/2),
    get_pose().y + get_radius()*sin(get_pose().theta * pi/180 + pi/2));

  left_food_->set_position(
    get_pose().x + get_radius()*cos(get_pose().theta*pi/180 - pi/2),
    get_pose().y + get_radius()*sin(get_pose().theta*pi/180 - pi/2));

  right_food_->set_position(
    get_pose().x + get_radius()*cos(get_pose().theta * pi/180 + pi/2),
    get_pose().y + get_radius()*sin(get_pose().theta * pi/180 + pi/2));

  // if the robot is close enough
  if (((left_food_->get_reading() >= 900)
        || (right_food_->get_reading()) >= 900)) {
    hunger_ = 3600;
  }

  if (mortal_) {
    if (hunger_ >= 0) {
      hunger_--;
    }
  }

  // Reset Sensor for next cycle
  sensor_touch_->Reset();
} /* TimestepUpdate() */

void ExplorerBoi::Reset() {
  Pose p = SetPoseRandomly();
  set_pose(p);
  sensor_touch_->Reset();
  set_lives(9);
  left_light_->set_position(p.x - this->get_radius(), p.y);
  right_light_->set_position(p.x + this->get_radius(), p.y);
  left_food_->set_position(p.x - this->get_radius(), p.y);
  right_food_->set_position(p.x + this->get_radius(), p.y);
  hunger_ = 3600;
  set_color(ROBOT_COLOR);
} /* Reset() */

void ExplorerBoi::HandleCollision(EntityType object_type,
                                  ArenaEntity * object) {
  switch (object_type) {
    case(kFood): break;
    case(kLight): break;
    case(kEntity):
    sensor_touch_->HandleCollision(object_type, object);
    break;
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
    case(kUndefined):
    default:  // walls will be different than lights
      if (timer_ == 0) {
        timer_ = 180;
      }
      break;
  }
}

Pose ExplorerBoi::SetPoseRandomly() {
  // Dividing arena into 19x14 grid. Each grid square is 50x50
  return {static_cast<double>((30 + (random() % 19) * 50)),
        static_cast<double>((30 + (random() % 14) * 50))};
}

int ExplorerBoi::SetRadiusRandomly() {
  // Choose randomly between the max and min possible radius
  // (biased towards smaller but that's fine).
  return {static_cast<int>(random() % (ROBOT_MAX_RADIUS-
          ROBOT_MIN_RADIUS+1) + ROBOT_MIN_RADIUS)};
}


NAMESPACE_END(csci3081);
