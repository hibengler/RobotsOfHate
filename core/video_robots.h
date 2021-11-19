#ifndef VIDEO_ROBOTS_H
#define VIDEO_ROBOTS_H 1
      
#include "video_planets.h"
#include "larry_harvey_has_a_possee.h"
#include "onecolor_specs.h"
#include <sys/time.h>


// Dodacehedron uses the icodahedron tables, but then modifies things by flatening some points


typedef struct video_robot_screen {
float point[4];
float size[4];
float status_point[4];
float status_size[4];
} video_robot_screen;

typedef struct video_robot {
float pos;
GLfloat rotation[3];
GLfloat rotation_delta[3];
int to_planet;
void *expansion1;
void *expansion2;
video_robot_screen robot_screens[HATE_NUMBER_PLAYERS];
} video_robot;


typedef struct video_robots {
struct video_robot robots[HATE_NUMBER_PLANETS][HATE_NUMBER_ROBOTS];
} video_robots;



extern void video_robot_init(hate_game *game,hate_frame *base_frame,int planet_id,int robot_id);

extern void video_robot_screen_init(hate_game *game,hate_frame *base_frame,int planet_id,int robot_id,int screen_id);

extern void video_robots_init(hate_game *game);

extern void video_robot_draw(hate_game *game,hate_screen *screen,int planet_id,int robot_id,int do_negative_z);



extern void video_robots_step(hate_game *game);

#endif




