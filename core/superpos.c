

/* derived from find_pos in the original superposition.c */

#include "superpos.h"

#include <stdio.h>
#include <math.h>
#include <stdlib.h>


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
double oldxx[5];
double oldyy[5];
double oldzz[5];

double newxx[5];
double newyy[5];
double newzz[5];

GLfloat vn[5][5];


GLfloat *oldxp[5];
  

  
for (newi=0;newi<count;newi++) {
  fprintf(stderr,"newi %ld\n",newi);
  unsigned long newoffset = 3*newi;

  int *ms;
  ms = superpos_map_to(context);
  int m0=ms[0];
  int m1=ms[1];
  int m2=ms[2];
  int m3=ms[3];
  int m4=ms[4];

  expanded_superpos er;
  set_from_xyz_in_context(context,&er,(super_point){xyz:{new_vertex[m0][newoffset],new_vertex[m0][newoffset+1],new_vertex[m0][newoffset+2]}},dist);
  

  for (unsigned long i=1;i<5;i++) {
    expanded_superpos bias;
    expanded_superpos final;
    
    fprintf(stderr,"back bias %f,%f,%f:  \n",new_vertex[ms[i]][newoffset],new_vertex[ms[i]][newoffset+1],new_vertex[ms[i]][newoffset+2]);
    set_from_xyz_in_context(context,&bias,(super_point) {xyz:{new_vertex[ms[i]][newoffset],new_vertex[ms[i]][newoffset+1],new_vertex[ms[i]][newoffset+2]}},dist);
     
     /* do a bias */
     for (int j=0;j<5;j++) {
       for (int k=0;k<5;k++) {
         final.d[j][k] = (er.d[j][k] + bias.d[j][k]) * 0.5;
         final.d[j][k] = (er.d[j][k]);
         }
       }
    
    super_point p2 = expanded_superpos_to_xyz_context(&final,ms[i]);
    fprintf(stderr,"nnnnew point %f,%f,%f  \n",new_vertex[ms[i]][newoffset],new_vertex[ms[i]][newoffset+1],new_vertex[ms[i]][newoffset+2]); 
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

    
    
    double xoff0 = epx->d[m0][m1] - epx->d[m0][m3]  - epx->d[m0][m4] + epx->d[m0][m2];
    
    double yoff0 = epx->d[m0][m4] - epx->d[m0][m2]  + epx->d[m0][m1] - epx->d[m0][m3];
    double zoff0 = epx->d[m0][m0];
    
    fprintf(stderr,"---->got 1,2,3,4 %f,%f,%f,%f\n", epx->d[m0][m1], epx->d[m0][m2], epx->d[m0][m3], epx->d[m0][m4]);
    fprintf(stderr," actually is model %d: %d,%d,%d,%d\n",m0,m1,m2,m3,m4);
    
    fprintf(stderr,"4-2 %f and 1-3 %f\n", epx->d[m0][m4] - epx->d[m0][m2], epx->d[m0][m1] - epx->d[m0][m3] );
    fprintf(stderr,"xoff %f yoff %f\n\n", xoff0,yoff0);

    return (super_point){xyz:{xoff0,yoff0,epx->d[m0][m0]}};
}



int set_from_origin_in_context(int context,expanded_superpos *ep,float dist) {
int *ms;
ms = superpos_map_to(context);
int m0=ms[0];
int m1=ms[1];
int m2=ms[2];
int m3=ms[3];
int m4=ms[4];

/* so the m allows us to map to any one of the possible contexts - 0 thru 4 - properly */
float hdist = dist * 0.5; /* if dist is 2 (-1 to 1) then dist is 1 */
float qdist = hdist * 0.5; /* if dist is 2 (-1 to 1) then dist is 0.5 */
    

    ep->d[m0][m1]=0.;
    ep->d[m0][m2]=0.;
    ep->d[m0][m3]=0.;
    ep->d[m0][m4]=0.;
    // z is ignored
    
    ep->d[m1][m0]=hdist;
    ep->d[m2][m0]=hdist;
    ep->d[m3][m0]=hdist;
    ep->d[m4][m0]=hdist;
    ep->d[m0][m0]=0.;
    ep->d[m1][m1]=0.;
    ep->d[m2][m2]=0.;
    ep->d[m3][m3]=0.;
    ep->d[m4][m4]=0.;
    
    ep->d[m1][m2] = qdist;
    ep->d[m2][m1] = qdist;
    ep->d[m1][m3] = qdist;
    ep->d[m3][m1] = qdist;
    ep->d[m1][m4] = qdist;
    ep->d[m4][m4] = qdist;
    
    ep->d[m2][m3] = qdist;
    ep->d[m3][m2] = qdist;
    ep->d[m2][m4] = qdist;
    ep->d[m4][m2] = qdist;
    
    ep->d[m3][m4] = qdist;
    ep->d[m4][m3] = qdist;
}



super_point xyz_from_context_to_context(int from_context,float dist,super_point new_point,int to_context) {
expanded_superpos en;
set_from_xyz_in_context(from_context,&en, new_point,dist);

return(expanded_superpos_to_xyz_context(&en,to_context));
}


int validate_superpos(expanded_superpos *ep, double dist) {
for(int a=0;a<5;a++) {
  for (int b=0;b<5;b++) {
    for (int c=0;c<5;c++) {
      if ((a != b) && (b != c) & (c != a)) {
        double suma = ep->d[a][b];
        double sumb = ep->d[b][c];
        double sumc = ep->d[c][a];
double hdist = dist * 0.5;
if ((suma + sumb + sumc -hdist)>0.002) {
  fprintf(stderr, "Aha [%d->%d = %f +[%d] -> %d = %f + %d->%d = %f goed to %f\n",a,b,suma,b,c,sumb,c,a,sumc,hdist- (suma + sumb + sumc));	
  }
  }
  }
  }
  }
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
double qdist = hdist * 0.5; /* if dist is 2 (-1 to 1) then qdist is 0,5 */
float x=p.xyz[0];
float y=p.xyz[1];
float z=p.xyz[2];

    ep->d[m0][m0] = z;        
    ep->d[m1][m1] = z;        
    ep->d[m2][m2] = z;        
    ep->d[m3][m3] = z;        
    ep->d[m4][m4] = z;        
    
    ep->d[m0][m1]=(x+y)/4.;
    ep->d[m0][m2]=(x-y)/4.;
    ep->d[m0][m3]=(-x-y)/4.;
    ep->d[m0][m4]=( -x +y)/4.;
    
    ep->d[m0][m1] -= ep->d[m0][m3]; 
    ep->d[m0][m3] =0.;
    ep->d[m0][m4] -=ep->d[m0][m2]; 
    ep->d[m0][m2] =0.;
    
      
    /* now the x,y delt might be too small */
#ifdef bullshit    
    float newx,newy;
    newx =  ep->d[m0][m2] -  ep->d[m0][m4] +  ep->d[m0][m1] -  ep->d[m0][m3];
    newy =  ep->d[m0][m1] +  ep->d[m0][m4] +  ep->d[m0][m2] -  ep->d[m0][m3];
    float deltax = x-newx;
    float deltay = y-newy;
    int q1; int q2;
    ep->d[m0][m4] -= deltax/2.;
    ep->d[m0][m1] += deltax/2;
    deltay += deltax;
    ep->d[m0][m4] += deltay/2.; // Leave deltax alone by going back and forth
    ep->d[m0][m1] += deltay/2;
    
    if (ep->d[m0][m4]<0) {
      ep->d[m0][m2] -= ep->d[m0][m4];
      ep->d[m0][m4] = 0.;
      }
    if (ep->d[m0][m1]<0) {
      ep->d[m0][m3] -= ep->d[m0][m1];
      ep->d[m0][m1] = 0.;
      }
 #endif     
    // z is ignored
fprintf(stderr,"x %f y %f z %f dist %f  ",x,y,z,dist);    
    ep->d[m1][m0]=hdist-ep->d[m0][m1];
    ep->d[m2][m0]=hdist-ep->d[m0][m2];
    ep->d[m3][m0]=hdist-ep->d[m0][m3];
    ep->d[m4][m0]=hdist-ep->d[m0][m4];
    
fprintf(stderr,"\n1 2 3 4 from - is %f,%f,%f,%f  ",ep->d[m0][m1],ep->d[m0][m2],ep->d[m0][m3],ep->d[m0][m4]);    
fprintf(stderr,"\nReally %d[%d],%d[%d],%d[%d],%d %d ",m0,m1,m0,m2,m0,m3,m0,m4);

    
    ep->d[m1][m2] = qdist-y;
    ep->d[m2][m1] = qdist+y;
    
    ep->d[m4][m3] = qdist-y;
    ep->d[m3][m4] = qdist+y;
    
    ep->d[m2][m3] = qdist- x;
    ep->d[m3][m2] = qdist+ x;
    
    ep->d[m4][m1] = qdist+x;
    ep->d[m1][m4] = qdist-x;
    
fprintf(stderr,"\n12 23 34 41 from - is %f,%f,%f,%f  ",ep->d[m1][m2],ep->d[m2][m3],ep->d[m3][m4],ep->d[m4][m1]);    
fprintf(stderr,"\n21 32 43 14 from - is %f,%f,%f,%f  ",ep->d[m2][m1],ep->d[m3][m2],ep->d[m4][m3],ep->d[m1][m4]);    
fprintf(stderr,"\nReally %d[%d],%d[%d],%d %d,%d,%d",m1,m2,m2,m3,m3,m4,m4,m1);
    

    { // block to cheat and find teh lengths of 1 to 3 and 2 to 4
      // but they have two dimensions of coarse - so I got the y delta done, but not the x delta
      
    
    double x3 =  ep->d[m4][m3];  /* 4,3 in number 1    qdist-x qdist+1*/
    double y3 =  ep->d[m3][m2];;
    
    
    double x2 = - ep->d[m2][m3];
    double y2 = - ep->d[m1][m2];
    float deltax2 = y2-x2;
        

 fprintf(stderr,"================================ contest %d\n",context);
     float deltax3 = (y3-x3);

     fprintf(stderr,"x2 %f,y2 %lf del %lf\n",x2,y2,deltax2);
     fprintf(stderr,"x3 %f,y3 %lf del %lf\n",x3,y3,deltax3);
     
     
    
     ep->d[m2][m4] =  y2-deltax2*2.;
     ep->d[m4][m2] =  -(y2-deltax2*2.);
     
    
    
    
     ep->d[m1][m3] = y3-deltax3*2.; 
     ep->d[m3][m1] = -(y3-deltax3*2.);; 


     fprintf(stderr,"got temp 13 as %f\n",ep->d[m1][m3]);
     fprintf(stderr,"got temp 24 as %f \n",ep->d[m2][m4]);
     fprintf(stderr,"deltax is %f 3 and %f 4\n",deltax2,deltax3);
     
     
     
     
     fprintf(stderr,"adding delta 3 back to m3\n");
     
      ep->d[m3][m1] += deltax3;
      ep->d[m1][m3] -= deltax3;
      
    
     fprintf(stderr,"adding delta to the 4:2 deal.\n");
      ep->d[m4][m2] += deltax3;
      ep->d[m2][m4] -= deltax3;

     
     fprintf(stderr," adding the other delta to the 4:2 deal");
     ep->d[m4][m2] += deltax2;
     
       

     }	    
			    	    
//    ep->d[m1][m3] =  ep->d[m1][m4] - ep->d[m1][m2] + ep->d[m1][m0] ;    
    
    
    
    ep->d[m3][m1] =  -ep->d[m3][m4]+ ep->d[m3][m0] - ep->d[m3][m2]; 
    
    //ep->d[m3][m1] = hdist- ep->d[m1][m3];
    fprintf(stderr,"it is m1->m3 is %f, reverse is %f\n",ep->d[m1][m3],ep->d[m3][m1]);
    
    
    /* now is the time for part 4:
      +2->3 - 2->1  - 2->0 + xxx24
      xxx24 = -2->3 +2->1  +20>0
      
      now 4->2
      40>0 + 4->3 - 4->1 - yyy
      yyy = -[4][0] + [4][1 - [4][3]
      */
      
      
      
    ep->d[m2][m4] = - ep->d[m2][m3]   + ep->d[m2][m1]  + ep->d[m2][m0];
  ep->d[m4][m2] = - ep->d[m4][0] + ep->d[m4][m1] + ep->d[m4][m3];
//     ep->d[m4][m2] =  -  ep->d[m2][m4] ;	
    fprintf(stderr,"----2 4 is %f   and 4 2 is %f\n\n",ep->d[m2][m4], ep->d[m4][m2]);
	
        
 
    
        
return 0;    
}
    
    


/* end of file */
