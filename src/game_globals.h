#pragma once

#include "graphics.h"
#include "RenderSystem.h"
#include "RenderQueue.h"
#include "WindowManager.h"
#include "MenuManager.h"


RenderSystem* g_rs = 0;
WindowManager g_windowManager;
MenuManager g_menuManager;

glm::mat4 cameraView, cameraProjection, identityMatrix, objectMatrix;
int gizmoCount = 1;
float camDistance = 12.0;

void drawHeaderBar();
void drawFooterBar();