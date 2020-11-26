#include "game_render_system.h"
#include "game_globals.h"
#include "system_globals.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <algorithm>
#include "imgui_internal.h" //itemflags

#include "game_screens.h"
#include "actions.h"

#include "launch_scene.h"
#include "processing_scene.h"
#include "mining_scene.h"


using namespace game_globals;


namespace game_render_system
{
  
void loop()
{
  // Logic stuff
  g_gameData.processing.tick();
  g_gameData.mining.tick();
  //g_gameData.launches.loop();


  // Render stuff
  g_rs->m_window.width = 1280;
  g_rs->m_window.height = 800;
  g_windowManager.width = 1280;
  g_windowManager.height = 800;

  g_gameData.update_timer();

  ImGuiIO& io = ImGui::GetIO();

  //printf("loop\n");

  g_rs->start();
  // //printf("rs: 0x%x\n", (size_t)g_rs);

  if (g_gameData.page == Launchpad)
  {
    draw_launch_scene(g_rs);
  }
  else if (g_gameData.page == Processing)
  {
    draw_processing_scene(g_rs);
  }
  else if (g_gameData.page == Mining)
  {
    draw_mining_scene(g_rs);
  }

  g_menuManager.predraw();
  
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

  if (g_gameData.page == MainMenu)
  {
    draw_mainmenu();
  }
  else if (g_gameData.page == Card)
  {
    // Draw Moon card
    float radius = g_windowManager.height / 3.5f;
    ImGui::GetWindowDrawList()->AddCircleFilled(
      ImVec2(0.5f * g_windowManager.width, 0.5f * g_windowManager.height),
      radius,
      ImColor(0xd5, 0x9e, 0x61, 0xff),
      51
    );

    if (no_dialog())
    {
      // Card ends.. go to Home Screen
      g_gameData.page = Home;
    }
  }
  else if (g_gameData.page == Home)
  {
    drawHeaderBar();
    
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
    drawContent(&mining_page);
    drawFooterBar();
  }

  else if (g_gameData.page == Processing)
  {
    drawHeaderBar();
    drawContent(&production_page);
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

  // Character dialog
  if (g_gameData.page != MainMenu)  
  {
    draw_dialog();  
  }
  
  ImGui::End();

  g_menuManager.postdraw();

  g_rs->end();
}


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


} // namespace game_render_system


// namespace 
// {

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


//} // namespace