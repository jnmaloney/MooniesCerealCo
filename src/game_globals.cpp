#include "game_globals.h"
#include "actions.h"


// extern global instance
static GameData hidden_gamedata;
GameData& g_gameData = hidden_gamedata;

// struct GameInstance
// {
// public:
//   std::vector<WeekData>    g_econ_history;
//   WeekData                 g_current_week_data;
//   std::vector<Ship>        g_fleet;
//   std::vector<Launch>      g_launches;

//   int                      g_week_counter = 0;
//   float                    g_time = 0;

//   int                      g_cash = 39000;;

//   PAGES                    g_page = Home;
// };

// static GameInstance g_instance;




//   std::vector<WeekData>&             SecretFunctions::get_econ_history()         { return g_instance.g_econ_history; }
//   WeekData&                          SecretFunctions::get_current_week_data()    { return g_instance.g_current_week_data; }
//   std::vector<Ship>&                 SecretFunctions::get_fleet()                { return g_instance.g_fleet; }
//   std::vector<Launch>&               SecretFunctions::get_launches()             { return g_instance.g_launches; }
//   int&                               SecretFunctions::get_week_counter()         { return g_instance.g_week_counter; } 
//   float&                             SecretFunctions::get_time()                 { return g_instance.g_time; }
//   int&                               SecretFunctions::get_cash()                 { return g_instance.g_cash; }
//   PAGES&                             SecretFunctions::get_page()                 { return g_instance.g_page; }

    static float next_day_counter = 1.0f;

void GameData::day_snapshot()
  {
    float value_a_x = cash; // income
    float value_a_y = (1.0f/7.0f) * ((float)(week_counter * 7) + next_day_counter); // day (week)
    float value_b_x = current_week_data.moon_rocks_total; // product
    float value_b_y = (1.0f/7.0f) * ((float)(week_counter * 7) + next_day_counter); // day

    //printf("%.1f, %i, %.1f, %.1f %i\n", value_a_y , week_counter, days, next_day_counter, plot_data_cursor);

    plot_data_cursor = (plot_data_cursor + 1) % (6 * 7 + 1);

    if (plot_a_x.size() == 0)
    {
      plot_a_x.push_back(value_a_x);
      plot_a_y.push_back(value_a_y);
      plot_b_x.push_back(value_b_x);
      plot_b_y.push_back(value_b_y);  
    }

    if (plot_a_x.size() <= plot_data_cursor)
    {
      plot_a_x.push_back(value_a_x);
      plot_a_y.push_back(value_a_y);
      plot_b_x.push_back(value_b_x);
      plot_b_y.push_back(value_b_y);
    }
    else
    {
      plot_a_x[plot_data_cursor] = value_a_x;
      plot_a_y[plot_data_cursor] = value_a_y;
      plot_b_x[plot_data_cursor] = value_b_x;
      plot_b_y[plot_data_cursor] = value_b_y;
    }
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