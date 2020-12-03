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
  void draw_floor(RenderSystem* rs, glm::mat4 x);
  void draw_pile(glm::mat4 x, Mesh* pile_verts);

  bool is_init = false;
  Mesh* conveyor_mesh;
  Mesh* conveyor_belt_mesh;
  Mesh* conveyor_load_mesh;
  Mesh* conveyor_floor_mesh;
  Mesh* pile;
  Mesh* stuff;
  unsigned int conveyor_texture[7];
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
  g_rm.getResource("conveyor load", (void**)&conveyor_load_mesh);
  g_rm.getResource("conveyor floor", (void**)&conveyor_floor_mesh);
  g_rm.getResource("pile", (void**)&pile);
  g_rm.getResource("square", (void**)&stuff);

  g_rm.getResource("conveyor body(tex)", conveyor_texture[0]);
  g_rm.getResource("conveyor belt(tex)", conveyor_texture[1]);
  g_rm.getResource("conveyor side(tex)", conveyor_texture[2]);
  g_rm.getResource("conveyor load(tex)", conveyor_texture[3]);
  g_rm.getResource("conveyor floor 1(tex)", conveyor_texture[4]);
  g_rm.getResource("conveyor floor 2(tex)", conveyor_texture[5]);
  g_rm.getResource("puff", conveyor_texture[6]);

  is_init = true;
  return true;
}


void ProcessingScene::draw(RenderSystem* rs)
{
  //rs->m_shaderManager.setShader("Default");
  rs->m_shaderManager.setParam("Scrolling", "scroll", 0.0f);

  // Some gl setting
  glActiveTexture(GL_TEXTURE0);

  // glm::mat4 x(1.0);
  // x = glm::translate(x, glm::vec3(9.f, 0.f, -2.5f));
  // for (auto& i : g_gameData.processing.conveyors)
  // {
  //   draw_conveyor(rs, x, i);
  //   x = glm::translate(x, glm::vec3(-8.f, 0.f, 0.f));
  // }

  int current_floor = g_gameData.current_floor;

  glm::mat4 x(1.0);
  x = glm::translate(x, glm::vec3(9.f, 0.f, -2.5f));
  for (int i = 0; i < 4; ++i)
  {
    if (i < g_gameData.processing.conveyors[current_floor].size())
      draw_conveyor(rs, x, g_gameData.processing.conveyors[current_floor][i]);
    else
      draw_floor(rs, x);
    x = glm::translate(x, glm::vec3(-8.f, 0.f, 0.f));
  }
}


void ProcessingScene::draw_floor(RenderSystem* rs, glm::mat4 t)
{
  // // Set local
  // glm::mat4 x = glm::rotate(t, (float)M_PI, glm::vec3(0.f, 0.f, 1.f));
  // rs->setModelLocal(x);

  // glBindTexture(GL_TEXTURE_2D, conveyor_texture[4]);
  // g_rs->bindMesh(conveyor_floor_mesh);
  // g_rs->bindMeshElement(conveyor_floor_mesh, 0);
  // g_rs->drawMesh();
}


void ProcessingScene::draw_conveyor(RenderSystem* rs, glm::mat4 t, game_globals::Conveyor& conveyor_object)
{
  // Set local
  glm::mat4 x = glm::rotate(t, (float)M_PI, glm::vec3(0.f, 0.f, 1.f));
  rs->setModelLocal(x);

  // // Floor
  // glBindTexture(GL_TEXTURE_2D, conveyor_texture[5]);
  // g_rs->bindMesh(conveyor_floor_mesh);
  // g_rs->bindMeshElement(conveyor_floor_mesh, 0);
  // g_rs->drawMesh();

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
  float scroll_amount = g_gameData.timer_speed * (scrolling ? 0.5 * 1.0f/60.0f : 0.0f);
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
  glBindTexture(GL_TEXTURE_2D, conveyor_texture[6]);

  //for (int i = 0; i < conveyor_object.timings.size(); ++i)
  for (auto& i : conveyor_object.timings)
  {
    // glm::mat4 y(cameraView);
    // y[3].x = t[3].x;
    // y[3].y = t[3].y;
    // y[3].z = t[3].z;

    glm::mat4 y(t);
    
    float offset = 4.f - f * ((float)i - (float)g_gameData.time_tick) / (float)conveyor_object.conveying_time;
    //y = glm::translate(y, glm::vec3(0.f, offset, 1.5f));
    y = glm::translate(y, glm::vec3(0.f, offset, 2.0f));

    draw_pile(y, pile);
  }
}


void ProcessingScene::draw_pile(glm::mat4 x, Mesh* pile_verts)
{
  //printf("%i\n", pile_verts->m_vertP.size());
  for (auto& i : pile_verts->m_obj_vertices)
  {
    glm::mat4 y = glm::translate(x, 0.8f * glm::vec3(i));
    y = glm::rotate(y, 0.25f * (float)M_PI, glm::vec3(0.f, 0.f, 1.f));
    y = glm::rotate(y, 0.5f * (float)M_PI, glm::vec3(0.f, 1.f, 0.f));
    y = glm::scale(y, glm::vec3(0.2f));
    g_rs->setModelLocal(y);
    g_rs->bindMesh(stuff);
    g_rs->bindMeshElement(stuff, 0);
    g_rs->drawMesh();
  }
}
