/* this is the module for superposition.  It computes the offsets and positions for 
all five players, based on the coordinates given by one set of gesture movements.
Although written for the mindset of player 0, it can be easily adapted for player 1.
It also can handle the ghost coordinates if the screen is too far away.

This translation works on deltas from teh old coordinates in spot 0 to the new coordinates on spot 4, and can be used
for analysis and such.

Some images might be ghosted, so the ghost coordinates are not directly provided.

this has very little dependencies. */


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
   
      
	    
/*
pass the following:
GLfloat **old_vertex[5];  // array of 5 of array of 3 vertices, one per dimenstion however, the vertices might replicated many times
                      // if null, 0.0.0
GLfloat **new_vertex[5];   // array of 5 of array of 3  new vertices.  Note - this can be the same addresses ad old to update in place
int count;   //number of vertices to do 
GLfloat *deltas[5];  // array of 5 of GLfloat deltas on the 3 corners - one for each direction.  this is also calculated
                     // for the first one - if you want to know that sort of thing

This allows changing vertices en masse. This can safely update verticies if you want.
The deltas are optional, if null, they are not saved.

It was written this way to be more compatible with opengles.
*/



extern void compute_superpos_vertices(GLfloat *old_vertex[5],GLfloat *new_vertex[5],
int count,GLfloat *deltas[5]);
