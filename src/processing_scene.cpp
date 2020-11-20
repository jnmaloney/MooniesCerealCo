#include "processing_scene.h"
#include "RenderSystem.h"
#include "Mesh.h"
#include "system_globals.h"
#include <sstream>
#include <iomanip>
#include "Random.h"
#include "game_globals.h"


class ProcessingScene
{
public:
  bool init();
  void draw(RenderSystem* rs);

protected:
  bool is_init = false;
  Mesh* conveyor_mesh;
  Mesh* conveyor_belt_mesh;
  unsigned int conveyor_texture[3];
};


void draw_processing_scene(RenderSystem* rs)
{
  static ProcessingScene s_processing_scene;
  if (s_processing_scene.init())
  {
    s_processing_scene.draw(rs);
  }
}


bool ProcessingScene::init()
{
  if (is_init) return true;

  g_rm.getResource("conveyor body", (void**)&conveyor_mesh);
  g_rm.getResource("conveyor belt", (void**)&conveyor_belt_mesh);

  g_rm.getResource("conveyor body(tex)", conveyor_texture[0]);
  g_rm.getResource("conveyor belt(tex)", conveyor_texture[1]);
  g_rm.getResource("conveyor side(tex)", conveyor_texture[2]);

  inspect(conveyor_mesh);
  inspect(conveyor_belt_mesh);

  is_init = true;
  return true;
}


void ProcessingScene::draw(RenderSystem* rs)
{
  //rs->m_shaderManager.setShader("Default");
  rs->m_shaderManager.setParam("Scrolling", "scroll", 0.0f);

  // Some gl setting
  glActiveTexture(GL_TEXTURE0);

  // Conveyor
  glBindTexture(GL_TEXTURE_2D, conveyor_texture[0]);

  // Set local
  glm::mat4 x(1.0);
  x = glm::rotate(x, (float)M_PI, glm::vec3(0.f, 0.f, 1.f));
  rs->setModelLocal(x);

  // Render
  rs->bindMesh(conveyor_mesh);
  for (int i = 0; i < conveyor_mesh->m_matIboElements.size(); ++i)
  {
    rs->bindMeshElement(conveyor_mesh, i);
    rs->drawMesh();
  }

  // Belt
  static float s_scroll = 0.f;
  s_scroll = fmod(s_scroll + 1.0/60.f, 1.f);

  // rs->m_shaderManager.setShader("Scrolling");
  // rs->m_shaderManager.update(rs);
  rs->m_shaderManager.setParam("Scrolling", "scroll", s_scroll);

  rs->bindMesh(conveyor_belt_mesh);

  glBindTexture(GL_TEXTURE_2D, conveyor_texture[1]);
  rs->bindMeshElement(conveyor_belt_mesh, 0);
  rs->drawMesh();

  //rs->m_shaderManager.setShader("Default");
  rs->m_shaderManager.setParam("Scrolling", "scroll", 0.0f);

  // setshader("scrolling_uv");
  glBindTexture(GL_TEXTURE_2D, conveyor_texture[2]);
  rs->bindMeshElement(conveyor_belt_mesh, 1);
  rs->drawMesh();
}
