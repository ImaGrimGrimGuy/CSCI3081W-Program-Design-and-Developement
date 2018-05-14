/**
 * @file light.h
 *
 * @copyright 2017 3081 Staff, All rights reserved.
 */

#ifndef SRC_LIGHT_H_
#define SRC_LIGHT_H_

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <string>

#include "src/arena_mobile_entity.h"
#include "src/common.h"
#include "src/motion_handler_light.h"
#include "src/motion_behavior_differential.h"
#include "src/entity_type.h"
#include "src/pose.h"

/*******************************************************************************
 * Namespaces
 ******************************************************************************/
NAMESPACE_BEGIN(csci3081);

/*******************************************************************************
 * Class Definitions
 ******************************************************************************/
/**
 * @brief Class representing an immobile light within the Arena.
 *
 * Since lights are immobile, the Light class is very simple.
 */
class Light : public ArenaMobileEntity {
 public:
  /**
   * @brief Constructor.
   */
  Light();

  /**
   * @brief Get the name of the Light for visualization purposes, and to
   * aid in debugging.
   */
  std::string get_name() const override {
    return "Light" + std::to_string(get_id());
  }

/**
   * @brief Update the Light's position and velocity after the specified
   * duration has passed.
   *
   * @param dt The # of timesteps that have elapsed since the last update.
   */
  void TimestepUpdate(unsigned int dt) override;

  /**
   * @brief Handles the collision by setting the sensor to activated.
   */
  void HandleCollision(EntityType object_type, ArenaEntity * object = NULL);

  /**
   * @brief Reset the light using the initialization parameters received
   * by the constructor.
   */
  void Reset() override;


  /**
   * @brief Handles the collision by setting the sensor to activated.
   */
  void BounceOff();

/**
  * @brief Getter for motion_handler_
  */
  MotionHandlerLight get_motion_handler() {return motion_handler_;}
/**
  * @brief Getter for motion_behavior_
  */
  MotionBehaviorDifferential get_motion_behavior() {return motion_behavior_;}



 private:
  /** 
   * @brief An attempt to not overlap any of the newly constructed entities.
   */
  // Set the pose of lights semi-randomly
  Pose SetPoseRandomly();
  // Set the radius of the lights semi-randomly
  int SetRadiusRandomly();
  /**
   * @brief pointers to the objects used in light motion.
   */
  // Manages pose and wheel velocities that change with time and collisions.
  MotionHandlerLight motion_handler_;
  // Calculates changes in pose foodd on elapsed time and wheel velocities.
  MotionBehaviorDifferential motion_behavior_;
  // how long to back up
  int timer_{0};
};

NAMESPACE_END(csci3081);

#endif  // SRC_LIGHT_H_
