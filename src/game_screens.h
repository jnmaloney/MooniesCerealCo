# pragma once


enum PAGES
{
  MainMenu = 0,
  Home,
  Launchpad,
  Mining,
  Econ,
  BuyFleet,
  Processing,
  UpgradeShip,
};


void drawHeaderBar();

void drawFooterBar();
void drawBackBar(PAGES page);

void econ_page();
void upgrade_ship_page();
void launch_page();

void draw_dialog();

void draw_mainmenu();

void mining_page();
