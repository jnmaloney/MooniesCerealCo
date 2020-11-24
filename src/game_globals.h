#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <algorithm>
#include <deque>
#include "imgui.h" //itemflags
#include "imgui_internal.h" //itemflags

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include "game_screens.h"



void upgrade_ship(int, int);


namespace game_globals
{

struct WeekData
{
  int moon_rocks_collected;
  int moon_rocks_total;
  int moon_rock_storage_cap;
  int processing_rate;
  int spent;
  int sales;
};
struct DayData
{
  int day;
  float week;
  int cash_in;
  int cash_out;
  int cash_flow;
  int rock_in;
  int rock_out;
  int rock_flow;
};


struct Order
{
  int pickup_location;
  int pickup_amount;
  int cost;
  bool repeat;
};
static Order nil_order = (Order){ 0, 0, 0 };

struct ShipData
{
  float transit_time;
  int filled;
  int capacity;
  int value;
  int location;
};
struct Ship
{
  Ship(ShipData a_data) : tag(boost::uuids::random_generator()()), data(a_data) {} 
  ShipData data;
  Order order = nil_order;
  boost::uuids::uuid tag;// = boost::uuids::random_generator()();
  int slot1 = 0;
  int slot2 = 0;
  int slot3 = 0;
  int delay; // order time
};
//struct LaunchPad
//{};


struct Conveyor
{
  int stock;
  int type;
  float health;
  std::vector<int> timings;
  void tick();
};
struct ProcessingRoom
{
  std::vector<Conveyor> conveyors;
  void tick();
};


struct Mine
{
  std::string name;
  int rate;
  int stock;
  void tick();
};
struct MoonMining
{
  std::vector<Mine> mines;
  void tick();
};


struct Launch
{
  Launch(const Launch& other) = default;//: ship(other.ship), order(other.order) { }
  //Launch& operator=(const Launch&) = default;
  Launch& operator=(const Launch&) { return *this; };
  //Launch& operator=(const Launch&);
  Ship& ship;
  Order order;
  float time_of_launch;
  float time_of_pickup;
  float time_of_return;
  bool picked_up;
  bool returned;
};
//Launch& Launch::operator=(const Launch&) = default;


static   std::vector<Ship> fleets = {
      Ship((ShipData){ 3.49, 1000, 35000, 0 }),
      Ship((ShipData){ 4.0, 1500, 70000, 0 }),
      Ship((ShipData){ 3.2, 800, 75000, 0}),
      Ship((ShipData){ 9.0, 7500, 900000, 0 }),
    };  




struct SecretFunctions
{


static std::vector<WeekData>&             get_econ_history();
static WeekData&                          get_current_week_data();
static std::vector<Ship>&                 get_fleet();
static std::vector<Launch>&               get_launches();
static int&                               get_week_counter();
static float&                             get_time();
static int&                               get_cash();
static PAGES&                             get_page();

};


class GameData
{
public:
  GameData();

 // Resources
 int                      cash;
 int                      rock;
 float                    fuel;
 WeekData                 current_week_data;
 DayData                 current_day_data;
 std::vector<Ship>        fleet;
 std::vector<Launch>      launches;

 // Factory
 ProcessingRoom           processing;
 MoonMining               mining;
 //

  // Resource history
 std::vector<WeekData>    econ_history;

 // Timer
 int                      week_counter = 0;
 float                    time = 0.f;
 // new
 float                    days = 0.f;
 float                    timer_speed = 1.0f;

 // Plots data
 std::vector<DayData> day_data;
 std::vector<float> plot_a; 
//  std::vector<float> plot_a_x; 
//  std::vector<float> plot_a_y; 
//  std::vector<float> plot_b_x; 
//  std::vector<float> plot_b_y; 
 int plot_data_cursor = 0;

 // Navigation
 PAGES                    page = MainMenu;

  void day_snapshot();
  void update_timer();

};

} // namespace


extern game_globals::GameData& g_gameData;

// std::vector<WeekData>&    g_econ_history          = SecretFunctions::get_econ_history();
// WeekData&                 g_current_week_data     = SecretFunctions::get_current_week_data();
// std::vector<Ship>&        g_fleet                 = SecretFunctions::get_fleet();
// std::vector<Launch>&      g_launches              = SecretFunctions::get_launches();

// int&                      g_week_counter          = SecretFunctions::get_week_counter();
// float&                    g_time                  = SecretFunctions::get_time();

// int&                      g_cash                  = SecretFunctions::get_cash();

// PAGES&                    g_page                  = SecretFunctions::get_page();

// }
