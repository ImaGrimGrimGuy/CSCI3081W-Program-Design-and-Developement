/**
 * @file scaredyboi.h
 *
 * @copyright 2017 3081 Staff, All rights reserved.
 */

#ifndef SRC_SCAREDYBOI_H_
#define SRC_SCAREDYBOI_H_

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <string>

#include "src/arena_mobile_entity.h"
#include "src/common.h"
#include "src/motion_handler_coward.h"
#include "src/motion_behavior_differential.h"
#include "src/entity_type.h"
#include "src/light_sensor.h"
#include "src/food_sensor.h"

/*******************************************************************************
 * Namespaces
 ******************************************************************************/
NAMESPACE_BEGIN(csci3081);

class MotionBehaviorDifferential;

/*******************************************************************************
 * Class Definitions
 ******************************************************************************/
/**
 * @brief Class representing a scaredyboi within the arena.
 *
 * ScaredyBois are composed of a motion handler, and a motion behavior.
 * These classes interact to maintain the pose (position and heading) of the
 * scaredyboi. At each time step, the wheel velocities are used to calculate the
 * next pose of the scaredyboi. The handler manages the pose and user requests. The
 * behavior calculates the new pose based on wheel velocities.
 *
 * Scaredybois can be controlled through keypress, which modify wheel velocities.
 *
 * The touch sensor is activated when the scaredyboi collides with an object.
 * The heading is modified after a collision to move the scaredyboi away from the
 * other object.
 */
class ScaredyBoi : public ArenaMobileEntity {
 public:
  /**
   * @brief Constructor.
   */

  ScaredyBoi();
  ScaredyBoi(const ScaredyBoi& other) = delete;
  ScaredyBoi operator=(const ScaredyBoi& other) = delete;

  /**
   * @brief Reset the ScaredyBoi to a newly constructed state (needed for reset
   * button to work in GUI).
   */

  void Reset() override;

  /**
   * @brief Update the ScaredyBoi's position and velocity after the specified
   * duration has passed.
   *
   * @param dt The # of timesteps that have elapsed since the last update.
   */
  void TimestepUpdate(unsigned int dt) override;

  /**
   * @brief Handles the collision by setting the sensor to activated.
   * @param object_type the type of object colliding with the entity
   * @param object the object colliding with the entity
   */
  void HandleCollision(EntityType object_type, ArenaEntity * object = NULL);

  /**
   * @brief Get the name of the ScaredyBoi for visualization and for debugging.
   */
  std::string get_name() const override { return "ScaredyBoi"; }

/**
  * @brief Getter for motion_handler_
  */
  MotionHandlerCoward get_motion_handler() {return motion_handler_;}

/**
  * @brief Getter for lives_
  */
  int get_lives() const { return lives_; }

/**
  * @brief Setter for lives_
  */

  void set_lives(int l) { lives_ = l; }

/**
  * @brief Getter for motion_behavior_
  */
  MotionBehaviorDifferential get_motion_behavior() {return motion_behavior_;}

/**
  * @brief Getters for light_sensors
  */
  LightSensor* get_left_light() {return left_light_;}

  LightSensor* get_right_light() {return right_light_;}

/**
  * @brief Getters for light_sensors
  */
  FoodSensor* get_left_food() {return left_food_;}

  FoodSensor* get_right_food() {return right_food_;}

/**
  * @brief Getter for hunger_
  */  
  int get_hunger() {return hunger_;}

/**
  * @brief Setters for mortal_
  */  

  void set_mortal() {mortal_ = true;}

  void set_immortal() {mortal_ = false;}

/**
  * @brief Getter for sensitivity_
  */  

  void set_sensitivity(int sensitivity) {sensitivity_ = sensitivity;}

/**
  * @brief Setter for sensitivity_
  */  

  int get_sensitivity() {return sensitivity_;}

 private:
  // Set the pose of lights semi-randomly
  Pose SetPoseRandomly();
  // Set the radius of the lights semi-randomly
  int SetRadiusRandomly();
  // Manages pose and wheel velocities that change with time and collisions.
  MotionHandlerCoward motion_handler_;
  // Calculates changes in pose based on elapsed time and wheel velocities.
  MotionBehaviorDifferential motion_behavior_;
  // sensors are kept here so they are easy to move with the scaredyboi
  // Lives are decremented when the scaredyboi collides with anything.
  // When all the lives are gone, the game is lost.
  int lives_;
  double x_offset_{0};
  double y_offset_{0};

  LightSensor* left_light_{nullptr};
  LightSensor* right_light_{nullptr};

  FoodSensor* left_food_{nullptr};
  FoodSensor* right_food_{nullptr};

  // how long to back up
  int timer_{0};

  // starvation timer
  int hunger_{3600};

  // mortality
  bool mortal_{true};

  // light sensitivity
  int sensitivity_{5};
};

NAMESPACE_END(csci3081);

#endif  // SRC_SCAREDYBOI_H_
