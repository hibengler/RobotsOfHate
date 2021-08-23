


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
float qdist = hdist*0.5;
float cy21 = y+qdist;
float cy41 = x+qdist;
while (cy21 > dist) cy21 -=dist;
while (cy21 < -hdist) cy21 +=dist;
while (cy41 > dist) cy41 -=dist;
while (cy41 < -hdist) cy41 +=dist;
if (cy21 > hdist) {
  if (cx41>hdist) { 
    }
  else if (cx41 < 0.f) {
    }
  else {
    }
  }
else if (cy21<0.f) {
  if (cx41>hdist) { 
    }
  else if (cx41 < 0.f) {
    }
  else {
    }
}
else if (cx21>hdist) { 
}
else if (cx21<-0.f) {
}


4 -> 1 = 3 -> 2
2 -> 1 = 4-> 3
0 -> 2 = 4-> 3
0 -> 4 = 3 ->2 = 4->1 = 1->3
0-> 1 = 0->2 = 4->3
0->4 = 2->1 == 4->3 = 


OK, need to add to 0,1 and subtract from 1,3
and versa voze for 0,4 



{ // normally else - where we are betweeen 0 and hdist 
if (cy21 > cx41) { // if more y than x quadrant 4012
  if (cx41 >= qdiff) {  // in quadrant 012, y21 is bigger
    d[1][0] = sqrt(((hdist-cx41)*(hdist-cx41)*2.)*one_over_sqrt2; // cx41 is now in 01, 10 is inverse
    d[0][1] = hdiff-[1][0];
    cy21 = cy21 - (cx41-qdiff); // make y smaller
    cx41 = qdiff; // adjust candidate cx41 to 0
    
    /* now for experimet - figure out screen 1 - 0    2
                                                    1
						 4    3 */
    nx = -d[1][0] /* hdist-oldcx41 */ + qdist /* 1->3 placeholder  */ + (hdist-cy21) + cx41 /*wdost (/
    // -hdist + oldcx41 +qdist + hdist -cy21 -qdist
    //           oldcx41         - (oldcy21 - (oldcx41-qdiff))
    //          oldcx41            - oldcy21 - oldcx41 + qdiff
    //             -oldcy21
    ny = d[1][0]  /* hdist-oldcx41 */ - qdist  /* 1->3 placeholder  */ +  (hdist-cy21) - cx41 /* qdist */
    // = hdist- oldcx1 - qdist                                         + hdist -cy21 - qdist
    //   hdist    - oldcx41                                            - cy21
    //   hdist   -oldxc41                                             - (oldcy21 - (oldcx41 - qdiff )
    // =      hdist             -oldcx41                              - (oldcy21 - oldcx41 + qdiff )
    // =      hdist       - oldcx41                                  - oldcy21  - oldcx41 + qdiff
    nxforscreen2 = -cy21 + qdist /* 1,3 */ - q[2][0] /* qdist */+ d[2][3]  which is qdist, or is it hdist - 41 - whell that is qdist now anyways*/
    = -(oldcy21 - (oldcx41-qdiff))  + qdist - qdist + qdist
    = -oldcy21 + (oldcx41 - qdiff) +                + qdist
    = -oldcy21 + (oldcx41) - qdiff
    nyforscreen2 = cy21 - qdist - qdist + qdist  
    = cy21 - qdist 
    huh might be right
    nxforscreen3 = -qdist + qdist -qdist + (hdist-cy21 )
    //                            qdist - cy21
    nyforscreen3 = cy21 - qdist + qdist - qdist = cy21 - qdist
    bxforscreen4 =- ( hdist - cy21) +  qdist 
                 = cy21 -qdist 

super_point base = (super_point){xyz:{x,y,z}};
int number_contexts = (to_context+5-from_context) % 5;
//fprintf(stderr,"D %f number contexts = %d pt = %f,%f\n",dist,number_contexts,x,y);
if (number_contexts ==0) {
  }
else if (number_contexts == 1) {
  base = (super_point){xyz:{-hdist,hdist,z}};
  base.xyz[0] += x;
  base.xyz[1] -= y;
  }
else if (number_contexts == 2) {
  base = (super_point){xyz:{-hdist,-hdist,z}};
  base.xyz[0] += y;
  base.xyz[1] += x;
  }
else if (number_contexts == 3) {
  base = (super_point){xyz:{hdist,-hdist,z}};
  base.xyz[0] -= x;
  base.xyz[1] += y;
  }
else if (number_contexts == 4) {
  base = (super_point){xyz:{hdist,hdist,z}};
  base.xyz[0] -=y;
  base.xyz[1] -=x;
  }
//fprintf(stderr, "from %d to %d 	is %f,%f\n",from_context,to_context,base.xyz[0],base.xyz[1]);
return(base);
}

/*
    x    1
    . \   y
0    

        2

4     1
   0  
3     2

p41 - q + p01 = x
p21 - q + p01 = y
p02 - q - p21 - 
    
neural network

x,y  (0,5) ->    25 a < b x,y sio 25 -> 200 then 200 -> 25 then 25 -> 200 then 200 -> 5
or

distance from 0 
angle from 

 


/* end of file */
