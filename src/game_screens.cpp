#include "game_screens.h"
#include "game_globals.h"
#include "system_globals.h"
#include "imgui_internal.h" //itemflags
#include "implot.h"
#include "palette.h"
//#include "launch_scene.h"
#include "actions.h"
#ifdef __EMSCRIPTEN__ // game credits
#include <emscripten.h>
#include <emscripten/html5.h>
#endif

using namespace game_globals;


EM_JS(void, open_tab_url, (const char* str), {
  //console.log('hello ' + UTF8ToString(str));
  var win = window.open(UTF8ToString(str), '_blank');
  //win.focus(); <- crashy?
});


void draw_game_credits()
{
  // Fade game screen
  // const ImU32 dim_bg_col = ImColor(1.f, 1.f, 1.f, 0.25f);
  // ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(0, 0), ImVec2(g_windowManager.width, g_windowManager.height), dim_bg_col);

  ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 12.f);
  //ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.25, 0.25, 0.25, 1));

  // Window
  int w = 520;
  int h = 300;
  ImGui::SetNextWindowPos(ImVec2(
     0.5 * (g_windowManager.width - w), 
     0.5 * (g_windowManager.height - h) ));
  //if (ImGui::BeginChild("Made by##credits", ImVec2(w, h), true, ImGuiWindowFlags_None))
  bool a = true;
  ImGui::SetNextWindowSize(ImVec2(w, h));
  //if (ImGui::Begin("Made by##credits", &a, ImGuiWindowFlags_NoDecoration))
  if (ImGui::BeginPopupModal("Made by##credits", &a, ImGuiWindowFlags_NoDecoration))
  {
    ImGui::Separator();
    ImGui::Text("Created for Game Off 2020 by");
    ImGui::Separator();

    ImGui::SetCursorPosY(125); 

    ImGui::AlignTextToFramePadding();
    ImGui::Text("Programming ");
    ImGui::SameLine();
    ImGui::SetCursorPosX(w - 305); 
    if (ImGui::Button("meatpudding", ImVec2(295, 0)))
    {
      open_tab_url("https://jnmaloney.itch.io/");
    }
    ImGui::AlignTextToFramePadding();
    ImGui::Text("Art ");
    ImGui::SameLine();
    ImGui::SetCursorPosX(w - 305); 
    if (ImGui::Button("cokyfish", ImVec2(295, 0)))
    {
      open_tab_url("https://cokyfish.itch.io/");
    }
    ImGui::AlignTextToFramePadding();
    ImGui::Text("Music");
    ImGui::SameLine();
    ImGui::SetCursorPosX(w - 305); 
    if (ImGui::Button("mit-mit", ImVec2(295, 0)))
    {
      open_tab_url("https://soundcloud.com/user-349094787");
    }
    ImGui::EndPopup();
    //ImGui::End();
    //ImGui::EndChild();
  }
  ImGui::OpenPopup("Made by##credits");

  ImGui::PopStyleVar();
  //ImGui::PopStyleColor();

  if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) | ImGui::IsMouseClicked(ImGuiMouseButton_Right))
  {
    //if (!ImGui::IsAnyWindowHovered())
    if (!ImGui::IsAnyItemHovered())
    {
      hide_game_credits();
    }
  }
}


  static bool inited = false;
void draw_dialog()
{
  //
  // Check global game events and attach dialog stems as necessary
  //

  // Introduction
  if (!inited) 
  {
    g_dialogManager.init();
    Dialog d;
    d.load("/data/intro.dialog");
    g_dialogManager.dialog = d;
    inited = true;
  }

  // First view (production)

  // First view (production, no rock)

  // First view (production, with rock)

  // First view (launchpad)

  // First view (mining)

  // First view (Econ)

  // First view (Home, bills)

  // Run out of fuel

  // Production all broken

  // Mining event needs power

  // Mining event needs resource

  // Mining event needs storage

  // Cash threshold event 1.  (Add floors...) ($10,000)

  // Cash threshold event 2. (Intro to mining...) ($100,000)

  //
  // Draw the dialog actually
  //
  g_dialogManager.draw();
}


bool no_dialog()
{
  return g_dialogManager.dialog.talkEvents.size() == 0;
}


