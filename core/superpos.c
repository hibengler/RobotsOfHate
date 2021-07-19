/* derived from find_pos in the original superposition.c */

#include "superpos.h"

#include <stdio.h>
#include <math.h>
#include <stdlib.h>


static GLfloat placeholder_old0[] = {0.,0.,0.};
static GLfloat * placeholder_old[] = {placeholder_old0,placeholder_old0,placeholder_old0,placeholder_old0,placeholder_old0};



void compute_superpos_vertices(GLfloat *old_vertex[5],GLfloat *new_vertex[5],int count,GLfloat *deltas[5])
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


  // offset in diagonals.  We can then add or subtract them to get the right result
  double oldx=oldxx[0];
  double oldy=oldyy[0];
  double oldz=oldzz[0];
  double newx=newxx[0]; 
  double newy=newyy[0];
  double newz=newzz[0];



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
    off[0] = sqrt(deltax*deltax+deltay*deltay+deltaz*deltaz);
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

    if (fabs(deltay)>((fabs(deltax)+fabs(deltay))/2.0)) { /* delta y is more */
      if (deltay>0.) {
        if (deltax>0.) {
          fprintf(stderr,"0 deltay is bigger, and positive, deltax is positive  ");
          double small;
      
          /* 0 to 1, then 0 to 4*/
          if (deltax>(deltay-deltax)) {
            small = deltay-deltax;
  	    }
          else { 
            small = deltax;
 	    }
          small = deltay-deltax;
	 
          double midpoint = (deltay+deltax)/2.0;
          fprintf(stderr,"small is %lf midpoint is %lf\n",small,midpoint);
          off[1] = midpoint;
          off[4] = small/2.0;
      
          }
        else { /* deltay>0 deltax <0 */
//          fprintf(stderr,"1 deltay is bigger, and positive, deltax is negative  ");
          double small;
          /* 0 to 1, then 0 to 4*/
          if ((-deltax)>(deltay+deltax)) {
            small = deltay+deltax;
	    }
          else { 
            small = (-deltax);
	    }
          small = deltay+deltax;
          double midpoint = (deltay-deltax)/2.0;
//          fprintf(stderr,"small is %lf midpoint is %lf\n",small,midpoint);
          off[4] = midpoint;
          off[1] =  small/2.0;
          }
        } /* if deltay >0 */
      else { /* deltay is <0, */     
        if (deltax>=0.) {
  //        fprintf(stderr,"2deltay is bigger, and negative, deltax is positive  ");

          double big = (deltay);      
          double startpoint= deltay;
  //        fprintf(stderr,"big offset is %lf startpoint is %lf\n",big,startpoint);
          off[4] = (big-deltax)*0.5;
          off[3] = (big-deltax)*0.5;
          off[1] = big;
          off[2] = 0;
          }  
        else { /* negative geltay, negative deltax */
    //      fprintf(stderr,"3 deltay is bigger, and negative, deltax is negative  ");
          double small;
          /* 0 to 1, then 0 to 4*/
          if ((-deltax)>(-(deltay)-(deltax))) {
            small = -(deltay)-(-deltax);
            }
          else { 
            small = -deltax;
	    }
          small = -(deltay)-(-deltax);
       
          double midpoint = ((-deltay)+(-deltax))/2.0;
          fprintf(stderr,"small is %lf midpoint is %lf\n",small,midpoint);
          off[3] = midpoint;
          off[2] =  (small/2.0);
      
    
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
	    }
          else { 
            small = deltay;
	    }
          small =  deltax-deltay; //??? hack
      
          double midpoint = (deltay+deltax)/2.0;
//          fprintf(stderr,"small is %lf midpoint is %lf\n",small,midpoint);
          off[1] = midpoint;
          off[2] = (small/2.0);
      
          }
        else { /* deltay>0 deltax <0 */
//          fprintf(stderr,"5 deltay is smaller, and positive, deltax is negative  ");
          double small;
          /* 0 to 1, then 0 to 4*/
          if ((deltay)>((-deltax)-deltay)) {
            small = (-deltax)-deltay;
 	    }
          else { 
            small = (deltay);
	    }
          small = (-deltax)-deltay;
      	
          double midpoint = (deltay-deltax)/2.0;
//          fprintf(stderr,"small is %lf midpoint is %lf\n",small,midpoint);
          off[4] = midpoint;
          off[3] = (small/2.0);
      
          }
        } /* if deltay >0 */
      else { /* deltay is <0, */     
        if (deltax>0.) {
//          fprintf(stderr,"6 deltay is smaller, and negative, deltax is positive  ");

          double big = (deltax);      
          double startpoint= deltay;
//          fprintf(stderr,"big offset is %lf startpoint is %lf\n",big,startpoint);
          off[2] = (big-deltay)*0.5;
          off[3] = (big-deltay)*0.5;
          off[1] = big;
          off[4] = 0;
          }  
        else { /* negative geltay, negative deltax */
  //        fprintf(stderr,"7 deltay is smaller, and negative, deltax is negative  ");
          double small;
          /* 0 to 1, then 0 to 4*/
          if ((-deltay)>(-(deltax)-(deltay))) {
            small = -(deltax)-(-deltay);
            }
          else { 
            small = -deltay;
	    }
          small = -(deltax)-(-deltay);
         
          double midpoint = ((-deltay)+(-deltax))/2.0;
          fprintf(stderr,"small is %lf midpoint is %lf\n",small,midpoint);
          off[3] = midpoint;
          off[4] = (small/2.0);
      
          } /* if negative y, negative x */
        } /* if negative x */
      } /* if deltay is bigger */



    fprintf(stderr,"	4 %lf, 1 %lf\n",off[4],off[1]);
    fprintf(stderr,"	3 %lf, 2 %lf\n",off[3],off[2]);
    
    double off_4_minus_2 = off[4] - off[2];
    double off_1_minus_3 = off[1] - off[3];
    
    fprintf(stderr,"		4-2 , %lf\n",off_4_minus_2);
    fprintf(stderr,"		1-3 , %lf\n",off_1_minus_3);
    double xoff0 = off_1_minus_3 - off_4_minus_2;
    double xoff1 = off_1_minus_3 - off_4_minus_2;
    double xoff2 = off_4_minus_2 - off_1_minus_3;
    double xoff3 = off_4_minus_2 + off_1_minus_3; // ???
    double xoff4 = -off_4_minus_2 -off_1_minus_3;
    
    double yoff0 = off_4_minus_2 +off_1_minus_3;
    double yoff1 = - off_4_minus_2 - off_1_minus_3;
    double yoff2 =   off_4_minus_2 + off_1_minus_3;
    double yoff3 =  -off_4_minus_2 - off_1_minus_3;  // ???
    double yoff4 =  off_4_minus_2 - off_1_minus_3;
    fprintf(stderr,"			offset_for_0 is %lf,%lf compared to original %lf,%lf\n",xoff0,yoff0,deltax,deltay);
    newxx[0] = oldxx[0]+xoff0;
    newyy[0] = oldyy[0]+yoff0;
    if ((newxx[0] != newx)||(newyy[0] != newy)) {
      fprintf(stderr,"errorrrrrrrrrrrrrrrRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRrrrrrrrr\n");
      }
    newxx[1] = oldxx[1] + xoff1;
    newyy[1] = oldyy[1] + yoff1;
    fprintf(stderr,"			offset_for_1 is %lf,%lf total %lf\n",xoff1,yoff1,fabs(xoff1)+fabs(yoff1));
    fprintf(stderr,"			offset_for_2 is %lf,%lftotal %lf\n",xoff2,yoff2,fabs(xoff2)+fabs(yoff2));
    fprintf(stderr,"			offset_for_3 is %lf ???,%lf ???total %lf\n",xoff3,yoff3,fabs(xoff4)+fabs(yoff4));
    fprintf(stderr,"			offset_for_4 is %lf,%lftotal %lf\n",xoff4,yoff4,fabs(xoff4)+fabs(yoff4));
    fprintf(stderr,"			actual distance is %lf without z is %lf\n",off[0],sqrt(deltax*deltax+deltay*deltay));

    newxx[2] = oldxx[2] +xoff2;
    newyy[2] = oldyy[2] +yoff2;

    newxx[3] = oldxx[3] +xoff3;
    newyy[3] = oldyy[3] -off[4] - off[1] + off[3] + off[2];

    newxx[4] = oldxx[4] +xoff4;
    newyy[4] = oldyy[4] + off[4] - off[2] + off[3] - off[1];
        
