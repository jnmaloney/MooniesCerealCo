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


void process_puffs()
{
  int process_rate = 285;
  int processed;
  if (process_rate > g_gameData.current_week_data.moon_rocks_total)
  {
    processed = g_gameData.current_week_data.moon_rocks_total;
  }
  else
  {
    processed = process_rate;
  }
  
  g_gameData.current_week_data.processing_rate = processed;
  
  g_gameData.current_week_data.moon_rocks_total -= processed;
}


void sell_puffs()
{
  //g_gameData.current_week_data.sales = 25 * g_gameData.current_week_data.processing_rate;

  g_gameData.cash += 20 * g_gameData.current_week_data.processing_rate;
}


int* g_current_ship_slot;
Ship* g_current_ship = 0;
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
void process_launch(Ship& i)
{
  g_gameData.launches.push_back(
          (Launch){ i, i.order, g_gameData.time, g_gameData.time + i.data.transit_time, g_gameData.time + 2 * i.data.transit_time, false, false }
        );
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


    std::vector<Ship> fleets = {
      Ship((ShipData){ 3.49, 1000, 35000, 0 }),
      Ship((ShipData){ 4.0, 1500, 70000, 0 }),
      Ship((ShipData){ 3.2, 800, 75000, 0}),
      Ship((ShipData){ 9.0, 7500, 900000, 0 }),
    };  




static bool s_buttons_disabled = false;
void im_disable_buttons()
{
  ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
  ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
  s_buttons_disabled = true;
}


void im_enable_buttons()
{
  ImGui::PopItemFlag();
  ImGui::PopStyleVar();
  s_buttons_disabled = false;
}


bool im_is_button_disabled() { return s_buttons_disabled; }



void drawContent(std::function<void()> f_ptr)
{
  ImGui::SetNextWindowPos(ImVec2(0, 84));
  ImGui::BeginChild(
    "Content", 
    ImVec2(0, g_windowManager.height - 2 * 84), 
    true, 
    ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDecoration);   
  
  if (f_ptr) f_ptr();

  ImGui::EndChild();
}


void loop()
{
  g_gameData.update_timer();

  ImGuiIO& io = ImGui::GetIO();

  g_rs->start();
  g_menuManager.predraw();

  // ImGuizmo::DrawGrid(glm::value_ptr( cameraView ), glm::value_ptr(  cameraProjection ), glm::value_ptr(  identityMatrix ), 100.f);
  // ImGuizmo::DrawCubes(glm::value_ptr( cameraView ), glm::value_ptr(  cameraProjection ), glm::value_ptr( objectMatrix ), gizmoCount);


  // static int FULL_SCREEN_FLAGS = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground | 
  //                         ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
  //                         ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse |ImGuiWindowFlags_NoCollapse |
  //                         ImGuiWindowFlags_NoBringToFrontOnFocus |
  //                         ImGuiWindowFlags_NoSavedSettings;
  
  static int FULL_SCREEN_FLAGS = ImGuiWindowFlags_NoTitleBar | 
                          ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                          ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse |ImGuiWindowFlags_NoCollapse |
                          ImGuiWindowFlags_NoBringToFrontOnFocus |
                          ImGuiWindowFlags_NoSavedSettings;
                          
  ImGui::SetNextWindowSize(ImVec2(g_windowManager.width, g_windowManager.height));
  ImGui::SetNextWindowPos(ImVec2(0, 0));
  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
  bool open = true;
	ImGui::Begin("Editor", &open, FULL_SCREEN_FLAGS);
  ImGui::PopStyleVar();

  if (g_gameData.page == Home)
  {
    drawHeaderBar();
    
    ImGui::Text("Home");
    // if (ImGui::Button("Mining")) { g_gameData.page = Mining; }
    // if (ImGui::Button("Launch Pad")) { g_gameData.page = Launchpad; }
    if (ImGui::Button("Next ->")) { end_week(); }

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

    drawFooterBar();
  }

  else if (g_gameData.page == BuyFleet)
  {
    drawHeaderBar();

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

      std::string title_status = "child_status_" + boost::uuids::to_string(i.tag);
      ImGui::BeginChild(
        title_status.c_str(), 
        ImVec2(300, 0), 
        true, 
        ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDecoration);   
      ImGui::Text("$%i", i.data.value);

      ImGui::SameLine();
      ImGui::Button("Appraise");

      ImGui::EndChild();
      ImGui::SameLine();

      std::string title_buy = "Buy##" + boost::uuids::to_string(i.tag);
      if (ImGui::Button(title_buy.c_str()))
      {
        toggle_button = true;
        to_buy = &i;
      }
      
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
      // ImGui::PushFont(io.Fonts->Fonts[1]);
      // ImGui::Text("Order");
      // ImGui::PopFont();
      // ImGui::Separator();
      
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

    drawBackBar(Launchpad);
  }

  else if (g_gameData.page == Launchpad)
  {
    drawHeaderBar();
    drawContent(&launch_page);
    drawFooterBar();
  }

  else if (g_gameData.page == Mining)
  {
    drawHeaderBar();
    drawContent(NULL);
    drawFooterBar();
  }

  else if (g_gameData.page == Processing)
  {
    drawHeaderBar();
    drawContent(NULL);
    drawFooterBar();
  } 
  else if (g_gameData.page == Econ)
  {
    drawHeaderBar();
    drawContent(&econ_page);
    drawFooterBar();
  }
  else if (g_gameData.page == UpgradeShip)
  {
    drawHeaderBar();
    drawContent(&upgrade_ship_page);
    drawBackBar(Launchpad);
  }


  ImGui::End();

  // ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);
  // ImGuizmo::ViewManipulate(glm::value_ptr( cameraView ), camDistance, ImVec2(io.DisplaySize.x - 128, 0), ImVec2(128, 128), 0x10101010);

  g_menuManager.postdraw();
};
