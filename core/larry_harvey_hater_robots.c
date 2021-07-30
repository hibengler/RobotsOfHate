#include <stdio.h>
/*
Silent Radiance wifi larry_harvey_hater_robots visualizer for digital silent disco.
Copyright (C) 2017-2019 Hibbard M. Engler

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License   
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/
/*$Revision: 1.33 $*/

/* The larry harvy memorial movement has the following scenes:
0 - robots got to burning man.
1 - UFO's lands here
2 - Thank you from cthulu - given paper.
3 - see paper - be like PD
4 - Light the fuse - pink bomb
5 - Larry harvey picture
6 - robots leave burning man - forever

Hippolito made the hater bots and gave them to anyone to help remember larry, so I ran with it.
 
*/

#include "larry_harvey_has_a_possee.h"
//#include "simple_processor.h"

#include "processor.h"

#include "packet_summary.h"
#include "main_menu.h"
#include "display_set.h"
/* ^ for the special pictures */
#include "default.h"
/* ^ for the default picture rect and stuff like that */
#include <sys/time.h>
#include "helper.h"
#include "glue.h"


#define GetRandomValue not_rl_GetRandomValue

#define NUM_PARTICIPANTS 2000
#ifdef __APPLE__
#define ROBOTS_TO_DISPENSE 700
#define ROBOTS_PER_TEMPO 1
#define ROBOTS_PER_BEAT 1
#else 
#ifdef ANDROID
#define ROBOTS_TO_DISPENSE 500
#define ROBOTS_PER_TEMPO 1
#define ROBOTS_PER_BEAT 1
#else 
#define ROBOTS_TO_DISPENSE 1500
#define ROBOTS_PER_TEMPO 2
#define ROBOTS_PER_BEAT 1
#endif
#endif

//extern int screen_orientation;

#define SPACESHIP_ROBOT (ROBOTS_TO_DISPENSE+2)
#define CTHULU_ALIEN (ROBOTS_TO_DISPENSE+3)
#define LARRY_ROBOT (ROBOTS_TO_DISPENSE+4)
#define FULL_PAPER (ROBOTS_TO_DISPENSE+5)
#define ROLLED_UP_PAPER (ROBOTS_TO_DISPENSE+6)

typedef struct RobotTrait {
    int action;  // 0 - nothing 1 - rotate to the right, -1 - left - movement also
    float movement_time_microseconds;
    float intensity;
    int percent_got_cue;
    int percent_clumsy;
    
    int state; /* 0,1,-1*/
    float current_time_microseconds;
    float db_intensity;
    Vector3 movement_full;
    } RobotTrait;

typedef struct RobotPersonality {
    RobotTrait beat;
    RobotTrait onset;
    } RobotPersonality;
    
  

typedef struct RobotHaters{
    Rectangle rec;

    Vector3 current;   /* where we are */
    Vector3 current_spun;  /* well, we are spinning */
    Vector3 current_danced;  /* well, dancing too */    
    Vector3 current_final;
    Vector3 destination;  /* where we are goint to */
    Vector3 speed;        	
    float distance;
    bool active;
    
    float current_point;
    float total_time;
    larry_harvey_robot *robot;
    RobotPersonality personality;
    int sparkle; /* wen very distant and inactie, does it sparkle? */
} RobotHaters;



static glMatrix modelMatrix;
static glMatrix effectMatrix; /* used to do effects like zoom near/zoom away */
static glMatrix viewMatrix;
static glMatrix MainMenuPerspectiveMatrix;
static glMatrix perspectiveMatrix;
static glMatrix MVMatrix;
static glMatrix MVPMatrix;
static GLuint mMVPMatrixHandle;
static int colorHandle;


//static GLfloat upper_left_test[3] = {-1.0f,1.0f,0.0f};
//static GLfloat upper_right_test[3] = {1.0f,1.0f,0.0f};
//static GLfloat lower_left_test[3] = {-1.0f,-1.0f,0.0f};
//static GLfloat lower_right_test[3] = {1.0f,-1.0f,0.0f};

static GLuint gvPositionHandle;
static GLuint mPositionHandle ;


static GLuint one_color_program;
static GLuint full_color_program;




static float width_in_pixels;
static float height_in_pixels;
static float width_in_inches;
static float height_in_inches;
static float height_to_inches;
static float rotation_in_radians;
static float relative_width;
static float relative_height;
static float width_to_inches;
static int circle_factor;  
static lcontext *lc;



static GLuint larry_harvey_hater_pd_joke_texture;
static GLuint larry_harvey_hater_pd_joke_buffer=0;


static float timer=600;
static float alpha1=1.f;
static float alpha2=1.f;

static int activeRobots=0;
static float angle=0;

larry_harvey_robot_league *larry_harvey_league;



static float angle_reducer = 1.f; /* set to smaller than 1 (.99) when it is time to calm the rotation down */
static float angle_sign; /* when spinning the people */
static float angle_size =   3.14159f / 20.58f;  


extern int ran_setup_graphics_once_before;

static float remember_beat_interval_in_microseconds=-1.;
static float robot_hater_entry_phase = 0.f;
static struct timeval last_sample_time = {.tv_sec=0,.tv_usec=0};
static int beat_count = 0;


static RobotHaters robot_haters[NUM_PARTICIPANTS];

static int movie_scene = 0;

static void perspectiveGL( glMatrix *projection,GLfloat fovY, GLfloat aspect, GLfloat zNear, GLfloat zFar );


extern void larry_harvey_onetime_setup3();

GLfloat ups[4][3] = {{0.f,1.f,0.f},
                     {-1.f,0.f,0.f},
		     {0.f,-1.f,0.f},
		     {1.f,0.f,0.f}};






int larry_harvey_setupGraphics(int w,int h,int screen_type)
{
    loadIdentity(&modelMatrix);
    loadIdentity(&effectMatrix);
    loadIdentity(&viewMatrix);
    loadIdentity(&perspectiveMatrix);  
    pass_menu_setup_info(
        &width_in_pixels,
        &height_in_pixels,
        &width_in_inches,
        &height_in_inches,
        &height_to_inches,
        &rotation_in_radians,
        &relative_width,
        &relative_height,
        &width_to_inches,
        &circle_factor,
        &one_color_program,
        &full_color_program,
        &gvPositionHandle,
        &MainMenuPerspectiveMatrix,
        &mPositionHandle
 );



    
if (ran_setup_graphics_once_before)  {
  if (larry_harvey_hater_pd_joke_buffer) {
    resize_vbo(
      sizeof(default_active_splash_rect), default_active_splash_rect, GL_STATIC_DRAW,&larry_harvey_hater_pd_joke_buffer);
    }
  return(1);   
  }


helper_init();
    
    // letters init
    lc = linit_context(); // we will get our own letter context

    // vvv this is done in main_menu.c
    //letters_opengles_setgraphics(one_color_program,colorHandle,gvPositionHandle,mPositionHandle); // needs to be set up once, with a single color program

    colorHandle = glGetUniformLocation(one_color_program, "v_Color");
    mPositionHandle = glGetAttribLocation(one_color_program, "vPosition");


    mMVPMatrixHandle = glGetUniformLocation(one_color_program, "u_MVPMatrix");
    checkGlError("glGetUniformLocation_bw");

    // Initialization
    //---------------------------------------------------------
    activeRobots = 0;
    alpha1 = 1.;
    alpha2 = 1.;
    timer=1200;
    angle=0.;
    
    angle_sign = 1.0;
    
    larry_harvey_league = new_larry_harvey_robot_league();
    larry_harvey_onetime_setup3();

//  movie_scene=2;
//  larry_harvey_setup2();
return 0;
}    






