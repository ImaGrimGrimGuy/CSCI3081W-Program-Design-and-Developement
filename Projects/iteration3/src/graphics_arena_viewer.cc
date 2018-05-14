/**
 * @file graphics_arena_viewer.cc
 *
 * @copyright 2017 3081 Staff, All rights reserved.
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <vector>
#include <iostream>
#include <string>
#include <algorithm>

#include "src/graphics_arena_viewer.h"
#include "src/arena_params.h"
#include "src/rgb_color.h"

/*******************************************************************************
 * Namespaces
 ******************************************************************************/
NAMESPACE_BEGIN(csci3081);

/*******************************************************************************
 * Constructors/Destructor
 ******************************************************************************/
GraphicsArenaViewer::GraphicsArenaViewer(
    const struct arena_params *const params,
    Arena * arena, Controller * controller) :
    GraphicsApp(
        params->x_dim + GUI_MENU_WIDTH + GUI_MENU_GAP * 2,
        params->y_dim,
        "Robot Simulation"),
    controller_(controller),
    arena_(arena) {
  auto *gui = new nanogui::FormHelper(screen());
  nanogui::ref<nanogui::Window> window =
      gui->addWindow(
          Eigen::Vector2i(1100 + GUI_MENU_GAP, 10),
          "Menu");

// vvvvvvvvvvvv    ADDED THIS ONE LINE to register the window  vvvvvvvvvvvv
  // gui->addGroup creates a heading within the window
  window->setLayout(new nanogui::GroupLayout());

  gui->addGroup("Simulation Control");
  playing_button_ =
    gui->addButton(
      "Play",
      std::bind(&GraphicsArenaViewer::OnPlayingBtnPressed, this));
  screen()->performLayout();
  reset_button_ =
    gui->addButton(
      "Reset",
      std::bind(&GraphicsArenaViewer::OnResetBtnPressed, this));

// vvvvvvvvvv  ADDED BELOW HERE (from nanogui example1.cc)   vvvvvvvvvvvvv

  gui->addGroup("Arena Configuration");

  hunger_button_ =
    gui->addButton(
      "Hunger ON",
      std::bind(&GraphicsArenaViewer::OnHungerBtnPressed, this));
  screen()->performLayout();
  // Creating a panel impacts the layout. Widgets, sliders, buttons can be
  // assigned to either the window or the panel.
  nanogui::Widget *panel = new nanogui::Widget(window);

  // *************** SLIDER 1 ************************//
  new nanogui::Label(panel, "Number of Robots", "sans-bold");
  nanogui::Slider *slider = new nanogui::Slider(panel);
  // The starting value (range is from 0 to 1)
  // Note that below the displayed value is 10* slider value.
  slider->setValue(1.0f);
  slider->setFixedWidth(100);

  // Display the corresponding value of the slider in this textbox
  nanogui::TextBox *textBox = new nanogui::TextBox(panel);
  textBox->setFixedSize(nanogui::Vector2i(60, 25));
  textBox->setFontSize(20);
  textBox->setValue("10");

  // This is the lambda function called while the user is moving the slider
  slider->setCallback(
    [textBox](float value) {
      textBox->setValue(
               std::to_string(std::max(1, static_cast<int>(value*10))));
    });

  slider->setFinalCallback(
    [&](float value) {
      update_ = true;
      robot_count_ = std::max(1, static_cast<int>(value*10));
      reset_button_->setCaption("Update Simulation");
    });

  // *************** SLIDER 2 ************************//
  new nanogui::Label(panel, "Number of Lights", "sans-bold");
  nanogui::Slider *slider2 = new nanogui::Slider(panel);
  slider2->setValue(0.4f);
  slider2->setFixedWidth(100);

  nanogui::TextBox *textBox2 = new nanogui::TextBox(panel);
  textBox2->setFixedSize(nanogui::Vector2i(60, 25));
  textBox2->setFontSize(20);
  textBox2->setValue("4");

  slider2->setCallback(
    [textBox2](float value) {
      textBox2->setValue(std::to_string(static_cast<int>(value*10)));
    });

  slider2->setFinalCallback(
    [&](float value) {
      update_ = true;
      light_count_ = static_cast<int>(value*10);
      reset_button_->setCaption("Update Simulation");
    });

  // *************** SLIDER 3 ************************//
  new nanogui::Label(panel, "Number of Fearful Bots", "sans-bold");
  nanogui::Slider *slider3 = new nanogui::Slider(panel);
  slider3->setValue(0.5f);
  slider3->setFixedWidth(100);

  nanogui::TextBox *textBox3 = new nanogui::TextBox(panel);
  textBox3->setFixedSize(nanogui::Vector2i(60, 25));
  textBox3->setFontSize(20);
  textBox3->setValue("5");

  slider3->setCallback(
    [textBox3, this](float value) {
      textBox3->setValue(std::to_string(
                static_cast<int>(value*robot_count_)));
    });

  slider3->setFinalCallback(
    [&](float value) {
      update_ = true;
      fearful_count_ = static_cast<int>(value*robot_count_);
      reset_button_->setCaption("Update Simulation");
    });

  // *************** SLIDER 4 ************************//
  new nanogui::Label(panel, "Number of Food Blobs", "sans-bold");
  nanogui::Slider *slider4 = new nanogui::Slider(panel);
  // The starting value (range is from 0 to 1)
  // Note that below the displayed value is 10* slider value.
  slider4->setValue(0.4f);
  slider4->setFixedWidth(100);

  // Display the corresponding value of the slider in this textbox
  nanogui::TextBox *textBox4 = new nanogui::TextBox(panel);
  textBox4->setFixedSize(nanogui::Vector2i(60, 25));
  textBox4->setFontSize(20);
  textBox4->setValue("4");

  // This is the lambda function called while the user is moving the slider
  slider4->setCallback(
    [textBox4](float value) {
      textBox4->setValue(std::to_string(static_cast<int>(value*10)));
    });

  slider4->setFinalCallback(
    [&](float value) {
      update_ = true;
      food_count_ = static_cast<int>(value*10);
      reset_button_->setCaption("Update Simulation");
    });


  // *************** SLIDER 5 ************************//
  new nanogui::Label(panel, "Light Sensitivity", "sans-bold");
  nanogui::Slider *slider5 = new nanogui::Slider(panel);
  slider5->setValue(0.5f);
  slider5->setFixedWidth(100);

  nanogui::TextBox *textBox5 = new nanogui::TextBox(panel);
  textBox5->setFixedSize(nanogui::Vector2i(60, 25));
  textBox5->setFontSize(20);
  textBox5->setValue("5");

  slider5->setCallback(
    [textBox5](float value) {
      textBox5->setValue(std::to_string(static_cast<int>(value*10)));
    });

  slider5->setFinalCallback(
    [&](float value) {
      update_ = true;
      light_sensitivity_ = static_cast<int>(value*10);
      reset_button_->setCaption("Update Simulation");
    });

  // Lays out all the components with "15" units of inbetween spacing
  panel->setLayout(new nanogui::BoxLayout(nanogui::Orientation::Vertical,
    nanogui::Alignment::Middle, 0, 15));
  panel->setLayout(new nanogui::BoxLayout(nanogui::Orientation::Vertical,
    nanogui::Alignment::Middle, 0, 15));


  screen()->performLayout();
}

