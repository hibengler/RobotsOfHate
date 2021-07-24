/*
Silent Radiance wifi broadcast for digital silent disco.
Copyright (C) 2017-2018 Hibbard M. Engler

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU Library Public License   
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Library Public License for more details.

You should have received a copy of the GNU Library Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/
#include "letters.h"
#include <memory.h>
#include <stdlib.h>
#include <stdio.h>

/* test:
gcc -o  letters -g -I .. letters.c letters_ascii.c letters_render_opengles2.c -lm
*/













int lcan_add_vertical_boundary(lcontext *lc) {
						  			  
lneeded_points *lnp = lc->needed_points;
fl down_distance = 1.25f;
lp next_start = (lp){lc->start_point.x,lc->start_point.y-down_distance};
if (next_start.y < lc->max_boundary.lower_left.y) {
  return(0);
  }

return 1;
}





int ladd_vertical_boundary(lcontext *lc) {
fl down_distance = 1.25f;

lp next_start = (lp){lc->start_point.x,lc->start_point.y-down_distance};
if (next_start.y < lc->max_boundary.lower_left.y) {
  return(0);
  }

lc->cursor = 0;
lc->point_cursor  = (lp) {0.f,lc->point_cursor.y-down_distance};

//lp next_start = (lp){lc->start_point.x,lc->start_point.y-down_distance};
lc->horizontal_string_boundary = (lr) {lc->point_cursor,lc->point_cursor};
lc->next_character_boundary = (lr) {lc->point_cursor,lc->point_cursor};
lc->next_character = '\0';
lc->previous_character_boundary = (lr) {lc->point_cursor,lc->point_cursor};
lc->previous_character = '\0';            



// adjust vertical to expand with horizontal  
if (lc->horizontal_string_boundary.upper_right.x > lc->vertical_string_boundary.upper_right.x) 
  lc->vertical_string_boundary.upper_right.x = lc->horizontal_string_boundary.upper_right.x;

if (lc->vertical_string_boundary.upper_right.y <  lc->horizontal_string_boundary.upper_right.y) 
  lc->vertical_string_boundary.upper_right.y =  lc->horizontal_string_boundary.upper_right.y;
  
if (lc->vertical_string_boundary.lower_left.x >  lc->horizontal_string_boundary.lower_left.x) 
  lc->vertical_string_boundary.lower_left.x =  lc->horizontal_string_boundary.lower_left.x;

if (lc->vertical_string_boundary.lower_left.y >  lc->horizontal_string_boundary.lower_left.y) 
  lc->vertical_string_boundary.lower_left.y =  lc->horizontal_string_boundary.lower_left.y;

// adjust vertal to expand with horizontal  
        
      

return 1;
}






int lcan_add_boundary(lcontext *lc) {
						  			  
lneeded_points *lnp = lc->needed_points;
fl next_end = lc->point_cursor.x
	      + lc->next_character_boundary.upper_right.x;
fl kern = lkern_ascii(lc);
if (lc->cursor) next_end += kern;

if (next_end > lc->max_boundary.upper_right.x) {
  return(0);
  }



if (lc->cursor) {
  lc->point_cursor.x += kern;
  }

return 1;
}