/* borrowed from http://stackoverflow.com/questions/12943164/replacement-for-gluperspective-with-glfrustrum */
static void perspectiveGL( glMatrix *projection,GLfloat fovY, GLfloat aspect, GLfloat zNear, GLfloat zFar )
{
    const GLfloat pi = 3.1415926535897932384626433832795;
    GLfloat fW, fH;

    //fH = tan( (fovY / 2) / 180 * pi ) * zNear;
    fH = tan( fovY / 360.f * pi ) * zNear;
    fW = fH * aspect;

    //glFrustum( -fW, fW, -fH, fH, zNear, zFar );
    frustumMatrix(projection,-fW, fW, -fH, fH, zNear, zFar );
}












void update_dance_moves(struct RobotHaters *h,float whole_sinangle,float whole_cosangle,float microseconds_passed,int has_beat,int has_onset,
float db_level_as_intensity) {
if (has_beat) {
  RobotTrait *t = &(h->personality.beat);
  if (!(t->state) ) {
    if (t->action) {
      int sign;
      if (t->action >=0) sign=1.; else sign=-1.;
      t->state = sign * (GetRandomValue(0,100) < t->percent_got_cue?1:0) * (GetRandomValue(0,100) > t->percent_clumsy?1:-1);
      t->current_time_microseconds=0.f;
      t->db_intensity = db_level_as_intensity;
      }
    }
  }
if (has_onset) {
  RobotTrait *t = &(h->personality.onset);
  if (!(t->state) ) {
    if (t->action) {
      int sign;
      if (t->action >=0) sign=1; else sign=-1;
      t->state = sign * (GetRandomValue(0,100) < t->percent_got_cue?1:0) * (GetRandomValue(0,100) > t->percent_clumsy?1:-1);
      t->current_time_microseconds=0.f;
      t->db_intensity = db_level_as_intensity;
      }
    }
  }

h->current_danced = h->current_spun;

{  
  RobotTrait *t = &(h->personality.beat); 
  if (t->state) {
    t->current_time_microseconds += microseconds_passed;
    if (t->current_time_microseconds > t->movement_time_microseconds) {
      t->state = 0;
      t->current_time_microseconds=0.f;
      }
    else {
      if ((t->action==1)||(t->action==-1)) {
        float p1 =  2.0*t->current_time_microseconds / t->movement_time_microseconds ;
        if (p1 >= 1.0) p1= 2.0-p1; 
	p1 *= t->db_intensity;
        h->current_danced.x += t->movement_full.x * p1;
        h->current_danced.y += t->movement_full.y * p1;
        h->current_danced.z += t->movement_full.z * p1;
        }
      }
    }
  } 

{  
  RobotTrait *t = &(h->personality.onset); 
  if (t->state) {
    t->current_time_microseconds += microseconds_passed;
    if (t->current_time_microseconds > t->movement_time_microseconds) {
      t->state = 0;
      t->current_time_microseconds=0.f;
      }
    else {
      if ((t->action==1)||(t->action==-1)) {
        float p1 =  2.0*t->current_time_microseconds / t->movement_time_microseconds ;
        if (p1 >= 1.0) p1= 2.0-p1; 
	p1 *= t->db_intensity;
        h->current_danced.x += t->movement_full.x * p1;
        h->current_danced.y += t->movement_full.y * p1;
        h->current_danced.z += t->movement_full.z * p1;
        }
      }
    }
  } 
  
    
}







typedef struct hater_environment {
  int has_beat;
  int has_onset;
  float pitch;
  float db_level_as_intensity;
  int plogi;
  float microseconds_passed;
  } hater_environment;





struct hater_environment larry_harvey_get_environment() {

/* look at our environment */
int has_beat=0;
int has_onset=0;
int try_forward=1;
int to_frame = packet_summary.now_frame+try_forward;
if (to_frame >packet_summary.end_frame) to_frame=packet_summary.end_frame;
int from_frame=to_frame-4;
if (from_frame<packet_summary.no_gap_frame) from_frame=packet_summary.no_gap_frame;
int f;
for (f=from_frame;f<to_frame;f++) {
            int index = (packet_summary.start_index + f -
                     packet_summary.start_frame) % PACKET_SUMMARY_SIZE;
            if (index<0) index += PACKET_SUMMARY_SIZE;
            struct packet *p;
            p = packet_summary.packets+index;
            if (p->has_beat) {
                has_beat=1;
                }
            if (p->has_onset) {
                has_onset=1;
                }
            }

  
    
float pitch;
int plogi;
float db_level_as_intensity =  0.3;
        

const float one_over_101 = 1.f / 101.f;
int index2 = (packet_summary.start_index + to_frame  - 
                       packet_summary.start_frame) % PACKET_SUMMARY_SIZE;
if (index2<0) index2 += PACKET_SUMMARY_SIZE;
struct packet *p = packet_summary.packets+index2;
if (p->has_statistics) {
  if ((packet_summary.beat_interval_in_microseconds>0)&&(packet_summary.beat_interval_in_microseconds != 500000)) {
    if (remember_beat_interval_in_microseconds >0) {
      remember_beat_interval_in_microseconds = (remember_beat_interval_in_microseconds *100.
	   + packet_summary.beat_interval_in_microseconds) * one_over_101;;
      }
    else {
      remember_beat_interval_in_microseconds = packet_summary.beat_interval_in_microseconds;
      }
    }
  
    pitch=p->pitch;
    float plog;

    if (pitch>=1.) {
      plog = logf(pitch)*1.44269504088884957607f;  
      plog = plog + 0.700;
        /* first constant is 1/log(2) because ln(x)/ln(2) = log2(x) */
      plogi = (int) plog;

      pitch = plog- (float)plogi;
      pitch = (pitch*8.);
      plogi = (int)pitch;
      plogi = 7-plogi;
      if (plogi>7) plogi=7;
      if (plogi<0) plogi=0;
      }
    else plogi = GetRandomValue(0,7);
    
    float db_level = p->db_level;    
    if (db_level > -10.f) db_level = -10.f;
    if (db_level < -24.f) db_level = -24.f;
    db_level = db_level + 24.; /* range 0 to 14 */
    db_level = db_level / 14.f; /* 0 to 1 - */
    db_level_as_intensity = 0.15 + (db_level*db_level)*0.85;
    
    }
  
float microseconds_passed;
if (last_sample_time.tv_sec==0) {
  last_sample_time = packet_summary.current_sample_time;
  microseconds_passed=1000000./60.;
  }
else {
  long bigdiff=time_difference_in_microseconds(packet_summary.current_sample_time,last_sample_time);
  if (bigdiff<0) bigdiff=-bigdiff;
  if (bigdiff>150000l) {
    last_sample_time = packet_summary.current_sample_time;
    microseconds_passed=1000000./60.; 
    }
  else if (bigdiff==0) {
    last_sample_time = packet_summary.current_sample_time;
    microseconds_passed=1000000./60.; 
    }
  else {
    microseconds_passed=bigdiff;
//    logit("%f md\n",microseconds_passed);
    last_sample_time = packet_summary.current_sample_time;
    }
  }

return (struct hater_environment){.has_beat=has_beat,
  .has_onset = has_onset,
  .pitch = pitch,
  .db_level_as_intensity = db_level_as_intensity,
  .plogi = plogi,
  .microseconds_passed = microseconds_passed
  };
}











extern int exclusive_display_stack;

void larry_harvey_step0(void);
void larry_harvey_step1(void);
void larry_harvey_step2(void);
void larry_harvey_step3(void);
void larry_harvey_step4(void);
void larry_harvey_step5(void);
void larry_harvey_step6(void);



void larry_harvey_step() {
if (movie_scene ==0) larry_harvey_step0();
else if (movie_scene ==1) larry_harvey_step1();
else if (movie_scene ==2) larry_harvey_step2();
else if (movie_scene ==3) larry_harvey_step3();
else if (movie_scene ==4) larry_harvey_step4();
else if (movie_scene ==5) larry_harvey_step5();
else if (movie_scene ==6) larry_harvey_step6();
}
  

