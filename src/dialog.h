#pragma once


#include <list>
#include <string>


class Dialog
{
public:
  Dialog();
  ~Dialog();

  struct TalkEvent
  {
    std::string name;
    int side;
    int expression;
    std::string text;
  };

  std::list<TalkEvent> talkEvents;

  void load(const char* filename);

  void next();

};