/* logical player -> where player is.  what mode, list of commands 
screen_robot - 

*/


#ifndef PLAYER2_H
#define PLAYER2_H

typedef unsigned char uchar;

#include "hateglue.h"
#include "superpos.h"
#include "mini_not_rl.h"
#include "helper.h"

#define GLFW_INCLUDE_ES2
#include <GLFW/glfw3.h>

#include "onecolor_specs.h"

#define HATE_NUMBER_ROBOTS 5
#define HATE_NUMBER_SCREENS 5
#define HATE_NUMBER_PLAYERS 5
#define HATE_NUMBER_PLAYERS_PLUS_1 (5+1)
#define HATE_NUMBER_PLANETS 5
#define HATE_NUMBER_LEAGUES 7
#define HATE_NUMBER_THINGS (1+5+5+5*5)
#define HATE_NUMBER_FRAMES 3

/* a hate frame is a point in time thats computed.
So we do the following:
1. copy everything.
2. reassign/eliminate older frames.
3. run through each  thing - for movement
4. for each frame under frame run through each thing - for battle
5. kill/grow strentgh for each thing.
*/

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
float rps[4];
} hate_rps;



#define HATE_SQRT3 0.57735026918962576450
#define HATE_SQRT3F 0.57735026918962576450f
#define HATE_ROBOT_INITIAL_R HATE_SQRT3F
#define HATE_ROBOT_INITIAL_P HATE_SQRT3F
#define HATE_ROBOT_INITIAL_S HATE_SQRT3F
#define HATE_ROBOT_INITIAL_D ((HATE_ROBOT_INITIAL_R+HATE_ROBOT_INITIAL_P+HATE_ROBOT_INITIAL_S) * 0.0333333333f)
// 10 dice

#define HATE_PLANET_INITIAL_R 2.f
#define HATE_PLANET_INITIAL_P 2.f
#define HATE_PLANET_INITIAL_S 2.f
#define HATE_PLANET_INITIAL_D ((HATE_PLANET_INITIAL_R+HATE_PLANET_INITIAL_P+HATE_PLANET_INITIAL_S) * 0.0333333333f*0.5f)
// 20 dice

#define HATE_ROBOT_MOVE_2_ATTACK 1
#define HATE_ROBOT_MOVE_2_DEFEND 2
#define HATE_ROBOT_MOVE_2_NEUTRAL 3
#define HATE_ROBOT_ATTACK 4
#define HATE_ROBOT_DEFEND 5
#define HATE_ROBOT_NEUTRAL 6



#define HATE_PLANET_PLAYING 1
#define HATE_PLANET_DESTROYED 2
#define HATE_PLANET_AVAILABLE 3

#define HATE_PLAYER_PLAYING 1
#define HATE_PLAYER_DESTROYED 2
#define HATE_PLAYER_AVAILABLE 3

#define HATE_NUMBER_ACTIONS 32766
// action 5461 actions per player




// thingid action objectid
// 3 attacking things
// 2 defending things
// 2 neutral things
// planet is defending
// planet + all defenders vs planet + all attackers.

typedef struct hate_thing {
  struct timeval global_ts; // 
  uchar controlling_player_id; // -1 if nothing
  uchar controlling_planet_id;
  uchar controlling_robot_id;
  uchar subject_planet_id;
  uchar subject_player_id;
  uchar subject_me_id;
  uchar object_planet_id;
  uchar object_player_id;
  uchar object_me_id;
  hate_rps strength;
  unsigned short action_id; // -1 if no action
  } hate_thing;
  
typedef struct hate_robot {
  uchar mode;
  uchar thing_id;
  } hate_robot;





// hate action state
#define HATE_ACTION_OFF 0
#define HATE_ACTION_NEW 1
#define HATE_ACTION_DELAYED 2
#define HATE_ACTION_ME 3
#define HATE_ACTION_DERIVING 4

#define HATE_ACTION_READY_PLAYER 5





typedef struct hate_action {
unsigned short length;
unsigned short action_id;
uchar hate_action_state;
uchar hate_action_code;
unsigned int now_frame;  // estimated frame when we sent the local timestamp, or 0 or -1
struct timeval sent_local_ts; // for the controlling player, this is the estimated timestamp of sending
float sender_time_bias; // global time is 0. this is the estimated difference 
float asked_for_start_time; // from the 
float try_again_offset;
float start_time;
float end_time;
float now_time;   // all these floats are offset from global_ts
hate_thing proposed_action_thing;
struct timeval global_ts_from_receivers_view;
float  derived_recevived_time_bias;  // this is the local time from when we received the action
} hate_action;