extern void larry_harvey_setup1(void);


void larry_harvey_renderFrame0(int screen,glMatrix *extra);
void larry_harvey_renderFrame1(int screen,glMatrix *extra);
void larry_harvey_renderFrame2(int screen,glMatrix *extra);
void larry_harvey_renderFrame3(int screen,glMatrix *extra);
void larry_harvey_renderFrame4(int screen,glMatrix *extra);
void larry_harvey_renderFrame5(int screen,glMatrix *extra);
void larry_harvey_renderFrame6(int screen,glMatrix *extra);

void larry_harvey_renderFrame(int screen,glMatrix *extra) {
if (movie_scene==0)      larry_harvey_renderFrame0(screen,extra);
else if (movie_scene==1) larry_harvey_renderFrame1(screen,extra);
else if (movie_scene==2) larry_harvey_renderFrame2(screen,extra);
else if (movie_scene==3) larry_harvey_renderFrame3(screen,extra);
else if (movie_scene==4) larry_harvey_renderFrame4(screen,extra);
else if (movie_scene==5) larry_harvey_renderFrame5(screen,extra);
else if (movie_scene==6) larry_harvey_renderFrame6(screen,extra);
}





    static int flip=1;

void larry_harvey_step0() {
struct hater_environment e;
e = larry_harvey_get_environment();

/* new robots  */
int letout = 0;
#ifdef __APPLE__
  if (flip) {
#endif  
if (e.has_onset) {
    letout=ROBOTS_PER_BEAT;
    }
if (e.has_beat) letout=ROBOTS_PER_TEMPO;	
#ifdef __APPLE__
}
  flip = !flip;
#endif

// vvv skip scene 0
//letout = ROBOTS_TO_DISPENSE+1;

if (activeRobots>ROBOTS_TO_DISPENSE) {
  letout=0;
  angle_reducer = 0.994;
  }

while (letout--) {
      struct RobotHaters *h = robot_haters+activeRobots;
	
      
      int playnum = GetRandomValue(0,4);
      if (playnum<0) playnum = 0;
      if (playnum>4) playnum = 4;
      //logit("plogi %d pnum %d\n",plogi,playnum);
      
      
      h->active = 1;
      h->robot = larry_harvey_league->teams[e.plogi].players+playnum;
      robot_hater_entry_phase += 0.269f + ((float)GetRandomValue(0,100))*0.002 + 3.1416;
//      robot_hater_entry_phase +=  3.1416/6.f;
//      robot_hater_entry_phase =  ((float)GetRandomValue(0,62832))*0.0001;
      if (robot_hater_entry_phase > 6.2832) {robot_hater_entry_phase -= 6.2832;}
      
      float throw = ((float)GetRandomValue(80,100))*0.008669;
      
	h->current_point = 0.;
	h->total_time = 30.f * 1000000.f;	  
            
		      
      h->current = (Vector3){.x=throw * hcos(robot_hater_entry_phase),
                    .y=throw * hsin(robot_hater_entry_phase),
                    .z=-1.69f};
		    
      float destination_angle = ((float)GetRandomValue(2000,10000))/12000.*6.2832;
      float destination_distance = ((float)GetRandomValue(14,25)) * 0.359f + ((float)GetRandomValue(-50,+50))* 0.00129f;
      
      h->distance = destination_distance;
      
      h->destination = (Vector3){.x=destination_distance * hsin(destination_angle),
                                 .y=destination_distance * hcos(destination_angle),
				 .z=-169.f};
      h->speed = (Vector3){.x=h->current.x - h->destination.x,
         .y=h->current.y - h->destination.y,
         .z=h->current.z - h->destination.z};
				 				 

      RobotTrait *t = &(h->personality.beat);
      t->action = 1; //GetRandomValue(0,16)&8;
      if (GetRandomValue(0,100) <35)  t->action = -t->action;
      t->movement_time_microseconds = ((float)GetRandomValue(120000,200000))*3.; /* in microseconds */
      t->intensity = ((float)GetRandomValue(8,20))*0.069;
      t->current_time_microseconds = 0.f;
      t->state = 0;
      t->percent_got_cue = GetRandomValue(85,100);
      t->percent_clumsy = GetRandomValue(0,23);
      t->movement_full = (Vector3) {.x= ((float)GetRandomValue(1,80)-40)*0.00269,
                                    .y= ((float)GetRandomValue(1,80)-40)*0.00269,
                                    .z= ((float)GetRandomValue(1,80)-40)*0.00269};
 // debug:     t->action = 0;				    

      
      t = &(h->personality.onset);
      t->action = 1;
      if (GetRandomValue(0,100) <35)  t->action = -t->action;
      t->movement_time_microseconds = ((float)GetRandomValue(120000,200000))*1.; /* in microseconds */
      t->intensity = ((float)GetRandomValue(8,20))*0.03;
      t->current_time_microseconds = 0.f;
      t->state = 0;
      t->percent_got_cue = GetRandomValue(60,100);
      t->percent_clumsy = GetRandomValue(0,23);
      t->movement_full = (Vector3) {.x= ((float)GetRandomValue(0,80)-40)*0.0007,
                                    .y= ((float)GetRandomValue(0,80)-40)*0.0007,
                                    .z= ((float)GetRandomValue(0,80)-40)*0.0007};

            
      activeRobots++;
      }

  
if (timer>1200) {}
else if (timer>480) {
  alpha1 = alpha1 - 1.f/480.f;
  }
else {
  alpha2 = alpha2 - 1.f/480.f;
  }
if (timer) timer--;

float sinangle;
float cosangle;


if (e.has_beat) {
  beat_count++;
  if (beat_count&1) {
    if (angle_sign >0) angle_sign=-1.f; else angle_sign=1.f;
    
    /* keep it centered */
    if ((angle_sign >0) && (angle < -angle_size)) {
      angle_sign = ((angle/-angle_size) + 1.f) * 0.5f;
      }
    else if ((angle_sign <0) && (angle > angle_size)) {
      angle_sign = ((angle/angle_size) + 1.f) * -0.5f;
      }
      

    }
  }

if (remember_beat_interval_in_microseconds<=0) {
  angle+=0.;
  }
else {
  angle +=  angle_sign * e.microseconds_passed / remember_beat_interval_in_microseconds * angle_size;
  }
angle *= angle_reducer;
angle_size *= angle_reducer;

sinangle = hsin(angle);
cosangle = hcos(angle);
int active_flag= !activeRobots;
for (int i=0;i<activeRobots;i++) {
  struct RobotHaters *h = robot_haters+i;
  h->current_point = h->current_point + e.microseconds_passed;
  h->sparkle = (GetRandomValue(0,1000)<33?1:0);
  
  if (h->active) {
    active_flag=1;
    float factor_in = (h->total_time-h->current_point)/h->total_time;
    float factor_out;
    factor_in=factor_in;
    if (factor_in<=0.5f) factor_out = 1.f-(factor_in*factor_in*4.f)*0.5f;
    else factor_out = ((1.f-factor_in)*(1.f-factor_in)*4.)*0.5;
    factor_out = 1. - factor_out;
    if (factor_in>=0.25) {
// FINISH LEVEL 0 right away
//    if (1==0) {
      h->current = (Vector3){.x=h->destination.x+h->speed.x*factor_out,
                   .y=h->destination.y+h->speed.y*factor_out,
                   .z=h->destination.z+h->speed.z*factor_out};
    
      /* angle is 90 degrees each beat, assuming 60 frames a second */
      h->current_spun = (Vector3){.x=h->current.x * cosangle + h->current.y * sinangle,
                             .y= -h->current.x * sinangle + h->current.y * cosangle,
			     .z= h->current.z};

      update_dance_moves(h,sinangle,cosangle,e.microseconds_passed,e.has_beat,e.has_onset,e.db_level_as_intensity);
      h->current_final = h->current_danced;        
      }
    else {
      h->active=0;
      h->current = h->destination;
      h->current_spun = h->destination;
      h->current_danced = h->destination;
      h->current_final = h->destination;
//      logit("A %f,%f,%f\n",h->destination.x,h->destination.y,h->destination.z);

      }
    } // if active
  }


if (!active_flag) { /* when we are all loaded, time to move on */
  movie_scene=1;
  larry_harvey_setup1();
  }


}







  
extern int screen_orientation;

