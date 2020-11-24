#include "game_globals.h"
#include "actions.h"


namespace game_globals {


// extern global instance
static GameData hidden_gamedata;
GameData& g_gameData = hidden_gamedata;

GameData::GameData()  
//   day_data( { DayData() } ),
//   current_day_data( day_data.back() )
{
}

    static float next_day_counter = 1.0f;


void GameData::day_snapshot()
{
  float value_week = (1.0f/7.0f) * ((float)(week_counter * 7) + next_day_counter); // day (week)
  day_data.push_back( DayData() );
  current_day_data = day_data.back();
  plot_a.push_back(value_week);
  plot_data_cursor = (plot_data_cursor + 1) % (6 * 7 + 1);
}


  void GameData::update_timer()
  {

    days += timer_speed * (1.0/30.);
    if (days >= 7.0f)
    {
      day_snapshot();

      days = 0;
      next_day_counter = 1.0f;
      //week_counter += 1;
      day_event();
      end_week(); //a c t i o n
    }
    else if (days >= next_day_counter)
    {
      day_snapshot();
      days = next_day_counter;
      next_day_counter += 1.0f;
      day_event();
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
    timings[i] += 1;
    if (timings[i] >= g_gameData.time)
    {
      sell_puffs(); // sell_unit()
      timings.pop_back();
    }
  }
}


}// namespace