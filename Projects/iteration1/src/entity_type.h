/**
 * @file entity_type.h
 *
 * @copyright Daniel Hartmann, All rights reserved.
 */

#ifndef SRC_ENTITY_TYPE_H_
#define SRC_ENTITY_TYPE_H_

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "src/common.h"

/*******************************************************************************
 * Namespaces
 ******************************************************************************/
NAMESPACE_BEGIN(csci3081);

enum EntityType {
  kRobot, kBad_Guy, kBase, kEntity,
  kRightWall, kLeftWall, kTopWall, kBottomWall,
  kUndefined
};

NAMESPACE_END(csci3081);

#endif  // SRC_ENTITY_TYPE_H_
