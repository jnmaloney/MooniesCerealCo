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
  //printf("current_day_data  %i\n", current_day_data.cash_flow);
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

  // for (int i = 0; i < 8; ++i)
  // {
  //   for (int j = 0; j < (int)day_data.size(); ++j)
  //   {
  //     float g = ((float*)day_data.data())[i + j * sizeof(DayData) / sizeof(float)];
  //     printf("%.1f\t", g);
  //   }
  //   printf("\n");
  // }
  // printf("\n");
  
  //printf("day snp %i,  %f\n", day_data.size(), value_week);
}


void GameData::update_timer()
{
  // Global frame counter
  time_tick += 1;

  // Stats update
  day_data[plot_data_cursor].cash_total = cash;
  day_data[plot_data_cursor].rock_total = rock;
  
  // printf("update_timer\n");
  // printf("days: %f\n", days);
  // printf("timer_speed: %f\n", timer_speed);
  days += timer_speed * (1.0/30.);
  // printf("days: %f\n", days);
  if (days >= 7.0f)
  {
    day_snapshot();

    days = 0;
    next_day_counter = 1.0f;
    //week_counter += 1;
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
  for (int i = 0; i < timings.size(); ++i)
  {
    if (g_gameData.time_tick >= timings[i])
    {
      sell_unit();
      timings.pop_front();
    }
  }

  // Timing for production
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