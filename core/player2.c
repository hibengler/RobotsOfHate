




#include "player2.h"
#include "larry_harvey_has_a_possee.h"
#include "onecolor_specs.h"

static int pick_leagues(int *robot_choices) {
for (int i=0;i<HATE_NUMBER_ROBOTS;) {
  unsigned int pick;
  pick = (rand() /19);
  pick = pick % (HATE_NUMBER_LEAGUES);
  int j;
  for (j=0;j<i;j++) {
    if (pick==robot_choices[j]) {
      break;
      }
    }
  if (j==i) { /* not picked before */
    robot_choices[i++] = pick;
    }
  }
//for (int i=0;i<HATE_NUMBER_ROBOTS;i++) {
//  robot_choices[i]=i;
//  }
  //??? make it easeirr for now
}




hate_prep_action hate_prepare_action(hate_game *game,struct timespec global_ts) {
hate_prep_action prep_action;

}


void game_init(hate_game *game,int my_player_id) {
game->dist = 0.6666666666666666666666666666;
float dist=game->dist;
float qdist = dist *0.25f;
float one_over_qdist = 1.0f/qdist;
game->my_player_id = my_player_id;
game->larry_harvey_robot_league = new_larry_harvey_robot_league();
hate_screens *screens = &(game->screens);
int *ms;
ms = superpos_map_to(0);


/* init screens */
for (int i=0;i<HATE_NUMBER_PLAYERS;i++) {
  glMatrix range;
  glMatrix ex;
  game->screens.enabled[i]=0;
  
  hate_screen *screen = &(screens->screens[i]);
  screen->player_id=i;
  pick_leagues(&(screen->robot_choices[0]));
  screen->robot_choices[0] = 0;
  screen->robot_choices[1] = 1;
  screen->robot_choices[2] = 2;
  screen->robot_choices[3] = 3;
  screen->robot_choices[4] = 4;
  
  
  if (i==0) {
    loadIdentity(&range);
    scaleMatrix(&range,qdist,qdist,qdist);    
    screen->screenViewMatrix = range;
    }
  else if (i==1) {
    loadIdentity(&range);
    scaleMatrix(&range,qdist,qdist,qdist); 
    translateMatrix(&range,4.f,4.f,0.f);
    screen->screenViewMatrix = range;
    }
  else if (i==2) {
    loadIdentity(&range);
    scaleMatrix(&range,qdist,qdist,qdist); 
    translateMatrix(&range,4.f,-4.f,0.f);
    screen->screenViewMatrix = range;
    }
  else if (i==3) {
    loadIdentity(&range);
    scaleMatrix(&range,qdist,qdist,qdist); 
    translateMatrix(&range,-4.f,-4.f,0.f);
    screen->screenViewMatrix = range;
    }
  else if (i==4) {
    loadIdentity(&range);
    scaleMatrix(&range,qdist,qdist,qdist); 
    translateMatrix(&range,-4.f,+4.f,0.f);
    screen->screenViewMatrix = range;
    }
  }
game->screens.enabled[0]=1;
game->screens.enabled[1]=1;
game->screens.enabled[2]=1;
game->screens.enabled[3]=1;
game->screens.enabled[4]=1;


#define HATE_PLANET_PLAYING 1
#define HATE_PLANET_DESTROYED 2
#define HATE_PLANET_AVAILABLE 3

#define HATE_PLAYER_PLAYING 1
#define HATE_PLAYER_DESTROYED 2
#define HATE_PLAYER_AVAILABLE 3

/* init planets and players */
for (int i=0;i<HATE_NUMBER_PLAYERS;i++) {
  hate_planet *planet=&(game->planets[i]);
  hate_player *player=&(game->players[i]);
  planet->mode=HATE_PLANET_PLAYING;
  planet->player_id=i;
  player->mode = HATE_PLAYER+PLAYING;
  player->player_id=i;
  for (int j=0;j<HATE_NUMBER_ROBOTS;j++) {
    hate_robot *robot = &(player->robots[j]);
    robot->player_id=i;
    robot->from_planet=i;
    robot->current_planet=i;
    robot->position=0;
    robot->strength = (hate_rps){rps:(float []) PHATE_ROBOT_INITIAL_R,HATE_ROBOT_INITIAL_P,HATE_ROBOT_INITIAL_S)};
    }
  }
}



static float angle=0.f;
void game_step(hate_game *game) {
for (int i=0;i<HATE_NUMBER_PLAYERS;i++) {
  hate_player *player=&(game->players[i]);   
  for (int j=0;j<HATE_NUMBER_ROBOTS;j++) {
    hate_robot *robot = &(player->robots[j]);
    
    robot->last_point = robot->current_point;
    float x= robot->current_point.xyz[0];
    float y= robot->current_point.xyz[1];
    robot->current_point.xyz[0] = x*cosf(angle) + y*sin(angle);
    robot->current_point.xyz[1] = -x*sinf(angle) + y*cos(angle);
    
    /* nop for now */
    }  
  }
angle += 1./16500.;
}



void game_draw(onecolor_specs *onec,hate_game *game) {
for (int s=0;s<HATE_NUMBER_PLAYERS;s++) {
  if (game->screens.enabled[s]) {
    hate_screen *screen = &(game->screens.screens[s]);
    onec->viewMatrix = screen->screenViewMatrix;
    
//    multMatrix(&onec->MVPMatrix,&onec->viewMatrix,&onec->projectionMatrix);
    glUniformMatrix4fv(onec->mMVPMatrixHandle, 1, GL_FALSE, (GLfloat *)(&onec->MVPMatrix));
    glUniformMatrix4fv(onec->mMVPMatrixHandle, 1, GL_FALSE, (GLfloat *)(&onec->viewMatrix));
    checkGlError("setmatrix");
	   
    for (int i=0;i<1;i++) { // for each player
      hate_player *player=&(game->players[i]);   
      for (int j=0;j<HATE_NUMBER_ROBOTS;j++) { // for each robot
        hate_robot *robot = &(player->robots[j]); 
	super_point np;
 	np = xyz_from_context_to_context(game->my_player_id,2.f,robot->current_point,i);
 	np = xyz_from_context_to_context(i,2.f,np,s);
        draw_larry_harvey_robot_3d(
	      &(game->larry_harvey_robot_league->teams[i/*screen->robot_choices[i]*/].players[j]),
	      (not_rl_Vector3){np.xyz[0],np.xyz[1],np.xyz[2]},
	      (not_rl_Vector3){robot->size,robot->size,robot->size}
	      );
	      
//	fprintf(stderr,"draw screen %d np %f,%f,%f size %f\n",
//	     s,np.xyz[0],np.xyz[1],np.xyz[2],(float)robot->size);
        } // for each robot that needs to be printed
      } // for each player
    }  // if we are to do this screen
  } // for each screen to print
}
