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
  
};