void drawHeaderBar()
{
  ImGui::GetWindowDrawList()->AddRectFilled(
    ImVec2(0, 0), 
    ImVec2(g_windowManager.width, 62), 
    IM_COL32(0x43, 0x47, 0xA0, 255),//g_palette["indigo_600"], 
    0.0f,
    ImDrawCornerFlags_None);
  ImGui::GetWindowDrawList()->AddLine(
    ImVec2(0, 66), 
    ImVec2(g_windowManager.width, 64), 
    IM_COL32(0x2E, 0x32, 0x7D, 255),//g_palette["indigo_800"], 
    8.0f);

  ImGui::BeginChild(
    "Header", 
    ImVec2(0, 68), 
    true, 
    ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDecoration);   
  
  int cursor_pos_y = 8;
  ImGui::SetCursorPosX(12);
  ImGui::SetCursorPosY(cursor_pos_y);

  ImGui::AlignTextToFramePadding();
  ImGui::Text("$%i", g_gameData.cash);

  ImGui::SameLine();
  ImGui::SetCursorPosX(g_windowManager.width / 2 - 140);
  //ImGui::SetCursorPosY(cursor_pos_y);
  ImGui::Text("Rocks %i/%i", g_gameData.rock, 2000); //g_current_week_data.moon_rock_storage_cap);

  // Moonies icon
  ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
  ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0, 0, 0, 0));
  ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0, 0, 0, 0));
  ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0, 0, 0, 0));

  const char* label = "Moonies Cereal Co";
  ImGui::SameLine();
  ImGui::SetCursorPosX(g_windowManager.width - ImGui::CalcTextSize(label).x - 36);
  //ImGui::SetCursorPosX(g_windowManager.width - ImGui::CalcTextSize(label).x - 12);
  //ImGui::SetCursorPosY(cursor_pos_y);
  if (ImGui::Button(label))
  {
    show_game_credits();
  }

  ImGui::PopStyleColor(4);

  ImGui::EndChild();
}


void drawFooterBar()
{
  ImGuiIO& io = ImGui::GetIO();
  ImGui::SetNextWindowPos(ImVec2(0, g_windowManager.height - 84));
  ImGui::BeginChild(
    "Footer", 
    ImVec2(0, 89), 
    true, 
    ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDecoration);   
  
  //
  // Locations
  //
  if (g_gameData.page == Home) im_disable_buttons();
  if (ImGui::Button("Home")) g_gameData.page = Home;
  if (im_is_button_disabled()) im_enable_buttons();
  ImGui::SameLine();

  if (g_gameData.page == Econ) im_disable_buttons();
  if (ImGui::Button("Econ")) g_gameData.page = Econ;
  if (im_is_button_disabled()) im_enable_buttons();
  ImGui::SameLine();

  if (g_gameData.page == Processing) im_disable_buttons();
  if (ImGui::Button("Processing")) g_gameData.page = Processing;
  if (im_is_button_disabled()) im_enable_buttons();
  ImGui::SameLine();

  if (g_gameData.page == Launchpad) im_disable_buttons();
  int i = 0;
  for (auto& j : g_gameData.fleet) if (j.data.location == 0) ++i;
  std::string name = std::string("(") + std::to_string(i) + std::string(") Launchpad");
  if (ImGui::Button(name.c_str())) g_gameData.page = Launchpad;
  if (im_is_button_disabled()) im_enable_buttons();
  ImGui::SameLine();

  if (g_gameData.page == Mining) im_disable_buttons();
  if (ImGui::Button("Mining")) g_gameData.page = Mining;
  if (im_is_button_disabled()) im_enable_buttons();

  //
  // Timers
  //
  bool dis = false;
  ImGui::SetCursorPosX(g_windowManager.width - 150);
  ImGui::SetCursorPosY(0);
  ImGui::Text("Week %i", g_gameData.week_counter + 1);

  ImGui::SetCursorPosX(g_windowManager.width - 150);
  ImGui::ProgressBar(g_gameData.days / 7.0f, ImVec2(-1, 26), "");

  ImGui::SetCursorPosX(g_windowManager.width - 270);
  ImGui::SetCursorPosY(6);
  ImGui::PushFont(io.Fonts->Fonts[2]);
  dis = g_gameData.timer_speed == 0;
  if (dis) im_disable_buttons();
  if (ImGui::Button("||", ImVec2(110, 32)))
  {
    // Pause
    g_gameData.timer_speed = 0;
  }
  if (dis) im_enable_buttons();
  ImGui::PopFont();
  if (ImGui::IsItemHovered())
  {
      ImGui::BeginTooltip();
      //ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
      ImGui::TextUnformatted("Pause");
      //ImGui::PopTextWrapPos();
      ImGui::EndTooltip();
  }

  ImGui::SetCursorPosX(g_windowManager.width - 270);
  ImGui::SetCursorPosY(44);
  dis = g_gameData.timer_speed == 1;
  if (dis) im_disable_buttons();
  if (ImGui::Button("##speed1", ImVec2(32, 32)))
  {
    g_gameData.timer_speed = 1;
  }
  if (dis) im_enable_buttons();
  if (ImGui::IsItemHovered())
  {
      ImGui::BeginTooltip();
      //ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
      ImGui::TextUnformatted("Slow");
      //ImGui::PopTextWrapPos();
      ImGui::EndTooltip();
  }

    ImGui::SetCursorPosX(g_windowManager.width - 230);
  ImGui::SetCursorPosY(44);
  dis = g_gameData.timer_speed == 3;
  if (dis) im_disable_buttons();
  if (ImGui::Button("##speed2", ImVec2(32, 32)))
  {
    g_gameData.timer_speed = 3;
  }
  if (dis) im_enable_buttons();
  if (ImGui::IsItemHovered())
  {
      ImGui::BeginTooltip();
      //ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
      ImGui::TextUnformatted("Fast");
      //ImGui::PopTextWrapPos();
      ImGui::EndTooltip();
  }

    ImGui::SetCursorPosX(g_windowManager.width - 190);
  ImGui::SetCursorPosY(44);
  dis = g_gameData.timer_speed == 6;
  if (dis) im_disable_buttons();
  if (ImGui::Button("##speed3", ImVec2(32, 32)))
  {
    g_gameData.timer_speed = 6;
  }
  if (dis) im_enable_buttons();
  if (ImGui::IsItemHovered())
  {
      ImGui::BeginTooltip();
      //ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
      ImGui::TextUnformatted("Fastest");
      //ImGui::PopTextWrapPos();
      ImGui::EndTooltip();
  }

  ImGui::EndChild();
}


