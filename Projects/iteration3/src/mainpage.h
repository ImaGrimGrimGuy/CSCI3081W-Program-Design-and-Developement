#ifndef SRC_MAINPAGE_H_
#define SRC_MAINPAGE_H_

/*! \mainpage My Personal Index Page
 *
 * \section intro_sec Introduction
 *
 * Copyright 2018 Daniel Hartmann
 * 
 * This program is a Braitenberg vehicle simulator which runs using the nanoVGL graphics engine and
 * nanoGUI. nanoVGL performs a loop which first calls an updating function in arena.cc, which in turn 
 * runs a loop that updates every entity in the simulation via two vectors called mobile_entities and
 * entities_. There are currently four types of entities in the arena:
 * 
 * Non-Sentient Entities: These are meant to be entities that do not react to anything around them
 * except to respond to collisions. They do not want or fear or need anything. Nothing interests them.
 *
 *   Lights: Lights are circles that travel around the arena in straight lines unless they collide
 *   with another light or a wall, in which case they reverse in an arc for a time and then head off
 *   in a different direction.
 *
 *   Food: Foods are stationary circles. They do not move, nor do they collide with anything.
 *
 * Sentient Entities: These are the actual BraitenBerg Vehicles. All of them are equipped with 2
 * sets of sensors which are connected to their two wheels. They all respond to lights in some
 * capacity, and after a set number of timesteps in the simulation, they exibit "agressive" behavior
 * towards food blobs. That is to say that their left food sensors control their right wheels (and
 * vice versa), and that sensor proximity to food increases speed.
 *
 *   ExplorerBoi: ExplorerBois are Braitenberg vehicles which exibit "exploratory" behavior. This
 *   means that the left light sensor of the  robot controls the right wheel of the vehicle (and vice
 *   versa) and also that proximity to lights decreases the speed of wheels.
 * 
 *   ScaredyBoi: ScaredyBois are Braitenberg vehicles which exibit "fearful" behavior. This means
 *   that the left light sensor of the  robot controls the left wheel of the vehicle (and vice versa)
 *   and also that proximity to lights increases the speed of wheels.
 *
 * The simulation is designed such that a developer can add new robot types or stimuli (provided they
 * can write the code for it). This is detailed in the Developer Guide.
 *
 * The simulation can be controlled with a gui present in the simulation window. This is detailed in 
 * the User Guide section.
 *
 * --------------------------------------------------------------------------------------------------
 *
 * \section devel_sec Developer Guide
 * Adding A New Stimulus:
 * Adding a stimulus to the simulation is not the easiest task, but it can be done with the right
 * mindset and a little bit of elbow grease. Here is how to do it:
 *
 *
 * --------------------------------------------------------------------------------------------------
 * 
 * Step 1: Define the class of Stimulus
 * The Stimulus will need to be either an ArenaEntity, or an ArenaMobile Entity. Light and Food are
 * excellent examples of the proper class interface declarations, and are shown below (it might be
 * useful to copy and paste them and then alter them to suit your needs):
 *
 * class Light : public ArenaMobileEntity {
 *  public:
 *    **
 *    * @brief Constructor.
 *    *
 *   Light();
 *
 *    **
 *    * @brief Get the name of the Light for visualization purposes, and to
 *    * aid in debugging.
 *    *
 *   std::string get_name() const override {
 *     return "Light" + std::to_string(get_id());
 *   }
 *
 *    **
 *    * @brief Update the Light's position and velocity after the specified
 *    * duration has passed.
 *    *
 *    * @param dt The # of timesteps that have elapsed since the last update.
 *    *
 *    void TimestepUpdate(unsigned int dt) override;
 *
 *    **
 *    * @brief Handles the collision by setting the sensor to activated.
 *    *
 *    void HandleCollision(EntityType object_type, ArenaEntity * object = NULL);
 *
 *    **
 *    * @brief Reset the light using the initialization parameters received
 *    * by the constructor.
 *    *
 *    void Reset() override;
 *
 *
 *    **
 *    * @brief Handles the collision by setting the sensor to activated.
 *    *
 *    void BounceOff();
 *
 *   **
 *   * @brief Getter for motion_handler_
 *   *
 *   MotionHandlerLight get_motion_handler() {return motion_handler_;}
 *   **
 *   * @brief Getter for motion_behavior_
 *   *
 *   MotionBehaviorDifferential get_motion_behavior() {return motion_behavior_;}
 *
 *
 *
 *  private:
 *    **
 *    * @brief An attempt to not overlap any of the newly constructed entities.
 *    *
 *   // Set the pose of lights semi-randomly
 *   Pose SetPoseRandomly();
 *   // Set the radius of the lights semi-randomly
 *   int SetRadiusRandomly();
 *    **
 *    * @brief pointers to the objects used in light motion.
 *    *
 *   // Manages pose and wheel velocities that change with time and collisions.
 *   MotionHandlerLight motion_handler_;
 *   // Calculates changes in pose foodd on elapsed time and wheel velocities.
 *   MotionBehaviorDifferential motion_behavior_;
 *   // how long to back up
 *   int timer_{0};
 * };
 *
 *
 * class Food : public ArenaImmobileEntity {
 *  public:
 *   **
 *   * @brief Constructor.
 *   *
 *   * @param params A food_params passed down from main.cc for the
 *   * initialization of the Food.
 *   *
 *   Food();
 *
 *   **
 *   * @brief Reset the Food using the initialization parameters received
 *   * by the constructor.
 *   *
 *   void Reset() override;
 *
 *   **
 *   * @brief Get the name of the Food for visualization purposes, and to
 *   * aid in debugging.
 *   *
 *   * @return Name of the Food.
 *   *
 *  std::string get_name() const override { return "Food"; }
 *
 *   **
 *   * @brief Getter for captured_, which is the state of the food
 *   *
 *   * @return true if captured.
 *   *
 *   bool IsCaptured() const { return captured_; }
 *
 *   **
 *   * @brief Setter for captured_, which is the state of the food
 *   *
 *   void set_captured(bool state) { captured_ = state; }
 *
 *  private:
 *   **
 *   * @brief An attempt to not overlap any of the newly constructed entities.
 *   *
 *   Pose SetPoseRandomly();
 *
 *
 *   bool captured_;
 * };
 *
 *
 * In general, any new stimulus will need some kind of constructor that sets its position, color, and
 * radius, a reset function that can be called when the arena is reset, and if the stimulus is mobile,
 * a TimestepUpdate() function to update its position in some day with each loop of the simulation. 
 * Follow the ArenaEntity.h or ArenaMobileEntity.h interfaces to most easily pull this off.
 *
 * --------------------------------------------------------------------------------------------------
 * 
 * Step 2: Create a Sensor Class for the New Stimulus
 * The sensor (assuming it is to be attached to at least one of the robots) must be an arena
 * mobile entity, and it would probably be a good idea to give it a vector that is capable of holding
 * pointers to all of the stimuli in the arena it will be affected by. A good example of how to do
 * this is the files light_sensor.h and light_sensor.cc:
 * 
 *
 * class LightSensor : public ArenaMobileEntity {
 *  public:
 *    **
 *    * @brief Constructor.
 *    *
 *   LightSensor();
 *   LightSensor(const LightSensor& other) = delete;
 *   LightSensor operator=(const LightSensor& other) = delete;
 *
 *
 *   LightSensor(const double xpose, const double ypose);
 *
 *    **
 *    * @brief Name to satisfy base class interface
 *    *
 *   std::string get_name() const override {
 *     return "Light Sensor";
 *   }
 *
 *    **
 *    * @brief Reset the LightSensor to a newly constructed state (needed for reset
 *    * button to work in GUI).
 *    *
 *   void Reset() override;
 *
 *    **
 *    * @brief Update the Light_Sensor's position and velocity after the specified
 *    * duration has passed.
 *    *
 *    * @param dt The # of timesteps that have elapsed since the last update.
 *    *
 *   void TimestepUpdate(unsigned int dt) override;  // not implemented
 *
 *    **
 *    * @brief Getter and setter for looking_at_, the pointer to the place were lights reside
 *    *
 *   void SetLookingAt(std::vector<class Light *> light_field) {
 *     for (auto light : light_field) {
 *       looking_at_.push_back(light);
 *     }
 *   }
 *
 *   std::vector<class Light *> GetLookingAt() {return looking_at_;}
 *
 *    **
 *    * @brief Getter for reading_, which scales with proximity to light objects.
 *    *
 *
 *   double get_reading();  // update to scan the list and calculate reading
 *
 *
 *   **
 *   * @brief Getter for motion_behavior_
 *   *
 *   MotionBehaviorDifferential get_motion_behavior() {return motion_behavior_;}
 *
 *
 *  private:
 *   // motion behavior so robots can update their sensor postitions
 *   MotionBehaviorDifferential motion_behavior_;
 *   // vector containing pointers to all the foods
 *   std::vector<class Light *> looking_at_;
 *   unsigned int flag{1};
 * };
 *
 *
 * LightSensor::LightSensor() : motion_behavior_(this),
 *                              looking_at_() {set_color(SENSOR_COLOR);
 *                                             set_radius(5);}
 *
 * LightSensor::LightSensor(const double xpose, const double ypose) :
 *   motion_behavior_(this),
 *   looking_at_() {set_color(SENSOR_COLOR);
 *                  set_position(xpose, ypose); set_radius(5);}
 *
 * void LightSensor::Reset() {  // not implemented (irrelevant)
 *   sensor_touch_->Reset();
 * }
 *
 * void LightSensor::TimestepUpdate(unsigned int dt) {
 *   flag = dt;
 *   // Reset Sensor for next cycle
 *   sensor_touch_->Reset();
 * }
 *
 *
 * double LightSensor::get_reading() {
 *   double reading = 0;
 *   double tempreading = 0;
 *   for (auto ent : looking_at_) {
 *     double distance = std::hypot((get_pose().x - ent->get_pose().x),
 *                                  (get_pose().y - ent->get_pose().y));
 *     tempreading += 1200/(pow(1.08,
 *                        distance-(ent->get_radius())));
 *   }
 *
 *   reading = std::min(tempreading, 1000.0);
 *
 *   return reading;
 * }
 * 
 * Generally, any sensor requires a get_reading() type of function that can report the status of the
 * sensor to a robot that is "listening" to it. It also needs some kind of function to re-assign the
 * stimuli that the sensor is keeping track of. In this case, that function is simply 
 * set_looking_at() in light_sensor.h. It is probably best to follow this formula pretty closely.
 *
 * --------------------------------------------------------------------------------------------------
 * 
 * Step 3: Integrate the new stimulus into the Arena, EntityFactory, Params, EntityType, and any
 * Robots that will be using it.
 *
 * Arena.cc functions as the main driver of the simulation. Any new stimuli will need to be added to 
 * the entities_ vector, and the mobile_entities_ vector if they are mobile. Additionally, for any
 * stimulus that is unique to a type of sensor (like light and food), there must be a vector in
 * arena that holds pointers to every instance of that stimulus. In arena, lights and foods are held
 * in these vectors:
 * 
 *  // All the foods in the arena (so robots know where they are)
 *  std::vector<class Light *> lights_;
 *
 *  // All the lights in the arena (so robots have something to pay attention to)
 *  std::vector<class Food *> foods_;
 *
 * So if a new stimulus is added, an appropriately named new vector to hold it in arena needs to also
 * be added. 
 *
 * Additionally, the arena currently posseses functions in its constructor, and in an overloaded reset
 * method that it uses to add entities to the arena. This is the method that serves that purpose:
 *
 * void Arena::AddEntity(EntityType type, int quantity) {
 *   if (type == kLight) {
 *     for (int i = 0; i < quantity; i++) {
 *       lightHolder_ =
 *       dynamic_cast<Light *>(factory_->CreateEntity(type, lights_, foods_));
 *       lights_.push_back(lightHolder_);
 *       mobile_entities_.push_back(lightHolder_);
 *       entities_.push_back(lightHolder_);
 *     }
 *   } else if ((type == kCoward) || (type == kExplorer)) {
 *     for (int i = 0; i < quantity; i++) {
 *       botPlaceholder_ =
 *       dynamic_cast<ArenaMobileEntity *>(factory_->CreateEntity(type,
 *                                                   lights_, foods_));
 *       mobile_entities_.push_back(botPlaceholder_);
 *       entities_.push_back(botPlaceholder_);
 *     }
 *   } else {
 *     for (int i = 0; i < quantity; i++) {
 *       foodHolder_ =
 *       dynamic_cast<Food *>(factory_->CreateEntity(type, lights_, foods_));
 *       foods_.push_back(foodHolder_);
 *       entities_.push_back(foodHolder_);
 *     }
 *   }
 * }
 *
 * There are two things of importance here. For one, whenever you add an entity to the arena, there
 * will need to be multiple pointers to that object. Thus, there will need to be a temporary varable
 * to hold the pointer while it gets placed in the appropriate vectors. The other important thing to
 * notice is the call to factory_, which is a pointer holding an instance of the class entity_factory.
 *
 * --------------------------------------------------------------------------------------------------
 * 
 * EntityFactory is a class called by arena when it needs to add an entity. If you add a new type of
 * stimulus, you will need to update entity_factory so that it is capable of producing that type of
 * entity. The way to do this is to change the call to create entity so that the stimulus vector
 * created in arena is passed in, add a case to construct the new stimulus, and add lines of code to
 * set sensors on robots to be looking at the vector of stimuli (assuming that was the intent).
 *
 * An example of this is shown below using the Light and ExplorerBoi classes:
 *
 *
 * It may be aparant from this example that sensors are set by making a call to the robot that holds
 * them. This leads in to the next step: Integrating the new sensors into one or more robots.
 *
 * --------------------------------------------------------------------------------------------------
 * 
 * To illustrate how this is typically done, the ExplorerBoi class will be used as an example.
 *
 * Adding a sensor to a robot requires adding one or more pointers to instances of the sensor to the
 * robot that will use them. In explorerboi.h, that looks like this:
 *
 *  LightSensor* left_light_{nullptr};
 *  LightSensor* right_light_{nullptr};
 *
 * And there need to be getters of course.
 * 
 *  LightSensor* get_left_light() {return left_light_;}
 *
 *  LightSensor* get_right_light() {return right_light_;}
 *
 * Now, in the motion handler for that robot, the true integration begins.
 *
 * In motion_handler_explorer.cc, the sensors come into play exclusively in the UpdateVelocity().
 * vleft and vright are set here, so you need to decide how you are going to incorporate the new
 * readings into calculating the velocity. Here is how readngs from LightSensors are incorporated:
 *
 *    vleft = get_max_speed() -
 *                    mag * get_max_speed()/1000.0 * (dynamic_cast<ExplorerBoi*>(entity_)
 *                                           ->get_right_light()->get_reading());
 *
 *    vright = get_max_speed() -
 *                     mag * get_max_speed()/1000.0 * (dynamic_cast<ExplorerBoi*>(entity_)
 *                                           ->get_left_light()->get_reading());
 *
 * If you've completed all these steps and can compile the program without errors, your new stimulus 
 * should be successfully integrated into the simulation.
 *
 * --------------------------------------------------------------------------------------------------
 * Adding A New Robot:
 * Adding a new robot to the simulation is a much more vaguely defined task than adding a stimulus 
 * However, there are still a few guidelines that must be followed.
 *
 * --------------------------------------------------------------------------------------------------
 *
 * The robot itself must be a derived class of ArenaMobileEntity, and must implement its interface.
 * If you are confused about how to do this, a suggestion would be to copy one of the existing robots
 * (such as scaredyboi or explorerboi), rename it, and alter the code within it.
 * 
 * EntityFactory will need to be updated so that it can produce these new robots when called to by
 * arena, and arena will need to be updated to that it creates these new robots. For reference, the 
 * process by which arena adds an explorerboi to itself is detailed below:
 *
 * First, AddEntity is called:
 *
 * AddEntity(kExplorer, 2);
 *
 * Which propagates down through a few functions:
 *
 * void Arena::AddEntity(EntityType type, int quantity) {
 * ...
 * ...
 * ...
 * else if ((type == kCoward) || (type == kExplorer)) {
 *    for (int i = 0; i < quantity; i++) {
 *      botPlaceholder_ =
 *      dynamic_cast<ArenaMobileEntity *>(factory_->CreateEntity(type,
 *                                                  lights_, foods_));
 *      mobile_entities_.push_back(botPlaceholder_);
 *      entities_.push_back(botPlaceholder_);
 *    }
 *  }
 *
 * At this point, EntityFactory gets called:
 *
 * ArenaEntity* EntityFactory::CreateEntity(EntityType etype,
 *                                         std::vector<class Light *> lights,
 *                                         std::vector<class Food *> foods) {
 * ...
 * ...
 * ...
 * ExplorerBoi* EntityFactory::CreateExplorerBoi(
 *  std::vector<class Light *> lights,
 *  std::vector<class Food *> foods) {
 *  auto* guy = new ExplorerBoi;
 *  guy->get_left_light()->SetLookingAt(lights);
 *  guy->get_right_light()->SetLookingAt(lights);
 *  guy->get_left_food()->SetLookingAt(foods);
 *  guy->get_right_food()->SetLookingAt(foods);
 *  ++entity_count_;
 *  ++robot_count_;
 *  guy->set_id(robot_count_);
 *  return guy;
 * }
 *
 * So This means that to create a new robot, you will need to add a similar logic path as this to add
 * it to the arena.
 *
 *
 * Other than that, the specifics of how the robot will work are up to the developer.
 *
 * --------------------------------------------------------------------------------------------------
 * Adding A New Slider:
 *
 * Adding a slider is almost exclusive to GraphicsArenaViewer. It consists of creating the actual
 * graphical object in the window, and then passing that information into the arena. 
 * 
 * --------------------------------------------------------------------------------------------------
 *
 * Adding the actual physical slider to the graphics window is fairly straightforward. In
 * GraphicsArenaViewer, a slider is implemented via a little block of code, like this:
 *
 *  new nanogui::Label(panel, "Number of Robots", "sans-bold");
 *  nanogui::Slider *slider = new nanogui::Slider(panel);
 *  // The starting value (range is from 0 to 1)
 *  // Note that below the displayed value is 10* slider value.
 *  slider->setValue(0.4f);
 *  slider->setFixedWidth(100);
 *
 *  // Display the corresponding value of the slider in this textbox
 *  nanogui::TextBox *textBox = new nanogui::TextBox(panel);
 *  textBox->setFixedSize(nanogui::Vector2i(60, 25));
 *  textBox->setFontSize(20);
 *   textBox->setValue("4");
 *
 *  // This is the lambda function called while the user is moving the slider
 *  slider->setCallback(
 *    [textBox](float value) {
 *      textBox->setValue(std::to_string(std::max(1,int(value*10))));
 *    }
 *  );
 *  // This is the lambda function called once the user is no longer manipulating the slider.
 *  // Note robot_count_ is set, which is a graphics_arena_ variable in this version, although
 *  // you should communicate that value to the controller so that it can configure the Arena.
 *  slider->setFinalCallback(
 *    [&](float value) {
 *      update_= true;
 *      robot_count_ = std::max(1,int(value*10));
 *      reset_button_->setCaption("Update Simulation");
 *    }
 *  );
 *
 * Basically, what is required is that a slider change the numerical value of some member variable in 
 * GraphicsArenaViewer, and that it set update_ to true, so that the user can update the arena.
 *
 * The simulation can only be updated when the user presses the update button. When the user does, the
 * following function is called in GraphicsArenaViewer.
 *
 * void GraphicsArenaViewer::OnResetBtnPressed() {
 *   arena_->Reset();
 *   playing_button_->setCaption("Play");
 *   paused_ = true;
 *   if (update_) {
 *     update_ = false;
 *     arena_->Reset(light_count_, robot_count_,
 *                   food_count_, fearful_count_,
 *                   hunger_, light_sensitivity_);
 *     reset_button_->setCaption("Reset");
 *   }
 *   else {
 *     arena_->Reset();
 *   }
 * }
 *
 * Note that there is an overloaded Reset function in arena that passes in parameters from
 * GraphicsArenaViewer. This overloaded Reset function needs to be somehow changed so that whatever
 * the new slider does affects things in the arena.
 *
 * --------------------------------------------------------------------------------------------------
 *
 * Any other changes to the program are considered to significantly alter the intended purpose of the
 * program, and are thus beyond the scope of this guide.
 *
 * --------------------------------------------------------------------------------------------------
 *
 * \section user_sec User Guide
 *
 * Generating Documentation:
 *
 * All documentation (aside from the uml) is generated via doxygen, if you are reading this document
 * and it is not in doxygen format, then enter the console on your computer, navigate to the docs
 * folder, and type "". If you don't know how to do that, please consult someone familiar with Linux
 * before going any further.
 *
 * --------------------------------------------------------------------------------------------------
 * 
 * Building and Running the Program:
 *
 * In the console, navigate to the /src folder in this project and type "Make clean; Make" This will
 * create an executable file called "arenaviewer". Double click on it to start the simulation.
 *
 * --------------------------------------------------------------------------------------------------
 *
 * Controlling the Simulation:
 *
 * To the left of the arena (a rectangle with colored circles in it) there should be a windowed menu.
 * There are several buttons and sliders on the menu:
 *
 * Play/Pause: start or pause the siulation.
 *
 * Reset/Update Simulation: Reset all robot, light and food positions randomly within the arena. When
 * any of the Arena Configuration options are changed, this button must be pressed to update the
 * simulation with your changes.
 *
 * Hunger ON/OFF: This toggles whether or not the robots in the arena will get hungry and look for
 * food.
 *
 * Number of Robots: This changes the number of total robots in the arena.
 *
 * Number of Fearful Bots: This changes how many robots out of the total robots exibit "fearful" 
 * behavior. Note that if you try and set the number of fearful bots higher than the total number of
 * robots (you can't if you've updated the total number of bots first) the simulation will simply
 * create as many fearful robots as there are total robots, and not more.
 *
 * Number of Food Blobs: This changes how many food blobs are on the screen at once, if hunger is on
 * and there are no food blobs, robots will all starve.
 *
 * Light Sensitivity: This is not an exact setting, but it scales the amount by which robots are
 * affected by lights in the arena. Play around with it a bit and see what it does.
 *
 * --------------------------------------------------------------------------------------------------
 *
 * Thats it! Enjoy!
 *
 */

#endif  // SRC_MAINPAGE_H_
