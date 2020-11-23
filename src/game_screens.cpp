#include "game_screens.h"
#include "game_globals.h"
#include "system_globals.h"
#include "imgui_internal.h" //itemflags
#include "implot.h"
#include "palette.h"
//#include "launch_scene.h"
#include "dialog_manager.h"


static DialogManager s_dialogManager;


  static bool inited = false;
void draw_dialog()
{
  if (!inited) 
  {
    s_dialogManager.init();
    Dialog d;
    d.load("/data/intro.dialog");
    s_dialogManager.dialog = d;
    inited = true;
  }

  s_dialogManager.draw();
}


bool no_dialog()
{
  return s_dialogManager.dialog.talkEvents.size() == 0;
}


void drawHeaderBar()
{
  ImGui::GetWindowDrawList()->AddRectFilled(
    ImVec2(0, 0), 
    ImVec2(g_windowManager.width, 63), 
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

  ImGui::SetCursorPosX(g_windowManager.width - 150);
  ImGui::ProgressBar(g_gameData.days / 7.0f, ImVec2(-1, 26), "");

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
  // ImGui::PlotLines("##Income", cash, IM_ARRAYSIZE(cash), 0, "Income", -1.0f, 1.0f, ImVec2(340, 280.0f));
  if (g_gameData.plot_a_x.size())
  {
    ImGuiIO& io = ImGui::GetIO();

    {
      const auto& v = g_gameData.plot_a_x;
      const auto [min, max] = std::minmax_element(begin(v), end(v));

      ImGui::PushFont(io.Fonts->Fonts[2]);
      if (g_gameData.plot_data_cursor < g_gameData.plot_a_x.size())
      {
        ImPlot::SetNextPlotLimits(
          g_gameData.plot_a_y[g_gameData.plot_data_cursor] - 6, 
          g_gameData.plot_a_y[g_gameData.plot_data_cursor], 
          *min - 1000, 
          *max + 1000, 
          ImGuiCond_Always);
      }
      else
      {
        ImPlot::SetNextPlotLimits(
          g_gameData.plot_a_y[g_gameData.plot_data_cursor - 1] - 6, 
          g_gameData.plot_a_y[g_gameData.plot_data_cursor - 1], 
          *min - 1000, 
          *max + 1000, 
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
        g_gameData.plot_a_y.data(), 
        g_gameData.plot_a_x.data(), 
        g_gameData.plot_a_x.size(),
        g_gameData.plot_data_cursor + 1);
      ImPlot::PlotScatter("##Income", 
        g_gameData.plot_a_y.data(), 
        g_gameData.plot_a_x.data(), 
        g_gameData.plot_a_x.size(),
        g_gameData.plot_data_cursor);
      ImPlot::EndPlot();
      ImGui::PopFont();
    }
  }
      
  ImGui::SetCursorPosX(750);
  ImGui::SetCursorPosY((g_windowManager.height - 2 * 84) / 2);     

  if (g_gameData.plot_b_x.size())
  {
    ImGuiIO& io = ImGui::GetIO();
    {
      const auto& v = g_gameData.plot_b_x;
      const auto [min, max] = std::minmax_element(begin(v), end(v));

      ImGui::PushFont(io.Fonts->Fonts[2]);
      if (g_gameData.plot_data_cursor < g_gameData.plot_b_x.size())
      {
        ImPlot::SetNextPlotLimits(
          g_gameData.plot_b_y[g_gameData.plot_data_cursor] - 6, 
          g_gameData.plot_b_y[g_gameData.plot_data_cursor], 
          *min - 1, 
          *max + 1, 
          ImGuiCond_Always);
      }
      else
      {
        ImPlot::SetNextPlotLimits(
          g_gameData.plot_b_y[g_gameData.plot_data_cursor - 1] - 6, 
          g_gameData.plot_b_y[g_gameData.plot_data_cursor - 1], 
          *min - 1, 
          *max + 1, 
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
      ImPlot::PlotLine("##Income", 
        g_gameData.plot_b_y.data(), 
        g_gameData.plot_b_x.data(), 
        g_gameData.plot_b_x.size(),
        g_gameData.plot_data_cursor + 1);
      ImPlot::PlotScatter("##Income", 
        g_gameData.plot_b_y.data(), 
        g_gameData.plot_b_x.data(), 
        g_gameData.plot_b_x.size(),
        g_gameData.plot_data_cursor);
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


void launch_page()
{
  //draw_launch_scene(g_rs);
  
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
  //ImGui::Text("Fleet");
    
  ImGui::BeginChild(
    "Fleet_container_child", 
    ImVec2(0, 480), 
    true, 
    0);  

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

    std::string title_id = "child_id_" + boost::uuids::to_string(i.tag);
    ImGui::BeginChild(
      title_id.c_str(), 
      ImVec2(130, 0), 
      true, 
      WINDOW_FLAGS_BORDERLESS);  
    ImGui::Text("A");
    // ImGui::PushFont(io.Fonts->Fonts[2]);
    // ImGui::Text("$%i", i.data.value);
    // ImGui::PopFont();
    
    // Upgrades
    ImGui::EndChild();
    ImGui::SameLine();
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
      // g_current_ship_slot = &(i.slot1);
      // g_current_ship = &i;
    }
    if (im_is_button_disabled()) im_enable_buttons();
    ImGui::SameLine();

    if (i.slot2) im_disable_buttons();
    if (ImGui::Button("2")) 
    {
      g_gameData.page = UpgradeShip;
      // g_current_ship_slot = &(i.slot2);
      // g_current_ship = &i;
    }
    if (im_is_button_disabled()) im_enable_buttons();
    ImGui::SameLine();
    
    if (i.slot3) im_disable_buttons();
    if (ImGui::Button("3")) 
    {
      g_gameData.page = UpgradeShip;
      // g_current_ship_slot = &(i.slot3);
      // g_current_ship = &i;
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
    ImGui::Text("Trip cost $%i", i.data.value);
    ImGui::Text("Cargo capacity %i", i.data.capacity);
    ImGui::PopFont();
    ImGui::EndChild();
    ImGui::SameLine();

    std::string title_status = "child_status_" + boost::uuids::to_string(i.tag);
    ImGui::BeginChild(
      title_status.c_str(), 
      ImVec2(220, 0), 
      true, 
      WINDOW_FLAGS_BORDERLESS);   
    // if (i.data.location == 0)   
    //   ImGui::Text("Ready");
    // else
    //   ImGui::Text("In Transit...");
    if (i.data.location == 0)   
      ImGui::Text("Ready");
    else if (i.data.location == 1)   
      ImGui::Text("Going up");
    else if (i.data.location == 2)
      ImGui::Text("Return");
    else
      ImGui::Text("Something...");      
    ImGui::EndChild();
    ImGui::SameLine();

    std::string title_order = "child_order_" + boost::uuids::to_string(i.tag);
    ImGui::BeginChild(
      title_order.c_str(), 
      ImVec2(0, 0), 
      true, 
      WINDOW_FLAGS_BORDERLESS);   

    if (i.order.pickup_amount == 0)
    {
      std::string title_button_order = "Order##" + boost::uuids::to_string(i.tag);
      if (ImGui::Button(title_button_order.c_str()))
      {
        toggle_button = !toggle_button;
        which_order = &i;
      }
    }
    else
    {
      // ImGui::Text("Order in place");
      // ImGui::SameLine();
      std::string title_button_order = "Change##" + boost::uuids::to_string(i.tag);
      if (ImGui::Button(title_button_order.c_str()))
      {
        toggle_button = !toggle_button;
        which_order = &i;
      }
    }
    ImGui::SameLine();
    if (ImGui::Checkbox("Repeat", &i.order.repeat))
    {

    }

    if (i.order.pickup_amount > 0)
    {
      ImGui::Text("Order placed");
    }

    ImGui::EndChild(); // order

    ImGui::EndChild(); // Fleet item
  }
  ImGui::EndChild(); // Fleet container

  // Popups
    if (toggle_button)
    {
      ImGui::OpenPopup("order");
    }
    else
    {
      ImGui::CloseCurrentPopup();
    }
    
    if (ImGui::BeginPopupModal("order", &toggle_button, ImGuiWindowFlags_AlwaysAutoResize))
    {
      // ImGui::PushFont(io.Fonts->Fonts[1]);
      // ImGui::Text("Order");
      // ImGui::PopFont();
      // ImGui::Separator();
      
      ImGui::Text("How much?");
      
      ImGui::PushFont(io.Fonts->Fonts[2]);
      ImGui::Text("Capacity %i", which_order->data.capacity);
      ImGui::PopFont();

      static bool s_repeat = false;
      ImGui::Checkbox("Repeat", &s_repeat);

      if (ImGui::Button("Collect from Joe's"))
      {
        toggle_button = false;
        which_order->order = (Order){ 1, which_order->data.capacity, 1, s_repeat };
      }       

      if (ImGui::Button("Do nothing"))
      {
        toggle_button = false;
        which_order->order = (Order){ 0, 0, 0 };
      }        

      ImGui::EndPopup();
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
    if (ImGui::Button("Free Play"))
    {
      //s_dialogManager.dialog.talkEvents.clear();
      inited = true;
      g_gameData.page = Home;
    }
    //ImGui::End();
  }
}


void mining_page()
{
  // not here... draw_mining_scene(g_rs);
}
