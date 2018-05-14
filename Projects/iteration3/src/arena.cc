/**
 * @file arena.cc
 *
 * @copyright 2018 Daniel Hartmann, All rights reserved.
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <algorithm>
#include <iostream>
#include <memory>

#include "src/arena.h"
#include "src/arena_params.h"

/*******************************************************************************
 * Namespaces
 ******************************************************************************/
NAMESPACE_BEGIN(csci3081);

/*******************************************************************************
 * Constructors/Destructor
 ******************************************************************************/
Arena::Arena(const struct arena_params *const params)
    : x_dim_(params->x_dim),
      y_dim_(params->y_dim),
      factory_(new EntityFactory),
      entities_(),
      mobile_entities_(),
      lights_(),
      foods_(),
      game_status_(PLAYING) {
  AddEntity(kFood, 4);  // params->n_foods
  AddEntity(kLight, 4);  // params->n_lights
  AddEntity(kExplorer, 5);  // params->n_explorer
  AddEntity(kCoward, 5);  // params->n_coward
  // AddRobot();
}

Arena::~Arena() {
  delete factory_;

  delete robot_;
  delete lightHolder_;
  for (auto ent : entities_) {
    delete ent;
  } /* for(ent..) */
  for (auto ent : lights_) {
    delete ent;
  }

  for (auto ent : entities_) {
    delete ent;
  }
  entities_.clear();
  mobile_entities_.clear();
  lights_.clear();
}

/*******************************************************************************
 * Member Functions
 ******************************************************************************/
void Arena::AddRobot() {
  robot_ =
  dynamic_cast<Robot *>(factory_->CreateEntity(kRobot, lights_, foods_));
  entities_.push_back(robot_);
}

void Arena::AddEntity(EntityType type, int quantity) {
  if (type == kLight) {
    for (int i = 0; i < quantity; i++) {
      lightHolder_ =
      dynamic_cast<Light *>(factory_->CreateEntity(type, lights_, foods_));
      lights_.push_back(lightHolder_);
      mobile_entities_.push_back(lightHolder_);
      entities_.push_back(lightHolder_);
    }
  } else if ((type == kCoward) || (type == kExplorer)) {
    for (int i = 0; i < quantity; i++) {
      botPlaceholder_ =
      dynamic_cast<ArenaMobileEntity *>(factory_->CreateEntity(type,
                                                  lights_, foods_));
      mobile_entities_.push_back(botPlaceholder_);
      entities_.push_back(botPlaceholder_);
    }
  } else {
    for (int i = 0; i < quantity; i++) {
      foodHolder_ =
      dynamic_cast<Food *>(factory_->CreateEntity(type, lights_, foods_));
      foods_.push_back(foodHolder_);
      entities_.push_back(foodHolder_);
    }
  }
}

void Arena::Reset() {
  for (auto ent : entities_) {
    ent->Reset();
    set_game_status(PLAYING);
  } /* for(ent..) */
} /* reset() */

// overloaded method for changing settings in the arena
void Arena::Reset(int numLights, int numBots,
                  int numFood, int numFearful,
                  bool hunger, int sensitivity) {
  for (auto ent : entities_) {
    delete ent;
  }

  entities_.clear();
  mobile_entities_.clear();
  lights_.clear();
  foods_.clear();
  factory_->Reset();

  hunger_active_ = hunger;
  if (hunger_active_) {
    AddEntity(kFood, numFood);
  }

  AddEntity(kLight, numLights);
  AddEntity(kExplorer, numBots - numFearful);
  AddEntity(kCoward, std::min(numFearful, numBots));

  if (hunger_active_) {
    for (auto ent : mobile_entities_) {
      if (ent->get_type() == kCoward) {
        dynamic_cast<ScaredyBoi *>(ent)->set_mortal();
      }
      if (ent->get_type() == kExplorer) {
        dynamic_cast<ExplorerBoi *>(ent)->set_mortal();
      }
    }
  } else {
    for (auto ent : mobile_entities_) {
      if (ent->get_type() == kCoward) {
        dynamic_cast<ScaredyBoi *>(ent)->set_immortal();
      }
      if (ent->get_type() == kExplorer) {
        dynamic_cast<ExplorerBoi *>(ent)->set_immortal();
      }
    }
  }

  for (auto ent : mobile_entities_) {
    if (ent->get_type() == kCoward) {
      dynamic_cast<ScaredyBoi *>(ent)->set_sensitivity(sensitivity);
    }
    if (ent->get_type() == kExplorer) {
      dynamic_cast<ExplorerBoi *>(ent)->set_sensitivity(sensitivity);
    }
  }
}

