




#include "player2.h"
#include "video_planets.h"
#include "larry_harvey_has_a_possee.h"
#include "onecolor_specs.h"
#include <sys/time.h>

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


static int pick_rgb_choices(int *rgb_choices) {
for (int i=0;i<3;) {
  unsigned int pick;
  pick = (rand() /19);
  pick = pick % (3);
  int j;
  for (j=0;j<i;j++) {
    if (pick==rgb_choices[j]) {
      break;
      }
    }
  if (j==i) { /* not picked before */
    rgb_choices[i++] = pick;
    }
  }
}


static int pick_planet_choices(int *planet_choices) {
for (int i=0;i<HATE_NUMBER_PLANETS;) {
  unsigned int pick;
  pick = (rand() /19);
  pick = pick % (HATE_NUMBER_PLANETS);
  int j;
  for (j=0;j<i;j++) {
    if (pick==planet_choices[j]) {
      break;
      }
    }
  if (j==i) { /* not picked before */
    planet_choices[i++] = pick;
    }
  }
}




void game_init_screens(hate_game *game,int my_player_id,float dist) {
float qdist = dist *0.25f;
float one_over_qdist = 1.0f/qdist;
float regdist = dist*0.75f;
glMatrix range; 
if (my_player_id ==5) { //supervisor and tie breaker 
  hate_screens *screens = &(game->screens);

  /* init screens */
  for (int i=0;i<HATE_NUMBER_PLAYERS;i++) {
    game->screens.enabled[i]=0;
    }
  for (int i=0;i<HATE_NUMBER_PLAYERS;i++) {
    hate_screen *screen = &(screens->screens[i]);
    screen->player_id=i;
    if (i==0) {
      pick_leagues(&(screen->robot_choices[0]));
      pick_planet_choices(&(screen->planet_choices[0]));
      } 
    else {
      for (int j=0;j<5;j++) {
        screen->robot_choices[j]= game->screens.screens[0].robot_choices[j];  
        screen->planet_choices[j]= game->screens.screens[0].planet_choices[j];  
	}
    screen->rps_choices[0] = 0;
    screen->rps_choices[1] = 1;
    screen->rps_choices[2] = 2;
    }  
  
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
  } // if we ar e super
else {
  /* init screens */
  for (int i=0;i<HATE_NUMBER_PLAYERS;i++) {
    game->screens.enabled[i]=0;
    }
  hate_screen *screen = &(game->screens.screens[0]);
  screen->player_id=my_player_id;
  pick_leagues(&(screen->robot_choices[0]));
  pick_rgb_choices(&(screen->rps_choices[0]));
  pick_planet_choices(&(screen->planet_choices[0]));

  loadIdentity(&range);
  scaleMatrix(&range,regdist,regdist,regdist);    
  screen->screenViewMatrix = range;
  
  game->screens.enabled[0]=1;
  }  
}  



static hate_rps add_rps(hate_rps a,hate_rps b) {
a.rps[0] += b.rps[0];
a.rps[1] += b.rps[1];
a.rps[2] += b.rps[2];
a.rps[3] += b.rps[3];
return a;
}

static void compute_player_strength(hate_frame *frame) {
for (int i=0;i<HATE_NUMBER_PLAYERS_PLUS_1;i++) { 
  frame->things[frame->players[i].thing_id].strength = (hate_rps)  {rps:  {0.f,0.f,0.f,0.f}}; // come back to this later
  }
for (int i=0;i<HATE_NUMBER_PLANETS;i++) { 
  hate_planet * planet= frame->planets+i;
  hate_rps robot_strength = (hate_rps)  {rps: {0.f,0.f,0.f,0.f}};
  for (int j=0;j<HATE_NUMBER_ROBOTS;j++) {
    robot_strength =add_rps(robot_strength,frame->things[planet->robots[j].thing_id].strength);
    }
    
  robot_strength = add_rps(robot_strength,frame->things[planet->thing_id].strength);
  hate_thing *player_thing = &frame->things[frame->players[planet->player_id].thing_id];
  player_thing->strength = add_rps(player_thing->strength, robot_strength);
  }
}
   
 
    
void map_frame_to_screen(hate_frame *frame,hate_screen *screen) {
// tbd ???
}
      

	  
		  
		  
static void load_fake_global_timeval(struct timeval *t) {
gettimeofday(t,NULL);
t->tv_sec -= 1; 
}		  
		  
		  
			  
				  
					  
// lots of assumptions

void player2_game_init(hate_game *game,int my_player_id) {


struct timeval fake_global_timeval;

load_fake_global_timeval(&fake_global_timeval); // current time - 1 second

game->dist = 0.6666666666666666666666666666;
float dist=game->dist;
float qdist = dist *0.25f;
float one_over_qdist = 1.0f/qdist;
game->my_player_id = my_player_id;
game->larry_harvey_robot_league = new_larry_harvey_robot_league();
game_init_screens(game,my_player_id,(float) dist);

game->current_frame = 0;
game->previous_frame = 0;
game->new_frame = 0;
hate_frame *frame = &game->frames[game->new_frame];

/* init planets and players */
int thing_id=0;
for (int i=0;i<HATE_NUMBER_PLAYERS_PLUS_1;i++) {
  hate_player *player=&(frame->players[i]);
  hate_thing *thing = &(frame->things[thing_id]);
  
  player->mode = HATE_PLAYER_PLAYING;
  player->player_id=i;
  player->thing_id=thing_id;
  thing->global_ts = fake_global_timeval;
  thing->controlling_player_id = player->player_id;
  thing->controlling_planet_id = 255;
  thing->controlling_robot_id = 255;
  thing->subject_planet_id = 255;
  thing->subject_player_id = player->player_id;
  thing->subject_me_id = thing_id;
  thing->object_planet_id = 255;
  thing->object_player_id = player->player_id;
  thing->object_me_id = thing_id;
  thing->strength = (hate_rps)  {rps:{0.f,0.f,0.f,0.f}}; // come back to this later
  thing->action_id=0;
  thing_id++;
  };   

for (int i=0;i<HATE_NUMBER_PLANETS;i++) {
  hate_planet *planet=&(frame->planets[i]);
  hate_thing *thing = &(frame->things[thing_id]);
  
  planet->mode = HATE_PLANET_PLAYING;
  planet->player_id=i;
  planet->thing_id=thing_id;
  
  thing->global_ts = fake_global_timeval;
  thing->controlling_player_id = planet->player_id;
  thing->controlling_planet_id = i;
  thing->controlling_robot_id = 255;
  thing->subject_planet_id = i;
  thing->subject_player_id = planet->player_id;
  thing->subject_me_id = thing_id;
  thing->object_planet_id = i;
  thing->object_player_id = planet->player_id;
  thing->object_me_id = thing_id;
  thing->strength = (hate_rps){rps:{HATE_PLANET_INITIAL_R,HATE_PLANET_INITIAL_P,HATE_PLANET_INITIAL_S,HATE_PLANET_INITIAL_D}};
  thing->action_id=0;
  thing_id++;
  }
        
for (int i=0;i<HATE_NUMBER_ACTIONS;i++) {
  hate_action *action=&(frame->actions[i]);
  memset((void *)action,0,(size_t)sizeof(hate_action));
  }
  
for (int i=0;i<HATE_NUMBER_PLAYERS_PLUS_1;i++) {
  frame->current_time_bias_to_global[i] = -1.;
  frame->average_local_now_frame_adjusted_to_max[i]=-1.;
  frame->send_to_receive_time[i] = 0.00;
  }
frame->stats_are_useful=0;


  
compute_player_strength(frame);
  
for (int i=0;i<HATE_NUMBER_PLAYERS;i++) {
  if (game->screens.enabled[i]) {
    map_frame_to_screen(frame,&game->screens.screens[i]);
    }
  }


}
 
static float angle=0.f;





void player2_init_graphics(hate_game *game) {
video_planet_init();
}



void player2_game_step(hate_game *game) {
#ifdef junnnk
for (int i=0;i<HATE_NUMBER_PLANETS;i++) {
  hate_player *player=&(frame->players[i]);   
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
#endif
angle += 1./16500.;


video_planet_step(game);

}

		  
void player2_game_draw(hate_game *game) {
for (int s=0;s<HATE_NUMBER_PLAYERS;s++) {
  if (game->screens.enabled[s]) {
    hate_screen *screen = &(game->screens.screens[s]);
    onec->viewMatrix = screen->screenViewMatrix;
    
    
    multMatrix(&onec->MVPMatrix,&onec->viewMatrix,&onec->projectionMatrix);
    
    glUniformMatrix4fv(onec->mMVPMatrixHandle, 1, GL_FALSE, (GLfloat *)(&onec->MVPMatrix));
//    glUniformMatrix4fv(onec->mMVPMatrixHandle, 1, GL_FALSE, (GLfloat *)(&onec->viewMatrix));
    checkGlError("setmatrix");
    
  for (int i=0;i<HATE_NUMBER_PLANETS;i++) { // for each  planet
    video_planet_draw(game,screen,i);
    }
    
	    
#ifdef OLDER	   
    
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
#endif      
    }  // if we are to do this screen
  } // for each screen to print
}

