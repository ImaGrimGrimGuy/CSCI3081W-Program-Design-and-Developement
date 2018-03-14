/**
 * @file bad_guy.h
 *
 * @copyright Daniel Hartmann, All rights reserved.
 */

#ifndef SRC_BAD_GUY_H_
#define SRC_BAD_GUY_H_

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <string>

#include "src/arena_mobile_entity.h"
#include "src/common.h"
#include "src/motion_handler_bad_guy.h"
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
 * @brief Class representing an immobile bad_guy within the Arena.
 *
 * Since bad_guys are immobile, the Bad_Guy class is very simple.
 */
class Bad_Guy : public ArenaMobileEntity {
 public:
  /**
   * @brief Constructor.
   */
  Bad_Guy();

  /**
   * @brief Get the name of the Bad_Guy for visualization purposes, and to
   * aid in debugging.
   */
  std::string get_name() const override {
    return "Bad_Guy" + std::to_string(get_id());
  }

/**
   * @brief Update the Bad_Guy's position and velocity after the specified
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
   * @brief Reset the bad_guy using the initialization parameters received
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
  MotionHandlerBad_Guy get_motion_handler() {return motion_handler_;}
/**
  * @brief Getter for motion_behavior_
  */
  MotionBehaviorDifferential get_motion_behavior() {return motion_behavior_;}



 private:
  /** 
   * @brief An attempt to not overlap any of the newly constructed entities.
   */
  // Set the pose of bad_guys semi-randomly
  Pose SetPoseRandomly();
  // Set the radius of the bad_guys semi-randomly
  int SetRadiusRandomly();
  /**
   * @brief pointers to the objects used in bad_guy motion.
   */
  // Manages pose and wheel velocities that change with time and collisions.
  MotionHandlerBad_Guy motion_handler_;
  // Calculates changes in pose based on elapsed time and wheel velocities.
  MotionBehaviorDifferential motion_behavior_;
  // how long to back up
  int timer_{0};
};

NAMESPACE_END(csci3081);

#endif  // SRC_BAD_GUY_H_