#ifdef ANDROID
//#define ASPECT_RATIO (width_in_pixels/height_in_pixels)
#else
//#define ASPECT_RATIO (width_in_pixels/height_in_pixels)
#endif
#define ASPECT_RATIO 1.f


void larry_harvey_renderFrame0(int screen,glMatrix *extra) {

glUseProgram(one_color_program);
checkGlError("glup44");   
glEnable(GL_DEPTH_TEST);
mMVPMatrixHandle = glGetUniformLocation(one_color_program, "u_MVPMatrix");
  

//display_set_gl_base_viewport();




loadIdentity(&modelMatrix);
rotate_generic(screen_orientation,&modelMatrix);
GLfloat eye[3] = {0.,0.,0.f};
GLfloat center[3] = {0.f,0.f,-400.f};
GLfloat *up = ups[screen_orientation-1];
ViewLookAt(&viewMatrix,eye,center,up);



//loadIdentity(&perspectiveMatrix);
perspectiveMatrix = MainMenuPerspectiveMatrix;

//logit("%f\n",ASPECT_RATIO);
perspectiveGL(&perspectiveMatrix,45.f,  ASPECT_RATIO,  1., 1500.f);
         
multMatrix(&MVMatrix,&modelMatrix,&viewMatrix);
multMatrix(&MVPMatrix,&MVMatrix,&perspectiveMatrix);

  
glUniformMatrix4fv(mMVPMatrixHandle, 1, GL_FALSE, (GLfloat *)(&MVPMatrix));
checkGlError("glmatm4p");
		

glUseProgram(one_color_program);
checkGlError("glup44qq");   
            
            for (int i = 0; i < activeRobots; i++)
            {

	       struct RobotHaters *h = robot_haters+i;
               if (!(h->active)) {
	             if (h->sparkle) {
                       draw_larry_harvey_robot_container_3d_sparkle(h->robot,
		       (Vector3){h->current_final.x,h->current_final.y,h->current_final.z},
 		       (Vector3){0.30f,0.30f,0.30f}); 
		       }
		     else {
                       draw_larry_harvey_robot_container_3d(h->robot,
		       (Vector3){h->current_final.x,h->current_final.y,h->current_final.z},
		       (Vector3){0.30f,0.30f,0.30f});
		       } 
		    }
	    
	    }
	    
        checkGlError("koutbase"); 
            for (int i = 0; i < activeRobots; i++) {
	       struct RobotHaters *h = robot_haters+i;
               if (h->active) {
                 {
//		   if (i==0) {
//		     logit("%d %f,%f,%f\n",i,h->current_final.x,h->current_final.y,h->current_final.z);
//		     }
//                   draw_larry_harvey_robot_2d(h->robot,(Vector2){h->current_final.x,h->current_final.y},(Vector2){20.f,20.f}); 
		   int simple=0;
                   if (h->current_final.z<-36.9f) { simple=1;}
		   
		   Vector3 current_final = h->current_final;
		   
		   
		   float angle1 = 0;
		   RobotTrait *t = &(h->personality.beat);
		   if (t->state) {
		     int dir=0;
		     if (t->state>0) {dir=1;} else {dir=-1;}
                     angle1 = 2.0*t->current_time_microseconds/t->movement_time_microseconds;
		     if (angle1>1.) angle1 = 2.0-angle1;
		     angle1 *= ((float)dir)*t->intensity;
		     }
		   
		   float angle2 = 0;
		    t = &(h->personality.onset);
		   if (t->state) {
		     int dir;
		     if (t->state>0) {dir=1;} else {dir=-1;}
                     angle2 = 2.0*t->current_time_microseconds/t->movement_time_microseconds;
		     if (angle2>1.) angle2 = 2.0-angle2;
		     angle2 *= ((float)dir)*t->intensity;
		     }
		   
		   float angle = angle1+angle2;
		   float asin;
		   float acos;
		   asin = hsin(angle);acos = hcos(angle); 
		     
		   if (simple) {
		     draw_larry_harvey_robot_container_3d_rotate(h->robot,current_final,
		     (Vector3){0.15f*1.5,0.15f*2.5f,0.15f},asin,acos);
        checkGlError("koutr1"); 
		     }
		   else {
                     draw_larry_harvey_robot_3d_rotate(h->robot,current_final,
  		     (Vector3){0.15f,0.15f,0.15f},asin,acos); 
        checkGlError("koutr2"); 
                     }		     
		   } // block
                  } // if active
		 
            } // for


								     

	    

        
          if (timer) {	    
//            display_set_gl_base_viewport();
glUseProgram(one_color_program);
checkGlError("glup44qq77");   

lc->needed_points->line_width=0.100f;
      if (alpha1>0.3) {
        checkGlError("kout00"); 
        glUniform4f(colorHandle, alpha1,alpha1,alpha1,1.0f);
        checkGlError("kout2"); 
//#define WORLD_Larry_qt_s_Possee_the_Robot_Haters "Larry's Possee the Robot Haters"
        letters_out(lc,0.037f,-0.375,0.25f,-1.25f,WORLD_Larry_qt_s_Possee_the_Robot_Haters);
        checkGlError("kout2A"); 
//#define WORLD_Developed_by_Hipolito_Medrano "Developed by Hipolito Medrano"
        letters_out(lc,0.04f,-0.4,0.1f,-1.5f,WORLD_Developed_by_Hipolito_Medrano);
//#define WORLD_and_Hibbard_Engler "and Hibbard Engler"
        letters_out(lc,0.04f,-0.25,0.02f,-1.7f,WORLD_and_Hibbard_Engler);
        checkGlError("kout2B"); 
	}
      checkGlError("kout1dba");
      glUniform4f(colorHandle, alpha2*1.0f,alpha2*0.6f,alpha2*0.85f,1.0f);
      checkGlError("kout1dbb");
      lc->needed_points->line_width=0.120f;
//#define WORLD_In_Rememberance_of "In Rememberance of"
      letters_out(lc,0.06f,-0.4,-0.15f,-1.5f,WORLD_In_Rememberance_of);
//#define WORLD_Larry_Harvey "Larry Harvey"
      letters_out(lc,0.06f,-0.4,-0.25f,-1.5f,WORLD_Larry_Harvey);
      
      checkGlError("kout1d");
      }


/* test for handling the alighment fo thte hater robots */
//#define TESTA
#ifdef TESTA
	float out=0.42f;
        glUniform4f(colorHandle, 1.f,1.f,1.f,1.0f);
        checkGlError("kout2"); 
//#define WORLD_display_larry_harvey_hater_robots_E "E"
        letters_out(lc,0.037f,out,0.f,-1.5f,WORLD_display_larry_harvey_hater_robots_E);
        checkGlError("kout2"); 
//#define WORLD_display_larry_harvey_hater_robots_W "W"
        letters_out(lc,0.037f,-out,0.f,-1.5f,WORLD_display_larry_harvey_hater_robots_W);
        checkGlError("kout2"); 
//#define WORLD_display_larry_harvey_hater_robots_S "S"
        letters_out(lc,0.037f,0.f,-out,-1.5f,WORLD_display_larry_harvey_hater_robots_S);
        checkGlError("kout2"); 
//#define WORLD_display_larry_harvey_hater_robots_N "N"
        letters_out(lc,0.037f,0.f,out,-1.5f,WORLD_display_larry_harvey_hater_robots_N);
        checkGlError("kout2"); 
	out = out / 2.;
//#define WORLD_display_larry_harvey_hater_robots_e "e"
        letters_out(lc,0.027f,out,0.f,-1.5f,WORLD_display_larry_harvey_hater_robots_e);
        checkGlError("kout2"); 
//#define WORLD_display_larry_harvey_hater_robots_w "w"
        letters_out(lc,0.027f,-out,0.f,-1.5f,WORLD_display_larry_harvey_hater_robots_w);
        checkGlError("kout2"); 
//#define WORLD_display_larry_harvey_hater_robots_s "s"
        letters_out(lc,0.027f,0.f,-out,-1.5f,WORLD_display_larry_harvey_hater_robots_s);
        checkGlError("kout2"); 
//#define WORLD_display_larry_harvey_hater_robots_n "n"
        letters_out(lc,0.027f,0.f,out,-1.5f,WORLD_display_larry_harvey_hater_robots_n);
        checkGlError("kout2"); 
//#define WORLD_display_larry_harvey_hater_robots__plus_ "+"
        letters_out(lc,0.06f,0.f,0.f,-1.5f,WORLD_display_larry_harvey_hater_robots__plus_);
        checkGlError("kout2"); 
#endif
    glLineWidth(1.f);
      
}


































