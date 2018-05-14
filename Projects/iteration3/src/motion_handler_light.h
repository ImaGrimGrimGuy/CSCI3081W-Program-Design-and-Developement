/**
 * @file motion_handler_light.h
 *
 * @copyright 2018 3081 Staff, All rights reserved.
 */

#ifndef SRC_MOTION_HANDLER_LIGHT_H_
#define SRC_MOTION_HANDLER_LIGHT_H_

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <cassert>
#include <iostream>

#include "src/common.h"
#include "src/motion_handler.h"
#include "src/sensor_touch.h"
#include "src/communication.h"

/*******************************************************************************
 * Namespaces
 ******************************************************************************/
NAMESPACE_BEGIN(csci3081);

/*******************************************************************************
 * Classes
 ******************************************************************************/

/**
 * @brief Class managing an Lights's speed and heading angle foodd
 * on collisions and user inputs.
 */
class MotionHandlerLight : public MotionHandler {
 public:
  explicit MotionHandlerLight(ArenaMobileEntity * ent)
      : MotionHandler(ent) {}

  MotionHandlerLight(const MotionHandlerLight& other) = default;
  MotionHandlerLight&
  operator=(const MotionHandlerLight& other) = default;

  /**
  * @brief Update the speed and the pose angle according to the sensor readings.
  *
  * @param[in] timer is the countdown for recovering
  * @param[in] isrecovering is the light recovering 
  */
  void SwitchVelocity();
};

NAMESPACE_END(csci3081);

#endif  // SRC_MOTION_HANDLER_LIGHT_H_
