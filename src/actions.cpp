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