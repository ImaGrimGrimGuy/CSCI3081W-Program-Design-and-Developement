/**
 * @file sensor_touch.h
 *
 * @copyright 2017 3081 Staff, All rights reserved.
 */

#ifndef SRC_FOOD_SENSOR_H_
#define SRC_FOOD_SENSOR_H_

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <utility>
#include <vector>
#include <string>
#include <iostream>

#include "src/arena_mobile_entity.h"
#include "src/motion_behavior_differential.h"
#include "src/common.h"
#include "src/pose.h"
#include "src/food.h"

/*******************************************************************************
 * Namespaces
 ******************************************************************************/
NAMESPACE_BEGIN(csci3081);

/*******************************************************************************
 * Classes
 ******************************************************************************/
/**
   * @brief 
   *
   */

class FoodSensor : public ArenaMobileEntity {
 public:
  /**
   * @brief Constructor.
   */
  FoodSensor();
  FoodSensor(const FoodSensor& other) = delete;
  FoodSensor operator=(const FoodSensor& other) = delete;


  FoodSensor(const double xpose, const double ypose);

  /**
   * @brief Name to satisfy base class interface
   */
  std::string get_name() const override {
    return "Food Sensor";
  }

  /**
   * @brief Reset the FoodSensor to a newly constructed state (needed for reset
   * button to work in GUI).
   */
  void Reset() override;

  /**
   * @brief Update the Food_Sensor's position and velocity after the specified
   * duration has passed.
   *
   * @param dt The # of timesteps that have elapsed since the last update.
   */
  void TimestepUpdate(unsigned int dt) override;  // not implemented

  /**
   * @brief Getter and setter for looking_at_, the pointer to the place were foods reside
   */
  void SetLookingAt(std::vector<class Food *> food_field) {
    for (auto food : food_field) {
      looking_at_.push_back(food);
    }
  }

  /**
   * @brief Getter for looking_at_
   */
  std::vector<class Food *> GetLookingAt() {return looking_at_;}

  /**
   * @brief Getter for reading_, which scales with proximity to food objects.
   */

  double get_reading();  // update to scan the list and calculate reading


/**
  * @brief Getter for motion_behavior_
  */
  MotionBehaviorDifferential get_motion_behavior() {return motion_behavior_;}


 private:
  // motion behavior so robots can update their sensor postitions
  MotionBehaviorDifferential motion_behavior_;
  // vector containing pointers to all the foods
  std::vector<class Food *> looking_at_;
  unsigned int flag{1};
};

NAMESPACE_END(csci3081);

#endif  // SRC_FOOD_SENSOR_H_
