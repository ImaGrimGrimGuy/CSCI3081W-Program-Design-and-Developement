/**
 * @file entity_factory.cc
 *
 * @copyright Daniel Hartmann, All rights reserved.
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <string>
#include <ctime>
#include <iostream>

#include "src/common.h"
#include "src/entity_factory.h"
#include "src/entity_type.h"
#include "src/params.h"
#include "src/pose.h"
#include "src/rgb_color.h"

/*******************************************************************************
 * Namespaces
 ******************************************************************************/
NAMESPACE_BEGIN(csci3081);

/*******************************************************************************
 * Class Definitions
 ******************************************************************************/

EntityFactory::EntityFactory() {
  srand(time(nullptr));
}

ArenaEntity* EntityFactory::CreateEntity(EntityType etype) {
  switch (etype) {
    case (kRobot):
      return CreateRobot();
      break;
    case (kBad_Guy):
      return CreateBad_Guy();
      break;
    case (kBase):
      return CreateBase();
      break;
    default:
      std::cout << "FATAL: Bad entity type on creation\n";
      assert(false);
  }
  return nullptr;
}

Robot* EntityFactory::CreateRobot() {
  auto* robot = new Robot;
  robot->set_type(kRobot);
  robot->set_color(ROBOT_COLOR);
  robot->set_pose(ROBOT_INIT_POS);
  robot->set_radius(ROBOT_RADIUS);
  ++entity_count_;
  ++robot_count_;
  robot->set_id(robot_count_);
  return robot;
}

Bad_Guy* EntityFactory::CreateBad_Guy() {
  auto* bad_guy = new Bad_Guy;
  bad_guy->set_type(kBad_Guy);
  bad_guy->set_color(BAD_GUY_COLOR);
  bad_guy->set_pose(SetPoseRandomly());
  bad_guy->set_radius(SetRadiusRandomly());
  ++entity_count_;
  ++bad_guy_count_;
  bad_guy->set_id(bad_guy_count_);
  return bad_guy;
}

Base* EntityFactory::CreateBase() {
  auto* base = new Base;
  base->set_type(kBase);
  base->set_color(BASE_COLOR);
  base->set_pose(SetPoseRandomly());
  base->set_radius(BASE_RADIUS);
  ++entity_count_;
  ++base_count_;
  base->set_id(base_count_);
  return base;
}

Pose EntityFactory::SetPoseRandomly() {
  // Dividing arena into 19x14 grid. Each grid square is 50x50
  return {static_cast<double>((30 + (random() % 19) * 50)),
        static_cast<double>((30 + (random() % 14) * 50))};
}

int EntityFactory::SetRadiusRandomly() {
  // Choose randomly between the max and min possible radius
  // (biased towards smaller but that's fine).
  return {static_cast<int>(random() % (BAD_GUY_MAX_RADIUS-
          BAD_GUY_MIN_RADIUS+1) + BAD_GUY_MIN_RADIUS)};
}

NAMESPACE_END(csci3081);
