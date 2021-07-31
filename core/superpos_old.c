


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







super_point expanded_superpos_to_xyz_context(expanded_superpos *epx,int context) {
int *ms;
ms = superpos_map_to(context);
int m0=ms[0];
int m1=ms[1];
int m2=ms[2];
int m3=ms[3];
int m4=ms[4];

    /* think of it as             3
                          4     1
                             0
			  3     2
			          4 
	so that we can keep 0,2, 2-> 0 normatl
				  
				  */

        
    double xoff0 = epx->d[m0][m1] - epx->d[m0][m3] 
                 - epx->d[m0][m4] + epx->d[m0][m2];
    double xoff2 =  ((epx->d[m1][m3]  + epx->d[m2][m4]) - ( epx->d[m4][m2] +epx->d[m3][m1]))  * 0.5;

    
    double yoff2 = (epx->d[m1][m3] - epx->d[m2][m4] + epx->d[m4][m2] - epx->d[m3][m1]) * 0.5;
    double yoff0 = epx->d[m0][m4] - epx->d[m0][m2]  + epx->d[m0][m1] - epx->d[m0][m3];
    double zoff0 = epx->d[m0][m0];
    
    fprintf(stderr,"---->got 1,2,3,4 %f,%f,%f,%f\n", epx->d[m0][m1], epx->d[m0][m2], epx->d[m0][m3], epx->d[m0][m4]);
    fprintf(stderr," 	actually is model %d: %d,%d,%d,%d\n",m0,m1,m2,m3,m4);
    
    fprintf(stderr,"4-2 %f and 1-3 %f\n", epx->d[m0][m4] - epx->d[m0][m2], epx->d[m0][m1] - epx->d[m0][m3] );
    fprintf(stderr,"xoff %f yoff %f\n", xoff0,yoff0);
    fprintf(stderr,"Plus (%f+%f -%f-%f)/2,  (%f+%f -%f-%f)/2 ->(%f-%f)/2,(%f-%f/2) -> %f,%f is %f,%f\n\n",
        epx->d[m1][m3],epx->d[m2][m4],
	epx->d[m4][m2],epx->d[m3][m1], // xoff2
        epx->d[m4][m2],epx->d[m1][m3],
	epx->d[m2][m4],epx->d[m3][m1], // yoff2
	(epx->d[m2][m4]+epx->d[m1][m3]),-(epx->d[m4][m2],epx->d[m3][m1]), // xx adding 
        (epx->d[m4][m2]+epx->d[m1][m3]),-(epx->d[m2][m4]+ epx->d[m3][m1]) ,xoff2,yoff2,xoff0+xoff2,yoff0+yoff2);
    
    return (super_point){xyz:{xoff0+xoff2,yoff0+yoff2,epx->d[m0][m0]}};
}








 
super_point xyz_from_context_to_context(int from_context,float dist,super_point new_point,int to_context) {
expanded_superpos en;

set_from_xyz_in_context(from_context,&en, new_point,dist);
 

return(expanded_superpos_to_xyz_context(&en,to_context));
}











