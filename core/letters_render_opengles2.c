/*
Silent Radiance wifi broadcast for digital silent disco.
Copyright (C) 2017-2018 Hibbard M. Engler

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







/*$Revision: 1.12 $*/

#ifdef ANDROID
#include <jni.h>
#endif




#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "glue.h"
#include "helper.h"
#include "letters.h"



/* These are used to do cheap rotation of x,y around a circle once every packet.  This will make the liesegu look better */
static float cheap_sin_512[512];


static int mColorHandle;
static GLuint gProgram;
static GLuint gvPositionHandle;
static GLint mPositionHandle ;

static GLfloat letter_scale=1.f;
static GLfloat letter_xoffset=0.f;
static GLfloat letter_yoffset=0.f;
static GLfloat letter_zoffset=0.f;

void lg_start_drawing(lcontext *lc) {
glUseProgram(gProgram);
}


unsigned int letters_opengles_color_handle() {
return mColorHandle;
}
unsigned int letters_opengles_position_handle() {
return mPositionHandle;
}

unsigned int letters_opengles_program_handle() {
return gProgram;
}

void letters_opengles_setgraphics(GLuint xgProgram,int xmColorHandle,GLuint xgvPositionHandle,GLuint xmPositionHandle) {
int i;
for (i=0;i<512;i++) {
  cheap_sin_512[i] = sinf(((float) i)*0.0122818463);  /* which is 2*pi/512 */
  }
gProgram = xgProgram; /* single color */
mColorHandle = xmColorHandle;
gvPositionHandle = xgvPositionHandle;
mPositionHandle = xmPositionHandle;
}



// we do our own model view translation 
void lg_set_scale_translate(fl scale,fl x,fl y, fl z) {
letter_scale=scale;
letter_xoffset=x;
letter_yoffset=y;
letter_zoffset=z;
}


#define TFORM(p) {p.x=letter_xoffset+(p.x*letter_scale);p.y=letter_yoffset+(p.y*letter_scale);}

/* by specifying a width, we convert a line to a rectangle  - the rectangle is w wide with the line points in the center */
static void find_line_to_rectangle_by_width(GLfloat x1p, GLfloat y1p, 
                                   GLfloat x2p, GLfloat y2p, 
				   GLfloat w, 
				   GLfloat *px1a, GLfloat *py1a, 
				   GLfloat *px2a, GLfloat *py2a, 
				   GLfloat *px1b, 
				   GLfloat *py1b, 
				   GLfloat *px2b, GLfloat *py2b) {
float x1 = (x1p<x2p?x1p:x2p);
float x2 = (x1p<x2p?x2p:x1p);
float y1 = (x1p<x2p?y1p:y2p);
float y2 = (x1p<x2p?y2p:y1p);

// handle no slope 
if ((x2-x1)==0.f) {
  float w_over_2 = w*0.5;
  *px1a = x1-w_over_2;
  *py1a = y1;
  *px2a = x2-w_over_2;
  *py2a = y2;
  
  *px1b = x1+w_over_2;
  *py1b = y1;
  *px2b = x2+w_over_2;
  *py2b = y2;
  }
// handle slope of zero
else if ((y2-y1)==0.f) {
  float w_over_2 = w*0.5;
  *px1a = x1;
  *py1a = y1-w_over_2;
  *px2a = x2;
  *py2a = y2-w_over_2;
  
  *px1b = x1;
  *py1b = y1+w_over_2;
  *px2b = x2;
  *py2b = y2+w_over_2;
  }
else {
  float perpindicular_slope = (x1-x2)/(y2-y1); // - the inverse
  float c =  w*0.5;
  float xdist = c/sqrtf(perpindicular_slope*perpindicular_slope + 1.f);
  float ydist = xdist * perpindicular_slope;
  *px1a = x1 + xdist;
  *py1a = y1 + ydist;
  *px2a = x2 + xdist;
  *py2a = y2 + ydist;
  
  *px1b = x1 - xdist;
  *py1b = y1 - ydist;
  *px2b = x2 - xdist;
  *py2b = y2 - ydist;
  }
}




/* Essentially draws a thick line from endpoint to endpoint 
by converting the line into a rectangle, possibly at an angle.
A circle of diameter w can alternately be added at the endpoint to smooth out the work
This could be used by font work 
This is a 2d thick line
*/
void lg_wide_line(lp p1,lp p2,fl line_width) {
TFORM(p1);
TFORM(p2);
line_width *= letter_scale;
//fprintf(stderr,"lg_wide_line %f,%f to %f,%f w %f   s %f\n",p1.x,p1.y,p2.x,p2.y,line_width,letter_scale);
// where o1 is x1,y1 and p2 is x2,y2
/*    x1a          x2a  */
/*    w/2  */
/*    x1     ->    x2 */
/*    w/2  */
/*    x1b          x2b */
/* find the end points of the line */
float x1a,y1a,x2a,y2a,x1b,y1b,x2b,y2b;
find_line_to_rectangle_by_width(p1.x,p1.y,p2.x,p2.y,line_width,&x1a,&y1a,&x2a,&y2a,&x1b,&y1b,&x2b,&y2b);
GLfloat vertices[] = { x1a,y1a,letter_zoffset,x1b,y1b,letter_zoffset,x2b,y2b,letter_zoffset,
                       x2b,y2b,letter_zoffset,x2a,y2a,letter_zoffset,x1a,y1a,letter_zoffset};
// get handle to vertex shader's vPosition member
//mPositionHandle = glGetAttribLocation(one_color_program, "vPosition");

// Enable a handle to the triangle vertices
glEnableVertexAttribArray(mPositionHandle);

// Prepare the triangle coordinate data
glVertexAttribPointer(mPositionHandle, 3,
                                 GL_FLOAT, GL_FALSE,12
                                 ,vertices);
// Draw the triangle
glDrawArrays(GL_TRIANGLES, 0, 6);
// Disable vertex array
glDisableVertexAttribArray(mPositionHandle);
}



              

   






