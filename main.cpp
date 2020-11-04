#include <stdio.h>
#include "graphics.h"
#include "RenderSystem.h"
#include "RenderQueue.h"
#include "WindowManager.h"
#include "MenuManager.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <emscripten/html5.h>
#endif
#include "ImGuizmo.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <algorithm>
//#include <format>
#include "imgui_internal.h" //itemflags

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>


RenderSystem* g_rs = 0;
WindowManager g_windowManager;
MenuManager g_menuManager;

glm::mat4 cameraView, cameraProjection, identityMatrix, objectMatrix;
int gizmoCount = 1;
float camDistance = 12.0;

void drawHeaderBar();
void drawFooterBar();

struct WeekData
{
  int moon_rocks_collected;
  int moon_rocks_total;
  int moon_rock_storage_cap;
  int processing_rate;
  int spent;
  int sales;
};
std::vector<WeekData> g_econ_history;
WeekData g_current_week_data;

struct Order
{
  int pickup_location;
  int pickup_amount;
  int cost;
};
Order nil_order = (Order){ 0, 0, 0 };

struct ShipData
{
  float transit_time;
  int capacity;
  int value;
  int location;
};
struct Ship
{
  Ship(ShipData a_data) : tag(boost::uuids::random_generator()()), data(a_data) {} 
  ShipData data;
  Order order = nil_order;
  boost::uuids::uuid tag;// = boost::uuids::random_generator()();
  int slot1 = 0;
  int slot2 = 0;
  int slot3 = 0;
};
std::vector<Ship> g_fleet;

struct Launch
{
  Launch(const Launch& other) = default;//: ship(other.ship), order(other.order) { }
  //Launch& operator=(const Launch&) = default;
  Launch& operator=(const Launch&) { return *this; };
  //Launch& operator=(const Launch&);
  Ship& ship;
  Order order;
  float time_of_launch;
  float time_of_pickup;
  float time_of_return;
  bool picked_up;
  bool returned;
};
//Launch& Launch::operator=(const Launch&) = default;
std::vector<Launch> g_launches;

int g_week_counter = 0;
float g_time = 0.f;

int g_cash = 39000;

enum PAGES
{
  Home = 0,
  Launchpad,
  Mining,
  Econ,
  BuyFleet,
  Processing,
  UpgradeShip,
};
PAGES g_page;


int fulfil(int location, int amount)
{
  int unit_cost = 10;
  if ((g_week_counter + 2) % 4 == 0) unit_cost = 5;

  // always succeed
  int given_amount = amount;
  int given_cost = amount * unit_cost;
  return given_cost;

  // if (given_cost > g_cash)
  // {
  //   given_amount = 0;
  //   given_cost = 0;
  // }
}


void pickup_cargo(Order& order)
{
  // ?? needs to adjust "cargo in transit"
  int cost = fulfil(order.pickup_location, order.pickup_amount);
  g_current_week_data.spent += cost; 
}


void return_cargo(Order& order)
{
  g_current_week_data.moon_rocks_collected += order.pickup_amount;  
}


void process_puffs()
{
  int process_rate = 1600;
  int processed;
  if (process_rate > g_current_week_data.moon_rocks_total)
  {
    processed = g_current_week_data.moon_rocks_total;
  }
  else
  {
    processed = process_rate;
  }
  
  g_current_week_data.processing_rate = processed;
  
  g_current_week_data.moon_rocks_total -= processed;
}


void sell_puffs()
{
  g_current_week_data.sales = 25 * g_current_week_data.processing_rate;
}