/*  newxx[1] = oldxx[1] +off[1] - off[4] -off[3] +off[2];
    newyy[1] = oldyy[1] -off[1] - off[4] +off[3] +off[2];

    newxx[2] = oldxx[2] -off[2] + off[4] - off[1] + off[3];
    newyy[2] = oldyy[2] -off[3] - off[2] +off[4] + off[1];

    newxx[3] = oldxx[3] -off[3] - off[2] +off[4] + off[1];
    newyy[3] = oldyy[3] -off[4] - off[1] + off[3] + off[2];

    newxx[4] = oldxx[4] - off[4] + off[2] + off[3] - off[1];
    newyy[4] = oldyy[4] + off[4] - off[2] + off[3] - off[1];
    */
    } /* if we have an offset */
  if (deltas) {
    deltas[0][newi] = off[0];
    deltas[1][newi] = off[1];
    deltas[2][newi] = off[2];
    deltas[3][newi] = off[3];
    deltas[4][newi] = off[4];
    }
  // assign the new values at the end so new vectors can be old vectors
  for (unsigned long i=0;i<5;i++) {   
    new_vertex[i][newoffset] = newxx[i];
    new_vertex[i][newoffset+1l] = newyy[i];
    new_vertex[i][newoffset+2l] = newzz[i];
    }
  
  
  
  } /* for each vector */   
}


/* end of file */
