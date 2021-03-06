


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
float hdist = dist*0.5;
float sqrt2 = sqrtf(2.0);
float one_over_sqrt2 = sqrtf(2.0)*0.5;
float fabsx,fabsy; // absolute of deltas
float diagx,diagy; // smalles(fabs(x),fabs(y) * sign(diagx), smalles(fabs(x),fabs(y)) * sign(diagy)
fabsx = fabs(x);
fabsy = fabs(y);
diagx=0.;
diagy=0.;
float diffx=0.; // difference in deltas as if if was from a diagonal.  
float diffy=0.; 
// so x=0.6,y=0.5, diagx=0.5, diagy = 0.5 diffx = 0.1/sqrt2, diffy = 0
if (fabsy > fabsx) {
  if ((x)>=0.) {
    diagx = fabsx;
    if (y>=0.) {
      fprintf(stderr,"y11	");
      diagy = fabsx;
      diffy = (fabsy-fabsx)*one_over_sqrt2;
      }
    else {
      fprintf(stderr,"y1m	");
      diagy = -fabsx;
      diffy = -(fabsy-fabsx)*one_over_sqrt2;
      }
    }
  else { /* x < 0 */
    diagx= - fabsx;
    if ((y)>=0.) {
      fprintf(stderr,"ym1	");
      diagy=fabsx;
      diffy= (fabsy-fabsx)*one_over_sqrt2;
      }
    else {
      fprintf(stderr,"ymm	");
      diagy =  -fabsx;
      diffy= -(fabsy-fabsx)*one_over_sqrt2;
      }
    } /* x < 0 */
  }
else { /* x>y */
  if ((y)>=0.) {
    diagy = fabsy;
    if (x>=0.) {
      fprintf(stderr,"x11	");      
      diagx = fabsy;
      diffx = (fabsx-fabsy)*one_over_sqrt2;
      }
    else {
      fprintf(stderr,"xm1	");      
      diagx = -fabsy;
      diffx = -(fabsx-fabsy)*one_over_sqrt2;
      }
    } 
  else { /* y<0 */
    fprintf(stderr,"x1m	");      
    diagy= - fabsy;
    if ((x)>=0.) {
      diagx=fabsy;
      diffx= (fabsy-fabsx)*one_over_sqrt2;
      }
    else {
      fprintf(stderr,"xmm	");      
      diagx =  -fabsy;
      diffx= -(fabsy-fabsx)*one_over_sqrt2;
      }
    } /* x < 0 */
  }  
fprintf(stderr,"start %f,%f diag %f,%f diff %f,%f\n",x,y,diagx,diagy,diffx,diffy);
      
super_point base = (super_point){xyz:{x,y,z}};
int number_contexts = (to_context+5-from_context) % 5;
//fprintf(stderr,"D %f number contexts = %d pt = %f,%f\n",dist,number_contexts,x,y);
if (number_contexts ==0) {
  }
else if (number_contexts == 1) {
  base = (super_point){xyz:{-hdist,hdist,z}};
  base.xyz[0] += diagx + diffx - diffy;  /* 0 -> 1 +x,-y, 4->1 +x,-y  2->1 -y, */
  base.xyz[1] += -diagy - diffy -diffx;
  }
else if (number_contexts == 2) {
  base = (super_point){xyz:{-hdist,-hdist,z}};
  base.xyz[0] += diagy*sqrt2 - diffx*sqrt2       -diffy*sqrt2;
  base.xyz[1] += diagx*sqrt2 - diffx*sqrt2 + diffy*sqrt2;
  }
else if (number_contexts == 3) {
  base = (super_point){xyz:{hdist,-hdist,z}};
  base.xyz[0] -= (diagx*sqrt2 +  diffx);
  base.xyz[1] += (diagy*sqrt2 + diffy);
  }
else if (number_contexts == 4) {
  base = (super_point){xyz:{hdist,hdist,z}};
  base.xyz[0] -= -(diagy * sqrt2 + diffy*sqrt2);
  base.xyz[1] -= (diagx*sqrt2 + diffx*sqrt2);
  }
//fprintf(stderr, "from %d to %d 	is %f,%f\n",from_context,to_context,base.xyz[0],base.xyz[1]);
return(base);
}




  
    

    
    


/* end of file */
