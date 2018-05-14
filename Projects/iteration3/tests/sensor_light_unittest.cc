 #ifdef SENSOR_LIGHT_TESTS

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <gtest/gtest.h>
#include <vector>
#include "src/light.h"
#include "src/light_sensor.h"
#include "src/arena.h"
#include "src/arena_params.h"

class LightSensorTest : public ::testing::Test {

 protected:
  virtual void SetUp() {
    // Initialize Arena for Collections of TESTS
   /* csci3081::arena_params aparams;
    arena = new csci3081::Arena(&aparams);
    robot = arena->robot(); */
  } /*
  csci3081::Arena * arena;
  csci3081::Robot * robot; */
};

/*******************************************************************************
 * Test Cases
 ******************************************************************************/

TEST_F(LightSensorTest, Constructor) {
  csci3081::LightSensor lightsensor(10, 5);
  EXPECT_EQ(lightsensor.get_pose().x, 10);
  EXPECT_EQ(lightsensor.get_pose().y, 5); // check that pose sets correctly in constructor
};

TEST_F(LightSensorTest, Movement) {
  csci3081::LightSensor lightsensor(0, 0);
  lightsensor.set_position(25,-30);
  EXPECT_EQ(lightsensor.get_pose().x, 25);
  EXPECT_EQ(lightsensor.get_pose().y, -30); // check that pose updates correctly
};

TEST_F(LightSensorTest, OutputTest) {
  csci3081::LightSensor lightsensor(0, 0);
  std::vector<csci3081::Light*> light_field;
  auto* light1 = new csci3081::Light;
  auto* light2 = new csci3081::Light;
  auto* light3 = new csci3081::Light;
  light1->set_radius(1);
  light2->set_radius(1);
  light3->set_radius(1);
  light1->set_pose({10000,10000,0});
  light2->set_pose({10000,10000,0});
  light3->set_pose({10000,10000,0});
  light_field.push_back(light1);
  light_field.push_back(light2);
  light_field.push_back(light3);

  lightsensor.SetLookingAt(light_field);
  EXPECT_LT(lightsensor.get_reading(),0.0001); // basically 0

  light1->set_pose({0,0,0});
  EXPECT_EQ(lightsensor.get_reading(),1000); // max reading

  light1->set_pose({1000,1000,0});
  EXPECT_LT(lightsensor.get_reading(),0.0001); // basically 0

  light1->set_pose({100,0,0});
  light2->set_pose({0,100,0});
  EXPECT_GT(lightsensor.get_reading(),0); 
  EXPECT_LT(lightsensor.get_reading(),1000);

  // Move other two lights on top of sensor
  light1->set_pose({0,0,0});
  EXPECT_EQ(lightsensor.get_reading(),1000); // also, shouldn't be able to go above max

  light2->set_pose({0,0,0});
  EXPECT_EQ(lightsensor.get_reading(),1000); // also, shouldn't be able to go above max


};

/*
TEST_F(LightSensorTest, OutputTest) {
  csci3081::LightSensor lightsensor(0, 0);
  std::vector<csci3081::Light*> light_field;
  auto* light1 = new csci3081::Light;
  auto* light2 = new csci3081::Light;
  auto* light3 = new csci3081::Light;
  light_field.push_back(light1);
  light_field.push_back(light2);
  light_field.push_back(light3);
  light1->set_radius(1);
  light2->set_radius(1);
  light3->set_radius(1);
  light1->set_pose({1000,1000,0});
  light2->set_pose({1000,1000,0});
  light3->set_pose({1000,1000,0});
  lightsensor.SetLookingAt(light_field);

  // Can't believe I have to ask, but are things stored in the proper vector?

  // Should be 0 right now
  EXPECT_EQ(lightsensor.get_reading(),0);

  // One light on top of sensor
  light1->set_pose({0,0,0});
  lightsensor.SetLookingAt(light_field);
  EXPECT_EQ(lightsensor.get_reading(),1000); // max reading

  // move light into range but not on top
  light1->set_pose({500,0,0});
  EXPECT_GT(lightsensor.get_reading(),0);
  EXPECT_LT(lightsensor.get_reading(),1000);

  // move light back out of range
  light1->set_pose({1000,0,0});
  EXPECT_EQ(lightsensor.get_reading(),0);

  // Two lights equidistant from sensor
  light1->set_pose({700,0,0});
  light2->set_pose({0,700,0});
  EXPECT_GT(lightsensor.get_reading(),0); 
  EXPECT_LT(lightsensor.get_reading(),1000);

  // Move other two lights on top of sensor
  light1->set_pose({0,0,0});
  light2->set_pose({0,0,0});
  EXPECT_EQ(lightsensor.get_reading(),1000); // also, shouldn't be able to go above max

};
*/
#endif /* SENSOR_LIGHT_TESTS */