// The primary driver of simulation movement. Called from the Controller
// but originated from the graphics viewer.
void Arena::AdvanceTime(double dt) {
  if (!(dt > 0)) {
    return;
  }
  for (size_t i = 0; i < entities_.size(); ++i) {
    UpdateEntitiesTimestep();
  } /* for(i..) */
} /* AdvanceTime() */

void Arena::UpdateEntitiesTimestep() {
  /*
   * First, update the position of all entities, according to their current
   * velocities.
   * @TODO: Should this be just the mobile entities ??
   */
  for (auto ent : entities_) {
    ent->TimestepUpdate(1);
  }
  for (auto &ent : mobile_entities_) {
    MobileEntityUpdate(ent);
  }
}  // UpdateEntitiesTimestep()

  void Arena::MobileEntityUpdate(ArenaMobileEntity* ent) {
    /* Determine if any mobile entity is colliding with wall.
     * Adjust the position accordingly so it doesn't overlap.
     */
    EntityType wall = GetCollisionWall(ent);
    if (kUndefined != wall) {
      AdjustWallOverlap(dynamic_cast<ArenaMobileEntity*>(ent), wall);
      if (ent->get_type() == kLight) {
        dynamic_cast<Light*>(ent)->HandleCollision(wall);
      } else if (ent->get_type() == kExplorer) {
        dynamic_cast<ExplorerBoi*>(ent)->HandleCollision(wall);
      } else if (ent->get_type() == kCoward) {
        dynamic_cast<ScaredyBoi*>(ent)->HandleCollision(wall);
      }
    }
    CheckOtherEnts(ent);
  }

  void Arena::CheckOtherEnts(ArenaMobileEntity* ent) {
    for (auto &ent2 : entities_) {
      if (ent2 == ent) { continue; }
      if (IsColliding(ent, ent2)) {
        if (ent->get_type() == kLight) {
          if (ent2->get_type() == kLight) {
            AdjustEntityOverlap(ent, ent2);
            dynamic_cast<Light*>(ent)->
                        HandleCollision(ent2->get_type(), ent2);
          }
        } else if (ent->get_type() == kCoward) {
          if ((ent2->get_type() == kCoward)
              || (ent2->get_type() == kExplorer)) {
            AdjustEntityOverlap(ent, ent2);
            dynamic_cast<ScaredyBoi*>(ent)->
                        HandleCollision(ent2->get_type(), ent2);
          }
        } else if (ent->get_type() == kExplorer) {
          if ((ent2->get_type() == kExplorer)
              || ent2->get_type() == kCoward) {
            AdjustEntityOverlap(ent, ent2);
            dynamic_cast<ExplorerBoi*>(ent)->
                        HandleCollision(ent2->get_type(), ent2);
          }
        }
      }
    }
  }

