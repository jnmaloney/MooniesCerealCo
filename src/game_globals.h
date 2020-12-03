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
#include "dialog_manager.h"


inline DialogManager g_dialogManager;



void upgrade_ship(int, int);


namespace game_globals
{

struct WeekData
{
  int week;
  int moon_rocks_collected;
  int moon_rocks_total;
  int moon_rock_storage_cap;
  int processing_rate;
  int spent;
  int sales;
  int cash;
};
struct DayData
{
  float day = 0;
  float week = 0;
  float cash_in = 0;
  float cash_out = 0;
  float cash_flow = 0;
  float cash_total = 0;
  float rock_in = 0;
  float rock_out = 0;
  float rock_flow = 0;
  float rock_total = 0;
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

  // Upgrades
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
  float scroll;
  int process_timer = 0; 
  int processing_time = 232;
  int conveying_time = 600;
  std::deque<int> timings;
  bool upgraded;
  void tick();
};
struct ProcessingRoom
{
  std::vector<
    std::vector<Conveyor> 
      > conveyors;
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
      Ship((ShipData){ 3.49, 80, 4000, 0 }),
      Ship((ShipData){ 4.0, 110, 5000, 0 }),
      Ship((ShipData){ 3.8, 320, 13000, 0 }),
      Ship((ShipData){ 3.0, 200, 25000, 0}),
      Ship((ShipData){ 9.0, 800, 40000, 0 }),
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


class JoesMine
{
  int rock_taken;
  void tick();
};


class KimsMine
{
  bool open_kims_mine = false;
  int next_invoice_tick;
  void tick();
};


class BobsMine
{
  bool open_bobs_mine = false;
  int next_invoice_tick;
  void tick();
};


class MooniesMine
{

};


class YourMine
{
  int processing;
  int power;
  int storage;
  void tick();
  int rocks_available;
};


class GameData
{
public:
  GameData();

 // Resources
 int                      cash = 150;
 int                      rock = 0;
 float                    fuel;
 WeekData                 current_week_data;
 std::vector<Ship>        fleet;
 std::vector<Launch>      launches;

 // Factory
 ProcessingRoom           processing;
 MoonMining               mining;
 //

  // Resource history
 std::deque<WeekData>    econ_history;

 // Timer
 int                      week_counter = 0;
 int                      time_tick = 0;
 //float                    time = 0.f;
 // new
 float                    days = 0.f;
 int                      timer_speed = 1;

 // Plots data
 std::vector<DayData>     day_data;
 //DayData                  dummy_day_data;
 //DayData&                 current_day_data;
//  std::vector<float> plot_a_x; 
//  std::vector<float> plot_a_y; 
//  std::vector<float> plot_b_x; 
//  std::vector<float> plot_b_y; 
 int plot_data_cursor = 0;

 // Navigation
 PAGES                    page = MainMenu;

  void day_snapshot();
  void update_timer();

  JoesMine joesMine;
  KimsMine kimsMine;
  BobsMine bobsMine;
  MooniesMine mooniesMine;
  YourMine yourMine;

  int science_level = 0;
  bool science_unlock = false;
  int next_science_level;

  int current_floor = 0;
  bool mine_open;
  bool mine_miner;
  bool mine_power;
  bool mine_silo;
  bool tax_bill = true;
  bool game_over;
};


struct GameDialogs
{
  bool intro;
  bool first_econ;
  bool first_launchpad;
  bool first_processing;
  bool first_mining;
  bool first_bills;
  bool first_science;
  bool production_broken;
  bool production_broken_again;
  bool threshold_1;
  bool threshold_2;
  void tick();
};


} // namespace


inline game_globals::GameDialogs g_gameDialogs;
inline game_globals::GameData g_gameData = game_globals::GameData();

inline int* g_current_ship_slot;
inline game_globals::Ship* g_current_ship = 0;

// std::vector<WeekData>&    g_econ_history          = SecretFunctions::get_econ_history();
// WeekData&                 g_current_week_data     = SecretFunctions::get_current_week_data();
// std::vector<Ship>&        g_fleet                 = SecretFunctions::get_fleet();
// std::vector<Launch>&      g_launches              = SecretFunctions::get_launches();

// int&                      g_week_counter          = SecretFunctions::get_week_counter();
// float&                    g_time                  = SecretFunctions::get_time();

// int&                      g_cash                  = SecretFunctions::get_cash();

// PAGES&                    g_page                  = SecretFunctions::get_page();

// }