void drawBackBar(PAGES page)
{
  ImGui::SetNextWindowPos(ImVec2(0, g_windowManager.height - 84));
  ImGui::BeginChild(
    "Footer", 
    ImVec2(0, 84), 
    true, 
    ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDecoration);   
  
  if (ImGui::Button("Back")) { g_gameData.page = page; }

  ImGui::EndChild();
}


void econ_page()
{  
  ImGuiIO& io = ImGui::GetIO();
  ImGui::SetCursorPosY(0);
  ImGui::PushFont(io.Fonts->Fonts[2]);
  ImGui::TextUnformatted("week");
      ImGui::Text("");
      ImGui::Text("");
      ImGui::Text("");
  ImGui::TextUnformatted("moon rocks collected");
      ImGui::Text("");
      ImGui::Text("");
      ImGui::Text("");
  ImGui::Text("moon rocks total");
      ImGui::Text("");
      ImGui::Text("");
      ImGui::Text("");
  ImGui::Text("processed");
      ImGui::Text("");
      ImGui::Text("");
      ImGui::Text("");
  ImGui::Text("spent");
      ImGui::Text("");
      ImGui::Text("");
      ImGui::Text("");
  ImGui::TextUnformatted("sales");
      ImGui::Text("");
      ImGui::Text("");      
      ImGui::Text("");      
  ImGui::Text("cash");

  float cash[4];
  float stock[4];
  float di = 0;
  for (auto& i : g_gameData.econ_history)
  {
    ImGui::SetCursorPosY(16);

      int dx = 60;      
      ImGui::Text("");
      ImGui::SetCursorPosX(di + dx);
      ImGui::Text("%i", i.week + 1);
      ImGui::Text("");
      ImGui::Text("");
      ImGui::Text("");
      ImGui::SetCursorPosX(di + dx);
      ImGui::Text("%i", i.moon_rocks_collected);
      ImGui::Text("");
      ImGui::Text("");
      ImGui::Text("");
      ImGui::SetCursorPosX(di + dx);
      ImGui::Text("%i", i.moon_rocks_total);
      ImGui::Text("");
      ImGui::Text("");
      ImGui::Text("");
      ImGui::SetCursorPosX(di + dx);
      ImGui::Text("%i", i.processing_rate);
      ImGui::Text("");
      ImGui::Text("");
      ImGui::Text("");
      ImGui::SetCursorPosX(di + dx);
      ImGui::Text("$%i", i.spent);
      ImGui::Text("");
      ImGui::Text("");
      ImGui::Text("");
      ImGui::SetCursorPosX(di + dx);
      ImGui::Text("$%i", i.sales);
      ImGui::Text("");
      ImGui::Text("");
      ImGui::Text("");
      ImGui::SetCursorPosX(di + dx);
      ImGui::Text("$%i", i.cash);
    di += 130.f;
  }
  ImGui::PopFont();

  ImGui::SetCursorPosX(750);
  ImGui::SetCursorPosY(8);

  if (g_gameData.day_data.size())
  {

    // Plot Cash

    {
      //const auto& v = g_gameData.day_data;
      //const auto [min, max] = std::minmax_element(begin(v), end(v));

      int h = 10009;
      if (g_gameDialogs.threshold_1) h = 100009;
      if (g_gameDialogs.threshold_2) h = 1000009;

      ImGui::PushFont(io.Fonts->Fonts[2]);
      if (g_gameData.plot_data_cursor < g_gameData.day_data.size())
      {
        ImPlot::SetNextPlotLimits(
          g_gameData.day_data[g_gameData.plot_data_cursor].week - 6, 
          g_gameData.day_data[g_gameData.plot_data_cursor].week, 
          -1, 
          h, 
          ImGuiCond_Always);
      }
      else
      {
        ImPlot::SetNextPlotLimits(
          g_gameData.day_data[g_gameData.plot_data_cursor + 1].week - 6, 
          g_gameData.day_data[g_gameData.plot_data_cursor + 1].week, 
          -1, 
          h, 
          ImGuiCond_Always);
      }
      ImPlot::BeginPlot(
        "Income##IncomePlot", 
        "week", 
        "",
        ImVec2(-1, 0),
        ImPlotFlags_None,
        ImPlotAxisFlags_Lock,
        ImPlotAxisFlags_NoGridLines
        );    
      ImPlot::PlotLine("##Income", 
        &g_gameData.day_data[0].week, 
        &g_gameData.day_data[0].cash_total, 
        g_gameData.day_data.size(),
        g_gameData.plot_data_cursor + 1,
        sizeof(DayData));
      ImPlot::PlotScatter("##Incomea", 
        &g_gameData.day_data[0].week, 
        &g_gameData.day_data[0].cash_total, 
        g_gameData.day_data.size(),
        g_gameData.plot_data_cursor + 1,
        sizeof(DayData));
      ImPlot::EndPlot();
      ImGui::PopFont();
    }
  }
      
  // Plot Rocks

  ImGui::SetCursorPosX(750);
  ImGui::SetCursorPosY((g_windowManager.height - 2 * 84) / 2);     

  if (g_gameData.day_data.size())
  {
    ImGuiIO& io = ImGui::GetIO();
    {
      //const auto& v = g_gameData.day_data;
      //const auto [min, max] = std::minmax_element(begin(v), end(v));

      ImGui::PushFont(io.Fonts->Fonts[2]);
      if (g_gameData.plot_data_cursor < g_gameData.day_data.size())
      {
        ImPlot::SetNextPlotLimits(
          g_gameData.day_data[g_gameData.plot_data_cursor].week - 6, 
          g_gameData.day_data[g_gameData.plot_data_cursor].week, 
          -1, 
          2000, 
          ImGuiCond_Always);
      }
      else
      {
        ImPlot::SetNextPlotLimits(
          g_gameData.day_data[g_gameData.plot_data_cursor - 1].week - 6, 
          g_gameData.day_data[g_gameData.plot_data_cursor - 1].week, 
          -1, 
          2000, 
          ImGuiCond_Always);
      }
      ImPlot::BeginPlot(
        "Moon Rock##ProductPlot", 
        "week", 
        "",
        ImVec2(-1, 0),
        ImPlotFlags_None,
        ImPlotAxisFlags_Lock,
        ImPlotAxisFlags_NoGridLines
        );    
      ImPlot::PlotLine("##Incomeff", 
        &g_gameData.day_data[0].week, 
        &g_gameData.day_data[0].rock_total, 
        g_gameData.day_data.size(),
        g_gameData.plot_data_cursor + 1,
        sizeof(DayData));
      ImPlot::PlotScatter("##Incomefffdd", 
        &g_gameData.day_data[0].week, 
        &g_gameData.day_data[0].rock_total, 
        g_gameData.day_data.size(),
        g_gameData.plot_data_cursor + 1,
        sizeof(DayData));
      ImPlot::EndPlot();
      ImGui::PopFont();
    }
  }
}


