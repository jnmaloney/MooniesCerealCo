#include "system_globals.h"


static WindowManager s_windowManager;
static MenuManager s_menuManager;
static ResourceManager s_resourceManager;


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