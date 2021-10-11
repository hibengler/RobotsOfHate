


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


/* glory goes up based on a win, and down on a loose, but glory gets its own role
with the reverse risk, there is more glory in attacking
Glory has no use right now.
each role does different values on each other role.
r -> p 1 to 3
r -> s 3 to 1
R -> r even
etc.
The white balanced have higher default sqrt(r^2+p^2+s^2/white bar / sqrt2) or something.
*/
typedef struct hate_rps { // rock, paper,scissors
float rps[3];
} hate_rps;

typedef struct hate_robot {
  uchar mode; // attack, defend, neutral
  uchar current_planet;
  uchar destination_planet;
  uchar position;  // position to destination, if on destination, that planet controlls the battle
  hate_rps strength;
} hate_robot;


typedef struct hate_player {
  uchar mode; // disconnected, playing, lobby
  uchar player_id; /* 0-5 */
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