int ladd_boundary(lcontext *lc) {
fl next_end = lc->point_cursor.x
	      + lc->next_character_boundary.upper_right.x;

if (next_end > lc->max_boundary.upper_right.x) {
  return(0);
  }

// adjust horizopntal to expand with character    
if (next_end > lc->horizontal_string_boundary.upper_right.x) lc->horizontal_string_boundary.upper_right.x = next_end;

if (lc->horizontal_string_boundary.upper_right.y <  lc->next_character_boundary.upper_right.y) 
  lc->horizontal_string_boundary.upper_right.y =  lc->next_character_boundary.upper_right.y;
  
if (lc->horizontal_string_boundary.lower_left.x >  lc->next_character_boundary.lower_left.x) 
  lc->horizontal_string_boundary.lower_left.x =  lc->next_character_boundary.lower_left.x;

if (lc->horizontal_string_boundary.lower_left.y >  lc->next_character_boundary.lower_left.y) 
  lc->horizontal_string_boundary.lower_left.y =  lc->next_character_boundary.lower_left.y;

// adjust horizopntal to expand with character    

  
    


// adjust vertical to expand with horizontal  
if (lc->horizontal_string_boundary.upper_right.x > lc->vertical_string_boundary.upper_right.x) 
  lc->vertical_string_boundary.upper_right.x = lc->horizontal_string_boundary.upper_right.x;

if (lc->vertical_string_boundary.upper_right.y <  lc->horizontal_string_boundary.upper_right.y) 
  lc->vertical_string_boundary.upper_right.y =  lc->horizontal_string_boundary.upper_right.y;
  
if (lc->vertical_string_boundary.lower_left.x >  lc->horizontal_string_boundary.lower_left.x) 
  lc->vertical_string_boundary.lower_left.x =  lc->horizontal_string_boundary.lower_left.x;

if (lc->vertical_string_boundary.lower_left.y >  lc->horizontal_string_boundary.lower_left.y) 
  lc->vertical_string_boundary.lower_left.y =  lc->horizontal_string_boundary.lower_left.y;


// adjust vertal to expand with horizontal  
        
      
lc->cursor++;
lc->point_cursor.x = next_end;

return 1;
}













void linit_for_string(lcontext *lc,fl scale,fl x, fl y, fl z) {
lc->next_character_boundary = (lr) {{0.f,0.f},{0.f,0.f}};
lc->next_character = '\0';
lc->previous_character_boundary = (lr) {{0.f,0.f},{0.f,0.f}};
lc->previous_character = '\0';

lc->horizontal_string_boundary = (lr) {{0.f,0.f},{0.f,0.f}};
lc->vertical_string_boundary = (lr) {{0.f,0.f},{0.f,0.f}};
//lc->max_boundary = (lr) {{0.f,0.f},{1000000.f,1000000.f}};
lc->cursor=0;
lc->point_cursor = (lp) {0.f,0.f};
lc->scale =scale;
lc->start_point= (lp) {x,y};
lc->start_point_z=z;

//lc->word_wrap_flag=0;
}


/**/
void letters_out(lcontext *lc,fl scale,fl x,fl y,fl z,char *string) {

lneeded_points *lnp =lc->needed_points;

linit_for_string(lc,scale,x,y,z);
lg_start_drawing(lc);
int i=0;
char ch;
while ((ch=string[i])!= '\0') {
  if (ch<0) { // handle >128 characters
    i++;
    continue;
    }
  if (ch=='\n') {
    int fitv;
    fitv=lcan_add_vertical_boundary(lc);
    if (fitv) {
      ladd_vertical_boundary(lc);
      i++;
      continue;
      }
    else return;
    }  
  lc->previous_character_boundary = lc->next_character_boundary;
  lc->previous_character = lc->next_character;
  lc->next_character = ch;
  
  int fit=lc->interface->lsize[ch](lc);

  
      
  if (fit) {
    /* since the line width could extend us beyound our borders, we scale the letter inside the box */
/*    float scale_internal = lc->scale/(1.f + lnp->line_width); 
    lg_set_scale_translate(scale_internal,lc->start_point.x + lc->scale * lc->point_cursor.x + (scale_internal*lnp->line_width*0.5),
                                          lc->start_point.y + lc->scale * lc->point_cursor.y + (scale_internal*lnp->line_width*0.5),
					  lc->start_point_z);*/
    float scale_internal = lc->scale;
    lg_set_scale_translate(scale_internal,lc->start_point.x + lc->scale * lc->point_cursor.x,
                                          lc->start_point.y + lc->scale * lc->point_cursor.y,
					  lc->start_point_z);
  
    lc->interface->ldraw[ch](lnp);
    ladd_boundary(lc);
    }
  else  {
    int fitv;
    fitv=lcan_add_vertical_boundary(lc);
    if (fitv) {
      ladd_vertical_boundary(lc);
      continue; /* try again with character on new line */ 
      }
    else return;
    }
  i++;
  }
}