int* g_current_ship_slot;
Ship* g_current_ship = 0;
void upgrade_ship(int upgrade, int cost)
{
  //g_current_week_data.spent += cost;
  g_cash -= cost;

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


void end_week()
{
  // Create the order list
  for (auto& i: g_fleet)
  {
    if (i.data.location == 0) // is grounded
    {
      if (i.order.pickup_amount > 0)
      {
        g_launches.push_back(
          (Launch){ i, i.order, g_time, g_time + i.data.transit_time, g_time + 2 * i.data.transit_time, false, false }
        );
        g_current_week_data.spent += i.data.value; // ?? launch_cost
        i.data.location = 1;
      }
    }
  }

  //
  // TIME-FRAME NOW ADVENCES
  //
  g_week_counter += 1;
  g_time += 7.0f * (float)g_week_counter;

  // Do all pickup orders in next time frame
  for (auto& i : g_launches)
  {
    if (!i.picked_up && i.time_of_pickup <= g_time)
    {
      pickup_cargo(i.order);
      i.picked_up = true;
    }
  }

  // Do all cargo returns in next time frame
  for (auto& i : g_launches)
  {
    if (!i.returned && i.time_of_return <= g_time)
    {
      return_cargo(i.order);
      i.returned = true;
      i.ship.data.location = 0;
      i.ship.order = nil_order; // ?
    }
  }
  g_launches.erase(std::remove_if(g_launches.begin(),
                                  g_launches.end(),
                                  [](decltype(g_launches)::value_type const& elem) 
                                  {
                                    return elem.returned;
                                  }),
                                  g_launches.end());

  // Apply collection
  g_current_week_data.moon_rocks_total += g_current_week_data.moon_rocks_collected;

  // Process moon rocks into moon puffs
  process_puffs();

  // Sell moon puffs
  sell_puffs();

  // Apply cash
  g_cash -= g_current_week_data.spent; 
  g_cash += g_current_week_data.sales;

  // Done
  g_econ_history.push_back(g_current_week_data);
  g_current_week_data = {
    0,
    g_current_week_data.moon_rocks_total,
    11,
    0,
    0,
    0,
  };
}


    std::vector<Ship> fleets = {
      Ship((ShipData){ 3.49, 1000, 35000, 0 }),
      Ship((ShipData){ 4.0, 1500, 70000, 0 }),
      Ship((ShipData){ 3.2, 800, 75000, 0}),
      Ship((ShipData){ 9.0, 7500, 900000, 0 }),
    };  



void drawHeaderBar()
{
  ImGui::BeginChild(
    "Header", 
    ImVec2(0, 84), 
    true, 
    ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDecoration);   
  
  ImGui::Text("$%i", g_cash);

  ImGui::SetCursorPosX(g_windowManager.width / 2 - 50);
  ImGui::SetCursorPosY(0);
  ImGui::Text("Week %i", g_week_counter + 1);

  ImGui::SetCursorPosX(g_windowManager.width - 500);
  ImGui::SetCursorPosY(0);
  ImGui::Text("Moonies Cereal Co");

  ImGui::EndChild();
}


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


void drawFooterBar()
{
  ImGui::SetNextWindowPos(ImVec2(0, g_windowManager.height - 84));
  ImGui::BeginChild(
    "Footer", 
    ImVec2(0, 84), 
    true, 
    ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDecoration);   
  
  if (g_page == Home) im_disable_buttons();
  if (ImGui::Button("Home")) g_page = Home;
  if (s_buttons_disabled) im_enable_buttons();
  ImGui::SameLine();

  if (g_page == Econ) im_disable_buttons();
  if (ImGui::Button("Econ")) g_page = Econ;
  if (s_buttons_disabled) im_enable_buttons();
  ImGui::SameLine();

  if (g_page == Launchpad) im_disable_buttons();
  int i = 0;
  for (auto& j : g_fleet) if (j.data.location == 0 && j.order.pickup_amount == 0) ++i;
  //std::string name = std::format("({}) Launchpad", i)
  std::string name = std::string("(") + std::to_string(i) + std::string(") Launchpad");
  if (ImGui::Button(name.c_str())) g_page = Launchpad;
  if (s_buttons_disabled) im_enable_buttons();
  ImGui::SameLine();

  if (g_page == Processing) im_disable_buttons();
  if (ImGui::Button("Processing")) g_page = Processing;
  if (s_buttons_disabled) im_enable_buttons();
  ImGui::SameLine();

  if (g_page == Mining) im_disable_buttons();
  if (ImGui::Button("Mining")) g_page = Mining;
  if (s_buttons_disabled) im_enable_buttons();

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
  
  if (ImGui::Button("Back")) { g_page = page; }

  ImGui::EndChild();
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


void econ_page()
{  
  // ImGui::Text("moon_rocks_collected"); ImGui::SameLine();
  // ImGui::Text("moon_rocks_total"); ImGui::SameLine();
  // //ImGui::Text(""); ImGui::SameLine();
  // ImGui::Text("processing_rate"); ImGui::SameLine();
  // ImGui::Text("spent"); ImGui::SameLine();
  // ImGui::Text("sales");

  // for (auto i : g_econ_history)
  // {
  //   ImGui::Text("%i", i.moon_rocks_collected); ImGui::SameLine();
  //   ImGui::Text("%i", i.moon_rocks_total); ImGui::SameLine();
  //   //ImGui::Text("%i", i.); ImGui::SameLine();
  //   ImGui::Text("%i", i.processing_rate); ImGui::SameLine();
  //   ImGui::Text("$%i", i.spent); ImGui::SameLine();
  //   ImGui::Text("$%i", i.sales);
  // }

  float cash[4];
  float stock[4];
  for (int i = 0; i < 4; ++i)
  {
    ImGui::SetCursorPosY(0);
    if (i < g_econ_history.size())
    {
      ImGui::SetCursorPosX(i * 160);
      ImGui::Text("%i", g_econ_history[i].moon_rocks_collected);
      ImGui::SetCursorPosX(i * 160);
      ImGui::Text("%i", g_econ_history[i].moon_rocks_total);
      ImGui::SetCursorPosX(i * 160);
      ImGui::Text("%i", g_econ_history[i].processing_rate);
      ImGui::SetCursorPosX(i * 160);
      ImGui::Text("$%i", g_econ_history[i].spent);
      ImGui::SetCursorPosX(i * 160);
      ImGui::Text("$%i", g_econ_history[i].sales);

      cash[3 - i] = g_econ_history[i].sales;
      stock[3 - i] = g_econ_history[i].moon_rocks_total;
    }
    else
    {
      cash[3 - i] = 39000;
      stock[3 - i] = 0;
    }
  }

  ImGui::SetCursorPosX(640);
  ImGui::SetCursorPosY(0);
  ImGui::PlotLines("Income", cash, IM_ARRAYSIZE(cash), 0, "Income", -1.0f, 1.0f, ImVec2(0, 280.0f));
      
  ImGui::SetCursorPosX(640);
  ImGui::SetCursorPosY((g_windowManager.height - 2 * 84) / 2);     
  ImGui::PlotLines("Stock", stock, IM_ARRAYSIZE(stock), 0, "Stock", -1.0f, 1.0f, ImVec2(0, 280.0f));
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
    g_page = Launchpad;
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
    g_page = Launchpad;
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
    g_page = Launchpad;
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
    g_page = Launchpad;
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
    g_page = Launchpad;
  }
  ImGui::Text("Reduces damage to the");
  ImGui::Text("ship, requiring fewer");
  ImGui::Text("repairs.");
  ImGui::Text("$4 000");
  ImGui::EndChild();
}


void loop()
{
  ImGuiIO& io = ImGui::GetIO();

  g_menuManager.predraw();

	ImGuizmo::BeginFrame();
  
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
  bool open = true;
	ImGui::Begin("Editor", &open, FULL_SCREEN_FLAGS);

  if (g_page == Home)
  {
    drawHeaderBar();
    
    ImGui::Text("Home");
    // if (ImGui::Button("Mining")) { g_page = Mining; }
    // if (ImGui::Button("Launch Pad")) { g_page = Launchpad; }
    if (ImGui::Button("Next ->")) { end_week(); }

    if (g_week_counter == 0)
    {
      ImGui::Text("Um..");
      ImGui::Text("Welcome to the cereal factory. It's");
      ImGui::Text("your first day.");
      ImGui::Text("Place some orders, before clicking");
      ImGui::Text("next.");
    }
    else
    {
      ImGui::Text("Cash: $%i", g_cash);
      ImGui::Text("Moon rock collected this week: %i", g_current_week_data.moon_rocks_collected);
      ImGui::Text("%i / %i moon rock in storage", g_current_week_data.moon_rocks_total, g_current_week_data.moon_rock_storage_cap);
      ImGui::Text("%i processed to moon puffs this week", g_current_week_data.processing_rate);
      ImGui::Text("Spent: $%i", g_current_week_data.spent);
      ImGui::Text("Sales: $%i", g_current_week_data.sales); 
    }
    



    drawFooterBar();
  }

  else if (g_page == BuyFleet)
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
        g_cash -= to_buy->data.value;
        g_fleet.push_back(*to_buy);
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

  else if (g_page == Launchpad)
  {
    ImGui::Text("$%i", g_cash);

    // Title
    ImGui::PushFont(io.Fonts->Fonts[1]);
    ImGui::Text("Launchpad");
    ImGui::PopFont();

    ImGui::SameLine();
    if (ImGui::Button("Buy More Fleet..."))
    {
      g_page = PAGES::BuyFleet;
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

    for (auto& i : g_fleet)
    {
      static int WINDOW_FLAGS = 0;
      static int WINDOW_FLAGS_BORDERLESS = ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDecoration;
  
      std::string title = "child_" + boost::uuids::to_string(i.tag);
      //ImGui::SetNextWindowSize(ImVec2(g_windowManager.width, 220));
      ImGui::BeginChild(
        title.c_str(), 
        ImVec2(g_windowManager.width - 20, 130), 
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
        g_page = UpgradeShip;
        g_current_ship_slot = &(i.slot1);
        g_current_ship = &i;
      }
      if (s_buttons_disabled) im_enable_buttons();
      ImGui::SameLine();

      if (i.slot2) im_disable_buttons();
      if (ImGui::Button("2")) 
      {
        g_page = UpgradeShip;
        g_current_ship_slot = &(i.slot2);
        g_current_ship = &i;
      }
      if (s_buttons_disabled) im_enable_buttons();
      ImGui::SameLine();
      
      if (i.slot3) im_disable_buttons();
      if (ImGui::Button("3")) 
      {
        g_page = UpgradeShip;
        g_current_ship_slot = &(i.slot3);
        g_current_ship = &i;
      }
      if (s_buttons_disabled) im_enable_buttons();
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
      if (i.data.location == 0)   
        ImGui::Text("Ready");
      else
        ImGui::Text("In Transit...");
      ImGui::EndChild();
      ImGui::SameLine();

      std::string title_order = "child_order_" + boost::uuids::to_string(i.tag);
      ImGui::BeginChild(
        title_order.c_str(), 
        ImVec2(0, 0), 
        true, 
        WINDOW_FLAGS);   

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
        ImGui::Text("Order in place");
        ImGui::SameLine();
        std::string title_button_order = "Change##" + boost::uuids::to_string(i.tag);
        if (ImGui::Button(title_button_order.c_str()))
        {
          toggle_button = !toggle_button;
          which_order = &i;
        }
      }

      ImGui::EndChild(); // order

      ImGui::EndChild(); // Fleet item
    }
    ImGui::EndChild(); // Fleet container

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

        if (ImGui::Button("Collect from Joe's"))
        {
          toggle_button = false;
          which_order->order = (Order){ 1, which_order->data.capacity, 1 };
        }       

        if (ImGui::Button("Do nothing"))
        {
          toggle_button = false;
          which_order->order = (Order){ 0, 0, 0 };
        }        

        ImGui::EndPopup();
      }

    // if (ImGui::Button("Buy More Fleet..."))
    // {
    //   g_page = PAGES::BuyFleet;
    // }
    // ImGui::SameLine();
    //if (ImGui::Button("Back")) { g_page = Home; }
    drawFooterBar();
  }

  else if (g_page == Mining)
  {
    drawHeaderBar();
    drawContent(NULL);
    drawFooterBar();
  }

  else if (g_page == Processing)
  {
    drawHeaderBar();
    drawContent(NULL);
    drawFooterBar();
  } 
  else if (g_page == Econ)
  {
    drawHeaderBar();
    drawContent(&econ_page);
    drawFooterBar();
  }
  else if (g_page == UpgradeShip)
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

  // Fonts
  ImGuiIO& io = ImGui::GetIO();

  io.Fonts->Clear();
  io.Fonts->AddFontFromFileTTF("/data/font/Readable9x4.ttf", 72);
  io.Fonts->AddFontFromFileTTF("/data/font/Readable9x4.ttf", 96);
  io.Fonts->AddFontFromFileTTF("/data/font/Readable9x4.ttf", 28);
  io.Fonts->Build();

  // Style
  ImGuiStyle& style = ImGui::GetStyle();
  style.WindowPadding.x = 0;
  style.WindowPadding.y = 0;
  style.FramePadding.x = 20;
  style.FramePadding.y = 8;

  //style.Colors[ImGuiCol_WindowBg] = ImColor(0x01, 0x10, 0x14, 0xff); // Rich black FOGRA
  style.Colors[ImGuiCol_WindowBg] = ImColor(115, 140, 253, 0xff);
  style.Colors[ImGuiCol_Text] = ImColor(0xffffffff);
  style.Colors[ImGuiCol_Button] = ImColor(0x04, 0x3E, 0x4E, 0xff); // Midnight Green Eagle Green
  style.Colors[ImGuiCol_ButtonHovered] = ImColor(0x07, 0x6C, 0x88,  0xff); // Blue Sapphire
  style.Colors[ImGuiCol_ButtonActive] = ImColor(0x0A, 0x9A, 0xC2, 0xff); // Blue Green

  // Initialiser
  g_fleet.push_back(Ship((ShipData){ 3.4f, 2000, 18000, 0 }));

  #ifdef __EMSCRIPTEN__
  emscripten_set_main_loop(loop, 0, 1);
  #endif

  glfwTerminate();

  return 0;
}