// Determine if the entity is colliding with a wall.
// Always returns an entity type. If not collision, returns kUndefined.
EntityType Arena::GetCollisionWall(ArenaMobileEntity *const ent) {
  if (ent->get_pose().x + ent->get_radius() >= x_dim_) {
    return kRightWall;  // at x = x_dim_
  } else if (ent->get_pose().x - ent->get_radius() <= 0) {
    return kLeftWall;  // at x = 0
  } else if (ent->get_pose().y + ent->get_radius() >= y_dim_) {
    return kBottomWall;  // at y = y_dim
  } else if (ent->get_pose().y - ent->get_radius() <= 0) {
    return kTopWall;  // at y = 0
  } else {
    return kUndefined;
  }
} /* GetCollisionWall() */

/* The entity type indicates which wall the entity is colliding with.
* This determines which way to move the entity to set it slightly off the wall. */
void Arena::AdjustWallOverlap(ArenaMobileEntity *const ent, EntityType object) {
  Pose entity_pos = ent->get_pose();
  switch (object) {
    case (kRightWall):  // at x = x_dim_
    ent->set_position(x_dim_-(ent->get_radius()+5), entity_pos.y);
    break;
    case (kLeftWall):  // at x = 0
    ent->set_position(ent->get_radius()+5, entity_pos.y);
    break;
    case (kTopWall):  // at y = 0
    ent->set_position(entity_pos.x, ent->get_radius()+5);
    break;
    case (kBottomWall):  // at y = y_dim_
    ent->set_position(entity_pos.x, y_dim_-(ent->get_radius()+5));
    break;
    default:
    {}
  }
}

/* Calculates the distance between the center points to determine overlap */
bool Arena::IsColliding(
  ArenaMobileEntity * const mobile_e,
  ArenaEntity * const other_e) {
    double delta_x = other_e->get_pose().x - mobile_e->get_pose().x;
    double delta_y = other_e->get_pose().y - mobile_e->get_pose().y;
    double distance_between = sqrt(delta_x*delta_x + delta_y*delta_y);
    return
    (distance_between <= (mobile_e->get_radius() + other_e->get_radius()));
}

bool Arena::IsColliding(
  ArenaEntity * const e,
  ArenaEntity * const other_e) {
    double delta_x = other_e->get_pose().x - e->get_pose().x;
    double delta_y = other_e->get_pose().y - e->get_pose().y;
    double distance_between = sqrt(delta_x*delta_x + delta_y*delta_y);
    return
    (distance_between <= (e->get_radius() + other_e->get_radius()));
}

/* This is called when it is known that the two entities overlap.
* We determine by how much they overlap then move the mobile entity to
* the edge of the other
*/
/* @TODO: Add functionality to Pose to determine the distance distance_between two instances (e.g. overload operator -)
*/
void Arena::AdjustEntityOverlap(ArenaMobileEntity * const mobile_e,
  ArenaEntity *const other_e) {
    double delta_x = mobile_e->get_pose().x - other_e->get_pose().x;
    double delta_y = mobile_e->get_pose().y - other_e->get_pose().y;
    double distance_between = sqrt(delta_x*delta_x + delta_y*delta_y);
    double distance_to_move =
      mobile_e->get_radius() + other_e->get_radius() - distance_between + 5;
    double angle = atan2(delta_y, delta_x);
    mobile_e->set_position(
      mobile_e->get_pose().x+cos(angle)*distance_to_move,
      mobile_e->get_pose().y+sin(angle)*distance_to_move);
}

// Accept communication from the controller. Dispatching as appropriate.
/** @TODO: Call the appropriate Robot functions to implement user input
  * for controlling the robot.
  */
void Arena::AcceptCommand(Communication com) {
  switch (com) {
    case(kIncreaseSpeed):  // robot_->IncreaseSpeed(); break;
    case(kDecreaseSpeed):  // robot_->DecreaseSpeed(); break;
    case(kTurnLeft):  // robot_->TurnLeft(); break;
    case(kTurnRight):  // robot_->TurnRight(); break;
    case(kPlay): break;
    case(kPause): break;
    case(kReset):  // robot_->Reset(); break;
    case(kNone): break;
    default: break;
  }
} /* AcceptCommand */

NAMESPACE_END(csci3081);
