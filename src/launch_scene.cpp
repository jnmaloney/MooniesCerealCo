#include "launch_scene.h"
#include "RenderSystem.h"
#include "Mesh.h"
#include "Mesh_obj.h"
#include "Texture.h"
#include "system_globals.h"
#include <sstream>
#include <iomanip>
#include "Random.h"
#include "game_globals.h"


class LaunchScene
{
public:
  bool init();
  void draw(RenderSystem* rs);

protected:
  bool is_init = false;
  Mesh_obj rocket_mesh;
  Mesh_obj platform_mesh;
  Mesh_obj square_mesh;
  Texture platform_texture;
  Texture rocket_texture[3];
  Texture puff_texture[25];
  glm::vec3 puff_locale[25];
};


void draw_launch_scene(RenderSystem* rs)
{
  static LaunchScene s_launch_scene;
  if (s_launch_scene.init())
  {
    s_launch_scene.draw(rs);
  }
}


std::string ZeroPadNumber(int num)
{
    std::ostringstream ss;
    ss << std::setw( 2 ) << std::setfill( '0' ) << num;
    return ss.str();
}


bool LaunchScene::init()
{
  if (is_init) return true;

  Random::init();

  rocket_mesh.load_obj("data/objects/rocket/retro toy rocket.obj");
  rocket_mesh.upload();
  platform_mesh.load_obj("data/objects/rocket/launch_platform.obj");
  platform_mesh.upload();
  square_mesh.load_obj("data/square.obj");
  square_mesh.upload();

  platform_texture.loadPng("data/objects/rocket/launch_texture.png");
  rocket_texture[0].loadPng("data/objects/rocket/mtl4.png");
  rocket_texture[1].loadPng("data/objects/rocket/mtl3.png");
  rocket_texture[2].loadPng("data/objects/rocket/mtl1.png");

  for (int i = 0; i < 25; ++i)
  {
    puff_texture[i].loadPng((std::string("data/objects/White puff/whitePuff") + ZeroPadNumber(i) + std::string(".png")).c_str());
  }

  for (int i = 0; i < 25; ++i)
  {
    puff_locale[i].x = Random::uniform_circle(0.5f);
    puff_locale[i].y = Random::uniform_circle(0.5f);
    puff_locale[i].z = 2.0 * i / 25.f;
  }

  is_init = true;
  return true;
}


void LaunchScene::draw(RenderSystem* rs)
{
  // Camera
  // glm::vec3 camera_eye(100, 70, 100);
  // glm::vec3 centre(0, 0, 0);
  // glm::vec3 up(0, 1, 0);
  // //glm::vec3 up(0, 0, 1);
  // glm::mat4 cameraView = glm::lookAt(camera_eye, centre, up);
  // glm::mat4 cameraProjection = glm::perspective(glm::radians(14.5f), (float)g_windowManager.width / (float)g_windowManager.height, 1.0f, 200.0f);
  // rs->setViewProj(cameraProjection * cameraView);

  // Platforms
  platform_texture.bind();
  for (int i = -1; i < 2; ++i)
  {
    for (int j = 0; j < 2; ++j)
    {
      // Set local
      glm::mat4 x(1.0);
      x = glm::translate(x, 7.f * glm::vec3(-1.15*(i-j), -1.15*-i, 0));
      rs->setModelLocal(x);

      // Render
      rs->bindMesh(&platform_mesh);
      rs->bindMeshElement(&platform_mesh, 0);
      rs->drawMesh();
    }
  }

  // Rockets
  float rocket_height = 0;
  // if (g_gameData.days > 3.5)
  if (g_gameData.fleet[0].order.pickup_amount > 0) // && !g_gameData.fleet[0].data.location)
  {
    rocket_height = 2.0;
  }

      // Set local
      glm::mat4 x(1.0);
      x = glm::translate(x, glm::vec3(0, 0, rocket_height));
      rs->setModelLocal(x);

      // Render
      rs->bindMesh(&rocket_mesh);
      for (int i = 0; i < rocket_mesh.m_matIboElements.size(); ++i)
      {
        rocket_texture[i].bind();
        rs->bindMeshElement(&rocket_mesh, i);
        rs->drawMesh();
      }

  // Smoke
  glDepthMask(false);
  if (g_gameData.fleet[0].order.pickup_amount > 0)
  {
    // glm::vec3 scale;
    // glm::quat rotation;
    // glm::vec3 translation;
    // glm::vec3 skew;
    // glm::vec4 perspective;
    // glm::decompose(cameraView, scale, rotation, translation, skew, perspective);

    for (int i = 0; i < 25; ++i)
    {
      glm::mat4 x(1.0);

      x = glm::translate(x, puff_locale[i]);

      x = glm::rotate(x, 0.5f * (float)M_PI, glm::vec3(1.f, 0.f, 0.f));
      x = glm::rotate(x, 0.25f * (float)M_PI, glm::vec3(0.f, 0.f, 1.f));

      x = glm::scale(x, glm::vec3(2.f, 2.f, 2.f));
      rs->setModelLocal(x);

      puff_texture[i].bind();
      rs->bindMesh(&square_mesh);
      rs->bindMeshElement(&square_mesh, 0);
      rs->drawMesh();  
    }
  }
  glDepthMask(true);
}