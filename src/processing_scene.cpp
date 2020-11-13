#include "processing_scene.h"
#include "RenderSystem.h"
#include "Mesh.h"
#include "Mesh_obj.h"
#include "Texture.h"
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
  Mesh_obj conveyor_mesh;
  Mesh_obj conveyor_belt_mesh;
  Texture conveyor_texture[3];
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

  conveyor_mesh.load_obj("data/objects/conveyor/conveyor bake.obj");
  conveyor_mesh.upload();
  conveyor_belt_mesh.load_obj("data/objects/conveyor/conveyor belt isolated.obj");
  conveyor_belt_mesh.upload();

  conveyor_texture[0].loadPng("data/objects/conveyor/conveyor_tile.png");
  conveyor_texture[1].loadPng("data/objects/conveyor/conveyor_side.png");
  conveyor_texture[2].loadPng("data/objects/conveyor/ao_target.png");


  is_init = true;
  return true;
}


void ProcessingScene::draw(RenderSystem* rs)
{
  // Camera
  // glm::vec3 camera_eye(100, 70, 100);
  // glm::vec3 centre(0, 0, 0);
  // glm::vec3 up(0, 1, 0);
  // //glm::vec3 up(0, 0, 1);
  // glm::mat4 cameraView = glm::lookAt(camera_eye, centre, up);
  // glm::mat4 cameraProjection = glm::perspective(glm::radians(14.5f), (float)g_windowManager.width / (float)g_windowManager.height, 1.0f, 200.0f);
  // rs->setViewProj(cameraProjection * cameraView);

  // Conveyor
  conveyor_texture[2].bind();

  // Set local
  glm::mat4 x(1.0);
  x = glm::rotate(x, (float)M_PI, glm::vec3(0.f, 0.f, 1.f));
  rs->setModelLocal(x);

  // Render
  rs->bindMesh(&conveyor_mesh);
  for (int i = 0; i < conveyor_mesh.m_matIboElements.size(); ++i)
  {
    rs->bindMeshElement(&conveyor_mesh, i);
    rs->drawMesh();
  }

  // Belt
  rs->bindMesh(&conveyor_belt_mesh);

  conveyor_texture[0].bind();
  rs->bindMeshElement(&conveyor_belt_mesh, 0);
  rs->drawMesh();

  conveyor_texture[1].bind();
  rs->bindMeshElement(&conveyor_belt_mesh, 1);
  rs->drawMesh();
}