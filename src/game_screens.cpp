#include "game_screens.h"
#include "game_globals.h"
#include "system_globals.h"
#include "imgui_internal.h" //itemflags
#include "implot.h"
#include "palette.h"


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
    ImVec2(g_windowManager.width, 66), 
    IM_COL32(0x2E, 0x32, 0x7D, 255),//g_palette["indigo_800"], 
    8.0f);

  ImGui::BeginChild(
    "Header", 
    ImVec2(0, 70), 
    true, 
    ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDecoration);   
  
  ImGui::Text("$%i", g_gameData.cash);

  ImGui::SetCursorPosX(g_windowManager.width / 2 - 140);
  ImGui::SetCursorPosY(0);
  ImGui::Text("Rocks %i/%i", 0, 0); //g_current_week_data.moon_rocks_total, g_current_week_data.moon_rock_storage_cap);

  const char* label = "Moonies Cereal Co";
  ImGui::SetCursorPosX(g_windowManager.width - ImGui::CalcTextSize(label).x - 8);
  ImGui::SetCursorPosY(0);
  ImGui::Text("%s", label);

  ImGui::EndChild();
}


void drawFooterBar()
{
  ImGui::SetNextWindowPos(ImVec2(0, g_windowManager.height - 84));
  ImGui::BeginChild(
    "Footer", 
    ImVec2(0, 89), 
    true, 
    ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDecoration);   
  
  if (g_gameData.page == Home) im_disable_buttons();
  if (ImGui::Button("Home")) g_gameData.page = Home;
  if (im_is_button_disabled()) im_enable_buttons();
  ImGui::SameLine();

  if (g_gameData.page == Econ) im_disable_buttons();
  if (ImGui::Button("Econ")) g_gameData.page = Econ;
  if (im_is_button_disabled()) im_enable_buttons();
  ImGui::SameLine();

  if (g_gameData.page == Launchpad) im_disable_buttons();
  int i = 0;
  //for (auto& j : g_fleet) if (j.data.location == 0 && j.order.pickup_amount == 0) ++i;
  std::string name = std::string("(") + std::to_string(i) + std::string(") Launchpad");
  if (ImGui::Button(name.c_str())) g_gameData.page = Launchpad;
  if (im_is_button_disabled()) im_enable_buttons();
  ImGui::SameLine();

  if (g_gameData.page == Processing) im_disable_buttons();
  if (ImGui::Button("Processing")) g_gameData.page = Processing;
  if (im_is_button_disabled()) im_enable_buttons();
  ImGui::SameLine();

  if (g_gameData.page == Mining) im_disable_buttons();
  if (ImGui::Button("Mining")) g_gameData.page = Mining;
  if (im_is_button_disabled()) im_enable_buttons();

  ImGui::SetCursorPosX(g_windowManager.width - 150);
  ImGui::SetCursorPosY(0);
  ImGui::Text("Week %i", g_gameData.week_counter + 1);

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
  // ImGui::Text("moon_rocks_collected"); ImGui::SameLine();
  // ImGui::Text("moon_rocks_total"); ImGui::SameLine();
  // //ImGui::Text(""); ImGui::SameLine();
  // ImGui::Text("processing_rate"); ImGui::SameLine();
  // ImGui::Text("spent"); ImGui::SameLine();
  // ImGui::Text("sales");

  // for (auto i : g_gameData.econ_history)
  // {
  //   ImGui::Text("%i", i.moon_rocks_collected); ImGui::SameLine();
  //   ImGui::Text("%i", i.moon_rocks_total); ImGui::SameLine();
  //   //ImGui::Text("%i", i.); ImGui::SameLine();
  //   ImGui::Text("%i", i.processing_rate); ImGui::SameLine();
  //   ImGui::Text("$%i", i.spent); ImGui::SameLine();
  //   ImGui::Text("$%i", i.sales);
  // }

  ImGui::SetCursorPosY(0);
  ImGui::TextUnformatted("moon rocks collected");
      ImGui::Text("");
      ImGui::Text("");
  ImGui::Text("moon rocks total");
      ImGui::Text("");
      ImGui::Text("");
  ImGui::Text("processed");
      ImGui::Text("");
      ImGui::Text("");
  ImGui::Text("spent");
      ImGui::Text("");
      ImGui::Text("");
  ImGui::Text("sales");

  float cash[4];
  float stock[4];
  for (int i = 0; i < 4; ++i)
  //for (int i = 4 - 1; i >= 0; --i)
  {
    ImGui::SetCursorPosY(16);
    if (i < g_gameData.econ_history.size())
    {
      int dx = 120;
      ImGui::Text("");
      ImGui::SetCursorPosX(i * 160 + dx);
      ImGui::Text("%i", g_gameData.econ_history[i].moon_rocks_collected);
      ImGui::Text("");
      ImGui::Text("");
      ImGui::SetCursorPosX(i * 160 + dx);
      ImGui::Text("%i", g_gameData.econ_history[i].moon_rocks_total);
      ImGui::Text("");
      ImGui::Text("");
      ImGui::SetCursorPosX(i * 160 + dx);
      ImGui::Text("%i", g_gameData.econ_history[i].processing_rate);
      ImGui::Text("");
      ImGui::Text("");
      ImGui::SetCursorPosX(i * 160 + dx);
      ImGui::Text("$%i", g_gameData.econ_history[i].spent);
      ImGui::Text("");
      ImGui::Text("");
      ImGui::SetCursorPosX(i * 160 + dx);
      ImGui::Text("$%i", g_gameData.econ_history[i].sales);

      cash[3 - i] = g_gameData.econ_history[i].sales;
      stock[3 - i] = g_gameData.econ_history[i].moon_rocks_total;
    }
    else
    {
      cash[3 - i] = 39000;
      stock[3 - i] = 0;
    }
  }

  ImGui::SetCursorPosX(750);
  ImGui::SetCursorPosY(8);
  ImGui::PlotLines("##Income", cash, IM_ARRAYSIZE(cash), 0, "Income", -1.0f, 1.0f, ImVec2(340, 280.0f));
      
  ImGui::SetCursorPosX(750);
  ImGui::SetCursorPosY((g_windowManager.height - 2 * 84) / 2);     
  ImGui::PlotLines("##Stock", stock, IM_ARRAYSIZE(stock), 0, "Stock", -1.0f, 1.0f, ImVec2(340, 280.0f));
}


