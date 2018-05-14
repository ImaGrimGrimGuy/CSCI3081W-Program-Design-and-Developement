/**
 * @file sensor_touch.h
 *
 * @copyright 2017 3081 Staff, All rights reserved.
 */

#ifndef SRC_LIGHT_SENSOR_H_
#define SRC_LIGHT_SENSOR_H_

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <utility>
#include <vector>
#include <string>
#include <iostream>

#include "src/common.h"
#include "src/pose.h"
#include "src/light.h"

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

class LightSensor : public ArenaMobileEntity {
 public:
  /**
   * @brief Constructor.
   */
  LightSensor();
  LightSensor(const LightSensor& other) = delete;
  LightSensor operator=(const LightSensor& other) = delete;


  LightSensor(const double xpose, const double ypose);

  /**
   * @brief Name to satisfy base class interface
   */
  std::string get_name() const override {
    return "Light Sensor";
  }

  /**
   * @brief Reset the LightSensor to a newly constructed state (needed for reset
   * button to work in GUI).
   */
  void Reset() override;

  /**
   * @brief Update the Light_Sensor's position and velocity after the specified
   * duration has passed.
   *
   * @param dt The # of timesteps that have elapsed since the last update.
   */
  void TimestepUpdate(unsigned int dt) override;  // not implemented

  /**
   * @brief Getter and setter for looking_at_, the pointer to the place were lights reside
   */
  void SetLookingAt(std::vector<class Light *> light_field) {
    for (auto light : light_field) {
      looking_at_.push_back(light);
    }
  }

  std::vector<class Light *> GetLookingAt() {return looking_at_;}

  /**
   * @brief Getter for reading_, which scales with proximity to light objects.
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
  std::vector<class Light *> looking_at_;
  unsigned int flag{1};
};

NAMESPACE_END(csci3081);

#endif  // SRC_LIGHT_SENSOR_H_
