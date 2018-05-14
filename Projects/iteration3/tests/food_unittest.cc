/* STUDENTS:  DO NOT EDIT THIS FILE.  INSTEAD, MODIFY YOUR OWN PROJECT
 * CODE TO FIT THE SPECIFICATION GIVEN BY THE TESTS IN THIS FILE.
 *
 * If you DO modify it, we overwrite it with a fresh version from our repo
 * when grading, so you can't use it to fudge the tests anyway.
 *
 * This file is used by the CS3081W auto-grading system.
 *
 * Please note, the assessment tests for grading, will use the same include
 * files, class names, and function names for accessing students' code that you
 * find in this file.  So students, if you write your code so that it passes
 * these feedback tests, you can be assured that the auto-grader will at least
 * be able to properly link with your code.
 */


 #ifdef FOOD_TESTS

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <gtest/gtest.h>
#include "src/food.h"
#include "src/arena.h"
#include "src/arena_params.h"


class FoodTest : public ::testing::Test {

 protected:
  virtual void SetUp() {
    // Initialize Arena for Collections of TESTS
    csci3081::arena_params aparams;
    arena = new csci3081::Arena(&aparams);
    robot = arena->robot();
  }
  csci3081::Arena * arena;
  csci3081::Robot * robot;
};

/*******************************************************************************
 * Test Cases
 ******************************************************************************/

TEST_F(FoodTest, Constructor) {
  csci3081::Food food;
  EXPECT_EQ(food.get_name(), "Food");
  EXPECT_EQ(food.get_type(), csci3081::kFood);
  EXPECT_EQ(food.get_radius(), FOOD_RADIUS);
  EXPECT_FALSE(food.is_mobile());
  EXPECT_FALSE(food.IsCaptured());
};

TEST_F(FoodTest, ChangeColorOnCaptured) {
  for(auto& ent : arena->get_entities()) {
    if(ent->get_type() == csci3081::kFood) {
      csci3081::RgbColor color = ent->get_color();
      ent->set_position(200, 200);
      robot->set_position(200, 200);
      arena->AdvanceTime(1);
      csci3081::RgbColor color_new = ent->get_color();
      EXPECT_NE(std::make_tuple(color_new.r, color_new.g ,color_new.b),
                std::make_tuple(color.r, color.g ,color.b));
      break;
    }
  }
};

#endif /* FOOD_TESTS */