void upgrade_ship_page()
{
  int updrade_x = 220;
  int upgrade_y = 450;
  ImGui::BeginChild(
    "Upgrade_1", 
    ImVec2(updrade_x, upgrade_y), 
    true, 
    ImGuiWindowFlags_AlwaysAutoResize);   

  if (ImGui::Button("Afterburner"))
  {
    upgrade_ship(1, 2000);
    g_gameData.page = Launchpad;
  }
  ImGui::Text("Reignites exhaust fuels to");
  ImGui::Text("go further.");
  ImGui::Text("$2 000");
  ImGui::EndChild();
  ImGui::SameLine();

  ImGui::BeginChild(
    "Upgrade_2", 
    ImVec2(updrade_x, upgrade_y), 
    true, 
    ImGuiWindowFlags_AlwaysAutoResize);   
  if (ImGui::Button("Launch Stabiliser"))
  {
    upgrade_ship(2, 1000);    
    g_gameData.page = Launchpad;
  }
  ImGui::Text("Reduces atmospheric");
  ImGui::Text("turbulence for a smoother");
  ImGui::Text("launch.");
  ImGui::Text("$2 000");
  ImGui::EndChild();
  ImGui::SameLine();

  ImGui::BeginChild(
    "Upgrade_3", 
    ImVec2(updrade_x, upgrade_y), 
    true, 
    ImGuiWindowFlags_AlwaysAutoResize);   
  if (ImGui::Button("Cargo Space"))
  {    
    upgrade_ship(3, 2500);    
    g_gameData.page = Launchpad;
  }
  ImGui::Text("Increases storage");
  ImGui::Text("capacity by 500.");
  ImGui::Text("$2 500");
  ImGui::EndChild();
  ImGui::SameLine();

  ImGui::BeginChild(
    "Upgrade_4", 
    ImVec2(updrade_x, upgrade_y), 
    true, 
    ImGuiWindowFlags_AlwaysAutoResize);   
  if (ImGui::Button("Flight Control"))
  {
    upgrade_ship(4, 3000);        
    g_gameData.page = Launchpad;
  }
  ImGui::Text("Increased planning accuracy");
  ImGui::Text("reduces journey time.");
  ImGui::Text("$3 000");
  ImGui::EndChild();
  ImGui::SameLine();

  ImGui::BeginChild(
    "Content", 
    ImVec2(updrade_x, upgrade_y), 
    true, 
    ImGuiWindowFlags_AlwaysAutoResize);   
  if (ImGui::Button("Shield"))
  {
    upgrade_ship(5, 4000);           
    g_gameData.page = Launchpad;
  }
  ImGui::Text("Reduces damage to the");
  ImGui::Text("ship, requiring fewer");
  ImGui::Text("repairs.");
  ImGui::Text("$4 000");
  ImGui::EndChild();
}
