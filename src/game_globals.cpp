#include "game_globals.h"
#include "actions.h"
#include <algorithm>


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


namespace game_globals {


// extern global instance
// static GameData hidden_gamedata = GameData();
// GameData& g_gameData = hidden_gamedata;

GameData::GameData() :
  day_data( { DayData() } )
  //current_day_data( dummy_day_data )
{
  //printf("init'ing GameData: current_day_data  %i\n", current_day_data.cash_flow);
}

    static float next_day_counter = 1.0f;


void GameData::day_snapshot()
{
  int size = (6 * 7 + 1);
  float value_week = (1.0f/7.0f) * ((float)(week_counter * 7) + next_day_counter); // day (week)

  if (day_data.size() < size)
  {
    day_data.push_back( DayData() );
    plot_data_cursor = day_data.size() - 1;
  }
  else
  {
    plot_data_cursor = (plot_data_cursor + 1) % size;
    day_data[plot_data_cursor] = DayData();
  }
  day_data[plot_data_cursor].week = value_week;
  day_data[plot_data_cursor].cash_total = cash;
  day_data[plot_data_cursor].rock_total = rock;
}


void GameData::update_timer()
{
  // Global frame counter
  time_tick += 1;

  // Stats update
  day_data[plot_data_cursor].cash_total = cash;
  day_data[plot_data_cursor].rock_total = rock;
  
  days += timer_speed * (1.0/30.);
  if (days >= 7.0f)
  {
    day_snapshot();

    days = 0;
    next_day_counter = 1.0f;
    end_week(); //a c t i o n
  }
  else if (days >= next_day_counter)
  {
    day_snapshot();
    days = next_day_counter;
    next_day_counter += 1.0f;
  }

  // Update fleets... (new)
  for (auto& i : g_gameData.fleet)
  {
    if (g_gameData.time_tick >= i.delay)
    {
      if (i.data.location == 1) ship_launch(i);
      else if (i.data.location == 2) ship_collect_rock(i);
      else if (i.data.location == 3) ship_return(i);
      else if (i.data.location == 4) ship_unload_rock(i);
      else
      {
        // printf("launching a nyway\n");
        // ship_launch(i);
      }
    }

  }
}


void MoonMining::tick()
{
  for (auto& i : mines) i.tick();
}


void Mine::tick()
{
  // Add rock to stock
}


void ProcessingRoom::tick()
{
  for (auto& i : conveyors) i.tick();
}


void Conveyor::tick()
{
  //
  // Processing...
  //
  for (int i = 0; i < timings.size(); ++i)
  {
    if (g_gameData.time_tick >= timings[i])
    {
      sell_unit();
      timings.pop_front();
    }
  }

  //
  // Damage...
  //
  if (health <= 0)
  {
    return;
  }
  else
  {
    float ticks_per_day = 600.f;
    float tick_lifetime = ticks_per_day * 7.0f * 5.0f;
    health -= 1.0f / tick_lifetime;
  }
  
  //
  // Timing for production
  //
  if (process_timer >= processing_time)
  {
    // Stock for prodution
    if (g_gameData.rock > 5)
    {
      consume_rock(5);
      process_timer = 0;
      timings.push_back(g_gameData.time_tick + conveying_time);
    }
  }
  else
  {
    process_timer += 1;
  }  
}


}// namespace