void upgrade_ship_page()
{
  ImGuiIO& io = ImGui::GetIO();
  
  int updrade_x = 220;
  int upgrade_y = 450;
  ImGui::BeginChild(
    "Upgrade_1", 
    ImVec2(updrade_x, upgrade_y), 
    true, 
    ImGuiWindowFlags_AlwaysAutoResize);   

  bool mute = (g_current_ship->slot1 == 1) || (g_current_ship->slot2 == 1) || (g_current_ship->slot3 == 1);
  if (mute) im_disable_buttons();
  if (ImGui::Button("Afterburner"))
  {
    upgrade_ship(1, 1000);
    g_gameData.page = Launchpad;
  }
  if (mute) im_enable_buttons();
  ImGui::Text("Reignites exhaust fuels to");
  ImGui::Text("go faster.");
  ImGui::Text("$1000");
  ImGui::EndChild();
  ImGui::SameLine();

  ImGui::BeginChild(
    "Upgrade_2", 
    ImVec2(updrade_x, upgrade_y), 
    true, 
    ImGuiWindowFlags_AlwaysAutoResize);   

  mute = (g_current_ship->slot1 == 2) || (g_current_ship->slot2 == 2) || (g_current_ship->slot3 == 2);
  if (mute) im_disable_buttons();
  if (ImGui::Button("Launch Stabiliser"))
  {
    upgrade_ship(2, 1000);    
    g_gameData.page = Launchpad;
  }
  if (mute) im_enable_buttons();
  ImGui::Text("Reduces atmospheric");
  ImGui::Text("turbulence for a smoother");
  ImGui::Text("launch.");
  ImGui::Text("$1000");
  ImGui::EndChild();
  ImGui::SameLine();

  ImGui::BeginChild(
    "Upgrade_3", 
    ImVec2(updrade_x, upgrade_y), 
    true, 
    ImGuiWindowFlags_AlwaysAutoResize);   
  mute = (g_current_ship->slot1 == 3) || (g_current_ship->slot2 == 3) || (g_current_ship->slot3 == 3);
  if (mute) im_disable_buttons();
  if (ImGui::Button("Cargo Space"))
  {    
    upgrade_ship(3, 1500);    
    g_gameData.page = Launchpad;
  }
  if (mute) im_enable_buttons();
  ImGui::Text("Increases storage");
  ImGui::Text("capacity by 50.");
  ImGui::Text("$1500");
  ImGui::EndChild();
  ImGui::SameLine();

  ImGui::BeginChild(
    "Upgrade_4", 
    ImVec2(updrade_x, upgrade_y), 
    true, 
    ImGuiWindowFlags_AlwaysAutoResize);   
  mute = (g_current_ship->slot1 == 4) || (g_current_ship->slot2 == 4) || (g_current_ship->slot3 == 4);
  if (mute) im_disable_buttons();
  if (ImGui::Button("Flight Control"))
  {
    upgrade_ship(4, 2000);        
    g_gameData.page = Launchpad;
  }
  if (mute) im_enable_buttons();
  ImGui::Text("Increased planning accuracy");
  ImGui::Text("reduces journey time.");
  ImGui::Text("$2000");
  ImGui::EndChild();
  ImGui::SameLine();

  ImGui::BeginChild(
    "Content", 
    ImVec2(updrade_x, upgrade_y), 
    true, 
    ImGuiWindowFlags_AlwaysAutoResize);   
  // if (ImGui::Button("Shield"))
  // {
  //   upgrade_ship(5, 4000);           
  //   g_gameData.page = Launchpad;
  // }
  ImGui::Text("Reduces damage to the");
  ImGui::Text("ship, requiring fewer");
  ImGui::Text("repairs.");
  ImGui::Text("$4000");
  ImGui::EndChild();
}


