#pragma once

#include "graphics.h"
#include "RenderSystem.h"
#include "RenderQueue.h"
#include "WindowManager.h"
#include "MenuManager.h"



WindowManager& gs_windowManager();
MenuManager& gs_menuManager();


namespace 
{
RenderSystem* g_rs = 0;
WindowManager& g_windowManager = gs_windowManager();
MenuManager& g_menuManager = gs_menuManager();

glm::mat4 cameraView, cameraProjection, identityMatrix, objectMatrix;
int gizmoCount = 1;
float camDistance = 12.0;
}

void im_disable_buttons();
void im_enable_buttons();
bool im_is_button_disabled();