


/* derived from find_pos in the original superposition.c */

#include "superpos.h"

#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define SUPERPOS_DIMENSIONS 6
/* sixth one for additional adds after the fact - translation and stuff */

static GLfloat placeholder_old0[] = {0.,0.,0.};
static GLfloat * placeholder_old[] = {placeholder_old0,placeholder_old0,placeholder_old0,placeholder_old0,placeholder_old0};

  static int mapping0[5] =  {0,1,2,3,4};
  static int mapping1[5] =   {1,2,3,4,0};
  static int mapping2[5] =    {2,3,4,0,1};
  static int mapping3[5] =    {3,4,0,1,2};
  static int mapping4[5] =    {4,0,1,2,3};
  
  
  
  
  static int *mapping[5] = (int *[]){ mapping0,mapping1,mapping2,mapping3,mapping4};
  
  
  /*
  static int umapping[5][5] = {{0,1,2,3,4}, {4,0,1,2,3}, {3,4,0,1,2},{2,3,4,0,1},{1,2,3,4,0}};
*/
  
 int * superpos_map_to(int context) {
 return mapping[context];
 }
/*
int * superpos_map_from(int context) {
 return (int *)umapping+context;
 }
*/




void compute_superpos_vertices(int context,GLfloat *new_vertex[5],int count,GLfloat dist)
{

/* a test program to see if superpositioning could be figured out by cheating with triangles.
You see - delta x and delta y domt mean much here, and the distances are all sqrt(2) from the deltas.

But by keeping track of the distances from the 4 points, it is a distinace using diagonals.
The diagonals can be used in each version. it is much easier than doing arbitrary triangle position calculation if the distances
were not exact diagonals.   But this mans that most offsets are L's
*/
unsigned long newi;

  
for (newi=0;newi<count;newi++) {
//  fprintf(stderr,"newi %ld\n",newi);
  unsigned long newoffset = 3*newi;

  int *ms;
  ms = superpos_map_to(context);
  int m0=ms[0];
  int m1=ms[1];
  int m2=ms[2];
  int m3=ms[3];
  int m4=ms[4];

  

  for (unsigned long i=1;i<5;i++) {
    

     super_point p2 = xyz_from_context_to_context(context,dist,
                        (super_point){xyz:{new_vertex[ms[i]][newoffset],new_vertex[ms[i]][newoffset+1],new_vertex[ms[i]][newoffset+2]}},
			ms[i]);
             
//    fprintf(stderr,"nnnnew point %f,%f,%f  \n",new_vertex[ms[i]][newoffset],new_vertex[ms[i]][newoffset+1],new_vertex[ms[i]][newoffset+2]); 
    new_vertex[ms[i]][newoffset] = p2.xyz[0];
    new_vertex[ms[i]][newoffset+1] = p2.xyz[1];
    new_vertex[ms[i]][newoffset+2] = p2.xyz[2];    
    
    }
  

 

  /* 4 \  /  1
        0
       / \
      3   2 
     
      0    2
        1
      4    3
     
      1     3
         2
      0     4
     
     
      2     4
         3
      1     0
      
      3     0
         4
      2     1
     
     This can be deduced - noo it cant
     */


  
  } /* for each vector */   
}


