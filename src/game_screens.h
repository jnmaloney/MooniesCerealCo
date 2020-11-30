# pragma once


enum PAGES
{
  MainMenu = 0,
  Card, 
  Home,
  Launchpad,
  Mining,
  Econ,
  BuyFleet,
  Processing,
  UpgradeShip,
};


void draw_game_credits();

void drawHeaderBar();

void drawFooterBar();
void drawBackBar(PAGES page);

void econ_page();
void upgrade_ship_page();
void launch_page();

void draw_dialog();
bool no_dialog();

void draw_mainmenu();

void mining_page();
void production_page();
void home_page();
