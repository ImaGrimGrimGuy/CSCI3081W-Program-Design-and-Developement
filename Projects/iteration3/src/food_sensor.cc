/**
 * @file food_sensor.cc
 *
 * @copyright 2017 3081 Staff, All rights reserved.
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <iostream>
#include <algorithm>
#include "src/food_sensor.h"

/*******************************************************************************
 * Namespaces
 ******************************************************************************/
NAMESPACE_BEGIN(csci3081);

/*******************************************************************************
 * Member Functions
 ******************************************************************************/

FoodSensor::FoodSensor() : motion_behavior_(this),
                             looking_at_() {set_color(SENSOR_COLOR);
                                            set_radius(5);}

FoodSensor::FoodSensor(const double xpose, const double ypose) :
  motion_behavior_(this),
  looking_at_() {set_color(SENSOR_COLOR);
                 set_position(xpose, ypose); set_radius(5);}

void FoodSensor::Reset() {  // not implemented (irrelevant)
  sensor_touch_->Reset();
} /* Reset() */

void FoodSensor::TimestepUpdate(unsigned int dt) {
  flag = dt;
  // Reset Sensor for next cycle
  sensor_touch_->Reset();
} /* TimestepUpdate() */


double FoodSensor::get_reading() {
  double reading = 0;
  double tempreading = 0;
  for (auto ent : looking_at_) {
    double distance = std::hypot((get_pose().x - ent->get_pose().x),
                                 (get_pose().y - ent->get_pose().y));
    tempreading += 1200/(pow(1.08,
                       distance-(ent->get_radius())));
  }

  reading = std::min(tempreading, 1000.0);

  return reading;
}

NAMESPACE_END(csci3081);