super_point xyz_from_context_to_context (int from_context,float dist, super_point start,int to_context) {
  int *ms;
  ms = superpos_map_to(from_context);
  int m0=ms[0];   
  int m1=ms[1];
  int m2=ms[2];
  int m3=ms[3];
  int m4=ms[4];

// assume the point is from the origin x and y;
float x=start.xyz[0];
float y=start.xyz[1];
float z=start.xyz[2];
/* test without checks 
while (x > dist) x -=dist;
while (x < -dist) x +=dist;
while (y > dist) y -=dist;
while (y < -dist) y +=dist;
*/
float d[5][5];
float hdist = dist*0.5;
float qdist = hdist*0.5;
float absx=fabs(x);
float absy=fabs(y);
for (int i=0;i<5;i++) {
  d[i][i] = z;
  for (int j=0;j<5;j++) {
    if (i!= j) {
      d[i][j] = qdist;
      }
    }
  d[0][i] = 0.f;
  d[i][0] = hdist;
  }
// set to half way everywhere.
float delta,initial;  
float qdistxx=0.f;
if (absy>absx) {
  if (y>=0.f) { // Y>=0
    if (x>=0.f) {  // Y>=0,x>=0
      delta = (absy-absx)*0.5;
      initial = absx+ delta;
      
      d[0][1] = qdistxx + initial;
      d[1][0] = hdist-d[0][1];
//  d[3][1] = qdistxx + initial;
//  d[1][3] = hdist-d[3][1];
	
      d[0][4] = qdistxx+delta;
      d[4][0] = hdist-d[0][4];
//   d[2][4] = qdistxx+delta;
//   d[4][2] = hdist-d[2][4];
      }
    else { // Y>=0, x<0 
      delta = (absy-absx)*0.5;
      initial = absx+ delta;
      
      d[0][4] = qdistxx + initial;
      d[4][0] = hdist-d[0][4];
//   d[2][4] = qdistxx + initial;
//   d[4][2] = hdist-d[2][4];
	
      d[0][1] = qdistxx+delta;
      d[1][0] = hdist-d[0][1];
//   d[3][1] = qdistxx+delta;
//   d[1][3] = hdist-d[3][1];
      }  
    }
  else { // Y<0 
    if (x>=0.f) { // -Y,x
      delta = (absy-absx)*0.5;
      initial = absx+ delta;
      d[0][2] = qdistxx + initial;
      d[2][0] = hdist-d[0][2];
//   d[4][2] = qdistxx + initial;
//   d[2][4] = hdist - d[4][2];
      
      d[0][3] = qdistxx+delta;
      d[3][0] = hdist - d[0][3];
//   d[1][3] =  qdistxx+delta;
//   d[3][1] = hdist - d[1][3];
      }
    else {  //-x, -Y
      delta = (absy-absx)*0.5;
      initial = absx+ delta;
      d[0][3] = qdistxx + initial;
      d[3][0] = hdist-d[0][3];
//   d[1][3] = qdistxx + initial;
//   d[3][1] = hdist - d[1][3];
      
      d[0][2] = qdistxx+delta;
      d[2][0] = hdist - d[0][2];
//   d[4][2] =  qdistxx+delta;
//   d[2][4] = hdist - d[4][2];
      }
    } // Y<0
  } // Y>x
else { // X>y
  if (y>=0.f) { // y>=0
    if (x>=0.f) {  // y>=0,X>=0
 fprintf(stderr," y>=0,X>=0   k\n");
      delta = (absx-absy)*0.5;
      initial = absy+ delta;
   fprintf(stderr,"initial %f delta %f\n",initial,delta);
      d[0][1] = qdistxx + initial;
      d[1][0] = hdist-d[0][1];
//     d[1][3] = qdist + initial;
//   d[3][1] = hdist-d[1][3];
	
      d[0][2] = qdistxx+delta;
      d[2][0] = hdist-d[0][2];
//   d[4][2] = qdist+delta;
//   d[2][4] = hdist-d[4][2];
      fprintf(stderr,"0,1 %f, 0,2 %f\n",d[0][1],d[0][2]);
      }
    else { // y>=0, X<=0
      delta = (absx-absy)*0.5;
      initial = absy+ delta;
      
      d[0][4] = qdistxx + initial;
      d[4][0] = hdist-d[0][4];
      d[2][4] = qdistxx + initial;
      d[4][2] = hdist-d[2][4];
	
      d[0][3] = qdistxx+delta;
      d[3][0] = hdist-d[0][3];
//   d[1][3] = qdistxx+delta;
//   d[3][1] = hdist-d[1][3];
      }
    } // y>=0
  else { //y<=0
    if (x>=0.) {  // y<0. X>=0
    fprintf(stderr," y<0. X>=0\n");
      delta = (absx-absy)*0.5;
      initial = absy+ delta;
      
      d[0][2] = qdistxx + initial;
      d[2][0] = hdist-d[0][2];
//   d[4][2] = qdistxx + initial;
//   d[2][4] = hdist-d[4][2];
	
      d[0][1] = qdistxx+delta;
      d[1][0] = hdist-d[0][1];
//   d[3][1] = qdistxx+delta;
//   d[1][3] = hdist-d[3][1];
      }
    else { // y<0, X<0    	
    fprintf(stderr," y<0. X<0\n");
      delta = (absx-absy)*0.5;
      initial = absy+ delta;
      
      d[0][3] = qdistxx + initial;
      d[3][0] = hdist-d[0][3];
//   d[1][3] = qdistxx + initial;
//   d[3][1] = hdist-d[1][3];
	
      d[0][4] = qdistxx+delta;
      d[4][0] = hdist-d[0][4];
//   d[2][4] = qdistxx+delta;
//   d[4][2] = hdist-d[2][4];
      }
    } // y<0	
  } // X>y
// quick check 
for (int i=0;i<5;i++) {
  for (int j=0;j<5;j++) {
    if (i!=j) {
      if (fabs(d[i][j]- ( hdist - d[j][i])) >0.001) {
        fprintf(stderr,"error %f [%d][%d] is not hdist - %f[%d][%d] (%f)\n",d[i][j],i,j,d[j][i],j,i,hdist-d[j][i]);
	}
      }
    }
  }

super_point base = (super_point){xyz:{x,y,z}};
int number_contexts = (to_context+5-from_context) % 5;
//fprintf(stderr,"D %f number contexts = %d pt = %f,%f\n",dist,number_contexts,x,y);
if (number_contexts ==0) {
  float x1 = -d[0][4] + d[0][2] -d[0][3] + d[0][1];
  float y1 = d[0][1] + d[0][4] - d[0][3] - d[0][2];
  if ((fabs(x1-x) >0.001)||(fabs(y1 -y)>0.001)) {
    fprintf(stderr,"identify error original %f,%f fixed %f,%f\n",x,y,x1,y1);
    fprintf(stderr,"1,2,3,4: %f,%f,%f,%f\n",d[0][1],d[0][2],d[0][3],d[0][4]);
    fprintf(stderr,"1+4 is %f - 2-3 is %f\n",  d[0][1] + d[0][4], - d[0][3] - d[0][2]);
    }
  }
else if (number_contexts == 1) {
  base = (super_point){xyz:{d[0][2]   - d[1][0]-d[0][3], 
                            d[1][0]-d[0][4] +d[0][3],z}};
//  base = (super_point){xyz:{d[1][2]+d[1][3] - d[1][0] - d[1][4], 
//                            d[1][0]+d[1][2] - d[1][4] - d[1][3],z}};
  }
else if (number_contexts == 2) {
  base = (super_point){xyz:{ -hdist + d[0][4]+d[0][2],
                             -hdist + d[0][2] + d[0][1],z}};
//  base = (super_point){xyz:{d[2][3] + d[2][4] - d[2][1] - d[2][0],
//                            d[2][3] + d[2][1] - d[2][0] - d[2][4],z}};
  }
else if (number_contexts == 3) {
  base = (super_point){xyz:{d[3][4] + d[3][0] - d[3][2] - d[3][1],
                            d[3][2] + d[3][4] - d[3][0] - d[3][1],z}};
  }
else if (number_contexts == 4) {
  base = (super_point){xyz:{d[4][0] + d[4][1] - d[4][3] - d[4][2],
                            d[4][3] + d[4][0] - d[4][2] - d[4][1],z}};
  }
fprintf(stderr, "from %d to %d 	is %f,%f\n",from_context,to_context,base.xyz[0],base.xyz[1]);
return(base);
}


 


/* end of file */
