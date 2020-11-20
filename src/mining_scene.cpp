#include "mining_scene.h"
#include "RenderSystem.h"
#include "Mesh.h"
#include "system_globals.h"
#include <sstream>
#include <iomanip>
#include "Random.h"
#include "game_globals.h"


class MiningScene
{
public:
  bool init();
  void draw(RenderSystem* rs);

protected:
  bool is_init = false;
  Mesh* mining_mesh;
  Mesh* mining_cog_mesh;
  unsigned int mining_texture;
};


void draw_mining_scene(RenderSystem* rs)
{
  static MiningScene s_mining_scene;
  if (s_mining_scene.init())
  {
    s_mining_scene.draw(rs);
  }
}


bool MiningScene::init()
{
  if (is_init) return true;

  g_rm.getResource("mining body", (void**)&mining_mesh);
  g_rm.getResource("mining cog", (void**)&mining_cog_mesh);
  inspect(mining_mesh);
  inspect(mining_cog_mesh);

  g_rm.getResource("mining body(tex)", mining_texture);

  is_init = true;
  return true;
}


void MiningScene::draw(RenderSystem* rs)
{
  rs->m_shaderManager.setParam("Scrolling", "scroll", 0.0f);
  // Some gl setting
  glActiveTexture(GL_TEXTURE0);

  glBindTexture(GL_TEXTURE_2D, mining_texture);

  // Set local
  glm::mat4 x(1.0);
  //x = glm::rotate(x, (float)M_PI, glm::vec3(0.f, 0.f, 1.f));
  x = glm::scale(x, glm::vec3(3.f, 3.f, 3.f));
  rs->setModelLocal(x);

  // Render
  rs->bindMesh(mining_mesh);
  rs->bindMeshElement(mining_mesh, 0);
  rs->drawMesh();

  // Set local
  static float t = 0.f;
  t = fmod(t + 0.015f, 2.f * M_PI);

  glm::mat4 y(1.0);
  //y = glm::rotate(y, (float)M_PI, glm::vec3(0.f, 0.f, 1.f));
  y = glm::scale(y, glm::vec3(3.f, 3.f, 3.f));
  y = glm::translate(y, glm::vec3(0.f, 0.f, 0.1f * sinf(t)));
  rs->setModelLocal(y);

  // Render
  rs->bindMesh(mining_cog_mesh);
  rs->bindMeshElement(mining_cog_mesh, 0);
  rs->drawMesh();


}