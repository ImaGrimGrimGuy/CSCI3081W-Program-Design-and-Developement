/**
 * @file sensor_touch.cc
 *
 * @copyright Daniel Hartmann, All rights reserved.
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <iostream>

#include "src/sensor_touch.h"

/*******************************************************************************
 * Namespaces
 ******************************************************************************/
NAMESPACE_BEGIN(csci3081);

/*******************************************************************************
 * Member Functions
 ******************************************************************************/

// this function is quite boring for now...
void SensorTouch::HandleCollision(
  EntityType object_type,
  __unused ArenaEntity * object) {
  switch (object_type) {
    case(kBad_Guy): output_ = true;
      break;
    case(kTopWall): output_ = true;
      break;
    case(kRightWall): output_ = true;
      break;
    case(kBottomWall): output_ = true;
      break;
    case(kLeftWall): output_ = true;
      break;
    case(kRobot): output_ = true;
      break;
    default:  // do nothing to "slide over" other entities (future)
      break;
  }
}

NAMESPACE_END(csci3081);
