/**
 * @file motion_handler_bad_guy.h
 *
 * @copyright 2018 3081 Staff, All rights reserved.
 */

#ifndef SRC_MOTION_HANDLER_BAD_GUY_H_
#define SRC_MOTION_HANDLER_BAD_GUY_H_

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
 * @brief Class managing an Bad_Guys's speed and heading angle based
 * on collisions and user inputs.
 */
class MotionHandlerBad_Guy : public MotionHandler {
 public:
  explicit MotionHandlerBad_Guy(ArenaMobileEntity * ent)
      : MotionHandler(ent) {}

  MotionHandlerBad_Guy(const MotionHandlerBad_Guy& other) = default;
  MotionHandlerBad_Guy&
  operator=(const MotionHandlerBad_Guy& other) = default;

  /**
  * @brief Update the speed and the pose angle according to the sensor readings.
  *
  * @param[in] timer is the countdown for recovering
  * @param[in] isrecovering is the bad_guy recovering 
  */
  void SwitchVelocity();
};

NAMESPACE_END(csci3081);

#endif  // SRC_MOTION_HANDLER_BAD_GUY_H_