/*******************************************************************************
 * Member Functions
 ******************************************************************************/

// This is the primary driver for state change in the arena.
// It will be called at each iteration of nanogui::mainloop()
void GraphicsArenaViewer::UpdateSimulation(double dt) {
  if (!paused_) {
    controller_->AdvanceTime(dt);
  }
}

/*******************************************************************************
 * Handlers for User Keyboard and Mouse Events
 ******************************************************************************/
void GraphicsArenaViewer::OnPlayingBtnPressed() {
  if (!paused_) {
    paused_ = true;
    playing_button_->setCaption("Play");
  } else {
    paused_ = false;
    playing_button_->setCaption("Pause");
  }
}

void GraphicsArenaViewer::OnResetBtnPressed() {
  arena_->Reset();
  playing_button_->setCaption("Play");
  paused_ = true;
  if (update_) {
    update_ = false;
    arena_->Reset(light_count_, robot_count_,
                  food_count_, fearful_count_,
                  hunger_, light_sensitivity_);
    reset_button_->setCaption("Reset");
  } else {
    arena_->Reset();
  }
}

void GraphicsArenaViewer::OnHungerBtnPressed() {
  if (!hunger_) {
    hunger_ = true;
    hunger_button_->setCaption("Hunger ON");
  } else {
    hunger_ = false;
    hunger_button_->setCaption("Hunger OFF");
  }
  update_ = true;
  reset_button_->setCaption("Update Simulation");
}
/** OnSpecialKeyDown is called when the user presses down on one of the
  * special keys (e.g. the arrow keys).
  */
