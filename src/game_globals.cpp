#include "game_globals.h"
#include "actions.h"
#include <algorithm>



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

  // science updata
  if (science_unlock && time_tick >= next_science_level)
  {
    science_unlock = false;
    science_level += 1;
  }

  // Stats update
  day_data[plot_data_cursor].cash_total = cash;
  day_data[plot_data_cursor].rock_total = rock;
  
  //days += timer_speed * (1.0/30.);
  days += (1.0/16.0) * (1.0/30.);   // Slow
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
        if (i.order.pickup_location)
          ship_launch(i);
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
  for (auto& floor : conveyors)
    for (auto& i : floor) i.tick();
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
  else if (timings.size())
  {
    float ticks_per_day = 600.f;
    float tick_lifetime = ticks_per_day * 7.0f * 5.0f;
    if (upgraded) tick_lifetime *= 3;
    health -= 1.0f / tick_lifetime;
  }
  
  //
  // Timing for production
  //
  float factor = 1.0;
  if (upgraded) factor = 2.2f;
  if (process_timer * factor >= processing_time)
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


void GameDialogs::tick()
{
  // What events need to happen?
  // bool intro;
  // bool first_econ;
  if (first_econ == false && g_gameData.page == Econ)
  {
    first_econ = true;
    g_dialogManager.dialog.load("/data/first_econ.dialog");    
  }
  // bool first_launchpad;
  if (first_launchpad == false && g_gameData.page == Launchpad)
  {
    first_launchpad = true;
    g_dialogManager.dialog.load("/data/first_launchpad.dialog");    
  }
  // bool first_processing;
  if (first_processing == false && g_gameData.page == Processing)
  {
    first_processing = true;
    g_dialogManager.dialog.load("/data/first_processing.dialog");    
  }
  // bool first_mining;
  if (first_mining == false && g_gameData.page == Mining)
  {
    first_mining = true;
    g_dialogManager.dialog.load("/data/first_mining.dialog");    
  }
  // bool first_bills;
  // bool first_science;
  // bool production_broken;
  bool all_broken = true;
  for (auto& floor: g_gameData.processing.conveyors)
  {
    for (auto& i: floor)
    {
      if (i.health > 0)
      {
        all_broken = false;
        break;
      }
    }
    if (all_broken == false) break;
  }
  if (all_broken)
  {
    if (production_broken == false)
    {
      g_dialogManager.dialog.load("/data/all_broken.dialog");
      production_broken = true;
    }
    else if (production_broken_again == false)
    {
      g_dialogManager.dialog.load("/data/all_broken_again.dialog");
      production_broken_again = true;
    }
    
  }
  // bool threshold_1;
  if (threshold_1 == false && (g_gameData.cash > 10000 || g_gameData.week_counter >= 20))
  {
    threshold_1 = true;
    // New floor
    g_gameData.processing.conveyors.push_back( std::vector<Conveyor>() );
    // Dialog
    g_dialogManager.dialog.load("/data/threshold_1.dialog");
  }
  // bool threshold_2;
  if (threshold_2 == false && (g_gameData.cash > 100000 || g_gameData.week_counter >= 39))
  {
    threshold_2 = true;
    // Open mine
    g_gameData.mine_open = true;
    // Dialog
    g_dialogManager.dialog.load("/data/threshold_2.dialog");
  }
}


}// namespace