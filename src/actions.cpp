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
  g_gameData.processing.conveyors.push_back(std::vector<Conveyor>()); // floor 1
  g_gameData.processing.conveyors[0].push_back((Conveyor){ 0, 1, 1.0f });
  g_gameData.processing.conveyors[0].push_back((Conveyor){ 0, 1, 0.0f });

  // Starting ship
  g_gameData.fleet.push_back(Ship((ShipData){ 3.4f, 80, 12800, 0 }));

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

  // gameover
  if (g_gameData.week_counter == 52 && g_gameData.tax_bill)
  {
    g_dialogManager.dialog.load("/data/game_lose.dialog");
    g_gameData.page = Card;
    g_gameData.game_over = true;
  }

  // mine bonus
  if ( g_gameData.mine_miner &&  g_gameData.mine_power &&  g_gameData.mine_silo )
  {
    int x = 50000;
    g_gameData.cash += x;
    g_gameData.current_week_data.sales += x;
    g_gameData.day_data[g_gameData.plot_data_cursor].cash_flow += x;
    g_gameData.day_data[g_gameData.plot_data_cursor].cash_in += x; 
  }
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
  int x = 20 + 10 * g_gameData.science_level;
  if (g_gameData.week_counter % 4 == 3) x *= 3; // full moon
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
    float delay = ship.data.transit_time * 16.f * 30.f;
    ship.delay = g_gameData.time_tick + delay;
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
  float delay = ship.data.transit_time * 16.f * 30.f;
  ship.delay = g_gameData.time_tick + (int)delay;
  return true;
}


bool ship_unload_rock(Ship & ship)
{
  if (g_gameData.rock + ship.data.capacity <= 2000)
  {
    ship.data.location = 1;
    collect_rock(ship.data.capacity);
    ship.delay = g_gameData.time_tick + 30;
  }
  return true;
}


void buy_new_machine(int current_floor)
{
  // Spending
  spend_cash(1800);

  // Creating
  g_gameData.processing.conveyors[current_floor].push_back((Conveyor){ 0, 1, 1.0f });
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


void buy_new_science(int cost)
{
  spend_cash(cost);
  g_gameData.science_unlock = true;
  g_gameData.next_science_level = g_gameData.time_tick + 16 * 30 * 7;
}


void upgrade_ship(int upgrade, int cost)
{
  //g_gameData.current_week_data.spent += cost;
  if (g_gameData.cash < cost) return;
  spend_cash(cost);

  *g_current_ship_slot = upgrade;

  if (g_current_ship == 0) return;

  if (upgrade == 1)
  {
    g_current_ship->data.transit_time *= 0.9;
  }
  if (upgrade == 2)
  {
    g_current_ship->data.transit_time *= 0.9; // ?? launch_cost
  }
  if (upgrade == 3)
  {
    g_current_ship->data.capacity += 50;
  }
  if (upgrade == 4)
  {
    g_current_ship->data.transit_time *= 0.8; // ?? launch_cost
  }
  if (upgrade == 5)
  {

  }
}
