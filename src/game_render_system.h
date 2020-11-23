#pragma once
#include <algorithm>


namespace game_render_system
{
  
  extern void loop();
  extern void drawContent(std::function<void()> f_ptr);

}