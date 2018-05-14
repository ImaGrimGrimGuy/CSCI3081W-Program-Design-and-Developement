/**
 * @file motion_handler_coward.h
 *
 * @copyright 2018 3081 Staff, All rights reserved.
 */

#ifndef SRC_MOTION_HANDLER_COWARD_H_
#define SRC_MOTION_HANDLER_COWARD_H_

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <iostream>
#include "src/common.h"
#include "src/motion_handler.h"
#include "src/light_sensor.h"
#include "src/food_sensor.h"

/*******************************************************************************
 * Namespaces
 ******************************************************************************/
NAMESPACE_BEGIN(csci3081);

/*******************************************************************************
 * Classes
 ******************************************************************************/

/**
 * @brief Class managing an agressive Robot's speed and heading angle based
 * on light and food sensor readings
 */
class MotionHandlerCoward : public MotionHandler {
 public:
  explicit MotionHandlerCoward(ArenaMobileEntity * ent)
      : MotionHandler(ent) {}

  MotionHandlerCoward(const MotionHandlerCoward& other) = default;
  MotionHandlerCoward& operator=(const MotionHandlerCoward& other) = default;

  /**
  * @brief Update the speed and the pose angle according to the sensor readings.
  */
  void UpdateVelocity() override;

  void SwitchVelocity();

 private:
  double clamp_vel(double vel, char wheel);
};

NAMESPACE_END(csci3081);

#endif  // SRC_MOTION_HANDLER_COWARD_H_