/**
 * @TODO: Check for arrow key presses using GLFW macros, then
 * convert to appropriate enum Communication and relay to controller
 */
void GraphicsArenaViewer::OnSpecialKeyDown(int key,
  __unused int scancode, __unused int modifiers) {
    Communication key_value = kNone;
    switch (key) {
      case GLFW_KEY_LEFT: key_value = kKeyLeft;
           break;
      case GLFW_KEY_RIGHT: key_value = kKeyRight;
           break;
      case GLFW_KEY_UP: key_value = kKeyUp;
           break;
      case GLFW_KEY_DOWN: key_value = kKeyDown;
           break;
      default: {}
    }
  controller_->AcceptCommunication(key_value);
}

/*******************************************************************************
 * Drawing of Entities in Arena
 ******************************************************************************/
void GraphicsArenaViewer::VictoryOrDefeat(NVGcontext *ctx, int status) {
  if (status == WON) {  // print "YOU WIN!" in the middle
  nvgFillColor(ctx, nvgRGBA(0, 255, 0, 255));
  nvgText(ctx,
          static_cast<float>(512),
          static_cast<float>(393),
          "YOU WIN!", nullptr);
  nvgFillColor(ctx, nvgRGBA(0, 0, 0, 255));
  paused_ = true;
  playing_button_->setCaption("Play");
  } else {  // print "YOU LOSE!" in the middle
  nvgFillColor(ctx, nvgRGBA(255, 0, 0, 255));
  nvgText(ctx,
          static_cast<float>(512),
          static_cast<float>(393),
          "YOU LOSE!", nullptr);
  nvgFillColor(ctx, nvgRGBA(0, 0, 0, 255));
  paused_ = true;
  playing_button_->setCaption("Play");
  }
}

void GraphicsArenaViewer::DrawRobot(NVGcontext *ctx,
                                     const Robot *const robot) {
  // translate and rotate all graphics calls that follow so that they are
  // centered, at the position and heading of this robot
  nvgSave(ctx);
  nvgTranslate(ctx,
               static_cast<float>(robot->get_pose().x),
               static_cast<float>(robot->get_pose().y));
  nvgRotate(ctx,
            static_cast<float>(robot->get_pose().theta * M_PI / 180.0));

  // robot's circle
  nvgBeginPath(ctx);
  nvgCircle(ctx, 0.0, 0.0, static_cast<float>(robot->get_radius()));
  nvgFillColor(ctx,
               nvgRGBA(robot->get_color().r, robot->get_color().g,
                       robot->get_color().b, 255));
  nvgFill(ctx);
  nvgStrokeColor(ctx, nvgRGBA(0, 0, 0, 255));
  nvgStroke(ctx);

  // robot id text label
  nvgSave(ctx);
  nvgRotate(ctx, static_cast<float>(M_PI / 2.0));
  nvgFillColor(ctx, nvgRGBA(0, 0, 0, 255));
  nvgText(ctx, 0.0, 10.0, robot->get_name().c_str(), nullptr);
  // robot lives text label
  nvgText(ctx, 0.0, -3.0, std::to_string(robot->get_lives()).c_str(), nullptr);
  nvgRestore(ctx);
  nvgRestore(ctx);
}
void GraphicsArenaViewer::DrawArena(NVGcontext *ctx) {
  nvgBeginPath(ctx);
  // Creates new rectangle shaped sub-path.
  nvgRect(ctx, 0, 0, arena_->get_x_dim(), arena_->get_y_dim());
  nvgStrokeColor(ctx, nvgRGBA(255, 255, 255, 255));
  nvgStroke(ctx);
}

