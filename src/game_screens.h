# pragma once


enum PAGES
{
  Home = 0,
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