typedef struct camera_path {
  float from_eye[3];
  float to_eye[3];
  float from_up[3];
  float to_up[3];
  float from_center[3];
  float to_center[3];
  float from_translate[3];
  float to_translate[3];
  float from_spaceship[3];
  float to_spaceship[3];
  float total_microseconds;
  } camera_path;

/*
static camera_path scene1_camera_paths[1] =  { (camera_path) {
  .from_eye = {0.f,0.f,0.f},
  .to_eye = {0.f,0.f,0.f},
  .from_up = {0.f,1.f,0.f},
  .to_up = {0.3f, 0.7f,0.f},
  .from_center = {0.f,0.f,f},
  .to_center = {-2.f,40.f,-169.f+5.f},
  .total_microseconds = 40.f * 1000000.f,
 } };
*/

static camera_path scene1_camera_paths[1] =  { (camera_path) {
  .from_eye = {0.f,0.f,0.f},
  .to_eye = {0.f,0.f,5.f},
  .from_up = {0.f,1.f,0.f},
  .to_up = {0.f, 1.f,5.f},
  .from_center = {0.f,0.f,-400.f},
  .to_center = {0.f,0.f,5.f},
  .from_translate = {0.f,0.f,0.f},
  .to_translate = {0.f,0.f,1.25f},
  .from_spaceship = {-120.f,45.f,10.f},
  .to_spaceship = {-10.f,25.45f,-169.f},
  .total_microseconds = 18.f * 1000000.f,
 } };

static camera_path scene2_camera_paths[1] =  { (camera_path) { /* just uses total microseconds */
  .from_eye = {0.f,0.f,0.f},
  .to_eye = {0.f,0.f,5.f},
  .from_up = {0.f,1.f,0.f},
  .to_up = {0.f, 1.f,5.f},
  .from_center = {0.f,0.f,-400.f},
  .to_center = {0.f,0.f,5.f},
  .from_translate = {0.f,0.f,0.f},
  .to_translate = {0.f,0.f,1.25f},
  .from_spaceship = {-120.f,45.f,10.f},
  .to_spaceship = {-10.f,25.45f,-169.f},
  .total_microseconds = 36.f * 1000000.f,
 } };

static camera_path scene3_camera_paths[1] =  { (camera_path) { /* just uses total microseconds */
  .from_eye = {0.f,0.f,0.f},
  .to_eye = {0.f,0.f,0.f},
  .from_up = {0.f,1.f,0.f},
  .to_up = {0.f, 1.f,0.f},
  .from_center = {0.f,0.f,-200.f},
  .to_center = {0.f,0.f,-400.f},
  .from_translate = {0.f,0.f,0.f},
  .to_translate = {0.f,0.f,0.f},
  .from_spaceship = {-10.f,25.45f,-169.f},
  .to_spaceship = {-120.f,45.f,10.f},
  .total_microseconds = 8.f * 1000000.f,
 } };


#define interpolate(from,to,point,now) {now[0]=(to[0]-from[0])*point + from[0]; now[1]=(to[1]-from[1])*point + from[1]; now[2]=(to[2]-from[2])*point + from[2]; }
#define interpolate_into_vector3(from,to,point,now) {now.x=(to[0]-from[0])*point + from[0]; now.y=(to[1]-from[1])*point + from[1]; now.z=(to[2]-from[2])*point + from[2]; }
#define quadratic(aaa) ((aaa<0.5)?(2.0f*aaa*aaa):(0.5f+2.0f*(aaa-0.5f)*(aaa-0.5f)))
#define triangle(aaa) ((aaa<0.5)?(2.0f*aaa):(1.f-2.0f*(aaa-0.5f)))

static float point0 = 0.f;
static float point=0.f;
static float point_space=0.50f;

#define LARRY_HARVEY_DISPLAY_SET 9
void larry_harvey_setup1() {
   exclusive_display_stack=LARRY_HARVEY_DISPLAY_SET;
   point = 0.f;
   point0 = 0.f;
   point_space = 0.5f;
   struct RobotHaters *h = robot_haters+SPACESHIP_ROBOT;
   h->robot = larry_harvey_league->teams[8].players+0;
}

void larry_harvey_step1() {
struct hater_environment e;
e = larry_harvey_get_environment();

  point0 = point0 +  e.microseconds_passed /scene1_camera_paths[0].total_microseconds ;
  point = quadratic(point0);
  float pt2 = point0*1.3 + 0.5;
  if (pt2>1.) pt2=1.;
  point_space=quadratic(pt2);


for (int i=0;i<activeRobots;i++) {
  struct RobotHaters *h = robot_haters+i;
  h->sparkle = (GetRandomValue(0,1000)<33?1:0);
  }

if (point0>=0.7) {
  void larry_harvey_setup2(void);
  movie_scene=2;
  larry_harvey_setup2();
  }
}


void render_trash_fence(void);




