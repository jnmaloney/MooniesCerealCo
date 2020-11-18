#include <stdio.h>
#include "imgui.h"
#include "implot.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <emscripten/html5.h>
#endif
#include "random_stuff.h"
#include "system_globals.h"
#include "game_globals.h"
#include "palette.h"


//extern "C" 
int main(int argc, char** argv)
{
  g_windowManager.width = 1280;
  g_windowManager.height = 800;
  g_windowManager.init("Project");
  g_menuManager.init(g_windowManager);

  g_rs = new RenderSystem();
  g_rs->init();
  g_rs->setClearColour(52, 101, 164);

  glm::vec3 eye = camDistance * glm::vec3(1, 1, 0.45);
  glm::vec3 centre(0, 0, 0.9);
  glm::vec3 up(0, 0, 1);
  cameraView = glm::lookAt(eye, centre, up);
  cameraProjection = glm::perspective(45.0f, (float)g_windowManager.width / (float)g_windowManager.height, 1.0f, 200.0f);
  identityMatrix = glm::mat4(1.0);
  objectMatrix = glm::mat4(1.0);
  g_rs->setViewProj(cameraProjection * cameraView);

  // Palette
  load_palette("/data/material_palette.csv");

  // Fonts
  ImGuiIO& io = ImGui::GetIO();

  io.Fonts->Clear();
  io.Fonts->AddFontFromFileTTF("/data/font/NovaSquare-Regular.ttf", 36);
  io.Fonts->AddFontFromFileTTF("/data/font/NovaSquare-Regular.ttf", 48);
  io.Fonts->AddFontFromFileTTF("/data/font/NovaSquare-Regular.ttf", 14);
  io.Fonts->Build();

  // Style
  ImGuiStyle& style = ImGui::GetStyle();
  style.WindowPadding.x = 12;
  style.WindowPadding.y = 6;
  style.FramePadding.x = 20;
  style.FramePadding.y = 8;

  style.WindowRounding = 0.0f;
  style.FrameBorderSize = 6.0f;
  style.FrameRounding = 12.0f;

  //style.Colors[ImGuiCol_WindowBg] = ImColor(52, 101, 164, 0xff);
  style.Colors[ImGuiCol_WindowBg] = ImColor(0, 0, 0, 0);
  style.Colors[ImGuiCol_Text] = ImColor(g_palette["indigo_200"]);
  style.Colors[ImGuiCol_Button] = ImColor(g_palette["indigo_600"]);
  style.Colors[ImGuiCol_ButtonHovered] = ImColor(g_palette["indigo_400"]);
  style.Colors[ImGuiCol_ButtonActive] = ImColor(g_palette["indigo_300"]);
  style.Colors[ImGuiCol_Border] = ImColor(g_palette["indigo_800"]);
  
  style.Colors[ImGuiCol_PlotLines] = ImColor(g_palette["indigo_200"]);
  style.Colors[ImGuiCol_PlotHistogram] = ImColor(g_palette["grey_400"]);

  // ImPlot
  ImPlot::CreateContext();
  ImPlot::GetStyle().LineWeight = 8.0f;
  ImPlot::GetStyle().AntiAliasedLines = true;
  ImPlot::GetStyle().Colors[ImPlotCol_Line] = ImColor(g_palette["indigo_200"]);
  
  // Resources (async loading)

  // UI
  g_rm.addResource("ui_title", "resources/ui_elemnt_1.png", ResourceManager::PNG);

  // Misc
  g_rm.addResource("square", "resources/square.obj", ResourceManager::OBJ);

  // - Portraits - 
  g_rm.addResource("Girl(angr_alt)", "resources/portrait/girl/angr_alt.png", ResourceManager::PNG);
  g_rm.addResource("Girl(angr_smile)", "resources/portrait/girl/angr_smile.png", ResourceManager::PNG);
  g_rm.addResource("Girl(neutral)", "resources/portrait/girl/neutral.png", ResourceManager::PNG);
  g_rm.addResource("Girl(smiler)", "resources/portrait/girl/smiler.png", ResourceManager::PNG);
 
  g_rm.addResource("Moonie(neutral)", "resources/portrait/moonie/neutral.png", ResourceManager::PNG);
  g_rm.addResource("Moonie(smiler)", "resources/portrait/moonie/smiler.png", ResourceManager::PNG);
 
  g_rm.addResource("Bot(ex)", "resources/portrait/bot/exclamation_mark.png", ResourceManager::PNG);
  g_rm.addResource("Bot(neutral)", "resources/portrait/bot/neutral.png", ResourceManager::PNG);
  g_rm.addResource("Bot(qu)", "resources/portrait/bot/question_mark.png", ResourceManager::PNG);
  g_rm.addResource("Bot(smile)", "resources/portrait/bot/smile_emote.png", ResourceManager::PNG);
  g_rm.addResource("Bot(zzz)", "resources/portrait/bot/zzz.png", ResourceManager::PNG);

  // - 3D's -
  g_rm.addResource("conveyor body", "resources/objects/conveyor/conveyor bake.obj", ResourceManager::OBJ);
  g_rm.addResource("conveyor belt", "resources/objects/conveyor/conveyor belt isolated.obj", ResourceManager::OBJ);
  g_rm.addResource("conveyor body(tex)", "resources/objects/conveyor/ao_target.png", ResourceManager::PNG);
  g_rm.addResource("conveyor belt(tex)", "resources/objects/conveyor/conveyor_tile.png", ResourceManager::PNG);

  g_rm.addResource("rocket", "resources/objects/rocket/retro toy rocket.obj", ResourceManager::OBJ);
  g_rm.addResource("launch platform", "resources/objects/rocket/launch_platform.obj", ResourceManager::OBJ);
  g_rm.addResource("mtl1", "resources/objects/rocket/mtl1.png", ResourceManager::PNG);
  g_rm.addResource("mtl3", "resources/objects/rocket/mtl3.png", ResourceManager::PNG);
  g_rm.addResource("mtl4", "resources/objects/rocket/mtl4.png", ResourceManager::PNG);
  g_rm.addResource("launch_texture", "resources/objects/rocket/launch_texture.png", ResourceManager::PNG);

  // Particles
  for (int i = 0; i < 25; ++i)
  {
    std::string filename = (std::string("resources/objects/White puff/whitePuff") + ZeroPadNumber(i) + std::string(".png"));
    std::string resource = (std::string("whitePuff") + ZeroPadNumber(i));
    g_rm.addResource(resource.c_str(), filename.c_str(), ResourceManager::PNG);
  }

  // - Audio - 

  // Start the async load
  g_rm.start_async_load();

  // Initialiser (game)
  g_gameData.fleet.push_back(Ship((ShipData){ 3.4f, 2000, 18000, 0 }));

  #ifdef __EMSCRIPTEN__
  emscripten_set_main_loop(loop, 0, 1);
  #endif

  glfwTerminate();

  return 0;
}
