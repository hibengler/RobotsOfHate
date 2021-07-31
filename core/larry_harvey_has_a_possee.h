#ifndef LARRY_HARVEY_POSSEE_H
#define LARRY_HARVEY_POSSEE_H


// Note - this permanently deviates from the github version of larry harvey.
// cause we are ditching raylib and stuff
/* this is different than the git version and tailored somewhat to silent radiance */
#ifndef LARRY_GLES_2_DIRECT
#define LARRY_GLES_2_DIRECT 1
#endif

//#include "raylib.h"
#include "mini_not_rl.h"
typedef not_rl_Color Color;
typedef not_rl_Rectangle Rectangle;
typedef not_rl_Vector2 Vector2;
typedef not_rl_Vector3 Vector3;
#define CLITERAL NOT_RL_CLITERAL
#define BLACK      CLITERAL{ 0, 0, 0, 255 }         // Black
#define WHITE      CLITERAL{ 255, 255, 255, 255 }   // White
typedef enum { false, true } bool;
  




typedef struct larry_harvey_rectangle_section {
  not_rl_Rectangle the_section;
  } larry_harvey_rectangle_section;

typedef struct larry_harvey_robot {
  not_rl_Color usual_first_color;
  not_rl_Color usual_second_color;
  not_rl_Color usual_third_color;
  int number_first_color_sections;
  larry_harvey_rectangle_section *first_sections;
  int number_second_color_sections;
  larry_harvey_rectangle_section *second_sections;
  int number_third_color_sections;
  larry_harvey_rectangle_section *third_sections;
  } larry_harvey_robot;

    
typedef struct larry_harvey_basketball_team {
  larry_harvey_robot players[5];
  } larry_harvey_basketball_team;
  
typedef struct larry_harvey_robot_league {
  larry_harvey_basketball_team teams[9];
  } larry_harvey_robot_league;
   
extern larry_harvey_robot_league * new_larry_harvey_robot_league(void);
extern void free_larry_harvey_robot_league(larry_harvey_robot_league *league);

void draw_larry_harvey_robot_2d(larry_harvey_robot *probot,not_rl_Vector2 translate,not_rl_Vector2 scale);
void draw_larry_harvey_robot_3d(larry_harvey_robot *probot,not_rl_Vector3 translate,not_rl_Vector3 scale);
void draw_larry_harvey_robot_container_3d(larry_harvey_robot *probot,not_rl_Vector3 translate,not_rl_Vector3 scale);
/* the container is a box that will fit the robot and had the color of the robot.
   It can also used to draw the robot from far away */


/*sometimes you want to rotate */
void draw_larry_harvey_robot_3d_rotate(larry_harvey_robot *probot,not_rl_Vector3 translate,not_rl_Vector3 scale,float sinangle,float cosangle);
void draw_larry_harvey_robot_container_3d_rotate(larry_harvey_robot *probot,not_rl_Vector3 translate,not_rl_Vector3 scale,float sinangle,float cosangle);


/* sometimes you want to sparkle */
void draw_larry_harvey_robot_container_3d_sparkle(larry_harvey_robot *probot,not_rl_Vector3 translate,not_rl_Vector3 scale);




/* handle spaceship */
void draw_larry_harvey_robot_3d_no_eyes_rotate_colors(larry_harvey_robot *probot, 
not_rl_Vector3 translate,
not_rl_Vector3 scale,
float sinangle,float cosangle,
not_rl_Color color1,
not_rl_Color color2,
not_rl_Color color3);


void draw_larry_harvey_robot_eyes_only_3d_rotate(
  larry_harvey_robot *probot,not_rl_Vector3 translate,not_rl_Vector3 scale,
  float sinangle,float cosangle,Color eye_white, Color eye_yolk);

  
#endif
