#include "dialog.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>


Dialog::Dialog()
{

}



Dialog::~Dialog()
{

}



void Dialog::load(const char* filename)
{
  std::ifstream f;
  f.open(filename);
  std::string line;
  while (std::getline(f, line))
  {
    std::istringstream g = std::istringstream(line);
    std::string n, m;
    std::getline(g, n, ':');
    std::getline(g, m, '\n');

    talkEvents.push_back(
      (TalkEvent){ n, 0, 0, m }
    );   
  }
}


void Dialog::next()
{
  talkEvents.pop_front();
}