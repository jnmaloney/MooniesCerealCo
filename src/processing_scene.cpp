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
  
  void draw_conveyor(RenderSystem* rs, glm::mat4 x, game_globals::Conveyor& conveyor_object);

  bool is_init = false;
  Mesh* conveyor_mesh;
  Mesh* conveyor_belt_mesh;
  Mesh* stuff;
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
  g_rm.getResource("square", (void**)&stuff);

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

  glm::mat4 x(1.0);
  x = glm::translate(x, glm::vec3(9.f, 0.f, -2.5f));
  for (auto& i : g_gameData.processing.conveyors)
  {
    draw_conveyor(rs, x, i);
    x = glm::translate(x, glm::vec3(-8.f, 0.f, 0.f));
  }
}


void ProcessingScene::draw_conveyor(RenderSystem* rs, glm::mat4 t, game_globals::Conveyor& conveyor_object)
{
  // Set local
  glm::mat4 x = glm::rotate(t, (float)M_PI, glm::vec3(0.f, 0.f, 1.f));
  rs->setModelLocal(x);

  // Conveyor
  glBindTexture(GL_TEXTURE_2D, conveyor_texture[0]);

  // Render
  g_rs->bindMesh(conveyor_mesh);
  for (int i = 0; i < conveyor_mesh->m_matIboElements.size(); ++i)
  {
    g_rs->bindMeshElement(conveyor_mesh, i);
    g_rs->drawMesh();
  }

  // Belt
  // rs->m_shaderManager.setShader("Scrolling");
  bool scrolling = conveyor_object.timings.size() || conveyor_object.stock;
  float scroll_amount = scrolling ? 0.5 * 1.0f/60.0f : 0.0f;
  conveyor_object.scroll = fmod(scroll_amount + conveyor_object.scroll, 1.0f);
  rs->m_shaderManager.setParam("Scrolling", "scroll", conveyor_object.scroll);

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

  // Stuff on the conveyor belt
  // (1, 9) -> (1, -4)

  static float f = 13.0f;
  //static float offset = 0.f;
  //offset = fmod(offset + 0.10/60.0 * 2.0f, f);

  rs->bindMesh(stuff);
  rs->bindMeshElement(stuff, 0);

  //for (int i = 0; i < conveyor_object.timings.size(); ++i)
  for (auto& i : conveyor_object.timings)
  {
    glm::mat4 y(t);

    //printf("%.1f\n", ((float)i - (float)g_gameData.time_tick));
    
    float offset = 4.f - f * ((float)i - (float)g_gameData.time_tick) / (float)conveyor_object.conveying_time;
    y = glm::translate(y, glm::vec3(0.f, offset, 1.5f));
    
    y = glm::rotate(y, (float)M_PI*0.5f, glm::vec3(0.f, 1.f, 0.f));
    rs->setModelLocal(y);
 
    rs->drawMesh();
  }
}
