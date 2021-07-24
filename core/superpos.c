/* derived from find_pos in the original superposition.c */

#include "superpos.h"

#include <stdio.h>
#include <math.h>
#include <stdlib.h>


static GLfloat placeholder_old0[] = {0.,0.,0.};
static GLfloat * placeholder_old[] = {placeholder_old0,placeholder_old0,placeholder_old0,placeholder_old0,placeholder_old0};

  static int mapping[5][5] = { {0,1,2,3,4}, {1,2,3,4,0}, {2,3,4,0,1},{3,4,0,1,2},{4,0,1,2,3}};
  static int umapping[5][5] = {{0,1,2,3,4}, {4,0,1,2,3}, {3,4,0,1,2},{2,3,4,0,1},{1,2,3,4,0}};

  
 int * superpos_map_to(int context) {
 return mapping[context];
 }

int * superpos_map_from(int context) {
 return umapping[context];
 }


void compute_superpos_vertices(GLfloat *old_vertex[5],GLfloat *new_vertex[5],int count,GLfloat *deltas[5],GLfloat pvo[5][5],GLfloat pvn[5][5],GLfloat dist);
{

/* a test program to see if superpositioning could be figured out by cheating with triangles.
You see - delta x and delta y domt mean much here, and the distances are all sqrt(2) from the deltas.

But by keeping track of the distances from the 4 points, it is a distinace using diagonals.
The diagonals can be used in each version. it is much easier than doing arbitrary triangle position calculation if the distances
were not exact diagonals.   But this mans that most offsets are L's
*/
unsigned long newi;
int is_placeholder=0;
double oldxx[5];
double oldyy[5];
double oldzz[5];

double newxx[5];
double newyy[5];
double newzz[5];

GLfloat vo[5][5];
GLfloat vn[5][5];


GLfloat *oldxp[5];
if (!old_vertex) {
  fprintf(stderr,"using a placeholder\n");
  oldxp[0]=placeholder_old0;
  oldxp[1]=placeholder_old0;
  oldxp[2]=placeholder_old0;
  oldxp[3]=placeholder_old0;
  oldxp[4]=placeholder_old0;
  }  
else {
  oldxp[0] = old_vertex[0];
  oldxp[1] = old_vertex[1];
  oldxp[2] = old_vertex[2];
  oldxp[3] = old_vertex[3];
  oldxp[4] = old_vertex[4];
  }
  

  
for (newi=0;newi<count;newi++) {
  fprintf(stderr,"newi %ld\n",newi);
  unsigned long oldoffset;
  unsigned long newoffset = 3*newi;
  if(old_vertex) {
    oldoffset=3*newi;
    }
  else {
    oldoffset=0;
    }
  for (unsigned long i=0;i<5;i++) {
    oldxx[i]=oldxp[i][oldoffset];
    oldyy[i]=oldxp[i][oldoffset+1];
    oldzz[i]=oldxp[i][oldoffset+2];
    
    newxx[i]=new_vertex[i][newoffset];
    newyy[i]=new_vertex[i][newoffset+1];
    newzz[i]=new_vertex[i][newoffset+2];
    }
  

  double off[5];


  float *ms;
  ms = superpos_map_to(context);
  int m0=ms[0];
  int m1=ms[1];
  int m2=ms[2];
  int m3=ms[3];
  int m4=ms[4];



  // offset in diagonals.  We can then add or subtract them to get the right result
  double oldx=oldxx[m0];
  double oldy=oldyy[m0];
  double oldz=oldzz[m0];
  double newx=newxx[m0]; 
  double newy=newyy[m0];
  double newz=newzz[m0];

  // set up the right origins
  if (pvo)  {
    GLfloat *poqq[5][5] = pwo[newi];
    for (i = i;i<5;i++;) {
      for (j=0;j<5;j++) {
        vo[i][j] = *pvoqq[i][j];
        }
      }
    }
  else {
    set_from_origin_in_context(context,vo,(double)oldx,(double)y,(double) z, (double)dist));
    }
  
  
  for (int i = i;i<5;i++;) {
    for (int j=0;j<5;j++) {
      vn[i][j] = *pvn[i][j];
      }
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

  double deltax = newx-oldx;
  double deltay = newy-oldy;
  double deltaz = newz-oldz;
  
  // do distance in cartesian
  {
    int i;
    off[m0] = sqrt(deltax*deltax+deltay*deltay+deltaz*deltaz);
    for (i=1;i<5;i++) {
      off[i]=0.;  // offset is from zero to 
      }
    }
  
  
  if (deltax==0.&&deltay==0.) { // if we should skip because nothing is done
    // nothing
    }
  else {  
    fprintf(stderr, 
"\nfrom %lf,%lf to %lf,%lf delta %lf,%lf\n",oldx,oldy,newx,newy,deltax,deltay);

    double small,midpoint;
    if (fabs(deltay)>((fabs(deltax)+fabs(deltay))/2.0)) { /* delta y is more */
      if (deltay>0.) {
        if (deltax>0.) {
          fprintf(stderr,"0 deltay is bigger, and positive, deltax is positive  ");


          /* 0 to 1, then 0 to 4*/
          if (deltax>(deltay-deltax)) {
            small = deltay-deltax;
            midpoint = (deltay+deltax)/2.0;
 	    fprintf(stderr,"small deltay-deltax  \n";
  	    }
          else { 
            small = deltax;
            midpoint = deltay -small/2.0;;
 	    fprintf(stderr,"small deltax  \n";
            }
	 
          fprintf(stderr,"small is %lf midpoint is %lf\n",small,midpoint);
          off[m1] = midpoint;
          off[m4] = small/2.0;

          vn[m0][m1] += off[m1];
          vn[m1][m0] -= off[m1];

          vn[m0][m4] += off[m4];
          vn[m4][m0] -= off[m4];


// ???
          }
        else { /* deltay>0 deltax <0 */
//          fprintf(stderr,"1 deltay is bigger, and positive, deltax is negative  ");
          /* 0 to 1, then 0 to 4*/
          if ((-deltax)>(deltay+deltax)) {
            midpoint = (deltay-deltax)/2.0;
            small = deltay+deltax;
	    }
          else {             
            small = (-deltax);
            midpoint = (deltay-small)/2.0;
	    }
          fprintf(stderr,"small is %lf midpoint is %lf\n",small,midpoint);
          off[m4] = midpoint;
          off[m1] =  small/2.0;
          
          vn[m0][m1] += off[m1];
          vn[m1][m0] -= off[m1];

          vn[m0][m4] += off[m4];
          vn[m4][m0] -= off[m4];

          

          }
        } /* if deltay >0 */
      else { /* deltay is <0, */     
        if (deltax>=0.) {
  //        fprintf(stderr,"2deltay is bigger, and negative, deltax is positive  ");

          if ((deltax)>((-deltay)-deltax)) {
            midpoint = ((-deltay)+deltax)/2.0;
            small = (-deltay)-deltax;
	    }
          else {             
            small = (deltax);
            midpoint = ((-deltay)-small)/2.0;
	    }
          fprintf(stderr,"small is %lf midpoint is %lf\n",small,midpoint);
          off[m2] = midpoint;
          off[m3] =  small/2.0;
          
          vn[m0][m3] += off[m3];
          vn[m3][m0] -= off[m3];

          vn[m0][m2] += off[m2];
          vn[m2][m0] -= off[m2];

          

        else { /* negative geltay, negative deltax */
    //      fprintf(stderr,"3 deltay is bigger, and negative, deltax is negative  ");
          /* 0 to 1, then 0 to 4*/
          if ((-deltax)>((-deltay)-(-deltax))) {
            small = (-deltay)-(-deltax));
            midpoint = ((-deltay)+(-deltax))/2.0;
            }
          else { 
            small = -deltax;
            midpoint = ((-deltay)-small)/2.0;
	    }
       
  
          fprintf(stderr,"small is %lf midpoint is %lf\n",small,midpoint);
          off[m3] = midpoint;
          off[m2] =  (small/2.0);
      
          vn[m0][m3] += off[m3];
          vn[m3][m0] -= off[m3];

          vn[m0][m2] += off[m2];
          vn[m2][m0] -= off[m2];

    
          } /* if negative y, negative x */
        } /* if negative  */
      } /* if deltay is bigger */
    else { /* deltax is bigger */
      if (deltay>0.) {
        if (deltax>0.) {
//          fprintf(stderr,"4 deltay is smaller, and positive, deltax is positive  ");
          double small;
          /* 0 to 1, then 0 to 4  */
          if (deltay>(deltax-deltay)) {
            small = deltax-deltay;
            midpoint = (deltay+deltax)/2.0;
	    }
          else { 
            small = deltay;
            midpoint = (deltax-small)/2.0;
	    }
      
//          fprintf(stderr,"small is %lf midpoint is %lf\n",small,midpoint);
          off[m1] = midpoint;
          off[m2] = (small/2.0);

          vn[m0][m1] += off[m1];
          vn[m1][m0] -= off[m1];

          vn[m0][m2] += off[m2];
          vn[m2][m0] -= off[m2];

	  
      
          }
        else { /* deltay>0 deltax <0 */
//          fprintf(stderr,"5 deltay is smaller, and positive, deltax is negative  ");
          /* 0 to 1, then 0 to 4*/
          if ((deltay)>((-deltax)-deltay)) {
            small = (-deltax)-deltay;
            midpoint = (deltay-deltax)/2.0;
 	    }
          else { 
            small = (deltay);
	    midpoint = ((-deltax)-small)/2.0;
	    }
      	
 
//          fprintf(stderr,"small is %lf midpoint is %lf\n",small,midpoint);
          off[m4] = midpoint;
          off[m3] = (small/2.0);
      
          vn[m0][m4] += off[m4];
          vn[m4][m0] -= off[m4];

          vn[m0][m3] += off[m3];
          vn[m3][m0] -= off[m3];

          }
        } /* if deltay >0 */
      else { /* deltay is <0, */     
        if (deltax>0.) {
//          fprintf(stderr,"6 deltay is smaller, and negative, deltax is positive  ");
          if ((-deltay)>((deltax)-(-deltay))) {
            small = (deltax)-(-deltay);
            midpoint = ((-deltay)+(deltax))/2.0;
            }
          else { 
            small = -deltay;
            midpoint = (deltax - small)/2.0;
	    }
          off[m2] = midpoint;
          off[m3] = small/2.0;

          vm[m0][m2] += off[m2];
          mv[m2][m0] -= off[m2];

          vm[m0][m3] += off[m3];
          mv[m3][m0] -= off[m3];

          }  
        else { /* negative geltay, negative deltax */
  //        fprintf(stderr,"7 deltay is smaller, and negative, deltax is negative  ");
          double small;
          /* 0 to 1, then 0 to 4*/
          if ((-deltay)>((-deltax)-(-deltay))) {
            small = (-deltax)-(-deltay);
            midpoint = ((-deltay)+(-deltax))/2.0;
            }
          else { 
            small = -deltay;
            midpoint = (-deltax - small)/2.0;
	    }
         
          fprintf(stderr,"small is %lf midpoint is %lf\n",small,midpoint);
          off[m3] = midpoint;
          off[m4] = (small/2.0);
          vn[m0][m4] += off[m4];
          vn[m4][m0] -= off[m4];

          vn[m0][m3] += off[m3];
          vn[m3][m0] -= off[m3];
      
          } /* if negative y, negative x */
        } /* if negative x */
      } /* if deltay is bigger */

     vn[m1][m2] -= deltay/2.0;
     vn[m2][m1] += deltay/2.0;
     vn[m4][m1] += deltax/2,0;
     vn[m1][m4] -= deltax/2,0;
     vn[m3][m2] += deltax/2.0;
     vn[m2][m3] -= deltax/2.0;
     vn[m3][m4] += deltay/2.0;
     vn[m4][m3] -= deltay/2.0;

    // 1--3 and 2--4 and vise versa needs work.
     vn[m3][m1] += vn[m3][m0] + vn[m3][m4] + vn[m3][m2] - vn[m1][m4] - vn[m1][m0] - vn[m1][m2];
     vn[m1][m3] -= (vn[m3][m0] + vn[m3][m4] + vn[m3][m2] - vn[m1][m4] - vn[m1][m0] - vn[m1][m2]);
     vn[m4][m2] +=  vn[m4][m1] + vn[m4][m0] + vn[m4][m3] - vn[m2][m1] - vn[m2][m0] - vn[m2][m3];
     vn[m2][m4] -= ( vn[m4][m1] + vn[m4][m0] + vn[m4][m3] - vn[m2][m1] - vn[m2][m0] - vn[m2][m3]);

   

    fprintf(stderr,"	4 %lf, 1 %lf\n",off[m4],off[m1]);
    fprintf(stderr,"	3 %lf, 2 %lf\n",off[m3],off[m2]);
    
    double off_4_minus_2 = off[m4] - off[m2];
    double off_1_minus_3 = off[m1] - off[m3];
    
    fprintf(stderr,"		4-2 , %lf\n",off_4_minus_2);
    fprintf(stderr,"		1-3 , %lf\n",off_1_minus_3);
    double xoff0 = off_1_minus_3 - off_4_minus_2;
    double xoff1 = vn[m1][m2] + vn[m1][m4] - vn[m1][m0] - vn[m1][m3];
    double xoff2 = vn[m2][m3]+vn[m2][m4] - vn[2][1] - vn[2][0];

    double xoff3 = vn[m3][m4] + vn[m3][m0] - vn[m3][m2] - vn[m2][m1]; 
    double xoff4 = vn[m4][m0] + vn[m4][m1] - vn[m4][m2] - vn[m4][m3];
    
    double yoff0 = off_4_minus_2 +off_1_minus_3;
    double yoff1 = vn[m1][m0] + vn[m1][m2] - vn[m1][m3] - vn[m1][m4];
    double yoff2 =  vn[m2][m1] + vn[m2][m3] - vn[m2][m4] - vn[m2][m0];
    double yoff3 =  vn[m3][m4] + vn[m3][m0] - vn[m3][m2] - vn[m3][m1];
    double yoff4 =  vm[m4][m0] +vn[m4][m3] - vn[m4][m1] - vn[m4][m2];
    fprintf(stderr,"			offset_for_0 is %lf,%lf compared to original %lf,%lf\n",xoff0,yoff0,deltax,deltay);
    newxx[m0] = oldxx[m0]+xoff0;
    newyy[m0] = oldyy[m0]+yoff0;
    if ((newxx[m0] != newx)||(newyy[m0] != newy)) {
      fprintf(stderr,"errorrrrrrrrrrrrrrrRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRrrrrrrrr\n");
      }
    newxx[m1] = oldxx[m1] + xoff1;
    newyy[m1] = oldyy[m1] + yoff1;
    fprintf(stderr,"			offset_for_1 is %lf,%lf total %lf\n",xoff1,yoff1,fabs(xoff1)+fabs(yoff1));
    fprintf(stderr,"			offset_for_2 is %lf,%lftotal %lf\n",xoff2,yoff2,fabs(xoff2)+fabs(yoff2));
    fprintf(stderr,"			offset_for_3 is %lf ???,%lf ???total %lf\n",xoff3,yoff3,fabs(xoff4)+fabs(yoff4));
    fprintf(stderr,"			offset_for_4 is %lf,%lftotal %lf\n",xoff4,yoff4,fabs(xoff4)+fabs(yoff4));
    fprintf(stderr,"			actual distance is %lf without z is %lf\n",off[m0],sqrt(deltax*deltax+deltay*deltay));

    newxx[m2] = oldxx[m2] +xoff2;
    newyy[m2] = oldyy[m2] +yoff2;

    newxx[m3] = oldxx[m3] +xoff3;
    newyy[m3] = oldyy[m3] +yoff3

    newxx[m4] = oldxx[m4] +xoff4;
    newyy[m4] = oldyy[m4] +yoff4;
        
    } /* if we have an offset */
  if (deltas) {
    deltas[m0][newi] = off[m0];
    deltas[m1][newi] = off[m1];
    deltas[m2][newi] = off[m2];
    deltas[m3][newi] = off[m3];
    deltas[m4][newi] = off[m4];
    }
  // assign the new values at the end so new vectors can be old vectors
  for (unsigned long i=0;i<5;i++) {   
    new_vertex[i][newoffset] = newxx[i];
    new_vertex[i][newoffset+1l] = newyy[i];
    new_vertex[i][newoffset+2l] = newzz[i];
    }
  
  
  
  } /* for each vector */   
}



super_point expanded_superpos_to_xyz_context(expanded_superpos *epx,int context,float z) {
float *ms;
ms = superpos_map_to(context);
int m0=ms[m0];
int m1=ms[m1];
int m2=ms[m2];
int m3=ms[m3];
int m4=ms[m4];

    
    double off_4_minus_2 = epx-[m4] - off[m2];
    double off_1_minus_3 = off[m1] - off[m3];
    
    fprintf(stderr,"		4-2 , %lf\n",off_4_minus_2);
    fprintf(stderr,"		1-3 , %lf\n",off_1_minus_3);
    double xoff0 = epx->d[m0][m1] - epx->d[m0][m3]  - epx->d[m0][m4] + epx->d[m0][m2];
    double xoff1 = epx->d[m1][m2] + epx->d[m1][m4] - epx->d[m1][m0] - epx->d[m1][m3];
    double xoff2 = epx->d[m2][m3]+epx->d[m2][m4] - epx->d[2][1] - epx->d[2][0];

    double xoff3 = epx->d[m3][m4] + epx->d[m3][m0] - epx->d[m3][m2] - epx->d[m2][m1]; 
    double xoff4 = epx->d[m4][m0] + epx->d[m4][m1] - epx->d[m4][m2] - epx->d[m4][m3];
    
    double yoff0 = off_4_minus_2 +off_1_minus_3;
    double yoff1 = epx->d[m1][m0] + epx->d[m1][m2] - epx->d[m1][m3] - epx->d[m1][m4];
    double yoff2 =  epx->d[m2][m1] + epx->d[m2][m3] - epx->d[m2][m4] - epx->d[m2][m0];
    double yoff3 =  epx->d[m3][m4] + epx->d[m3][m0] - epx->d[m3][m2] - epx->d[m3][m1];
    double yoff4 =  vm[m4][m0] +epx->d[m4][m3] - epx->d[m4][m1] - epx->d[m4][m2];

    return (super_point){xyz:{xoff0,yoff0,z}};
}



int set_from_origin_in_context(int context,expanded_superpos *ep,double dist)) {
float *ms;
ms = superpos_map_to(context);
int m0=ms[m0];
int m1=ms[m1];
int m2=ms[m2];
int m3=ms[m3];
int m4=ms[m4];

/* so the m allows us to map to any one of the possible contexts - 0 thru 4 - properly */
double hdist = dist * 0.5; /* if dist is 2 (-1 to 1) then dist is 1 */
    

    ep->d[m0][m1]=0.;
    ep->d[m0][m2]=0.;
    ep->d[m0][m3]=0.;
    ep->d[m0][m4]=0.;
    // z is ignored
    
    ep->d[m1][m0]=hdist-ep->d[m0][m1];
    ep->d[m2][m0]=hdist-ep->d[m0][m2];
    ep->d[m3][m0]=hdist-ep->d[m0][m3];
    ep->d[m4][m0]=hdist-ep->d[m0][m4];
    ep->d[m0][m0]=0.;
    ep->d[m1][m1]=0.;
    ep->d[m2][m2]=0.;
    ep->d[m3][m3]=0.;
    ep->d[m4][m4]=0.;
    
    ep->d[m1][m2] = hdist;
    ep->d[m2][m1] = hdist;
    ep->d[m1][m3] = hdist;
    ep->d[m3][m1] = hdist;
    ep->d[m1][m4] = hdist;
    ep->d[m4][m4] = hdist;
    
    ep->d[m2][m3] = hdist;
    ep->d[m3][m2] = hdist;
    ep->d[m2][m4] = hdist;
    ep->d[m4][m2] = hdist;
    
    ep->d[m3][m4] = hdist;
    ep->d[m4][m3] = hdist;
}



super_point xyz_from_context_to_context(int from_context,float dist,super_point new_point;int to_context) {
expanded_superpos eo;
expanded_superpos en;
set_from_origin_in_context(from_context,&eo,(double)dist);
compute_expanded_superpos_from_xyz(from_context,float super_point,&eo,&en);

return(expanded_superpos_to_xyz_context(&en,to_context,new_point.xyz[z]));
}



int set_from_xyz_in_context(int context,GlFloat ep->d[5][5],double x,double y,double z, double dist)) {
int i;
int j;
float *ms;
ms = superpos_map_to(context);
int m0=ms[m0];
int m1=ms[m1];
int m2=ms[m2];
int m3=ms[m3];
int m4=ms[m4];

/* so the m allows us to map to any one of the possible contexts - 0 thru 4 - properly */
double hdist = dist * 0.5; /* if dist is 2 (-1 to 1) then dist is 1 */
    

        
    ep->d[m0][m1]=x*0.5f+y*0.5f;
    ep->d[m0][m2]=x*0.5f-y*0.5f;
    ep->d[m0][m3]=-x*0.5f - y*0.5f;
    ep->d[m0][m4]=-x*0.5f +y*0.5f;
    // z is ignored
    
    ep->d[m1][m0]=hdist-ep->d[m0][m1];
    ep->d[m2][m0]=hdist-ep->d[m0][m2];
    ep->d[m3][m0]=hdist-ep->d[m0][m3];
    ep->d[m4][m0]=hdist-ep->d[m0][m4];
    
    ep->d[m1][m2] = (dist-(ep->d[m2][m0] + ep->d[m0][m1])) * 0.5; /* half on 1->2, half on 2->1 */
    ep->d[m1][m3] = (dist-(ep->d[m3][m0] + ep->d[m0][m1])) * 0.5;
    ep->d[m1][m4] = (dist-(ep->d[m4][m0] + ep->d[m0][m1])) * 0.5;
    
    ep->d[m2][m1] = (dist-(ep->d[m1][m0] + ep->d[m0][m2])) * 0.5;
    ep->d[m2][m3] = (dist-(ep->d[m3][m0] + ep->d[m0][m2]) * 0.5;
    ep->d[m2][m4]=  (dist-(ep->d[m4][m0] + ep->d[m0][m2])) * 0.5;
    
    ep->d[m3][m1]= (dist-(ep->d[m1][m0] + ep->d[m0][m3])) * 0.5;
    ep->d[m3][m2]= (dist-(ep->d[m2][m0] + ep->d[m0][m3])) * 0.5;
    ep->d[m3][m4]= (dist-(ep->d[m4][m0] + ep->d[m0][m3])) * 0.5;
    
    ep->d[m4][m1]= (dist-(ep->d[m1][m0] + ep->d[m4][m0])) * 0.5;
    ep->d[m4][m2]= (dist-(ep->d[m2][m0] + ep->d[m4][m0])) * 0.5;
    ep->d[m4][m3]= (dist-(ep->d[m3][m0] + ep->d[m4][m0])) * 0.5;
return 0;    
}
    
    


/* end of file */
