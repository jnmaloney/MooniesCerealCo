# pragma once
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
#include "actions.h"

#include "launch_scene.h"
#include "processing_scene.h"
#include "mining_scene.h"
#include "ResourceManager.h"


int fulfil(int location, int amount)
{
  int unit_cost = 10;
  if ((g_gameData.week_counter + 2) % 4 == 0) unit_cost = 5;

  // always succeed
  int given_amount = amount;
  int given_cost = amount * unit_cost;
  return given_cost;

  // if (given_cost > g_gameData.cash)
  // {
  //   given_amount = 0;
  //   given_cost = 0;
  // }
}


int* g_current_ship_slot;
game_globals::Ship* g_current_ship = 0;
void upgrade_ship(int upgrade, int cost)
{
  //g_gameData.current_week_data.spent += cost;
  g_gameData.cash -= cost;

  *g_current_ship_slot = upgrade;

  if (g_current_ship == 0) return;

  if (upgrade == 1)
  {
    g_current_ship->data.value *= 0.9; // ?? launch_cost
  }
  if (upgrade == 2)
  {
    g_current_ship->data.value *= 0.9; // ?? launch_cost
  }
  if (upgrade == 3)
  {
    g_current_ship->data.capacity += 500;
  }
  if (upgrade == 4)
  {
    g_current_ship->data.value *= 0.8; // ?? launch_cost
  }
  if (upgrade == 5)
  {

  }
}


// ??
void process_launch(game_globals::Ship& i)
{
  // removed 'time'
  // g_gameData.launches.push_back(
  //         (game_globals::Launch){ i, i.order, g_gameData.time, g_gameData.time + i.data.transit_time, g_gameData.time + 2 * i.data.transit_time, false, false }
  //       );
  g_gameData.current_week_data.spent += i.data.value; // ?? launch_cost
  i.data.location = 1;  
}


// ??
void process_launch_list()
{
  for (auto& i: g_gameData.fleet)
  {
    if (i.data.location == 0) // is grounded
    {
      if (i.order.pickup_amount > 0)
      {
        process_launch(i);
      }
    }
  }
}