void launch_page()
{
  ImGuiIO& io = ImGui::GetIO();
  
  // Title
  ImGui::PushFont(io.Fonts->Fonts[1]);
  ImGui::Text("Launchpad");
  ImGui::PopFont();

  ImGui::SameLine();
  if (ImGui::Button("Buy More Fleet..."))
  {
    g_gameData.page = PAGES::BuyFleet;
  }
  // Fleet list
  static bool toggle_button = false;
  static Ship* which_order;

  for (auto& i : g_gameData.fleet)
  {
    static int WINDOW_FLAGS = 0;
    static int WINDOW_FLAGS_BORDERLESS = ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDecoration;

    std::string title = "child_" + boost::uuids::to_string(i.tag);
    //ImGui::SetNextWindowSize(ImVec2(g_windowManager.width, 220));
    ImGui::BeginChild(
      title.c_str(), 
      ImVec2(g_windowManager.width - 20, 78), 
      true, 
      WINDOW_FLAGS);      

    // Upgrades
    std::string title_ups = "child_ups_" + boost::uuids::to_string(i.tag);
    ImGui::BeginChild(
      title_ups.c_str(), 
      ImVec2(200, 0), 
      true, 
      WINDOW_FLAGS_BORDERLESS); 

    ImGui::PushFont(io.Fonts->Fonts[2]);
    ImGui::Text("Upgrades");

    if (i.slot1) im_disable_buttons();
    if (ImGui::Button("1")) 
    {
      g_gameData.page = UpgradeShip;
      g_current_ship_slot = &(i.slot1);
      g_current_ship = &i;
    }
    if (im_is_button_disabled()) im_enable_buttons();
    ImGui::SameLine();

    if (i.slot2) im_disable_buttons();
    if (ImGui::Button("2")) 
    {
      g_gameData.page = UpgradeShip;
      g_current_ship_slot = &(i.slot2);
      g_current_ship = &i;
    }
    if (im_is_button_disabled()) im_enable_buttons();
    ImGui::SameLine();
    
    if (i.slot3) im_disable_buttons();
    if (ImGui::Button("3")) 
    {
      g_gameData.page = UpgradeShip;
      g_current_ship_slot = &(i.slot3);
      g_current_ship = &i;
    }
    if (im_is_button_disabled()) im_enable_buttons();
    ImGui::SameLine();
    ImGui::PopFont();

    ImGui::EndChild();
    ImGui::SameLine();

    std::string title_stats = "child_stats_" + boost::uuids::to_string(i.tag);
    ImGui::BeginChild(
      title_stats.c_str(), 
      ImVec2(220, 0), 
      true, 
      WINDOW_FLAGS_BORDERLESS);      
    ImGui::PushFont(io.Fonts->Fonts[2]);
    // ImGui::Text("Stats");
    // ImGui::Separator();
    ImGui::Text("Trip time %.1f days", i.data.transit_time);
    ImGui::Text("Cargo capacity %i", i.data.capacity);

    ImGui::Text("status:  "); 
    ImGui::SameLine();
    if (i.data.location == 0)   
      ImGui::Text("No orders");
    else if (i.data.location == 1)   
      ImGui::Text("On Launchpad");
    else if (i.data.location == 2)   
      ImGui::Text("Going up");
    else if (i.data.location == 3)
      ImGui::Text("Collecting from moon mine");
    else if (i.data.location == 4)
      ImGui::Text("Returning");
    else
      ImGui::Text("Something...");  

    ImGui::PopFont();
    ImGui::EndChild();
    ImGui::SameLine();

    std::string title_order = "child_order_" + boost::uuids::to_string(i.tag);
    ImGui::BeginChild(
      title_order.c_str(), 
      ImVec2(150, 0), 
      true, 
      WINDOW_FLAGS_BORDERLESS);   

    int order = i.order.pickup_location;
    ImGui::Combo("##orders", &order, "No orders\0Collect from Moon\0");
    ImGui::SameLine();

    if (i.order.pickup_location != order)
    {
      i.order.pickup_location = order;
      if (order)
      {
        i.order.pickup_amount = 1; // ?
        i.delay = g_gameData.time_tick + 1;
      }
      else
      {
        i.order.pickup_amount = 0; // ?
        i.delay = 0;        
      }
    }

    //if (i.order.pickup_location)

    ImGui::EndChild(); // order

    ImGui::EndChild(); // Fleet item
  }
}


