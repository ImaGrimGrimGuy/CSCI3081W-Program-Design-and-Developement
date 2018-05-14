 #ifdef MOTION_HANDLER_TESTS

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <gtest/gtest.h>
#include "src/motion_handler_explorer.h"
#include "src/motion_handler_coward.h"
#include "src/entity_type.h"
#include "src/explorerboi.h"
#include "src/scaredyboi.h"
#include "src/light.h"
#include "src/food.h"


class MotionHandlerTest : public ::testing::Test {

 protected:
  virtual void SetUp() {}
};
/*******************************************************************************
 * Test Cases
 ******************************************************************************/

TEST_F(MotionHandlerTest, Constructor) {
  // there isnt really a good way to test the constructor without the bot constructor
  csci3081::ExplorerBoi* explorerboi = new csci3081::ExplorerBoi();
  csci3081::ScaredyBoi* scaredyboi = new csci3081::ScaredyBoi();

  EXPECT_EQ(scaredyboi->get_type(),csci3081::kCoward); // check if setting the type actually worked
  EXPECT_EQ(explorerboi->get_type(),csci3081::kExplorer); // basically did the constructor work
};

TEST_F(MotionHandlerTest, CowardHandlesInput) {
  csci3081::ScaredyBoi* scaredyboi = new csci3081::ScaredyBoi();
  scaredyboi->set_sensitivity(100);
  double pi = 3.14159265359;
  
  std::vector<csci3081::Light*> light_field;
  std::vector<csci3081::Food*> food_field;

  scaredyboi->get_right_light()->SetLookingAt(light_field);
  scaredyboi->get_left_light()->SetLookingAt(light_field);

  scaredyboi->get_right_food()->SetLookingAt(food_field);
  scaredyboi->get_left_food()->SetLookingAt(food_field);

  scaredyboi->set_position(0,0);

  // move the sensors to the right place
  scaredyboi->get_left_light()->set_position(scaredyboi->get_pose().x +
  scaredyboi->get_radius()*cos(scaredyboi->get_pose().theta*pi/180 - pi/2),
  scaredyboi->get_pose().y +
  scaredyboi->get_radius()*sin(scaredyboi->get_pose().theta*pi/180 - pi/2));

  scaredyboi->get_right_light()->set_position(scaredyboi->get_pose().x +
  scaredyboi->get_radius()*cos(scaredyboi->get_pose().theta * pi/180 + pi/2),
  scaredyboi->get_pose().y +
  scaredyboi->get_radius()*sin(scaredyboi->get_pose().theta * pi/180 + pi/2));

  scaredyboi->get_left_food()->set_position(scaredyboi->get_pose().x +
  scaredyboi->get_radius()*cos(scaredyboi->get_pose().theta*pi/180 - pi/2),
  scaredyboi->get_pose().y +
  scaredyboi->get_radius()*sin(scaredyboi->get_pose().theta*pi/180 - pi/2));

  scaredyboi->get_right_food()->set_position(scaredyboi->get_pose().x +
  scaredyboi->get_radius()*cos(scaredyboi->get_pose().theta * pi/180 + pi/2),
  scaredyboi->get_pose().y +
  scaredyboi->get_radius()*sin(scaredyboi->get_pose().theta * pi/180 + pi/2));

  scaredyboi->get_motion_handler().UpdateVelocity();

  EXPECT_EQ(scaredyboi->get_motion_handler().get_velocity().left, 0);
  EXPECT_EQ(scaredyboi->get_motion_handler().get_velocity().right, 0);

  auto* light1 = new csci3081::Light;
  light_field.push_back(light1);
  light1->set_radius(50);
  light1->set_position(0,0);

  scaredyboi->get_motion_handler().UpdateVelocity();  
  EXPECT_EQ(scaredyboi->get_motion_handler().get_velocity().left,
            scaredyboi->get_motion_handler().get_velocity().right);
  EXPECT_GT(scaredyboi->get_motion_handler().get_velocity().left, 0);
  EXPECT_GT(scaredyboi->get_motion_handler().get_velocity().right, 0);

  light1->set_position(10000,10000);

  for (int i = 0; i < 4000; i++) { // GET HUNGRY BOY!!!
    scaredyboi->TimestepUpdate(1);
  }

  scaredyboi->set_position(0,0);

  // move the sensors to the right place
  scaredyboi->get_left_light()->set_position(scaredyboi->get_pose().x +
  scaredyboi->get_radius()*cos(scaredyboi->get_pose().theta*pi/180 - pi/2),
  scaredyboi->get_pose().y +
  scaredyboi->get_radius()*sin(scaredyboi->get_pose().theta*pi/180 - pi/2));

  scaredyboi->get_right_light()->set_position(scaredyboi->get_pose().x +
  scaredyboi->get_radius()*cos(scaredyboi->get_pose().theta * pi/180 + pi/2),
  scaredyboi->get_pose().y +
  scaredyboi->get_radius()*sin(scaredyboi->get_pose().theta * pi/180 + pi/2));

  scaredyboi->get_left_food()->set_position(scaredyboi->get_pose().x +
  scaredyboi->get_radius()*cos(scaredyboi->get_pose().theta*pi/180 - pi/2),
  scaredyboi->get_pose().y +
  scaredyboi->get_radius()*sin(scaredyboi->get_pose().theta*pi/180 - pi/2));

  scaredyboi->get_right_food()->set_position(scaredyboi->get_pose().x +
  scaredyboi->get_radius()*cos(scaredyboi->get_pose().theta * pi/180 + pi/2),
  scaredyboi->get_pose().y +
  scaredyboi->get_radius()*sin(scaredyboi->get_pose().theta * pi/180 + pi/2));

  scaredyboi->get_motion_handler().UpdateVelocity();

  EXPECT_EQ(scaredyboi->get_motion_handler().get_velocity().left,
            scaredyboi->get_motion_handler().get_velocity().right);
  EXPECT_EQ(scaredyboi->get_motion_handler().get_velocity().left, 0.5);
  EXPECT_EQ(scaredyboi->get_motion_handler().get_velocity().left, 0.5);

  auto* food1 = new csci3081::Food;
  food_field.push_back(food1);
  food1->set_radius(50);
  food1->set_position(0,100);

  scaredyboi->get_motion_handler().UpdateVelocity();
  EXPECT_EQ(scaredyboi->get_motion_handler().get_velocity().left,
            scaredyboi->get_motion_handler().get_velocity().right);
  EXPECT_GT(scaredyboi->get_motion_handler().get_velocity().left, 0.5);
  EXPECT_GT(scaredyboi->get_motion_handler().get_velocity().left, 0.5);

  food1->set_position(0,0);  // EAT.

  scaredyboi->get_motion_handler().UpdateVelocity();

  EXPECT_EQ(scaredyboi->get_motion_handler().get_velocity().left, 0);
  EXPECT_EQ(scaredyboi->get_motion_handler().get_velocity().right, 0);

};


