#include "dialog_manager.h"
#include "imgui.h"
#include "system_globals.h"


void DialogManager::init()
{
}


void DialogManager::draw()
{
  if (dialog.talkEvents.size() == 0) return;

  // Window (fullscreen)
    static int FULL_SCREEN_FLAGS = ImGuiWindowFlags_NoTitleBar | 
                          ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                          ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse |ImGuiWindowFlags_NoCollapse |
                          ImGuiWindowFlags_NoBringToFrontOnFocus |
                          ImGuiWindowFlags_NoSavedSettings;
                          
  // ImGui::SetNextWindowSize(ImVec2(g_windowManager.width, g_windowManager.height));
  // ImGui::SetNextWindowPos(ImVec2(0, 0));
  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
  bool open = true;
	//ImGui::Begin("Dialog", &open, FULL_SCREEN_FLAGS);
  ImGui::SetCursorPos(ImVec2(0, 0));
  ImGui::BeginChild("Dialog", ImVec2(g_windowManager.width, g_windowManager.height), false, FULL_SCREEN_FLAGS);

  // Darken screen
  //const ImU32 dim_bg_col = GetColorU32(dim_bg_for_modal ? ImGuiCol_ModalWindowDimBg : ImGuiCol_NavWindowingDimBg, g.DimBgRatio);
  const ImU32 dim_bg_col = ImColor(1.f, 1.f, 1.f, 0.25f);
  //const ImU32 dim_bg_col = ImColor(1.f, 1.f, 1.f, 0.75f);
  ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(0, 0), ImVec2(g_windowManager.width, g_windowManager.height), dim_bg_col);

  // Character sprite
  const char* resource_name = dialog.talkEvents.front().name.c_str();
  unsigned int id = 0;
  g_rm.getResource(resource_name, id);
  ImVec2 min(0, 0);
  ImVec2 max(g_windowManager.width, g_windowManager.height);
  ImGui::GetWindowDrawList()->AddImage((ImTextureID)id, min, max);

  // Dialog box
  //ImGui::Text("%s", dialog.talkEvents.front().text.c_str());
  int x = 64;
  int y = g_windowManager.height * 0.66;
  int w = g_windowManager.width - 2 * x;
  int h = g_windowManager.height * 0.25;
  ImGui::SetCursorPosX(x);
  ImGui::SetCursorPosY(y);
  // std::string label_str = dialog.talkEvents.front().name + 
  //                         ": " + 
  //                         dialog.talkEvents.front().text;
  std::string label_str = dialog.talkEvents.front().text;
  const char* label = label_str.c_str();
  if (ImGui::Button(
    label, 
    ImVec2(w, h)))
  {
    dialog.next();
  }

  ImGui::SetCursorPosX(x);
  ImGui::SetCursorPosY(y);
  ImGui::Text("%s", dialog.talkEvents.front().name.c_str());

  // End window
  ImGui::PopStyleVar();
  ImGui::EndChild();
}