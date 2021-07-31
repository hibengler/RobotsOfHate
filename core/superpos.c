


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








super_point offset_by_one_context(float dist,super_point before) {
float hdist = dist*0.5;
double p2to_1 = before.xyz[1];
double p4to_1 = before.xyz[0];
fprintf(stderr,"x,y is %f,%f:	",p4to_1,p2to_1);
super_point base;
if (p2to_1>=0) {
  if (p4to_1>=0) {
    base = (super_point){xyz:{-hdist,hdist,before.xyz[3]}};
    fprintf(stderr,"0 base  %f,%f	",base.xyz[0],base.xyz[1]);
    base.xyz[1] -= p2to_1;
    base.xyz[0] += p4to_1;
    }
  else {
    base = (super_point){xyz:{0.,dist,before.xyz[3]}};
    fprintf(stderr,"1 base  %f,%f	",base.xyz[0],base.xyz[1]);
    base.xyz[1] += p4to_1;
    base.xyz[1] += p4to_1;
    base.xyz[1] -= p2to_1;
    base.xyz[0] -= p2to_1;
    base.xyz[0] -= (p2to_1+p4to_1);
    
    }
  }
else { /* y iss < 0 */
  if (p4to_1>0) {
    base = (super_point){xyz:{0.,hdist,before.xyz[3]}};
    fprintf(stderr,"2 base  %f,%f	",base.xyz[0],base.xyz[1]);
//    base.xyz[1] -= p4to_1;
//    base.xyz[0] += p4to_1;
    }
  else {
    base = (super_point){xyz:{0.,-hdist,before.xyz[3]}};
    fprintf(stderr,"3 base  %f,%f	",base.xyz[0],base.xyz[1]);
    base.xyz[0] -= p2to_1;
//    base.xyz[1] -= p4to_1;
    }
  }
  
fprintf(stderr,"	final to %f,%f	",base.xyz[0], base.xyz[1]);
fprintf(stderr,"		Note: original x %f,%f\n",p2to_1,p4to_1);

return(base);
}



  
    




super_point back_offset_by_one_context(float dist,super_point after) {
float hdist = dist*0.5; // an idea to go backward 
double p2to_1 = after.xyz[1];
double p4to_1 = after.xyz[0];
fprintf(stderr,"x,y is %f,%f:	",p4to_1,p2to_1);
super_point base;
if (p2to_1>=0) {
  if (p4to_1>=0) {
    base = (super_point){xyz:{hdist,hdist,after.xyz[3]}};
    fprintf(stderr,"0 base  %f,%f	",base.xyz[0],base.xyz[1]);
    base.xyz[1] += p2to_1;
    base.xyz[1] += p4to_1;
    base.xyz[0] += p4to_1;
    
    }
  else {
    base = (super_point){xyz:{0.,dist,after.xyz[3]}};
    fprintf(stderr,"1 base  %f,%f	",base.xyz[0],base.xyz[1]);
    base.xyz[1] += p4to_1;
    base.xyz[1] += p4to_1;
    base.xyz[1] -= p2to_1;
    base.xyz[0] -= p2to_1;
    base.xyz[0] -= (p2to_1+p4to_1);
    
    }
  }
else { /* y iss < 0 */
  if (p4to_1>0) {
    base = (super_point){xyz:{0.,hdist,after.xyz[3]}};
    fprintf(stderr,"2 base  %f,%f	",base.xyz[0],base.xyz[1]);
//    base.xyz[1] -= p4to_1;
//    base.xyz[0] += p4to_1;
    }
  else {
    base = (super_point){xyz:{0.,-hdist,after.xyz[3]}};
    fprintf(stderr,"3 base  %f,%f	",base.xyz[0],base.xyz[1]);
    base.xyz[0] -= p2to_1;
//    base.xyz[1] -= p4to_1;
    }
  }
  
fprintf(stderr,"	final to %f,%f	",base.xyz[0], base.xyz[1]);
fprintf(stderr,"		Note: original x %f,%f\n",p2to_1,p4to_1);

return(base);
}


        
    
      
	  
super_point xyz_from_context_to_context(int from_context,float dist,super_point new_point,int to_context) {
if ( (((from_context+1)%5)==to_context)||(((from_context+2)%5) == to_context)) {
  for (int ci = from_context;ci!=to_context; ci = ((ci+1)%5)) {
    fprintf(stderr,"converting %d  %f,%f	-> %d\n",ci,new_point.xyz[0],new_point.xyz[1],((ci+1)%5));
    new_point =offset_by_one_context(dist,new_point);
    fprintf(stderr,"	%f,%f\n",new_point.xyz[0],new_point.xyz[1]);    
    }
  }
else {
  for (int ci = to_context;ci!=from_context; ci = ((ci+6)%5)) {
    fprintf(stderr,"back converting %d  %f,%f	-> %d\n",ci,new_point.xyz[0],new_point.xyz[1],((ci+6)%5));
    new_point =offset_by_one_context(dist,new_point);
    fprintf(stderr,"	%f,%f\n",new_point.xyz[0],new_point.xyz[1]);    
    }
  }
return(new_point);
}



super_point *xyz_from_context_to_all_contexts
  (int from_context,float dist,super_point new_point,super_point points_by_context[5]) {
super_point old_point=new_point;
int ci = from_context;
points_by_context[ci] = new_point;
for (int i = 0;i<2;i++) {
  fprintf(stderr,"converting %d  %f,%f	-> %d\n",ci,new_point.xyz[0],new_point.xyz[1],((ci+1)%5));
  new_point =offset_by_one_context(dist,new_point);
  fprintf(stderr,"	%f,%f\n",new_point.xyz[0],new_point.xyz[1]);
  ci = ((ci+1)%5);
  points_by_context[ci] = new_point;  
  }
ci = from_context;

new_point = old_point;
for (int i = 0;i<2;i++) {
  fprintf(stderr,"back-converting %d  %f,%f	-> %d\n",ci,new_point.xyz[0],new_point.xyz[1],((ci+6)%5));
  new_point =back_offset_by_one_context(dist,new_point);
  fprintf(stderr,"	%f,%f\n",new_point.xyz[0],new_point.xyz[1]);
  ci = ((ci+6)%5);
  points_by_context[ci] = new_point;  
  }
  
return(points_by_context);
}


    
    


/* end of file */