void lg_circle(int segments,fl circleSize,lp center){
TFORM(center);
circleSize *= letter_scale;
GLfloat vertices[7000];
vertices[0] = center.x;
vertices[1] = center.y;
vertices[2] = letter_zoffset;
vertices[3] = center.x+circleSize;
vertices[4] = center.y;
vertices[5] = letter_zoffset;
if (segments>512) segments=512;
float difference = 512./((float)segments);
float offset=difference;
int i;
int i3=6;
for(i =1; i <segments; i++){
  unsigned int so = ((int)offset)&511;
  unsigned int co = (128-so)&511;
  vertices[i3++] = center.x + (circleSize * cheap_sin_512[co]);
  vertices[i3++] = center.y + (circleSize * cheap_sin_512[so]);
  vertices[i3++] = letter_zoffset;
  offset += difference;
  }
vertices[i3++] = center.x + circleSize;
vertices[i3++] = center.y;
vertices[i3++] = letter_zoffset;

// Enable a handle to the triangle vertices
glEnableVertexAttribArray(mPositionHandle);

// Prepare the triangle coordinate data
glVertexAttribPointer(mPositionHandle, 3,
  GL_FLOAT, GL_FALSE,12,vertices);
// Draw the triangle
glDrawArrays(GL_TRIANGLE_FAN, 0, segments+2);
glDisableVertexAttribArray(mPositionHandle); 
}




void lg_circle_notransform(int segments,fl circleSize,lp center){
GLfloat vertices[7000];
vertices[0] = center.x;
vertices[1] = center.y;
vertices[2] = letter_zoffset;
vertices[3] = center.x+circleSize;
vertices[4] = center.y;
vertices[5] = letter_zoffset;
if (segments>512) segments=512;
float difference = 512./((float)segments);
float offset=difference;
int i;
int i3=6;
for(i =1; i <segments; i++){
  unsigned int so = ((int)offset)&511;
  unsigned int co = (128-so)&511;
  vertices[i3++] = center.x + (circleSize * cheap_sin_512[co]);
  vertices[i3++] = center.y + (circleSize * cheap_sin_512[so]);
  vertices[i3++] = letter_zoffset;
  offset += difference;
  }
vertices[i3++] = center.x + circleSize;
vertices[i3++] = center.y;
vertices[i3++] = letter_zoffset;

// Enable a handle to the triangle vertices
glEnableVertexAttribArray(mPositionHandle);

// Prepare the triangle coordinate data
glVertexAttribPointer(mPositionHandle, 3,
  GL_FLOAT, GL_FALSE,12,vertices);
// Draw the triangle
glDrawArrays(GL_TRIANGLE_FAN, 0, segments+2);
glDisableVertexAttribArray(mPositionHandle); 
}



void lg_wide_arc(int segments,fl circleSize,lp center,fl startDegrees,fl lengthDegrees,fl lineWidth,int drawStartCap,
    int drawEndCap)
{
//fprintf(stderr,"\nb: %f,%f\n",center.x,center.y);
//fprintf(stderr,"	s %f o %f,%f\n",letter_scale,letter_xoffset,letter_yoffset);
TFORM(center);
circleSize *= letter_scale;
lineWidth  *= letter_scale;
//fprintf(stderr,"a: %f,%f\n",center.x,center.y);

GLfloat vertices[7000];
if (segments>512) segments=512;
float offset = startDegrees*256.f/180.f;
float difference = (lengthDegrees*256.f/180.f)/((float)(segments-1));
float circleSizeInside = circleSize - (lineWidth*0.5);
float circleSizeOutside = circleSize + (lineWidth*0.5);
    int i;
    int ix6 = 0;
    for(i =0; i <=segments; i++){
      unsigned int so = ((int)offset)&511;
      unsigned int co = (128-so)&511;
      GLfloat cosx = cheap_sin_512[co];
      GLfloat sinx = cheap_sin_512[so];
	
      vertices[ix6++] = center.x + (circleSizeInside * cosx);
      vertices[ix6++] = center.y + (circleSizeInside * sinx);
      vertices[ix6++] = letter_zoffset;	
      vertices[ix6++] = center.x + (circleSizeOutside * cosx);
      vertices[ix6++] = center.y + (circleSizeOutside * sinx);
      vertices[ix6++] = letter_zoffset;
      offset += difference;
      }

// Enable a handle to the triangle vertices
glEnableVertexAttribArray(mPositionHandle);

// Prepare the triangle coordinate data
glVertexAttribPointer(mPositionHandle, 3,
                                 GL_FLOAT, GL_FALSE,12
                                 ,vertices);


// Draw the triangle
glDrawArrays(GL_TRIANGLE_STRIP, 0, segments+segments);

// Disable vertex array
glDisableVertexAttribArray(mPositionHandle);


if (drawStartCap) {
  lg_circle_notransform(20,lineWidth*0.5f,(lp) {center.x + circleSize * cosf((startDegrees*3.14159265359f)/180.f),
                               center.y + circleSize * sinf((startDegrees*3.14159265359f)/180.f) });
  }
if (drawEndCap) {
  lg_circle_notransform(20,lineWidth*0.5f,(lp) {center.x + circleSize * cosf(((startDegrees+lengthDegrees)*3.14159265359f)/180.f),
                               center.y + circleSize * sinf(((startDegrees+lengthDegrees)*3.14159265359f)/180.f) } );
  }

}