void draw_mainmenu()
{
  unsigned int title_card_id = 0;
  if (g_rm.getResource("title card", title_card_id))
  {
    ImVec2 pivot_point(238, 198);
    ImGui::SetCursorPos(ImVec2(g_windowManager.width/2 - pivot_point.x, g_windowManager.height/2 - pivot_point.y));      
    ImGui::Image((ImTextureID)title_card_id, ImVec2(501, 355));

    ImVec2 pivot_point_a(238 - 353, 198 - 261);
    ImGui::SetCursorPos(ImVec2(g_windowManager.width/2 - pivot_point_a.x, g_windowManager.height/2 - pivot_point_a.y));      
    if (ImGui::Button("New Game"))
    {    
      g_gameData.page = Card;
    }

    ImVec2 pivot_point_b(238 - 353, 198 - 316);
    ImGui::SetCursorPos(ImVec2(g_windowManager.width/2 - pivot_point_b.x, g_windowManager.height/2 - pivot_point_b.y));      
    if (ImGui::Button("About"))
    {
      show_game_credits();
    }
  }
}


void mining_page()
{
  ImGui::Text("Moon mining base");
  if (g_gameData.mine_open)
  {
    bool mute = g_gameData.cash < 50000 || g_gameData.mine_miner;
    if (mute) im_disable_buttons();
    if (ImGui::Button("Build Mine: $50000"))
    {
      spend_cash(50000);
      g_gameData.mine_miner = true;
    }
    if (mute) im_enable_buttons();
    mute = g_gameData.cash < 50000 || g_gameData.mine_power;
    if (mute) im_disable_buttons();
    if (ImGui::Button("Build Power: $50000"))
    {
      spend_cash(50000);
      g_gameData.mine_power = true;
    }
    if (mute) im_enable_buttons();
    mute = g_gameData.cash < 50000 || g_gameData.mine_silo;
    if (mute) im_disable_buttons();
    if (ImGui::Button("Build Storage: $50000"))
    {
      spend_cash(50000);
      g_gameData.mine_silo = true;
    }
    if (mute) im_enable_buttons();

    if ( g_gameData.mine_miner &&  g_gameData.mine_power &&  g_gameData.mine_silo )
    {
      if (ImGui::Button("Visit dark side"))
      {
        g_dialogManager.dialog.load("/data/dark_side.dialog");
      }
    }
  }
  else
  {
    ImGui::Text("There is some mining activity here but you need a mining permit to build.");
  }
}


