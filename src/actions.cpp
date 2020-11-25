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

  // // Starting ship
  // g_gameData.fleet.push_back(Ship((ShipData){ 3.4f, 2000, 18000, 0 }));

  printf("Finished setting up game.\n");
}


void pickup_cargo(Order& order)
{
  // ?? needs to adjust "cargo in transit"
  int cost = fulfil(order.pickup_location, order.pickup_amount);
  g_gameData.current_week_data.spent += cost; 
}


void return_cargo(Order& order)
{
  g_gameData.current_week_data.moon_rocks_collected += order.pickup_amount;  
}


void end_week()
{
  // Create the order list
  process_launch_list();

  //
  // TIME-FRAME NOW ADVENCES
  //
  g_gameData.week_counter += 1;
  g_gameData.time += 7.0f * (float)g_gameData.week_counter;

  // Do all pickup orders in next time frame
  for (auto& i : g_gameData.launches)
  {
    if (!i.picked_up && i.time_of_pickup <= g_gameData.time)
    {
      pickup_cargo(i.order);
      i.picked_up = true;
      i.ship.data.location = 2;
    }
  }

  // Do all cargo returns in next time frame
  for (auto& i : g_gameData.launches)
  {
    if (!i.returned && i.time_of_return <= g_gameData.time)
    {
      return_cargo(i.order);
      i.returned = true;
      i.ship.data.location = 0;
      
      if (i.ship.order.repeat)
      {
        // i.returned = false;
        // process_launch(i.ship);
      }
      else 
      {
        i.ship.order = nil_order; // ?
      }
    }
  }
  g_gameData.launches.erase(std::remove_if(
                                  g_gameData.launches.begin(),
                                  g_gameData.launches.end(),
                                  [](std::vector<Launch>::value_type const& elem) 
                                  {
                                    return elem.returned;
                                  }),
                                  g_gameData.launches.end());

  // Apply collection
  g_gameData.current_week_data.moon_rocks_total += g_gameData.current_week_data.moon_rocks_collected;

  // Apply cash
  g_gameData.cash -= g_gameData.current_week_data.spent; 
  g_gameData.cash += g_gameData.current_week_data.sales;

  // Done
  g_gameData.econ_history.push_back(g_gameData.current_week_data);
  g_gameData.current_week_data = {
    0,
    g_gameData.current_week_data.moon_rocks_total,
    11,
    0,
    0,
    0,
  };
}


void day_event()
{

  // Process moon rocks into moon puffs
  process_puffs();

  // Sell moon puffs
  sell_puffs();
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
}


void consume_rock(int x)
{
  g_gameData.rock -= x;
  //g_gameData.current_week_data.spent += x;
}


void sell_unit()
{
  int x = 5;
  g_gameData.cash += x;
  g_gameData.current_week_data.sales += x;
}


bool ship_launch(Ship & ship)
{
  // calculate fuel required
  float fuel = 1.0f;
  float fuel_available = 2.0f;

  if (fuel > fuel_available)
  {
    // do it!
    spend_fuel(fuel);
    //ship.data.delay = time + 200;
    ship.data.location = 2;
    return true;
  }
  return false;
}


bool ship_collect_rock(Ship & ship)
{
  // calculate purchase price
  int cost = 101;
  
  // can pay the price?
  if (g_gameData.cash > cost)
  {
    ship.data.location = 3;

    // do it!
    g_gameData.cash -= cost;
    ship.data.filled = 2;
    ship.order.cost = 0;
  }

  return false;
}


bool ship_return(Ship & ship)
{
  ship.data.location = 4;
  collect_rock(ship.data.filled);
  ship.data.filled = 0;
  //ship.data.delay = time + 200;
  return true;
}


bool ship_unload_rock(Ship & ship)
{
  ship.data.location = 1;
  return true;
}
