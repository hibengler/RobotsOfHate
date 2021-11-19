      
#include "video_robots.h"
#include "larry_harvey_has_a_possee.h"
#include "onecolor_specs.h"
#include <sys/time.h>


// Dodacehedron uses the icodahedron tables, but then modifies things by flatening some points





video_robots the_video_robots;


void video_robot_screen_init(hate_game *game,hate_frame *base_frame,int planet_id,int robot_id,int screen_id) {
}


void video_robot_init(hate_game *game,hate_frame *base_frame,int planet_id,int robot_id) {
hate_robot *hrobot= &base_frame->planets[planet_id].robots[robot_id];

video_robots *vrobots = &the_video_robots;
video_robot *vrobot = &vrobots->robots[planet_id][robot_id];

memset((void *)vrobot,0,sizeof(video_robot));
for (int i=0;i<HATE_NUMBER_PLAYERS;i++) {
  video_robot_screen_init(game,base_frame,planet_id,robot_id,i);
  }
}


void video_robots_init(hate_game *game) {
hate_frame *base_frame = game->frames+game->new_frame;

for (int i=0;i<HATE_NUMBER_PLANETS;i++) {
  for (int j=0;j<HATE_NUMBER_ROBOTS;j++) {
    video_robot_init(game,base_frame,i,j);
    }
  }
}



void video_robot_draw(hate_game *game,hate_screen *screen,int planet_id,int robot_id,int do_negative_z) {
hate_frame *base_frame = game->frames+game->new_frame;

hate_robot *hrobot= &base_frame->planets[planet_id].robots[robot_id];

video_robots *vrobots = &the_video_robots;
video_robot *vrobot = &vrobots->robots[planet_id][robot_id];

if (hrobot->mode==HATE_ROBOT_NEUTRAL) {
  return;
  }
else if (hrobot->mode==0) {
  return;
  }
int screen_number = screen - &game->screens.screens[0];
video_robot_screen *vrobot_screen = &vrobot->robot_screens[screen_number];
larry_harvey_robot *probot = &game->larry_harvey_robot_league->teams[screen->robot_choices[planet_id]].players[robot_id];

if (  ((do_negative_z)&&(vrobot_screen->point[2]<0.f)) |
      ((!do_negative_z)&&(vrobot_screen->point[2]>=0.f)) ) {
  if (hrobot->mode==HATE_ROBOT_MOVE_2_ATTACK) {
    draw_larry_harvey_robot_3d(probot,
      (not_rl_Vector3){vrobot_screen->point[0], vrobot_screen->point[1], vrobot_screen->point[2]},
      (not_rl_Vector3){vrobot_screen->size[0], vrobot_screen->size[1], vrobot_screen->size[2]});
    }
  else if (hrobot->mode==HATE_ROBOT_MOVE_2_DEFEND) {
    draw_larry_harvey_robot_3d(probot,
      (not_rl_Vector3){vrobot_screen->point[0], vrobot_screen->point[1], vrobot_screen->point[2]},
      (not_rl_Vector3){vrobot_screen->size[0], vrobot_screen->size[1], vrobot_screen->size[2]});
    }
  else if (hrobot->mode==HATE_ROBOT_MOVE_2_NEUTRAL) {
    }
  else if (hrobot->mode==HATE_ROBOT_ATTACK) {
    draw_larry_harvey_robot_3d(probot,
      (not_rl_Vector3){vrobot_screen->point[0], vrobot_screen->point[1], vrobot_screen->point[2]},
      (not_rl_Vector3){vrobot_screen->size[0], vrobot_screen->size[1], vrobot_screen->size[2]});
    }
  else if (hrobot->mode==HATE_ROBOT_DEFEND) {
    draw_larry_harvey_robot_3d(probot,
      (not_rl_Vector3){vrobot_screen->point[0], vrobot_screen->point[1], vrobot_screen->point[2]},
      (not_rl_Vector3){vrobot_screen->size[0], vrobot_screen->size[1], vrobot_screen->size[2]});
    }
  } // if we should draw negative or positive
}





 
   
