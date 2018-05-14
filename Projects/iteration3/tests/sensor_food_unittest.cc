 #ifdef SENSOR_FOOD_TESTS

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <gtest/gtest.h>
#include <vector>
#include "src/food.h"
#include "src/food_sensor.h"
#include "src/arena.h"
#include "src/arena_params.h"

class FoodSensorTest : public ::testing::Test {

 protected:
  virtual void SetUp() {}
};

/*******************************************************************************
 * Test Cases
 ******************************************************************************/

TEST_F(FoodSensorTest, Constructor) {
  csci3081::FoodSensor foodsensor(10, 5);
  EXPECT_EQ(foodsensor.get_pose().x, 10);
  EXPECT_EQ(foodsensor.get_pose().y, 5); // check that pose sets correctly in constructor
};

TEST_F(FoodSensorTest, Movement) {
  csci3081::FoodSensor foodsensor(0, 0);
  foodsensor.set_position(25,-30);
  EXPECT_EQ(foodsensor.get_pose().x, 25);
  EXPECT_EQ(foodsensor.get_pose().y, -30); // check that pose updates correctly
};

TEST_F(FoodSensorTest, OutputTest) {
  csci3081::FoodSensor foodsensor(0, 0);
  std::vector<csci3081::Food*> food_field;
  auto* food1 = new csci3081::Food;
  auto* food2 = new csci3081::Food;
  auto* food3 = new csci3081::Food;
  food1->set_radius(1);
  food2->set_radius(1);
  food3->set_radius(1);
  food1->set_pose({10000,10000,0});
  food2->set_pose({10000,10000,0});
  food3->set_pose({10000,10000,0});
  food_field.push_back(food1);
  food_field.push_back(food2);
  food_field.push_back(food3);

  foodsensor.SetLookingAt(food_field);
  EXPECT_LT(foodsensor.get_reading(),0.0001); // basically 0

  food1->set_pose({0,0,0});
  EXPECT_EQ(foodsensor.get_reading(),1000); // max reading

  food1->set_pose({1000,1000,0});
  EXPECT_LT(foodsensor.get_reading(),0.0001); // basically 0

  food1->set_pose({100,0,0});
  food2->set_pose({0,100,0});
  EXPECT_GT(foodsensor.get_reading(),0); 
  EXPECT_LT(foodsensor.get_reading(),1000);

  // Move other two foods on top of sensor
  food1->set_pose({0,0,0});
  EXPECT_EQ(foodsensor.get_reading(),1000); // also, shouldn't be able to go above max

  food2->set_pose({0,0,0});
  EXPECT_EQ(foodsensor.get_reading(),1000); // also, shouldn't be able to go above max


};
#endif /* SENSOR_FOOD_TESTS */
