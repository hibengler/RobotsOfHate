#ifndef VIDEO_PLANETS_H
#define VIDEO_PLANETS_H 1
      
#include "player2.h"
#include "larry_harvey_has_a_possee.h"
#include "onecolor_specs.h"
#include <sys/time.h>


#define TETRAHEDRON_INDEXES 12
#define TETRAHEDRON_VERTEXES 4
#define CUBE_INDEXES 36
#define CUBE_VERTEXES 8
#define DIAMOND_INDEXES 24
#define DIAMOND_VERTEXES 6
#define ICOSAHEDRON_INDEXES 60
#define ICOSAHEDRON_VERTEXES 12
#define DODECAHEDRON_INDEXES 180
#define DODECAHEDRON_VERTEXES 32
// Dodacehedron uses the icodahedron tables, but then modifies things by flatening some points


typedef struct video_planet_color_info {
  hate_rps actual_rps;
  int rps_order[4];
  float rps_colors[5][4]; // type 4 is nothing, 0 is rock, 1 is paper, 2 is scissor, 3 is water
  float color_saturation; /* 0 to 1 */
  int number_triangles;
  float water_ratio;
  char *types;
  } video_planet_color_info;


typedef struct video_planet_data {
int number_vertexes;
int number_indexes;
video_planet_color_info color_info;
GLshort *indexes;
GLfloat *vertexes;
GLfloat *colors;
GLfloat *final_vertexes;
GLfloat *final_colors;
void *expansion1;
void *expansion2;
} video_planet_data;

typedef struct video_planet {
int gl_vertex_buf;
GLfloat rotation[3];
GLfloat rotation_delta[3];
video_planet_data *base;
video_planet_data *expanded;
video_planet_data *final;
void *expansion1;
void *expansion2;
} video_planet;











void video_planet_init();

extern void video_planet_draw(hate_game *game,hate_screen *screen,int planet_id);
extern void video_planet_step(hate_game *game);

#endif




