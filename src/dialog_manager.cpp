#include "dialog_manager.h"
#include "imgui.h"
#include "system_globals.h"


void DialogManager::init()
{
}


void DialogManager::draw()
{
  if (dialog.talkEvents.size() == 0) return;

  // Who?
  std::string name = dialog.talkEvents.front().name;
  unsigned int t = name.find("(");
  std::string name_only = name.substr(0, t);


  // Window (fullscreen)
    static int FULL_SCREEN_FLAGS = ImGuiWindowFlags_NoTitleBar | 
                          ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                          ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse |ImGuiWindowFlags_NoCollapse |
                          ImGuiWindowFlags_NoBringToFrontOnFocus |
                          ImGuiWindowFlags_NoSavedSettings;
                          
  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
  bool open = true;
  ImGui::SetCursorPos(ImVec2(0, 0));
  ImGui::BeginChild("Dialog", ImVec2(g_windowManager.width, g_windowManager.height), false, FULL_SCREEN_FLAGS);

  // Fade game screen for dialogue
  const ImU32 dim_bg_col = ImColor(1.f, 1.f, 1.f, 0.25f);
  ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(0, 0), ImVec2(g_windowManager.width, g_windowManager.height), dim_bg_col);

    int x = 64;
    int y = g_windowManager.height * 0.66;
    int w = g_windowManager.width - 2 * x;
    int h = g_windowManager.height * 0.25;

  if (name.length())
  {
    // Character sprite
    const char* resource_name = dialog.talkEvents.front().name.c_str();
    unsigned int id = 0;
    g_rm.getResource(resource_name, id);
    {
      float r = 0.63;
      float h = g_windowManager.height * 1.21;
      ImVec2 min;
      ImVec2 max;
      if (name_only == "Bot")
      {
        min = ImVec2(128, 16);
      }
      if (name_only == "Moonie")
      {
        r = 0.7283;
        min = ImVec2(g_windowManager.width / 2 + 64, 16);
      }
      if (name_only == "Girl")
      {
        r = 0.6255;
        min = ImVec2(g_windowManager.width / 2 + 64, 16);
      }
      float w = r * h;
      max = ImVec2(min.x + w, min.y + h);
      ImGui::GetWindowDrawList()->AddImage((ImTextureID)id, min, max);
    }

    // Dialog box
    ImGuiStyle& style = ImGui::GetStyle();
    ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(x, y), ImVec2(x + w, y + h), ImColor(style.Colors[ImGuiCol_Button]), style.FrameRounding);
    ImGui::GetWindowDrawList()->AddRect(ImVec2(x, y), ImVec2(x + w, y + h), ImColor(style.Colors[ImGuiCol_Border]), style.FrameRounding, ImDrawCornerFlags_All, style.FrameBorderSize);
    
    // Name tag
    ImGui::SetCursorPosX(x + w - 250);
    ImGui::SetCursorPosY(y - 45 + 3);
    unsigned int title_id = 0;
    if (g_rm.getResource("ui_character_name", title_id))
    {
      ImGui::Image((ImTextureID)title_id, ImVec2(179, 45));
    }
    ImGui::SetCursorPosX(x + w - 250 + 30);
    ImGui::SetCursorPosY(y - 45 + 8);
    ImGui::Text("%s", name_only.c_str());

    // Text
    ImGui::SetCursorPosX(x + 32);
    ImGui::SetCursorPosY(y + 42);
    //ImGui::SetNextItemWidth(150);//w - 2 * 42);
    ImGui::PushTextWrapPos(ImGui::GetCursorPos().x + w - 42);
    ImGui::TextWrapped("%s", dialog.talkEvents.front().text.c_str());
    ImGui::PopTextWrapPos();
  }

  // Next button
  ImGui::SetCursorPosX(x + w - 200);
  ImGui::SetCursorPosY(y + h - 70);
  if (ImGui::Button(" - > "))
  {
    dialog.next();
  }
 
  // End window
  ImGui::PopStyleVar();
  ImGui::EndChild();
}