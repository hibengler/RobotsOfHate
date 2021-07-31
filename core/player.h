


#ifndef PLAYER_H
#define PLAYER_H

#include "hateglue.h"
#include "superpos.h"
#include "mini_not_rl.h"
#include "helper.h"

#define GLFW_INCLUDE_ES2
#include <GLFW/glfw3.h>

#include "onecolor_specs.h"

#define HATE_NUMBER_ROBOTS 5
#define HATE_NUMBER_PLAYERS 5
#define HATE_NUMBER_LEAGUES 7


typedef struct hate_robot {
  super_point current_point; // ref player that played it. -hdist to hdist
  super_point last_point;
  float size;
  float sizetimessize;
  int need_to_show_ghost_last_point;
  int need_to_show_ghost_first_point;
  super_point current_ghost_point;
  super_point last_ghost_point;
} hate_robot;


typedef struct hate_player {
  int player_id; /* 0-5 */
  struct hate_robot robots[HATE_NUMBER_ROBOTS];
} hate_player;

typedef struct hate_screen {
  int player_id;
  glMatrix screenViewMatrix;
  int robot_choices[HATE_NUMBER_ROBOTS];
  } hate_screen; 
  
typedef struct hate_screens {
  int enabled[HATE_NUMBER_PLAYERS];
  hate_screen screens[HATE_NUMBER_PLAYERS];
  } hate_screens;

typedef struct hate_game {
  int my_player_id;
  float dist;
  hate_player players[HATE_NUMBER_PLAYERS];
  hate_screens screens;
  larry_harvey_robot_league *larry_harvey_robot_league;
  } hate_game;

extern void game_init(hate_game *game,int my_player_id);
extern void game_step(hate_game *game);
extern void game_draw(onecolor_specs *onec,hate_game *game);

  

#endif
