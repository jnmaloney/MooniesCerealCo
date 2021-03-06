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
  Mesh* energy_mesh;
  unsigned int mining_texture;
  unsigned int energy_texture;
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
  g_rm.getResource("mining energy", (void**)&energy_mesh);

  g_rm.getResource("mining body(tex)", mining_texture);
  g_rm.getResource("mining energy(tex)", energy_texture);

  is_init = true;
  return true;
}


void MiningScene::draw(RenderSystem* rs)
{
  rs->m_shaderManager.setParam("Scrolling", "scroll", 0.0f);
  // Some gl setting
  glActiveTexture(GL_TEXTURE0);

  if (g_gameData.mine_miner)
  {      
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
  }

  if (g_gameData.mine_power)
  {
    glBindTexture(GL_TEXTURE_2D, energy_texture);

    // Set local
    glm::mat4 y(1.0);
    y = glm::translate(y,  glm::vec3(-14.f, 4.f, -1.f));
    y = glm::scale(y, glm::vec3(1.5f, 1.5f, 1.5f));
    y = glm::rotate(y, (float)M_PI, glm::vec3(0.f, 0.f, 1.f));
    rs->setModelLocal(y);

    // Render
    rs->bindMesh(energy_mesh);
    rs->bindMeshElement(energy_mesh, 0);
    rs->drawMesh();
  }

  if (g_gameData.mine_silo)
  {
    glBindTexture(GL_TEXTURE_2D, energy_texture);

    // Set local
    glm::mat4 y(1.0);
    y = glm::translate(y,  glm::vec3(-14.f, -4.f, -1.f));
    y = glm::scale(y, glm::vec3(1.5f, 1.5f, 1.5f));
    y = glm::rotate(y, -0.5f * (float)M_PI, glm::vec3(0.f, 0.f, 1.f));
    rs->setModelLocal(y);

    // Render
    rs->bindMesh(energy_mesh);
    rs->bindMeshElement(energy_mesh, 0);
    rs->drawMesh();
  }
}