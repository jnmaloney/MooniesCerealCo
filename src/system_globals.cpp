#include "system_globals.h"


static WindowManager s_windowManager;
static MenuManager s_menuManager;


WindowManager& gs_windowManager()
{
  return s_windowManager;
}


MenuManager& gs_menuManager()
{
  return s_menuManager;
}
