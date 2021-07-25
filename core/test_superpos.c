#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>


#include "superpos.h"
/* a test program to see if superpositioning could be figured out by cheating with triangles.
You see - delta x and delta y domt mean much here, and the distances are all sqrt(2) from the deltas.

But by keeping track of the distances from the 4 points, it is a distinace using diagonals.
The diagonals can be used in each version. it is much easier than doing arbitrary triangle position calculation if the distances
were not exact diagonals.   But this mans that most offsets are L's
*/




/*   4 \  /  1
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

GLfloat olds[15];
GLfloat news[15];

GLfloat *old[5]= {olds,olds+3,olds+6,olds+9,olds+12};
GLfloat *neww[5]= {news,news+3,news+6,news+9,news+12};
GLfloat deltas[5];
GLfloat *deltaz[5]={deltas+0,deltas+1,deltas+2,deltas+3,deltas+4};

int main2 () {
double oldx,oldy;
double newx,newy;
oldx=0.;
oldy=0.;
newx=0.;
newy=0.;

old[0][0]=oldx;
old[0][1]=oldy;
old[0][2]=0.;
old[0][0]=oldx;
old[0][1]=oldy;
old[0][2]=0.;

double dist=100.;
old[1][0]=-dist;  
old[1][1]=dist; 
old[1][2]=dist;
   
old[2][0]=-dist;
old[2][1]=-dist;
old[2][2]=0.;
   
old[3][0]=dist; 
old[3][1]=-dist;
old[3][2]=0.;
      
old[4][0]=-dist;
old[4][1]=-dist;
old[4][2]=0.;
       
       

{
  int i,j;
  for (i=0;i<5;i++) {
    for (j=0;j<3;j++) {
      neww[i][j]=old[i][j];
      }
    }
  }
  
while (1) {
  int x = rand();
  double offx=( ((double)(x % 50)) )-25.;
  int y = rand();
  double offy=( ((double)(y % 50)) )-25.;
  neww[0][0] +=  offx;
  neww[0][1] +=  offy;
  if ((neww[0][0]>100)||(neww[0][0]<= -100)) {
    neww[0][0] -=  offx;
    neww[0][1] -=  offy;
    continue;
    }
  if ((neww[0][1]>100)||(neww[0][1]<= -100)) {
    neww[0][0] -=  offx;
    neww[0][1] -=  offy;  
    continue;
    }
  int context=0;
  compute_superpos_vertices(context,neww,1,dist);
  fprintf(stderr,
"	%f,%f %f,%f	 %f,%f	%f,%f	%f,%f -->\n"
,old[0][0],old[0][1],old[1][0],old[1][1],old[2][0],old[2][1],old[3][0],old[3][1],old[4][0],old[4][1]);
  fprintf(stderr,
"	%f,%f %f,%f	 %f,%f	%f,%f	%f,%f\n"
,neww[0][0],neww[0][1],neww[1][0],neww[1][1],neww[2][0],neww[2][1],neww[3][0],neww[3][1],neww[4][0],neww[4][1]);


int i; for (i=0;i<5;i++) {
  old[i][0]=neww[i][0];
  old[i][1]=neww[i][1];
  }
  
fprintf(stderr,"\n");
  } // while
}



int main() {
double dist=2.0;
super_point from = {xyz:{0,0.0,0.4}};
super_point to = {xyz:{0.,0.0,0.4}};

for (int i=0;i<10;i++) {
from.xyz[0] = (float)i * 0.10;


to =xyz_from_context_to_context(0,(float)dist,from,0);
fprintf(stderr,"to %f,%f,%f\n\n\n",to.xyz[0],to.xyz[1],to.xyz[2]);

to =xyz_from_context_to_context(0,(float)dist,from,1);
fprintf(stderr,"to %f,%f,%f\n\n\n",to.xyz[0],to.xyz[1],to.xyz[2]);

to =xyz_from_context_to_context(0,(float)dist,from,2);
fprintf(stderr,"to %f,%f,%f\n\n\n",to.xyz[0],to.xyz[1],to.xyz[2]);

to =xyz_from_context_to_context(0,(float)dist,from,3);
fprintf(stderr,"to %f,%f,%f\n\n\n",to.xyz[0],to.xyz[1],to.xyz[2]);

to =xyz_from_context_to_context(0,(float)dist,from,4);
fprintf(stderr,"to %f,%f,%f\n\n\n\n",to.xyz[0],to.xyz[1],to.xyz[2]);
}




}




