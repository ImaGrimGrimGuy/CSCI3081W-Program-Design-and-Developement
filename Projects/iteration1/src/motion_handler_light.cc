/**
 * @file motion_handler_bad_guy.cc
 *
 * @copyright 2018 3081 Staff, All rights reserved.
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "src/motion_handler_bad_guy.h"
#include "src/motion_behavior_differential.h"

/*******************************************************************************
 * Namespaces
 ******************************************************************************/
NAMESPACE_BEGIN(csci3081);

/*******************************************************************************
 * Member Functions
 ******************************************************************************/

void MotionHandlerBad_Guy::SwitchVelocity() {
  entity_->RelativeChangeHeading(+135);
}


NAMESPACE_END(csci3081);
