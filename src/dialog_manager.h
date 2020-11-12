#pragma once


#include "dialog.h"
#include "Texture.h"
#include <map>
#include <string>


class DialogManager
{
public:
  DialogManager() = default;
  ~DialogManager() = default;

  struct Character
  {
    Texture texture;
  };
  std::map<std::string, Character> characters;

  void init();

  void draw();

  Dialog dialog;  

  struct Transition
  {
    int in_out;
    float t;
  };
  Transition transition;
};