/* screen number*5+real_player_number */
static int placement[] = {0,1,2,3,4,
                 4,0,1,2,3,
                 3,4,0,1,2,
                 2,3,4,0,1,
                 1,2,3,4,0};

static float placement_scale = 0.4f;
static float translate_size = 1.33333333f;
static float pos_translate[] = {0.f,0.f,0.f,
                     1.f,1.f,0.f,
                     1.f,-1.f,0.f,
                     -1.f,-1.f,0.f,
                     -1.f,1.f,0.f};
  
static float pos_distances[]= {0.,1.414,1.414,1.414,1,414,
                              1.414,0.,2.,2.828,2,
			      1.414,2.,0.,2.,2.828,
			      1.414,2.828,2.,2.,2,
			      1.414,2.,2.828,2.,2};










void video_robot_step(hate_game *game,hate_frame *old_frame,hate_frame *current_frame,hate_frame *new_frame,int planet_id,int robot_id) {
hate_robot *hrobot= &new_frame->planets[planet_id].robots[robot_id];

video_robots *vrobots = &the_video_robots;
video_robot *vrobot = &vrobots->robots[planet_id][robot_id];


// init one of them
if ((robot_id<=0)&&/*(planet_id==0)&&*/(hrobot->mode!= HATE_ROBOT_MOVE_2_ATTACK)) {
  hrobot->mode = HATE_ROBOT_MOVE_2_ATTACK;
  vrobot->pos = 0.f;
  vrobot->to_planet=(planet_id+1+robot_id)%5;
  fprintf(stderr,"planet %d to %d\n",planet_id,vrobot->to_planet);
  }  

if (hrobot->mode==HATE_ROBOT_NEUTRAL) {
  return;
  }
else if (hrobot->mode==0) {
  return;
  }
if (hrobot->mode==HATE_ROBOT_MOVE_2_ATTACK) {
  (vrobot->pos) += 1.f/300.f;
  if (vrobot->pos>1.f) {
    vrobot->pos=1.f;
    // now we can also make the planet visible
    video_initiate_color_saturation(game,vrobot->to_planet);
    }
  
  for (int screen_no = 0;screen_no<HATE_NUMBER_PLAYERS;screen_no++) {
    if (game->screens.enabled[screen_no]) {    
      hate_screen *screen= &game->screens.screens[screen_no];
      int central_planet_id = screen->central_planet_id;
      
      int  fromplx = placement[central_planet_id*5+planet_id];
      int  toplx = placement[central_planet_id*5+ vrobot->to_planet]; 
      float closeness = 0.75;
      float pos2 = vrobot->pos*(closeness) + 0.0;
      
      
      float fromx = translate_size*pos_translate[fromplx*3];
      float fromy = translate_size*pos_translate[fromplx*3+1];
      float fromz = translate_size*pos_translate[fromplx*3+2]-0.25f;
      float tox = translate_size*pos_translate[toplx*3];
      float toy = translate_size*pos_translate[toplx*3+1];
      float toz = translate_size*pos_translate[toplx*3+2]+0.25f;
      float deltax = tox-fromx;
      float deltay = toy-fromy;
      float deltaz = toz-fromz;
      video_robot_screen *vrobot_screen = &vrobot->robot_screens[screen_no];
      vrobot_screen->point[0] = fromx + deltax*pos2;
      vrobot_screen->point[1] = fromy + deltay*pos2;
      vrobot_screen->point[2] = fromz + deltaz*pos2;
      vrobot_screen->size[0] = 0.25;
      vrobot_screen->size[1] = 0.25;
      vrobot_screen->size[2] = 0.25;
      }
    }
  }
}




void video_robots_step(hate_game *game) {
hate_frame *previous_frame = game->frames+game->previous_frame;
hate_frame *current_frame = game->frames+game->current_frame;
hate_frame *new_frame = game->frames+game->new_frame;
for (int i=0;i<HATE_NUMBER_PLANETS;i++) {
  for (int j=0;j<HATE_NUMBER_ROBOTS;j++) {
    video_robot_step(game,previous_frame,current_frame,new_frame,i,j);
    }
  }
}