void production_page()
{
  int& current_floor = g_gameData.current_floor;

  if (g_gameData.processing.conveyors.size() > 1)
  {
    if (g_gameData.processing.conveyors.size() < 4)
    {
      bool mute = g_gameData.cash < 24000;
      if (mute) im_disable_buttons();
      if (ImGui::Button("new floor"))
      {
        spend_cash(24000);
        g_gameData.processing.conveyors.push_back( std::vector<Conveyor>() );
      }
      if (mute) im_enable_buttons();
      if (ImGui::IsItemHovered())
      {
        ImGui::BeginTooltip();
        ImGui::TextUnformatted("$24000");
        ImGui::EndTooltip();
      }
    }

    for (int i = g_gameData.processing.conveyors.size(); i > 0; --i)
    {
      std::string name = std::string("Floor ") + std::to_string(i);
      if (current_floor == i - 1)
      {
        ImGui::Text("> %s", name.c_str());
      }
      else
      {
        if (ImGui::Button(name.c_str()))
        {
          current_floor = i - 1;
        }
      }
    }
  }

  int ypos = g_windowManager.height - 250;
  int xpos = g_windowManager.width - 390;
  for (auto& i: g_gameData.processing.conveyors[current_floor])
  {

    if (!i.upgraded)
    {
      ImGui::SetCursorPos(ImVec2(xpos-36, ypos));
      bool mute = g_gameData.cash < 2800;
      if (mute) im_disable_buttons();
      std::string text2 = "##" + std::to_string((unsigned long)&i);
      if (ImGui::Button(text2.c_str()))
      {
        spend_cash(2800);
        i.upgraded = true;
      }
      if (mute) im_enable_buttons();
      if (ImGui::IsItemHovered())
      {
          ImGui::BeginTooltip();
          ImGui::TextUnformatted("Upgrade: $2800");
          ImGui::EndTooltip();
      }
    }

    ImGui::SetCursorPos(ImVec2(xpos, ypos));
    ImGui::ProgressBar(i.health, ImVec2(124, 0), "Health");    
    if (i.health < 0.51)
    {
      ImGui::SameLine();
      bool disable = g_gameData.cash < 200;
      if (disable) im_disable_buttons();
      std::string text = "Repair ($200)";
      text += "##" + std::to_string((unsigned long)&i);
      if (ImGui::Button(text.c_str()))
      {
        spend_cash(200);
        i.health = 1.0f;
      }
      if (disable) im_enable_buttons();
    }
    ypos -= 70;
  }

  static bool toggle_button = false;

  if (g_gameData.processing.conveyors[current_floor].size() < 4)
  {
    ImGui::SetCursorPos(ImVec2(xpos, ypos));
    if (ImGui::Button("Buy New Machine"))
    {
      toggle_button = true;
    }
  }

  if (toggle_button)
  {
    ImGui::OpenPopup("Buy new machine");
  }
  else
  {
    ImGui::CloseCurrentPopup();
  }
  
  if (ImGui::BeginPopupModal("Buy new machine", &toggle_button, ImGuiWindowFlags_AlwaysAutoResize))
  {
    int cost = 1800;

    ImGui::Text("Buy new machine for $%i", cost);
    
    bool disable = g_gameData.cash < cost;
    if (disable) im_disable_buttons();
    if (ImGui::Button("Buy"))
    {
      buy_new_machine(current_floor);
      toggle_button = false;
    }        
    if (disable) im_enable_buttons();

    if (ImGui::Button("Cancel"))
    {
      toggle_button = false;
    }

    ImGui::EndPopup();
  }

  //
  // Science
  //
  static bool toggle_science = false;

  ypos = 42;
  xpos = g_windowManager.width - 390;
  ImGui::SetCursorPos(ImVec2(xpos, ypos));
  if (ImGui::Button("Research"))
  {
    toggle_science = true;
  }

  if (toggle_science)
  {
    ImGui::OpenPopup("Upgrades");
  }
  else
  {
    ImGui::CloseCurrentPopup();
  }
  
  if (ImGui::BeginPopupModal("Upgrades", &toggle_science, ImGuiWindowFlags_AlwaysAutoResize))
  {
    if (g_gameData.science_unlock)
    {
      ImGui::Text("Upgrading...");
      float a = (g_gameData.next_science_level - g_gameData.time_tick) / (7.0f * 17.f * 30.f);
      ImGui::ProgressBar(a, ImVec2(250.f, 0.f));
    }
    else if (g_gameData.science_level < 8)
    {
      int cost = (160 + g_gameData.science_level * 80) * (g_gameData.science_level + 1);

      ImGui::Text("Buy next upgrade for $%i", cost);
      
      bool disable = g_gameData.cash < cost;
      if (disable) im_disable_buttons();
      if (ImGui::Button("Upgrade"))
      {
        buy_new_science(cost);
      }        
      if (disable) im_enable_buttons();
    }
    else
    {
      ImGui::Text("Max research level reached");
    }

    ImGui::Separator();

    if (ImGui::Button("Close"))
    {
      toggle_science = false;
    }

    ImGui::EndPopup();
  }

  //
  // Floors
  //
}


