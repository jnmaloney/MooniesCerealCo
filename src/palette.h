#pragma once


#include <map>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>      // std::ifstream
#include "imgui.h"


namespace
{
std::map<std::string, ImColor> g_palette;


void load_palette(const char* filename)
{
  std::ifstream f;//(filename);
  f.open(filename, std::ifstream::in);

  for (std::string line; std::getline(f, line); ) 
  {
    std::string name;
    std::string col;
    __INT32_TYPE__ hex;

    if (line.length() <= 8) continue;

    name = line.substr(0, line.length() - 8);
    col = std::string("7F") + line.substr(line.length() - 6, 6);// + std::string("FF");
    hex = std::stoi(col, 0, 16);
    
    //float sc = 1.0f/255.0f;
    int b = (float)((hex>>IM_COL32_R_SHIFT)&0xFF);// * sc; 
    int g = (float)((hex>>IM_COL32_G_SHIFT)&0xFF);// * sc; 
    int r = (float)((hex>>IM_COL32_B_SHIFT)&0xFF);// * sc; 


    g_palette[name] = ImColor(r, g, b, 255);
  }
}

}