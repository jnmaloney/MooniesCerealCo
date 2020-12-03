#pragma once


#include "game_globals.h"


// Game actions
void show_game_credits();
void hide_game_credits();
bool showing_game_credits();


void set_up_game();


void end_week();

//void process_puffs();
//void sell_puffs();
void upgrade_ship(int upgrade, int cost);

// common resource actions
void spend_cash(int x);
void spend_fuel(float x);
void collect_rock(int x);
void consume_rock(int x);
void sell_unit();

// factory actions
void buy_new_machine(int current_floor);

// Ship transport cycle
bool ship_launch(game_globals::Ship & ship);
bool ship_collect_rock(game_globals::Ship & ship); // Mine& mine
bool ship_return(game_globals::Ship & ship);
bool ship_unload_rock(game_globals::Ship & ship);

void buy_new_science(int cost);