typedef struct hate_player {
  uchar mode; // disconnected, playing, lobby
  uchar player_id; /* 0-5 */
  uchar thing_id; /* 0-5 */
} hate_player;


typedef struct hate_planet {
  uchar mode; // disconnected, playing, lobby
  uchar player_id; /* 0-5 */
  struct hate_robot robots[HATE_NUMBER_ROBOTS];
  uchar thing_id; /* 0-5 */
} hate_planet;





typedef struct hate_screen {
  int central_planet_id;  // usually my_player_id unless multiscreen then the planet_id of the screenin the center
  glMatrix screenViewMatrix;
  int robot_choices[HATE_NUMBER_PLANETS];
  int rps_choices[3];  // rps_choices has the screen scramble up rps
  int planet_choices[HATE_NUMBER_PLANETS];
  } hate_screen; 
  
typedef struct hate_screens {
  int enabled[HATE_NUMBER_SCREENS];
  hate_screen screens[HATE_NUMBER_SCREENS];
  } hate_screens;
 

typedef struct hate_frame {
 hate_thing things[HATE_NUMBER_THINGS];
 hate_planet planets[HATE_NUMBER_PLANETS];
 hate_player players[HATE_NUMBER_PLAYERS_PLUS_1];
 hate_action actions[HATE_NUMBER_ACTIONS]; 
 double current_time_bias_to_global[HATE_NUMBER_PLAYERS_PLUS_1];

 
 int stats_are_useful; // set to one if we can start using this to help with the time.
 // these are statistics to adjust the global time for next time.  This is run by everybody, so the global time spread is reduced frame by frame - hopefully.
 double max_local_now_frame_current;
 double average_local_now_frame_adjusted_to_max[HATE_NUMBER_PLAYERS_PLUS_1];
 double max_local_now_frame;
 double min_local_now_frame;
 double time_spread;
 
 double average_time_bias_to_global[HATE_NUMBER_PLAYERS_PLUS_1];
 double global_time_spread;
 
 double send_to_receive_time[HATE_NUMBER_PLAYERS_PLUS_1];
 
 } hate_frame;
 
  
typedef struct hate_game {
  double dist;   // distance between the planets
  int my_player_id;
  int current_frame; // frame last drawed - first set to 0
  int previous_frame; // frame before we called step before -- also set to 0
  int new_frame;  // frame to be drawed - first set to 0,  then we go 1,0,2 then we go 2,1,0, then 0,1,2
  hate_frame frames[HATE_NUMBER_FRAMES]; // the computer will run this per polling - but display is probably a different thread, so
     // display will get the current frame and roll with it.
  hate_screens screens; 
  larry_harvey_robot_league *larry_harvey_robot_league;
  } hate_game;





#ifdef ramble

/* hate things are robots or planets or players
hate actions are 
from thing to thing
action type
when
from thing rps am amalgamate (for foreign rps)


with link lists sort by thing/startdate.
then take out earlier actions by thing
So each thing has an action and only one action.

for all thing/actions
   for all other actions that are after the thing
      need to collect total defense on thing, and offsnse on thing fo reach thing - round 2, then attack is round 3. - guessing on other battles, until get stats.
      do the subject/object action.  could be attacker to defender
       if actions clash - remove older actions
       if actions match - do them
       else do single action
       
       
       so each thing has one action. and an action needs to report to the other player involvedquicker than non involved players.
*/       

/* action will have
action_id
time offset from current global id for local
expected time to do rel to global time
expected next_widow to global time.
expected end time to first global time
(the player id is known from the network)
owning planet/player/robot
owning
*/ 
#endif


extern void player2_game_init(hate_game *game,int my_player_id);
extern void player2_init_graphics(hate_game *game);
// graphics is after game is init, and called after the graphics is set up


extern void player2_game_step(hate_game *game);
extern void player2_game_draw(hate_game *game);

extern void player2_game_do_init_player(hate_game *game,int player_id,hate_rps player_strength);
extern void player2_game_do_init_planet(hate_game *game,int planet_id,int player_id,hate_rps player_strength);
extern void player2_game_do_init_robot(hate_game *game,int player_id,int robot_id,hate_rps player_strength);

extern void player2_action_received_ready(hate_game *game,int action_id);
extern void player2_action_received(hate_game *game,int action_id);


#endif