int set_from_xyz_in_context(int context,expanded_superpos *ep,super_point p, float dist) {
int i;
int j;
int *ms;
ms = superpos_map_to(context);
int m0=ms[0];
int m1=ms[1];
int m2=ms[2];
int m3=ms[3];
int m4=ms[4];

/* so the m allows us to map to any one of the possible contexts - 0 thru 4 - properly */
double hdist = dist * 0.5; /* if dist is 2 (-1 to 1) then hdist is 1 */
double hdist2 = dist * 0.5; /* if dist is 2 (-1 to 1) then hdist is 1 */
double qdist = hdist * 0.5; /* if dist is 2 (-1 to 1) then qdist is 0,5 */

float x=p.xyz[0];
float y=p.xyz[1];
float z=p.xyz[2];

    for (int i=0;i<5;i++) {
      for (int j=0;j<5;j++) {
        ep->d[i][j] = 0.;
	}
      }
    ep->d[m0][m0] = z;        
    ep->d[m1][m1] = z;        
    ep->d[m2][m2] = z;        
    ep->d[m3][m3] = z;        
    ep->d[m4][m4] = z;        
    
    
    /* above _ the qdist puts us in dead center */
    
    
if (fabs(y) > fabs(x)) { //if y is bigger than x
  if (y>=0) { // if y is >0
    if (x>=0) {
      ep->d[m0][m1] = x;
      ep->d[m1][m0] = 0.;
      
      double halfdiff = (y-x) * 0.5;
      if (halfdiff<0.) {fprintf(stderr," bad halfdiff 0:%lf\n",halfdiff);exit(-1);}
      ep->d[m1][m3] = halfdiff;  // x up, y up
      ep->d[m4][m2] = halfdiff;  // down, y up
      }
    else {
      ep->d[m0][m4] = -x;
      ep->d[m4][m0] = 0.;
      
      double halfdiff = (y+x) * 0.5;
      if (halfdiff<0.) {fprintf(stderr," bad halfdiff 1:%lf\n",halfdiff);exit(-1);}
      ep->d[m1][m3] = halfdiff; // x up y down  
      ep->d[m4][m2] = halfdiff; // y up, x down
      }	
    }
  else {
    if (x>=0) {
      ep->d[m0][m2] = x;
      ep->d[m2][m0] = 0.;
      
      double halfdiff = ((-y)-x) * 0.5;
      if (halfdiff<0.) {fprintf(stderr," bad halfdiff 2:%lf\n",halfdiff);exit(-1);}
      ep->d[m3][m1] = halfdiff; // y down x down
      ep->d[m2][m4] = halfdiff; // y down x up
      }
    else {
      ep->d[m0][m3] = -x;
      ep->d[m3][m0] = 0.;
      
      double halfdiff=  ((-y)+x)*0.5;
      if (halfdiff<0.) {fprintf(stderr," bad halfdiff 3:%lf\n",halfdiff);exit(-1);}
      ep->d[m3][m1] = halfdiff;  // y down, x down
      ep->d[m2][m4] = halfdiff;  // y down, x up
      }     
    } 
  } /* if y is biffer than x */
else { /* x is bigger than y */
  if (y>=0) { // if y is >0
    if (x>=0) {
      ep->d[m0][m1] = y;
      ep->d[m1][m0] = 0.;
      
      double halfdiff=  (x-y)*0.5;
      if (halfdiff<0.) {fprintf(stderr," bad halfdiff 4:%lf\n",halfdiff);exit(-1);}
      
      ep->d[m1][m3] = halfdiff; // x up, y down
      ep->d[m2][m4] = halfdiff; // x up y up
      }
    else {
      ep->d[m0][m4] = y;
      ep->d[m4][m0] = 0.;
      
      double halfdiff=  (-x -y)*0.5;
      if (halfdiff<0.) {fprintf(stderr," bad halfdiff 5:%lf\n",halfdiff);exit(-1);}
      ep->d[m3][m1] = halfdiff; // x down, y down
      ep->d[m4][m2] = halfdiff; // x down, y up
      }	
    }
  else { // y is negative
    if (x>=0) {
      ep->d[m0][m2] = -y;
      ep->d[m2][m0] = 0.;
      
      double halfdiff=  (x +y)*0.5;
       if (halfdiff<0.) {fprintf(stderr," bad halfdiff 6:%lf\n",halfdiff);exit(-1);}
      ep->d[m1][m3] = halfdiff;  // x up, y up
      ep->d[m2][m4] = halfdiff;  // x down y down
      
      }
    else {
      ep->d[m0][m3] = -y;
      ep->d[m3][m0] = 0.;
      
      double halfdiff=  (+y + (-x))*0.5;
      if (halfdiff<0.){fprintf(stderr," bad halfdiff 7:%lf\n",halfdiff);exit(-1);}
      ep->d[m3][m1] = halfdiff;  // x down, y down
      ep->d[m4][m2] = halfdiff;  //x down, y up

      }      
    } // if y is negative
  } /* if y is biffer than x */

if (ep->d[m3][m1] != 0.) {
  ep->d[m1][m3] = - ep->d[m3][m1];
  }
else if (ep->d[m1][m3] != qdist) {
  ep->d[m3][m1] = - ep->d[m1][m3];
  }
if (ep->d[m2][m4] != qdist) {
  ep->d[m4][m2] = -ep->d[m2][m4];
  }
else if (ep->d[m4][m2] != qdist) {
  ep->d[m2][m4] = -ep->d[m4][m2];
  }
  



          
fprintf(stderr,"\n\n\nvvvvvvvvvvvvvvvvvvvvv\ncontext %d x %f y %f\n",context,p.xyz[0],p.xyz[1]);
for (int i=0;i<5;i++) {
  for (int j=0;j<5;j++) {
    if ((i!= j) &&(ep->d[i][j])!= qdist) {
      fprintf(stderr,"%d -> %d = %f	",i,j,ep->d[i][j]);
      }
    }  
  }
fprintf(stderr,"\n\n");    
        
return 0;    
}
    
    


/* end of file */