void larry_harvey_renderFrame1(int screen,glMatrix *extra) {

glUseProgram(one_color_program);
checkGlError("glup44");   
glEnable(GL_DEPTH_TEST);
mMVPMatrixHandle = glGetUniformLocation(one_color_program, "u_MVPMatrix");

//display_set_gl_base_viewport();

//loadIdentity(&modelMatrix);
camera_path p = scene1_camera_paths[0];
GLfloat translate[3] = {0.,0.,0.f};
GLfloat eye[3] = {0.,0.,0.f};
GLfloat center[3] = {0.f,0.f,-400.f};
GLfloat *up = ups[screen_orientation-1];
/*
interpolate(p.from_eye,p.to_eye,point,eye);
interpolate(p.from_up,p.to_up,point,up);
interpolate(p.from_center,p.to_center,point,center);
*/


interpolate(p.from_translate,p.to_translate,point,translate);
translateMatrix(&modelMatrix,translate[0],translate[1],translate[2]);

glMatrix mm;
mm=modelMatrix;
rotate_generic(screen_orientation,&mm);

ViewLookAt(&viewMatrix,eye,center,up);

perspectiveMatrix = MainMenuPerspectiveMatrix;
perspectiveGL(&perspectiveMatrix,45.f,  ASPECT_RATIO,  1.f+eye[2], 400.f+eye[2]);


glUseProgram(one_color_program);
checkGlError("glup44");   
       
multMatrix(&MVMatrix,&mm,&viewMatrix);
multMatrix(&MVPMatrix,&MVMatrix,&perspectiveMatrix);

glUniformMatrix4fv(mMVPMatrixHandle, 1, GL_FALSE, (GLfloat *)(&MVPMatrix));
checkGlError("glmatm4p");

		
/* trash fence*/
render_trash_fence();		

            
            for (int i = 0; i < activeRobots; i++)
            {

	       struct RobotHaters *h = robot_haters+i;
               if (!(h->active)) {
	             if (h->sparkle) {
                       draw_larry_harvey_robot_container_3d_sparkle(h->robot,(Vector3){h->current_final.x,h->current_final.y,h->current_final.z},
 		       (Vector3){0.30f,0.30f,0.30f}); 
		       }
		     else {
                       draw_larry_harvey_robot_container_3d(h->robot,(Vector3){h->current_final.x,h->current_final.y,h->current_final.z},
		       (Vector3){0.30f,0.30f,0.30f});
		       } 
		    }
	    
	    }
	    

								     
glUseProgram(one_color_program);
checkGlError("glup44qwe");   
if ( ((int)(point0*20.))&1) {
      glUniform4f(colorHandle, 0.f,0.9f,0.1f,1.0f);
      lc->needed_points->line_width=0.120f;
//#define WORLD_UFOs_LAND_HERE__minus__gt_ "UFOs LAND HERE ->"
      letters_out(lc,WORLD_UFOs_LAND_HERE__minus__gt_scale,
        WORLD_UFOs_LAND_HERE__minus__gt_x,
	WORLD_UFOs_LAND_HERE__minus__gt_y,
	WORLD_UFOs_LAND_HERE__minus__gt_z,WORLD_UFOs_LAND_HERE__minus__gt_);
      }


struct RobotHaters *h = robot_haters+SPACESHIP_ROBOT;
Vector3 spaceship_point;
interpolate_into_vector3(p.from_spaceship,p.to_spaceship,point_space,spaceship_point);
draw_larry_harvey_robot_3d_rotate(h->robot,spaceship_point,
                     (Vector3){1.0f,0.50f,0.50f},0.0f,1.0f);


}





/* alien, spaceship, and larry comes out, piece of paper. */


void larry_harvey_setup2() {
   exclusive_display_stack=LARRY_HARVEY_DISPLAY_SET;
   point = 0.f;
   point0 = 0.f;
   point_space = 0.5f;
   struct RobotHaters *h = robot_haters+SPACESHIP_ROBOT;
   h->robot = larry_harvey_league->teams[8].players+0;
   h = robot_haters+CTHULU_ALIEN;
   h->robot = larry_harvey_league->teams[8].players+1;
   h = robot_haters+LARRY_ROBOT;
   h->robot = larry_harvey_league->teams[8].players+2;
   h = robot_haters+FULL_PAPER;
   h->robot = larry_harvey_league->teams[8].players+3;
   h = robot_haters+ROLLED_UP_PAPER;
   h->robot = larry_harvey_league->teams[8].players+4;
}



void larry_harvey_step2() {
struct hater_environment e;
e = larry_harvey_get_environment();

  point0 = point0 +  e.microseconds_passed /scene2_camera_paths[0].total_microseconds ;
if (point0>=1.0f) {
  void larry_harvey_setup3();
  movie_scene=3;
  larry_harvey_setup3();
  return;
  }
  
if (point0>=1.) point0=1.;
point = quadratic(point0);
float pt2 = point0*1.2 + 0.5;
if (pt2>1.) pt2=1.;
point_space=quadratic(pt2);


}











