#pragma once


#include "dialog.h"
#include <map>
#include <string>


class DialogManager
{
public:
  DialogManager() = default;
  ~DialogManager() = default;

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