void GraphicsArenaViewer::DrawEntity(NVGcontext *ctx,
                                       ArenaEntity * entity) {
  // entity's circle
  nvgBeginPath(ctx);
  nvgCircle(ctx,
            static_cast<float>(entity->get_pose().x),
            static_cast<float>(entity->get_pose().y),
            static_cast<float>(entity->get_radius()));
  nvgFillColor(ctx,
               nvgRGBA(entity->get_color().r, entity->get_color().g,
                       entity->get_color().b, 255));
  nvgFill(ctx);
  nvgStrokeColor(ctx, nvgRGBA(0, 0, 0, 255));
  nvgStroke(ctx);

  // entity id text label
  nvgFillColor(ctx, nvgRGBA(0, 0, 0, 255));
  nvgText(ctx,
          static_cast<float>(entity->get_pose().x),
          static_cast<float>(entity->get_pose().y),
          entity->get_name().c_str(), nullptr);

  if (entity->get_type() ==  kCoward) {
    //////////////////////////// Light Sensor /////////////////////////////
    LightSensor* left = dynamic_cast<ScaredyBoi*>(entity)->get_left_light();
    LightSensor* right = dynamic_cast<ScaredyBoi*>(entity)->get_right_light();

    // Left sensor's circle
    nvgBeginPath(ctx);
    nvgCircle(ctx,
              static_cast<float>(left->get_pose().x),
              static_cast<float>(left->get_pose().y),
              static_cast<float>(left->get_radius()));
    nvgFillColor(ctx,
                 nvgRGBA(left->get_color().r, left->get_color().g,
                         left->get_color().b, 255));
    nvgFill(ctx);
    nvgStrokeColor(ctx, nvgRGBA(0, 0, 0, 255));
    nvgStroke(ctx);

    // Right sensor's circle
    nvgBeginPath(ctx);
    nvgCircle(ctx,
              static_cast<float>(right->get_pose().x),
              static_cast<float>(right->get_pose().y),
              static_cast<float>(right->get_radius()));
    nvgFillColor(ctx,
                 nvgRGBA(right->get_color().r, right->get_color().g,
                         right->get_color().b, 255));
    nvgFill(ctx);
    nvgStrokeColor(ctx, nvgRGBA(0, 0, 0, 255));
    nvgStroke(ctx);

    //////////////////////////// Food Sensor /////////////////////////////

    FoodSensor* left2 = dynamic_cast<ScaredyBoi*>(entity)->get_left_food();
    FoodSensor* right2 = dynamic_cast<ScaredyBoi*>(entity)->get_right_food();

    // Left sensor's circle
    nvgBeginPath(ctx);
    nvgCircle(ctx,
              static_cast<float>(left2->get_pose().x),
              static_cast<float>(left2->get_pose().y),
              static_cast<float>(left2->get_radius()));
    nvgFillColor(ctx,
                 nvgRGBA(left2->get_color().r, right2->get_color().g,
                         left2->get_color().b, 255));
    nvgFill(ctx);
    nvgStrokeColor(ctx, nvgRGBA(0, 0, 0, 255));
    nvgStroke(ctx);

    // Right sensor's circle
    nvgBeginPath(ctx);
    nvgCircle(ctx,
              static_cast<float>(right2->get_pose().x),
              static_cast<float>(right2->get_pose().y),
              static_cast<float>(right2->get_radius()));
    nvgFillColor(ctx,
                 nvgRGBA(right2->get_color().r, right2->get_color().g,
                         right2->get_color().b, 255));
    nvgFill(ctx);
    nvgStrokeColor(ctx, nvgRGBA(0, 0, 0, 255));
    nvgStroke(ctx);
  }

  if (entity->get_type() ==  kExplorer) {
    //////////////////////////// Light Sensor /////////////////////////////
    LightSensor* left = dynamic_cast<ExplorerBoi*>(entity)->get_left_light();
    LightSensor* right = dynamic_cast<ExplorerBoi*>(entity)->get_right_light();

    // Left sensor's circle
    nvgBeginPath(ctx);
    nvgCircle(ctx,
              static_cast<float>(left->get_pose().x),
              static_cast<float>(left->get_pose().y),
              static_cast<float>(left->get_radius()));
    nvgFillColor(ctx,
                 nvgRGBA(left->get_color().r, left->get_color().g,
                         left->get_color().b, 255));
    nvgFill(ctx);
    nvgStrokeColor(ctx, nvgRGBA(0, 0, 0, 255));
    nvgStroke(ctx);

    // Right sensor's circle
    nvgBeginPath(ctx);
    nvgCircle(ctx,
              static_cast<float>(right->get_pose().x),
              static_cast<float>(right->get_pose().y),
              static_cast<float>(right->get_radius()));
    nvgFillColor(ctx,
                 nvgRGBA(right->get_color().r, right->get_color().g,
                         right->get_color().b, 255));
    nvgFill(ctx);
    nvgStrokeColor(ctx, nvgRGBA(0, 0, 0, 255));
    nvgStroke(ctx);

    //////////////////////////// Food Sensor /////////////////////////////

    FoodSensor* left2 = dynamic_cast<ExplorerBoi*>(entity)->get_left_food();
    FoodSensor* right2 = dynamic_cast<ExplorerBoi*>(entity)->get_right_food();

    // Left sensor's circle
    nvgBeginPath(ctx);
    nvgCircle(ctx,
              static_cast<float>(left2->get_pose().x),
              static_cast<float>(left2->get_pose().y),
              static_cast<float>(left2->get_radius()));
    nvgFillColor(ctx,
                 nvgRGBA(left2->get_color().r, left2->get_color().g,
                         left2->get_color().b, 255));
    nvgFill(ctx);
    nvgStrokeColor(ctx, nvgRGBA(0, 0, 0, 255));
    nvgStroke(ctx);

    // Right sensor's circle
    nvgBeginPath(ctx);
    nvgCircle(ctx,
              static_cast<float>(right2->get_pose().x),
              static_cast<float>(right2->get_pose().y),
              static_cast<float>(right2->get_radius()));
    nvgFillColor(ctx,
                 nvgRGBA(right2->get_color().r, right2->get_color().g,
                         right2->get_color().b, 255));
    nvgFill(ctx);
    nvgStrokeColor(ctx, nvgRGBA(0, 0, 0, 255));
    nvgStroke(ctx);
  }
}

void GraphicsArenaViewer::DrawUsingNanoVG(NVGcontext *ctx) {
  // initialize text rendering settings
  nvgFontSize(ctx, 18.0f);
  nvgFontFace(ctx, "sans-bold");
  nvgTextAlign(ctx, NVG_ALIGN_CENTER | NVG_ALIGN_MIDDLE);
  DrawArena(ctx);
  switch (arena_->get_game_status()) {
    case WON: VictoryOrDefeat(ctx, WON); break;
    case LOST: VictoryOrDefeat(ctx, LOST); break;
    default: break;
  }
  std::vector<ArenaEntity *> entities = arena_->get_entities();
  for (auto &entity : entities) {
    DrawEntity(ctx, entity);
  } /* for(i..) */
  // DrawRobot(ctx, arena_->robot());
}

NAMESPACE_END(csci3081);