void larry_harvey_renderFrame2(int screen,glMatrix *extra) {
glUseProgram(one_color_program);
checkGlError("glup44");   
glEnable(GL_DEPTH_TEST);
mMVPMatrixHandle = glGetUniformLocation(one_color_program, "u_MVPMatrix");

//display_set_gl_base_viewport();
    loadIdentity(&modelMatrix);
    rotate_generic(screen_orientation,&modelMatrix);
    
    loadIdentity(&effectMatrix);
    loadIdentity(&viewMatrix);
    loadIdentity(&perspectiveMatrix);  

//loadIdentity(&modelMatrix);
GLfloat eye[3] = {0.,0.,0.f};
GLfloat center[3] = {0.f,0.f,-400.f};
GLfloat *up = ups[screen_orientation-1];

/*
interpolate(p.from_eye,p.to_eye,point,eye);
interpolate(p.from_up,p.to_up,point,up);
interpolate(p.from_center,p.to_center,point,center);
*/


//interpolate(p.from_translate,p.to_translate,point,translate);
//translateMatrix(&modelMatrix,translate[0],translate[1],translate[2]);

ViewLookAt(&viewMatrix,eye,center,up);

perspectiveMatrix = MainMenuPerspectiveMatrix;

perspectiveGL(&perspectiveMatrix,45.f,  ASPECT_RATIO,  1.f+eye[2], 400.f+eye[2]);


       
multMatrix(&MVMatrix,&modelMatrix,&viewMatrix);
multMatrix(&MVPMatrix,&MVMatrix,&perspectiveMatrix);

glUniformMatrix4fv(mMVPMatrixHandle, 1, GL_FALSE, (GLfloat *)(&MVPMatrix));
checkGlError("glmatm4p");

		
// draw spaceship
struct RobotHaters *h = robot_haters+SPACESHIP_ROBOT;
Vector3 spaceship_point =  {+0.3f,+1.f,-4.f};
// spaceship_point was set in frame 2
draw_larry_harvey_robot_3d_rotate(h->robot,spaceship_point,
                     (Vector3){0.75f,0.50f,0.50f},0.0f,1.0f);


if (point0<=0.2) {
  /* show larry, spaceship and cthulu */
  h = robot_haters+CTHULU_ALIEN;
  const Vector3 cthulu_point = {+0.55f - 0.45f,0.f,-2.f};
  draw_larry_harvey_robot_3d_rotate(h->robot,cthulu_point,
                     (Vector3){0.15f,0.15f,0.15f},0.f,1.f);
		  

  h = robot_haters+LARRY_ROBOT;
  const Vector3 larry_point = {0.416f-0.45f,0.f,-2.f};
  draw_larry_harvey_robot_3d_no_eyes_rotate_colors(h->robot,larry_point,
                     (Vector3){0.15f,0.15f,0.15f},0.f,1.f,(Color){100,100,100,255},
		     (Color){100,100,100,255},
		     (Color){100,100,100,255});
		  
  }
else if (point0 <= 0.35) {
  /* have cthulu prod larry with a rolled up paper */
  h = robot_haters+CTHULU_ALIEN;
  const Vector3 cthulu_point = {+0.55f-0.45f,0.f,-2.f};
  draw_larry_harvey_robot_3d_rotate(h->robot,cthulu_point,
                     (Vector3){0.15f,0.15f,0.15f},0.f,1.f);

  point = (point0-0.2)*40.0;
  if (point>1.) point = 1.;
  point = triangle(point);
  float pointangle = point*-3.14159*0.5;
  
  Color paper_color = (Color){255,255,255,255};
  if (point > 0.93f) {
    paper_color = (Color){255,0,128,255};
    }
    							  			  		  		  
  h = robot_haters+ROLLED_UP_PAPER;
  const Vector3 paper_point = {0.483f-0.45f+hsin(pointangle)*0.15f*(0.1f),
                               -0.006f+hcos(pointangle)*0.15f*(0.1f),-1.9f};
  draw_larry_harvey_robot_3d_no_eyes_rotate_colors(h->robot,paper_point,
                     (Vector3){0.15f,0.15f,0.15f},hsin(pointangle),hcos(pointangle),paper_color,
		     paper_color,paper_color);

  Color c1;
  Color c2;
  Color c3;
  Color ceye1;
  Color ceye2;
  h = robot_haters+LARRY_ROBOT;
  const Vector3 larry_point = {0.416f-0.45f,0.f,-2.f};
  if (point0 > 0.3) {
    point = (point0-0.3)*15.0;	  	 
    if (point>1.) point = 1.;
    c1 = (Color){100.f + (((float)(h->robot->usual_first_color.r))-100.f) * point,
                 100.f + (((float)(h->robot->usual_first_color.g))-100.f) * point,
                 100.f + (((float)(h->robot->usual_first_color.b))-100.f) * point,255};
    c2 = (Color){100.f + (((float)(h->robot->usual_second_color.r))-100.f) * point,
                 100.f + (((float)(h->robot->usual_second_color.g))-100.f) * point,
                 100.f + (((float)(h->robot->usual_second_color.b))-100.f) * point,255};
    c3 = (Color){100.f + (((float)(h->robot->usual_third_color.r))-100.f) * point,
                 100.f + (((float)(h->robot->usual_third_color.g))-100.f) * point,
                 100.f + (((float)(h->robot->usual_third_color.b))-100.f) * point,255};

    ceye1 = (Color){255.f + (0.f-255.f) * point,
                 0.f + (0.f-0.f) * point,
                 200.f + (0.f-200.f) * point,255};
    ceye2 = (Color){255.f + (255.f-255.f) * point,
                 0.f + (255.f-0.f) * point,
                 200.f + (255.f-200.f) * point,255};
		 
		 
    }
  else {
    c1 = (Color){100,100,100,255};
    c2 = (Color){100,100,100,255};
    c3 = (Color){100,100,100,255};
    ceye1 = (Color){255,0,200,255};
    ceye2 = (Color){255,0,200,255};
    }
  
  
  
  draw_larry_harvey_robot_3d_no_eyes_rotate_colors(h->robot,larry_point,
                     (Vector3){0.15f,0.15f,0.15f},0.f,1.f,c1,c2,c3);
		     
  if (point0>0.3) {
    draw_larry_harvey_robot_eyes_only_3d_rotate(
      h->robot,larry_point,(Vector3){0.15f,0.15f,0.15f},0.f,1.f,ceye1,ceye2);
    }  		     
				     		     
  }  
 
else if (point0 <= 1.) {


  /* here cthulu gives the letter to larry, and we read it. */
  h = robot_haters+CTHULU_ALIEN;
  const Vector3 cthulu_point = {+0.55f-0.45f,0.f,-2.f};
  draw_larry_harvey_robot_3d_rotate(h->robot,cthulu_point,
                     (Vector3){0.15f,0.15f,0.15f},0.f,1.f);
		     
  int pnum=0;
  if (point0 > 0.50f) {
    /* here cthulu tells larry that 
     The glory and good will stay.  
     And the problems no longer apply.		     
     */
    pnum = ((point0-0.50f)/0.50f)*4.f;
    if (pnum>3) pnum=3; 

    glUniform4f(colorHandle, 1.f,0.9f,0.3f,1.0f);
    lc->needed_points->line_width=0.069f;
    if (pnum==0) {
//#define WORLD_Larry_comma__thanks_for_Burning_Man "Larry, thanks for Burning Man"
      letters_out(lc,0.028f,0.22f-0.3f,0.15f,-1.9f,WORLD_Larry_comma__thanks_for_Burning_Man);
//#define WORLD_And_comma__well_comma__thats_about_it "And, well, thats about it"
      letters_out(lc,0.028f,0.24f-0.3f,0.10f,-1.85f,WORLD_And_comma__well_comma__thats_about_it);
      }  
    else if (pnum==1) {  
//#define WORLD_We_came_to_be_because_of_you "We came to be because of you"
      letters_out(lc,0.028f,0.22f-0.3f,0.15f,-1.9f,WORLD_We_came_to_be_because_of_you);
//#define WORLD_And_brought_you_here "And brought you here"
      letters_out(lc,0.028f,0.24f-0.3f,0.10f,-1.85f,WORLD_And_brought_you_here);
      }
    else if (pnum==2) {
//#define WORLD_All_of_the_good_times_you "All of the good times you"
      letters_out(lc,0.028f,0.22f-0.3f,0.15f,-1.9f,WORLD_All_of_the_good_times_you);
//#define WORLD_keep_dot__Take_this_letter "keep. Take this letter"
      letters_out(lc,0.028f,0.24f-0.3f,0.10f,-1.85f,WORLD_keep_dot__Take_this_letter);
      }
    else if (pnum==3) {
//#define WORLD_Thats_all_you_get "Thats all you get"
      letters_out(lc,0.028f,0.22f-0.3f,0.15f,-1.9f,WORLD_Thats_all_you_get);
//#define WORLD_Now_comma__lets_burn_something_excl_ "Now, lets burn something!"
      letters_out(lc,0.028f,0.24f-0.3f,0.10f,-1.85f,WORLD_Now_comma__lets_burn_something_excl_);
      }
  }
		     
		     

  Color paper_color = (Color){255,255,255,255};
  
  if (pnum<2) { /* has the paper in hand */
    h = robot_haters+ROLLED_UP_PAPER;
    const Vector3 paper_point = {0.483f-0.45f,-0.006f+1.f*0.15f*(0.1f),-1.9f};
    draw_larry_harvey_robot_3d_no_eyes_rotate_colors(h->robot,paper_point,
                     (Vector3){0.15f,0.15f,0.15f},0.f,1.f,paper_color,paper_color,paper_color);

    }
  else if (pnum<3) {
           /* cthulu gives the paper to larry */	  		   
    h = robot_haters+FULL_PAPER;
    const float paper_point_from[3] = {0.478f-0.45f,-0.006f,-1.9f};
    const float paper_point_to[3] = {0.428f-0.45f,-0.006f,-1.87f};
    Vector3 paper_point;
    float position =  ( ((point0-0.50f)/0.50f)*4.f - ((float)pnum));
    if (position>1) position=1.;
    position = quadratic(position);
    interpolate_into_vector3(paper_point_from,paper_point_to,position,paper_point);

    draw_larry_harvey_robot_3d_no_eyes_rotate_colors(h->robot,paper_point,
                     (Vector3){0.15f,0.15f,0.15f},0.0f,1.0f,paper_color,paper_color,paper_color);
    }
  else if (pnum<4) {
           /* larry reads the paper */	  		   
    h = robot_haters+FULL_PAPER;
    const Vector3 paper_point = {0.428f-0.45f,-0.006f,-1.9f};
    draw_larry_harvey_robot_3d_no_eyes_rotate_colors(h->robot,paper_point,
                     (Vector3){0.15f,0.15f,0.15f},0.0f,1.0f,paper_color,paper_color,paper_color);
    }

  h = robot_haters+LARRY_ROBOT;
  const Vector3 larry_point = {0.416f-0.45f,0.f,-2.f};
  draw_larry_harvey_robot_3d_rotate(
      h->robot,larry_point,(Vector3){0.15f,0.15f,0.15f},0.f,1.f);
  
  
  
  
  } 

}



/* show the paper */




void larry_harvey_onetime_setup3() {
//larry_harvey_hater_pd_joke_texture = load_png_asset_into_texture("larry_harvey_hater_pd_joke.png");
//larry_harvey_hater_pd_joke_buffer = create_vbo(sizeof(default_active_splash_rect), default_active_splash_rect, GL_STATIC_DRAW,
// &larry_harvey_hater_pd_joke_buffer);   
}


void larry_harvey_setup3() {
   exclusive_display_stack=LARRY_HARVEY_DISPLAY_SET;
   point = 0.f;
   point0 = 0.f;
   point_space = 0.5f;
   logit("start3\n");
}



void larry_harvey_step3() {
struct hater_environment e;
e = larry_harvey_get_environment();

  point0 = point0 +  e.microseconds_passed /scene3_camera_paths[0].total_microseconds ;
  if (point0>=1.) point0=1.;
  point = quadratic(point0);
  float pt2 = point0*1.2 + 0.5;
  if (pt2>1.) pt2=1.;
  point_space=quadratic(pt2);
  
if (point0==1.) {
  activeRobots=0;
  movie_scene=0;
  exclusive_display_stack=0;
  }


}






