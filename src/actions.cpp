#include "actions.h"
#include "game_globals.h"
#include "system_globals.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <algorithm>
//#include <format>
#include "imgui_internal.h" //itemflags

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

#include "game_screens.h"


using namespace game_globals;


void set_up_game()
{
  printf("Setting up game...\n");

  // processing room
  g_gameData.processing.conveyors.push_back((Conveyor){ 0, 1, 1.0f });
  g_gameData.processing.conveyors.push_back((Conveyor){ 0, 1, 0.0f });
  //g_gameData.processing.conveyors.push_back(Conveyor());

  // // Starting ship
  g_gameData.fleet.push_back(Ship((ShipData){ 3.4f, 800, 12800, 0 }));

  printf("Finished setting up game.\n");
}


void end_week()
{
  //
  // TIME-FRAME NOW ADVENCES
  //
  g_gameData.week_counter += 1;
  //g_gameData.time += 7.0f * (float)g_gameData.week_counter;

  // Apply collection
  g_gameData.current_week_data.moon_rocks_total += g_gameData.current_week_data.moon_rocks_collected;

  // Add history values  
  g_gameData.econ_history.push_back(g_gameData.current_week_data);
  if (g_gameData.econ_history.size() > 5) g_gameData.econ_history.pop_front();

  g_gameData.current_week_data = {
    g_gameData.week_counter,
    0,
    g_gameData.current_week_data.moon_rocks_total,
    11,
    0,
    0,
    0,   
    g_gameData.cash 
  };
}


void spend_cash(int x)
{
  g_gameData.cash -= x;
  g_gameData.current_week_data.spent += x;
}


void spend_fuel(float x)
{
  g_gameData.fuel -= x;
  //g_gameData.current_week_data.spent += x;
}


void collect_rock(int x)
{
  g_gameData.rock += x;
  g_gameData.current_week_data.moon_rocks_collected += x;
  g_gameData.day_data[g_gameData.plot_data_cursor].rock_flow += x;
  g_gameData.day_data[g_gameData.plot_data_cursor].rock_in += x;
}


void consume_rock(int x)
{
  g_gameData.rock -= x;
  //g_gameData.current_week_data.spent += x;
  g_gameData.day_data[g_gameData.plot_data_cursor].rock_flow -= x;
  g_gameData.day_data[g_gameData.plot_data_cursor].rock_out += x;
}


void sell_unit()
{
  int x = 5;
  g_gameData.cash += x;
  g_gameData.current_week_data.sales += x;
  g_gameData.day_data[g_gameData.plot_data_cursor].cash_flow += x;
  g_gameData.day_data[g_gameData.plot_data_cursor].cash_in += x;
}


bool ship_launch(Ship & ship)
{
  // calculate fuel required
  float fuel = 1.0f;
  float fuel_available = 2.0f;

  if (fuel <= fuel_available)
  {
    // do it!
    spend_fuel(fuel);
    //ship.data.delay = time + 200;
    ship.delay = g_gameData.time_tick + 220;
    ship.data.location = 2;
    return true;
  }
  return false;
}


bool ship_collect_rock(Ship & ship)
{
  // calculate purchase price
  int cost = 1;// 101;
  
  // can pay the price?
  if (g_gameData.cash > cost)
  {
    printf("collecting\n");
    ship.data.location = 3;

    // do it!
    spend_cash(cost);
    // ship.data.filled = 2;
    // ship.order.cost = 0;

    ship.delay = g_gameData.time_tick + 20;
  }

  return false;
}


bool ship_return(Ship & ship)
{
  ship.data.location = 4;
  //ship.data.filled = 0;
  //ship.data.delay = time + 200;
  ship.delay = g_gameData.time_tick + 220;
  return true;
}


bool ship_unload_rock(Ship & ship)
{
  ship.data.location = 1;
  printf("unloading\n");
  collect_rock(ship.data.capacity);
  ship.delay = g_gameData.time_tick + 30;
  return true;
}


void buy_new_machine()
{
  // Spending

  // Creating
  g_gameData.processing.conveyors.push_back((Conveyor){ 0, 1, 1.0f });

}


bool s_show_credits = false;
void show_game_credits()
{
  s_show_credits = true;
}


void hide_game_credits()
{
  s_show_credits = false;
}


bool showing_game_credits()
{
  return s_show_credits;
}