void draw_ship_yard()
{
  ImGuiIO& io = ImGui::GetIO();

    ImGui::PushFont(io.Fonts->Fonts[1]);
    ImGui::Text("Buy Fleet");
    ImGui::PopFont();
    ImGui::Separator();
      
    static bool toggle_button = false;
    static Ship* to_buy;

    for (auto& i: fleets)
    {
      if (i.data.value == 0) continue;

      std::string title = "child_" + boost::uuids::to_string(i.tag);
      //ImGui::SetNextWindowSize(ImVec2(g_windowManager.width, 220));
      ImGui::BeginChild(
        title.c_str(), 
        ImVec2(g_windowManager.width - 20, 130), 
        true, 
        0);      

      std::string title_id = "child_id_" + boost::uuids::to_string(i.tag);
      ImGui::BeginChild(
        title_id.c_str(), 
        ImVec2(330, 0), 
        true, 
        ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDecoration);  
      ImGui::Text("Fleet / Name / ID");
      ImGui::EndChild();
      ImGui::SameLine();

      std::string title_stats = "child_stats_" + boost::uuids::to_string(i.tag);
      ImGui::BeginChild(
        title_stats.c_str(), 
        ImVec2(300, 0), 
        true, 
        ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDecoration);      
      ImGui::PushFont(io.Fonts->Fonts[2]);
      // ImGui::Text("Stats");
      // ImGui::Separator();
      ImGui::Text("Trip time %.1f days", i.data.transit_time);
      ImGui::Text("Trip cost $1000");
      ImGui::Text("Cargo capacity %i", i.data.capacity);
      ImGui::PopFont();
      ImGui::EndChild();
      ImGui::SameLine();

      ImGui::SameLine();

      ImGui::Text("$%i", i.data.value);
      ImGui::SameLine();

      bool mute = g_gameData.cash < i.data.value;
      if (mute) im_disable_buttons();
      std::string title_buy = "Buy##" + boost::uuids::to_string(i.tag);
      if (ImGui::Button(title_buy.c_str()))
      {
        toggle_button = true;
        to_buy = &i;
      }
      if (mute) im_enable_buttons();
      
      ImGui::EndChild(); // Fleet item
    }
  
    if (toggle_button)
    {
      ImGui::OpenPopup("Buy");
    }
    else
    {
      ImGui::CloseCurrentPopup();
    }
    if (ImGui::BeginPopupModal("Buy", &toggle_button, ImGuiWindowFlags_AlwaysAutoResize))
    {
      ImGui::Text("Purchase for $%i?", to_buy->data.value);
      
      ImGui::PushFont(io.Fonts->Fonts[2]);
      ImGui::Text("Capacity %i", to_buy->data.capacity);
      ImGui::PopFont();

      if (ImGui::Button("Yes"))
      {
        toggle_button = false;
        // Buy
        g_gameData.cash -= to_buy->data.value;
        g_gameData.fleet.push_back(*to_buy);
        to_buy->data.value = 0;
      }        
      ImGui::SameLine();
      if (ImGui::Button("Nevermind"))
      {
        toggle_button = false;          
      }
      ImGui::EndPopup();
    }

    ImGui::Separator();
}


void home_page()
{
/*
  ImGui::Text("Home");
  if (g_gameData.week_counter == 0)
  {
    ImGui::Text("Um..");
    ImGui::Text("Welcome to the cereal factory. It's");
    ImGui::Text("your first day.");
    ImGui::Text("Place some orders, before clicking");
    ImGui::Text("next.");
  }
  else
  {
    ImGui::Text("Cash: $%i", g_gameData.cash);
    ImGui::Text("Moon rock collected this week: %i", g_gameData.current_week_data.moon_rocks_collected);
    ImGui::Text("%i / %i moon rock in storage", g_gameData.current_week_data.moon_rocks_total, g_gameData.current_week_data.moon_rock_storage_cap);
    ImGui::Text("%i processed to moon puffs this week", g_gameData.current_week_data.processing_rate);
    ImGui::Text("Spent: $%i", g_gameData.current_week_data.spent);
    ImGui::Text("Sales: $%i", g_gameData.current_week_data.sales); 
  }    
*/
  // blank if dialogue?

  // text sold this week
  // text sold last week
  // text processed this week
  // rock in storage
  // text vehicles in use
  // text mining...


  ImGui::Text("Factory Overview");
  ImGui::Separator();

    ImGui::Text("Cash: $%i", g_gameData.cash);
    ImGui::Text("Moon rock collected this week: %i", g_gameData.current_week_data.moon_rocks_collected);

  ImGui::Separator();

  // text bills outstanding
  if (g_gameData.tax_bill)
  {
    ImGui::Text("Factory Tax Bill: ");
    ImGui::SameLine();
    
    bool mute = g_gameData.cash < 1000000;
    if (mute) im_disable_buttons();
    if (ImGui::Button("  $1000000  "))
    {
      spend_cash(1000000);
      g_dialogManager.dialog.load("/data/game_win.dialog");
      g_gameData.tax_bill = false;
    }
    if (mute) im_enable_buttons();

    ImGui::SameLine();
    if (ImGui::Button("?"))
    {
      g_dialogManager.dialog.load("/data/about_tax.dialog");
    }
  }
}