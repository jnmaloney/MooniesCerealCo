#pragma once

#include "graphics.h"
#include "RenderSystem.h"
#include "RenderQueue.h"
#include "WindowManager.h"
#include "MenuManager.h"
#include "ResourceManager.h"



RenderSystem* gs_renderSystem();
WindowManager& gs_windowManager();
MenuManager& gs_menuManager();
ResourceManager& gs_resourceManager();


namespace 
{
RenderSystem* g_rs = gs_renderSystem();
WindowManager& g_windowManager = gs_windowManager();
MenuManager& g_menuManager = gs_menuManager();
ResourceManager& g_rm = gs_resourceManager();

glm::mat4 cameraView, cameraProjection, identityMatrix, objectMatrix;
float camDistance = 18.0;
}

void im_disable_buttons();
void im_enable_buttons();
bool im_is_button_disabled();


std::string ZeroPadNumber(int num);