void larry_harvey_renderFrame3(int screen,glMatrix *extra) {


glUseProgram(one_color_program);
checkGlError("glup44");   
glEnable(GL_DEPTH_TEST);
mMVPMatrixHandle = glGetUniformLocation(one_color_program, "u_MVPMatrix");

//display_set_gl_base_viewport();

//loadIdentity(&modelMatrix);
camera_path p = scene3_camera_paths[0];
GLfloat translate[3] = {0.,0.,0.f};
GLfloat eye[3] = {0.,0.,0.f};
GLfloat center[3] = {0.f,0.f,-400.f};
GLfloat *up = ups[screen_orientation-1];

interpolate(p.from_eye,p.to_eye,point,eye);
interpolate(p.from_up,p.to_up,point,up);
interpolate(p.from_center,p.to_center,point,center);



interpolate(p.from_translate,p.to_translate,point,translate);
translateMatrix(&modelMatrix,translate[0],translate[1],translate[2]);
glMatrix mm;
mm=modelMatrix;
rotate_generic(screen_orientation,&mm);

ViewLookAt(&viewMatrix,eye,center,up);

perspectiveMatrix = MainMenuPerspectiveMatrix;
perspectiveGL(&perspectiveMatrix,45.f,  ASPECT_RATIO,  1.f+eye[2], 400.f+eye[2]);


       
multMatrix(&MVMatrix,&mm,&viewMatrix);
multMatrix(&MVPMatrix,&MVMatrix,&perspectiveMatrix);

glUniformMatrix4fv(mMVPMatrixHandle, 1, GL_FALSE, (GLfloat *)(&MVPMatrix));
checkGlError("glmatm4p");

glUseProgram(one_color_program);
checkGlError("glup44t");   

		
/* trash fence*/
render_trash_fence();		

            
            for (int i = 0; i < activeRobots; i++)
            {

	       struct RobotHaters *h = robot_haters+i;
	       float factor;
	       factor = point0*5.0f;
	       if (factor>1.f) factor = 1.f;
	       factor = factor *sqrtf((h->current_final.x*h->current_final.x)+(h->current_final.y*h->current_final.y));
	       
               if (!(h->active)) {
	             if (h->sparkle) {
                       draw_larry_harvey_robot_container_3d_sparkle(h->robot,(Vector3){h->current_final.x,
		             h->current_final.y,h->current_final.z-factor},
 		       (Vector3){0.30f,0.30f,0.30f}); 
		       }
		     else {
                       draw_larry_harvey_robot_container_3d(h->robot,(Vector3){h->current_final.x,h->current_final.y,
		       h->current_final.z-factor},
		       (Vector3){0.30f,0.30f,0.30f});
		       } 
		    }
	    
	    }
	    

/*								     
if ( ((int)(point0*20.))&1) {
      glUniform4f(colorHandle, 0.f,0.9f,0.1f,1.0f);
      lc->needed_points->line_width=0.120f;
      letters_out(lc,1.f,-26.f,25.f,-169.f,"UFOs LAND HERE ->");
      }
*/


struct RobotHaters *h = robot_haters+SPACESHIP_ROBOT;
Vector3 spaceship_point;
interpolate_into_vector3(p.from_spaceship,p.to_spaceship,point_space,spaceship_point);
draw_larry_harvey_robot_3d_rotate(h->robot,spaceship_point,
                     (Vector3){1.0f,0.50f,0.50f},0.0f,1.0f);



}






void larry_harvey_setup4() {
}



void larry_harvey_step4() {
}




void larry_harvey_renderFrame4(int screen,glMatrix *extra) {
}





void larry_harvey_setup5() {
}



void larry_harvey_step5() {
}


void larry_harvey_renderFrame5(int screen,glMatrix *extra) {
}




void larry_harvey_setup6() {
}



void larry_harvey_step6() {
}


void larry_harvey_renderFrame6(int screen,glMatrix *extra) {
}


static void GLDrawLine3D(GLfloat x1,GLfloat y1,GLfloat z1,GLfloat x2,GLfloat y2,GLfloat z2) {
GLfloat vertices[70];
vertices[0] = x1;
vertices[1] = y1;
vertices[2] = z1;
vertices[3] = x2;
vertices[4] = y2;
vertices[5] = z2;
glUseProgram(one_color_program);
GLint mPositionHandle ;
// get handle to vertex shader's vPosition member
mPositionHandle = glGetAttribLocation(one_color_program, "vPosition");

// Enable a handle to the triangle vertices
glEnableVertexAttribArray(mPositionHandle);

// Prepare the triangle coordinate data
glVertexAttribPointer(mPositionHandle, 3,
                                 GL_FLOAT, GL_FALSE,0
                                 ,vertices);
// Draw the triangle
glDrawArrays(GL_LINES, 0, 2);
// Disable vertex array
glDisableVertexAttribArray(mPositionHandle);
}




void render_trash_fence() {
int i;
glUniform4f(colorHandle, 0.3f,0.17f,0.f,1.f);
for (i=0;i<5;i++) {
  float a1,a2;
  float distance;
  distance = 18.5f;
  a1 = ((float)i)*6.2832f/5.f;
  a2 = ((float)i+1)*6.2832f/5.f;
  GLDrawLine3D(hsin(a1)*distance,hcos(a1)*distance,-169.f,
               hsin(a2)*distance,hcos(a2)*distance,-169.f);
//logit("l %f,%f%f -> %f,%f,%f\n",hcos(a1)*distance,hsin(a1)*distance,-169.f,
//               hcos(a2)*distance,hsin(a2)*distance,-169.f);

  }  

}











void larry_harvey_renderBackground(int screen,glMatrix *extra) {
}

void larry_harvey_renderForeground(int screen,glMatrix *extra) {
}

void larry_harvey_finishStreaming() {
}

void larry_harvey_stepButton(void) {
}


static float button_rotate_angle3f = 0.f;
static int button_hater_team=7;
static int button_hater_player=2;

void larry_harvey_renderButton(int program,int mColorHandle
   ,int menu_position_handle,glMatrix *view,int screen,glMatrix *extra) {
      
glUseProgram(one_color_program);
//glUseProgram(program);
checkGlError("glUseProgram");


float sina,cosa;
button_rotate_angle3f += 0.0369;
if (button_rotate_angle3f>6.2832) {
  button_rotate_angle3f -= 6.2832;
  button_hater_team = GetRandomValue(0,7);
  button_hater_player = GetRandomValue(0,4);
  } 
sina = hsin(button_rotate_angle3f);
cosa = hcos(button_rotate_angle3f);

   
draw_larry_harvey_robot_3d_rotate(larry_harvey_league->teams[button_hater_team].players+button_hater_player,(Vector3){0.0f,-0.05f,0.f},
    (Vector3){1.7f,1.7f,1.7f},sina,cosa); /* show a purple guy */
checkGlError("robot3d_1");
}











// note - NK Guy did larry's picture







const display_set larry_harvey_display_set = {
//#define WORLD_larry_harvey "larry_harvey"
  .name = WORLD_larry_harvey,
  .setupGraphics = larry_harvey_setupGraphics,
  .init = NULL,
  .finish = NULL,
  .changeScreen = NULL,
  .step = larry_harvey_step,
  .renderBackground = larry_harvey_renderBackground,
  .renderMain  = larry_harvey_renderFrame,
  .renderForeground = larry_harvey_renderForeground,
  .stepButton = larry_harvey_stepButton,
  .renderButton = larry_harvey_renderButton,
  .touchEvent = default_touchEvent,
  .keyEvent = default_keyEvent,
  .finishStreaming = larry_harvey_finishStreaming
  };

/* end of file*/

