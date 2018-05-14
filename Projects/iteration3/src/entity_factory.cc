/**
 * @file entity_factory.cc
 *
 * @copyright 2017 3081 Staff, All rights reserved.
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
#include "src/arena.h"

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

ArenaEntity* EntityFactory::CreateEntity(EntityType etype,
                                         std::vector<class Light *> lights,
                                         std::vector<class Food *> foods) {
  switch (etype) {
    case (kRobot):
      return CreateRobot();
      break;
    case (kLight):
      return CreateLight();
      break;
    case (kFood):
      return CreateFood();
      break;
    case (kExplorer):
      return CreateExplorerBoi(lights, foods);
      break;
    case (kCoward):
      return CreateScaredyBoi(lights, foods);
      break;
    default:
      std::cout << "FATAL: Bad entity type on creation\n";
      assert(false);
  }
  return nullptr;
}

void EntityFactory::Reset() {
  entity_count_ = 0;
  robot_count_ = 0;
  light_count_ = 0;
  food_count_ = 0;
}

Robot* EntityFactory::CreateRobot() {
  auto* robot = new Robot;
  ++entity_count_;
  ++robot_count_;
  robot->set_id(robot_count_);
  return robot;
}


ExplorerBoi* EntityFactory::CreateExplorerBoi(
  std::vector<class Light *> lights,
  std::vector<class Food *> foods) {
  auto* guy = new ExplorerBoi;
  guy->get_left_light()->SetLookingAt(lights);
  guy->get_right_light()->SetLookingAt(lights);
  guy->get_left_food()->SetLookingAt(foods);
  guy->get_right_food()->SetLookingAt(foods);
  ++entity_count_;
  ++robot_count_;
  guy->set_id(robot_count_);
  return guy;
}

ScaredyBoi* EntityFactory::CreateScaredyBoi(std::vector<class Light *> lights,
                                            std::vector<class Food *> foods) {
  auto* guy = new ScaredyBoi;
  guy->get_left_light()->SetLookingAt(lights);
  guy->get_right_light()->SetLookingAt(lights);
  guy->get_left_food()->SetLookingAt(foods);
  guy->get_right_food()->SetLookingAt(foods);
  ++entity_count_;
  ++robot_count_;
  guy->set_id(robot_count_);
  return guy;
}

Light* EntityFactory::CreateLight() {
  auto* light = new Light;
  ++entity_count_;
  ++light_count_;
  light->set_id(light_count_);
  return light;
}

Food* EntityFactory::CreateFood() {
  auto* food = new Food;
  ++entity_count_;
  ++food_count_;
  food->set_id(food_count_);
  return food;
}

Pose EntityFactory::SetPoseRandomly() {
  // Dividing arena into 19x14 grid. Each grid square is 50x50
  return {static_cast<double>((30 + (random() % 19) * 50)),
        static_cast<double>((30 + (random() % 14) * 50))};
}

int EntityFactory::SetRadiusRandomly() {
  // Choose randomly between the max and min possible radius
  // (biased towards smaller but that's fine).
  return {static_cast<int>(random() % (LIGHT_MAX_RADIUS-
          LIGHT_MIN_RADIUS+1) + LIGHT_MIN_RADIUS)};
}

NAMESPACE_END(csci3081);
