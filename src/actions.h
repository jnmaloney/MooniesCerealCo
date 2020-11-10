#pragma once


#include "game_globals.h"


void end_week();
void day_event();

int fulfil(int location, int amount);
void pickup_cargo(Order& order);
void process_puffs();
void sell_puffs();
void upgrade_ship(int upgrade, int cost);
void process_launch(Ship& i);
void process_launch_list();
void pickup_cargo(Order& order);
void return_cargo(Order& order);
