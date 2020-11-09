#include <stdio.h>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <emscripten/html5.h>
#endif
#include "random_stuff.h"
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

  glm::vec3 eye = camDistance * glm::vec3(0, 0, 1);
  glm::vec3 centre(0, 0, 0);
  glm::vec3 up(0, 1, 0);
  cameraView = glm::lookAt(eye, centre, up);
  cameraProjection = glm::perspective(45.0f, (float)g_windowManager.width / (float)g_windowManager.height, 1.0f, 200.0f);
  identityMatrix = glm::mat4(1.0);
  objectMatrix = glm::mat4(1.0);

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

  style.FrameBorderSize = 6.0f;
  style.FrameRounding = 12.0f;

  //style.Colors[ImGuiCol_WindowBg] = ImColor(0x01, 0x10, 0x14, 0xff); // Rich black FOGRA
  style.Colors[ImGuiCol_WindowBg] = ImColor(52, 101, 164, 0xff);
  style.Colors[ImGuiCol_Text] = ImColor(g_palette["indigo_200"]);
  style.Colors[ImGuiCol_Button] = ImColor(g_palette["indigo_600"]);
  style.Colors[ImGuiCol_ButtonHovered] = ImColor(g_palette["indigo_400"]);
  style.Colors[ImGuiCol_ButtonActive] = ImColor(g_palette["indigo_300"]);
  style.Colors[ImGuiCol_Border] = ImColor(g_palette["indigo_800"]);
  
  style.Colors[ImGuiCol_PlotLines] = ImColor(g_palette["indigo_200"]);
  
  // style.Colors[ImGuiCol_Button] = ImColor(0x04, 0x3E, 0x4E, 0xff); // Midnight Green Eagle Green
  // style.Colors[ImGuiCol_ButtonHovered] = ImColor(0x07, 0x6C, 0x88,  0xff); // Blue Sapphire
  // style.Colors[ImGuiCol_ButtonActive] = ImColor(0x0A, 0x9A, 0xC2, 0xff); // Blue Green

  // Initialiser
  g_gameData.fleet.push_back(Ship((ShipData){ 3.4f, 2000, 18000, 0 }));

  #ifdef __EMSCRIPTEN__
  emscripten_set_main_loop(loop, 0, 1);
  #endif

  glfwTerminate();

  return 0;
}