void letters_size(lcontext *lc,fl scale,fl x,fl y,fl z,char *string) {

lneeded_points *lnp =lc->needed_points;

linit_for_string(lc,scale,x,y,z);
lg_start_drawing(lc);
int i=0;
char ch;
while ((ch=string[i])!= '\0') {
  if (ch<0) { // handle >128 characters
    i++;
    continue;
    }
  if (ch=='\n') {
    int fitv;
    fitv=lcan_add_vertical_boundary(lc);
    if (fitv) {
      ladd_vertical_boundary(lc);
      i++;
      continue;
      }
    else return;
    }
  lc->previous_character_boundary = lc->next_character_boundary;
  lc->previous_character = lc->next_character;
  lc->next_character = ch;
  
  int fit=lc->interface->lsize[ch](lc);

  
      
  if (fit) {
    /* since the line width could extend us beyound our borders, we scale the letter inside the box */
/*    float scale_internal = lc->scale/(1.f + lnp->line_width); 
    lg_set_scale_translate(scale_internal,lc->start_point.x + lc->scale * lc->point_cursor.x + (scale_internal*lnp->line_width*0.5),
                                          lc->start_point.y + lc->scale * lc->point_cursor.y + (scale_internal*lnp->line_width*0.5),
					  lc->start_point_z);*/
    float scale_internal = lc->scale;
    lg_set_scale_translate(scale_internal,lc->start_point.x + lc->scale * lc->point_cursor.x,
                                          lc->start_point.y + lc->scale * lc->point_cursor.y,
					  lc->start_point_z);
  
//    lc->interface->ldraw[ch](lnp); we are just sizing
    ladd_boundary(lc);
    }
  else  {
    int fitv;
    fitv=lcan_add_vertical_boundary(lc);
    if (fitv) {
      ladd_vertical_boundary(lc);
      continue; /* try again with character on new line */ 
      }
    else return;
    }
  i++;
  }
}




lcontext * linit_context() {
lcontext *lc=calloc(sizeof(lcontext),1);

// set to the default ascii renderer - yes, we could do more in the future - with alot of memory arn work
lc->needed_points = (lneeded_points *)  calloc(sizeof(lneeded_points),1);
linit_ascii_normal_points(lc->needed_points);

lc->interface = (linterface *) &letters_ascii_interface;
lc->next_character_boundary = (lr) {{0.f,0.f},{0.f,0.f}};
lc->horizontal_string_boundary = (lr) {{0.f,0.f},{0.f,0.f}};
lc->vertical_string_boundary = (lr) {{0.f,0.f},{0.f,0.f}};

lc->max_boundary = (lr) {{0.f,0.f},{1000000.f,1000000.f}};
lc->cursor=0;
lc->point_cursor = (lp) {0.f,0.f};
lc->scale =1.f;
lc->start_point = (lp) {.x=0.f, .y=0.f};
lc->start_point_z=0.f;
lc->word_wrap_flag=0;
return lc;
}


/* letters calls letters_ascii to make the letters that calls letters_render_opengles to draw */

#ifdef LETTERS_MAIN
int main() {
lcontext *lc = linit_context();
letters_opengles_setgraphics(1,2,3,4);
letters_out(lc,1.f,0.f,0.f,0.f,"! !!");





// just for now: ??? ??? ???         
//    glClearColor(0.0f, 0.0f, 0.f, 1.0f);
//    checkGlError("glClearColor");
// glClear( GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
//     checkGlError("glClear");
// ??? this should be removed

glUniform4f(colorHandle, 1.f,1.f,1.f,1.0f);



lc->needed_points->line_width = 0.04f;

letters_out(lc,0.16f,-2.f,1.f,-0.5f,"!\"#$%&'()*+,-./0123456789:;<=>?@");
letters_out(lc,0.16f,-2.f,0.75f,-0.5f,"ABCDEFGHIJKLMNOPQRSTUVWXYZ");
letters_out(lc,0.16f,-2.f,0.5f,-0.5f,"[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~\177");


glUniform4f(colorHandle, 1.f,0.f,1.f,1.0f);
lc->needed_points->line_width = 0.12f;

letters_out(lc,0.18f,-2.f,0.15f,-1.f,"The quick brown fox");
letters_out(lc,0.18f,-2.f,-0.15,-1.f,"jumped over the lazy dog.");

lc->needed_points->line_width = 0.04f;

return 0;
}
#endif



