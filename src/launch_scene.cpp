#include "launch_scene.h"
#include "RenderSystem.h"
#include "Mesh.h"
#include "system_globals.h"
#include "Random.h"
#include "game_globals.h"


class LaunchScene
{
public:
  bool init();
  void draw(RenderSystem* rs);

protected:
  bool is_init = false;
  Mesh* rocket_mesh;
  Mesh* platform_mesh;
  Mesh* square_mesh;
  unsigned int platform_texture;
  unsigned int rocket_texture[3];
  unsigned int puff_texture[25];
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


bool LaunchScene::init()
{
  if (is_init) return true;

  Random::init();

  unsigned int handle;
  g_rm.getResource("rocket", handle); rocket_mesh = (Mesh*)handle;
  g_rm.getResource("launch platform", handle); platform_mesh = (Mesh*)handle;
  g_rm.getResource("square", handle); square_mesh = (Mesh*)handle;

  g_rm.getResource("launch_texture", platform_texture);
  g_rm.getResource("mtl1", rocket_texture[0]);
  g_rm.getResource("mtl3", rocket_texture[1]);
  g_rm.getResource("mtl4", rocket_texture[2]);

  for (int i = 0; i < 25; ++i)
  {
    std::string resource = std::string("whitePuff") + ZeroPadNumber(i);
    g_rm.getResource(resource.c_str(), puff_texture[i]);
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
  // Some gl setting
  glActiveTexture(GL_TEXTURE0);
  
  // Platforms
  glBindTexture(GL_TEXTURE_2D, platform_texture);
  for (int i = -1; i < 2; ++i)
  {
    for (int j = 0; j < 2; ++j)
    {
      // Set local
      glm::mat4 x(1.0);
      x = glm::translate(x, 7.f * glm::vec3(-1.15*(i-j), -1.15*-i, 0));
      rs->setModelLocal(x);

      // Render
      rs->bindMesh(platform_mesh);
      rs->bindMeshElement(platform_mesh, 0);
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
      rs->bindMesh(rocket_mesh);
      for (int i = 0; i < rocket_mesh->m_matIboElements.size(); ++i)
      {
        glBindTexture(GL_TEXTURE_2D, rocket_texture[i]);
        rs->bindMeshElement(rocket_mesh, i);
        rs->drawMesh();
      }

  // Smoke
  glDepthMask(false);
  if (g_gameData.fleet[0].order.pickup_amount > 0)
  {
    for (int i = 0; i < 25; ++i)
    {
      glm::mat4 x(1.0);

      x = glm::translate(x, puff_locale[i]);

      x = glm::rotate(x, 0.5f * (float)M_PI, glm::vec3(1.f, 0.f, 0.f));
      x = glm::rotate(x, 0.25f * (float)M_PI, glm::vec3(0.f, 0.f, 1.f));

      x = glm::scale(x, glm::vec3(2.f, 2.f, 2.f));
      rs->setModelLocal(x);

      glBindTexture(GL_TEXTURE_2D, puff_texture[i]);
      rs->bindMesh(square_mesh);
      rs->bindMeshElement(square_mesh, 0);
      rs->drawMesh();  
    }
  }
  glDepthMask(true);
}