TEST_F(MotionHandlerTest, ExplorerHandlesInput) {
  csci3081::ExplorerBoi* explorerboi = new csci3081::ExplorerBoi();
  explorerboi->set_sensitivity(100);
  double pi = 3.14159265359;

  std::vector<csci3081::Light*> light_field;
  std::vector<csci3081::Food*> food_field;

  explorerboi->get_right_light()->SetLookingAt(light_field);
  explorerboi->get_left_light()->SetLookingAt(light_field);

  explorerboi->get_right_food()->SetLookingAt(food_field);
  explorerboi->get_left_food()->SetLookingAt(food_field);

  explorerboi->set_position(0,0);

  // move the sensors to the right place
  explorerboi->get_left_light()->set_position(explorerboi->get_pose().x +
  explorerboi->get_radius()*cos(explorerboi->get_pose().theta*pi/180 - pi/2),
  explorerboi->get_pose().y +
  explorerboi->get_radius()*sin(explorerboi->get_pose().theta*pi/180 - pi/2));

  explorerboi->get_right_light()->set_position(explorerboi->get_pose().x +
  explorerboi->get_radius()*cos(explorerboi->get_pose().theta * pi/180 + pi/2),
  explorerboi->get_pose().y +
  explorerboi->get_radius()*sin(explorerboi->get_pose().theta * pi/180 + pi/2));

  explorerboi->get_left_food()->set_position(explorerboi->get_pose().x +
  explorerboi->get_radius()*cos(explorerboi->get_pose().theta*pi/180 - pi/2),
  explorerboi->get_pose().y +
  explorerboi->get_radius()*sin(explorerboi->get_pose().theta*pi/180 - pi/2));

  explorerboi->get_right_food()->set_position(explorerboi->get_pose().x +
  explorerboi->get_radius()*cos(explorerboi->get_pose().theta * pi/180 + pi/2),
  explorerboi->get_pose().y +
  explorerboi->get_radius()*sin(explorerboi->get_pose().theta * pi/180 + pi/2));


  // shouldn't be zero with nothing to look at
  EXPECT_GT(explorerboi->get_motion_handler().get_velocity().left, 0);
  EXPECT_GT(explorerboi->get_motion_handler().get_velocity().right, 0);

  auto* light1 = new csci3081::Light;
  light_field.push_back(light1);
  light1->set_radius(50);
  light1->set_position(1000,1000);

  explorerboi->get_motion_handler().UpdateVelocity();  
 
  EXPECT_EQ(explorerboi->get_motion_handler().get_velocity().left,
            explorerboi->get_motion_handler().get_velocity().right);
  EXPECT_EQ(explorerboi->get_motion_handler().get_velocity().left, 0);
  EXPECT_EQ(explorerboi->get_motion_handler().get_velocity().right, 0);

  light1->set_position(10000,10000);

  for (int i = 0; i < 4000; i++) { // GET HUNGRY BOY!!!
    explorerboi->TimestepUpdate(1);
  }

  explorerboi->set_position(0,0);
  // move the sensors to the right place
  explorerboi->get_left_light()->set_position(explorerboi->get_pose().x +
  explorerboi->get_radius()*cos(explorerboi->get_pose().theta*pi/180 - pi/2),
  explorerboi->get_pose().y +
  explorerboi->get_radius()*sin(explorerboi->get_pose().theta*pi/180 - pi/2));

  explorerboi->get_right_light()->set_position(explorerboi->get_pose().x +
  explorerboi->get_radius()*cos(explorerboi->get_pose().theta * pi/180 + pi/2),
  explorerboi->get_pose().y +
  explorerboi->get_radius()*sin(explorerboi->get_pose().theta * pi/180 + pi/2));

  explorerboi->get_left_food()->set_position(explorerboi->get_pose().x +
  explorerboi->get_radius()*cos(explorerboi->get_pose().theta*pi/180 - pi/2),
  explorerboi->get_pose().y +
  explorerboi->get_radius()*sin(explorerboi->get_pose().theta*pi/180 - pi/2));

  explorerboi->get_right_food()->set_position(explorerboi->get_pose().x +
  explorerboi->get_radius()*cos(explorerboi->get_pose().theta * pi/180 + pi/2),
  explorerboi->get_pose().y +
  explorerboi->get_radius()*sin(explorerboi->get_pose().theta * pi/180 + pi/2));

  EXPECT_EQ(explorerboi->get_motion_handler().get_velocity().left,
            explorerboi->get_motion_handler().get_velocity().right);
  EXPECT_EQ(explorerboi->get_motion_handler().get_velocity().left, 0.5);
  EXPECT_EQ(explorerboi->get_motion_handler().get_velocity().left, 0.5);

  auto* food1 = new csci3081::Food;
  food_field.push_back(food1);
  food1->set_radius(50);
  food1->set_position(0,10);

  explorerboi->get_motion_handler().UpdateVelocity();
  EXPECT_EQ(explorerboi->get_motion_handler().get_velocity().left,
            explorerboi->get_motion_handler().get_velocity().right);
  EXPECT_GT(explorerboi->get_motion_handler().get_velocity().left, 0.5);
  EXPECT_GT(explorerboi->get_motion_handler().get_velocity().left, 0.5);

  food1->set_position(0,0);  // EAT.

  explorerboi->get_motion_handler().UpdateVelocity();

  EXPECT_EQ(explorerboi->get_motion_handler().get_velocity().left, 0);
  EXPECT_EQ(explorerboi->get_motion_handler().get_velocity().right, 0);

};
#endif /* MOTION_HANDLER_TESTS */
