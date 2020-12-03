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

#ifdef __EMSCRIPTEN__ // resize
#include <emscripten.h>
#include <emscripten/html5.h>
#endif
 
// EM_JS(int, canvas_get_width, (), {
//   return Module.canvas.clientWidth;
// });

// EM_JS(int, canvas_get_height, (), {
//   return Module.canvas.clientHeight;
// });

using namespace game_globals;


namespace game_render_system
{
  
void loop()
{
  // Window
  // g_windowManager.width = canvas_get_width();
  // g_windowManager.height = canvas_get_height();
  // g_rs->m_window.width = g_windowManager.width;
  // g_rs->m_window.height = g_windowManager.height;
  //glfwSetWindowSize(g_windowManager.g_window, g_windowManager.width, g_windowManager.height);
  // int width = canvas_get_width();
  // int height = canvas_get_height();
  // glfwSetWindowSize(g_rs->m_window.g_window, g_windowManager.width, g_windowManager.height);

  // Audio
  g_audioManager.tick();

  // Events
  g_gameDialogs.tick();

  //   Timer repetition
  if (no_dialog() && g_gameData.page != MainMenu && !g_gameData.game_over)
  for (int i = 0; i < g_gameData.timer_speed; ++i)
  {
    // Logic stuff
    g_gameData.processing.tick();
    g_gameData.mining.tick();
    //g_gameData.launches.loop();

    g_gameData.update_timer();
  }

  ImGuiIO& io = ImGui::GetIO();

  //printf("loop\n");

  // // Render stuff
  g_rs->m_window.width = 1280;
  g_rs->m_window.height = 800;
  g_windowManager.width = 1280;
  g_windowManager.height = 800;

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

  //   ImGui::Text("%i", g_windowManager.g_window);
  // ImGui::Text("%i", g_rs->m_window.g_window);
  // ImGui::Text("w  %i", g_windowManager.width);
  // ImGui::Text("h  %i", g_windowManager.height);
  
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

    if (no_dialog() && !g_gameData.game_over)
    {
      // Card ends.. go to Home Screen
      g_gameData.page = Home;
    }
  }
  else if (g_gameData.page == Home)
  {
    drawHeaderBar();
    drawContent(&home_page);
    drawFooterBar();
  }

  else if (g_gameData.page == BuyFleet)
  {
    drawHeaderBar();
    drawContent(&draw_ship_yard);
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

  if (showing_game_credits())
  {
    draw_game_credits();
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