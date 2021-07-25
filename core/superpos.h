/* this is the module for superposition.  It computes the offsets and positions for 
all five players, based on the coordinates given by one set of gesture movements.
Although written for the mindset of player 0, it can be easily adapted for player 1.
It also can handle the ghost coordinates if the screen is too far away.

This translation works on deltas from teh old coordinates in spot 0 to the new coordinates on spot 4, and can be used
for analysis and such.

Some images might be ghosted, so the ghost coordinates are not directly provided.

this has very little dependencies. */
#ifndef SUPERPOS_H
#define SUPERPOS_H

#include "hateglue.h"



#ifdef __EMSCRIPTEN__

#include <emscripten.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#define GL_GLEXT_PROTOTYPES 1
#include <SDL_opengles2.h>

#else

#include <SDL2/SDL.h>

#define GL_GLEXT_PROTOTYPES 1
#include <SDL2/SDL_opengles2.h>
//#include "../world_internationalization.h"
//#include "SDL_gles2_stubcall_overs_wrapper.h"

#endif
   
typedef struct super_point {
float xyz[3];
} super_point;

typedef struct context_point {
int context;
float direction[5]; 
float z; 
/*
1   2
  0
3   4    - for context 0

*/

} context_point;

typedef struct expanded_superpos {
float d[5][5]; /* distance from every point to every other point */
} expanded_superpos;	    
		        
	    
/*
pass the following:
super_point *old_vertex[5];  // array of 5 of array of 3 vertices, one per dimenstion however, the vertices might replicated many times
                      // if null, 0.0.0
super_point *new_vertex[5];   // array of 5 of array of 3  new vertices.  Note - this can be the same addresses ad old to update in place
int count;   //number of vertices to do 
GLfloat *deltas[5];  // array of 5 of GLfloat deltas on the 3 corners - one for each direction.  this is also calculated
                     // for the first one - if you want to know that sort of thing

This allows changing vertices en masse. This can safely update verticies if you want.
The deltas are optional, if null, they are not saved.

It was written this way to be more compatible with opengles.
*/



extern void compute_superpos_vertices(int context,
  float *new_vertex[5], /* new vertices for screen 0,1,2,3,4 */
int count, /* number of vertices to process */
GLfloat dist);


  
extern int * superpos_map_to(int context);

extern int * superpos_map_from(int context);

extern super_point xyz_from_context_to_context (int from_context,float dist,super_point new_point, int to_context);

extern super_point expanded_superpos_to_xyz_context(expanded_superpos *vn,int context);


int xyz_to_expanded_superpos(int from_context,struct expanded_superpos *exp, float dist);

extern int set_from_xyz_in_context(int context,expanded_superpos *ep,super_point p, float dist);




extern int set_from_origin_in_context(int context,expanded_superpos *ep,float dist);

#endif
