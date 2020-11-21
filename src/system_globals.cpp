#include "system_globals.h"
#include <sstream>
#include <iomanip>
#include "RenderSystem.h"


static RenderSystem* s_renderSystem = 0;
static WindowManager s_windowManager;
static MenuManager s_menuManager;
static ResourceManager s_resourceManager;


RenderSystem* gs_renderSystem()
{
  if (s_renderSystem == 0)
  {
    s_renderSystem = new RenderSystem();
  }
  return s_renderSystem;
}


WindowManager& gs_windowManager()
{
  return s_windowManager;
}


MenuManager& gs_menuManager()
{
  return s_menuManager;
}

ResourceManager& gs_resourceManager()
{
  return s_resourceManager;
}


std::string ZeroPadNumber(int num)
{
    std::ostringstream ss;
    ss << std::setw( 2 ) << std::setfill( '0' ) << num;
    return ss.str();
}


