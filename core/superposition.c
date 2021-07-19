#include <stdio.h>
#include <math.h>
#include <stdlib.h>

/* a test program to see if superpositioning could be figured out by cheating with triangles.
You see - delta x and delta y domt mean much here, and the distances are all sqrt(2) from the deltas.

But by keeping track of the distances from the 4 points, it is a distinace using diagonals.
The diagonals can be used in each version. it is much easier than doing arbitrary triangle position calculation if the distances
were not exact diagonals.   But this mans that most offsets are L's
*/


int find_pos(double *oldxx,double *oldyy,double *newxx, double *newyy) {




double off[5][5]; 
// offset in diagonals.  We can then add or subtract them to get the right result
double oldx=oldxx[0];
double oldy=oldyy[0];
double newx=newxx[0]; 
double newy=newyy[0];


double deltax,deltay;
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

if ((oldx==0.)&&(oldy==0.)&&(newx==0.)&&(newy==0.)) {
  double dist=100;
  oldxx[1] = dist;
  oldyy[1] = dist;
  oldxx[2] = dist;
  oldyy[2] = -dist;
  oldxx[3] = -dist;
  oldyy[3] = -dist;
  oldxx[4] = dist;
  oldyy[4] = -dist;
  return 0;
  }
deltax = newx-oldx;
deltay = newy-oldy;

{
int i,j;
for (i=0;i<5;i++) {
  for (j=0;j<5;j++) {
    off[i][j]=0;
    }
  }
}
fprintf(stderr, 
"\nfrom %lf,%lf to %lf,%lf delta %lf,%lf\n",oldx,oldy,newx,newy,deltax,deltay);

if (fabs(deltay)>((fabs(deltax)+fabs(deltay))/2.0)) { /* delta y is more */
  if (deltay>0.) {
    if (deltax>0.) {
      fprintf(stderr,"deltay is bigger, and positive, deltax is positive  ");
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
      off[0][1] = midpoint;
      off[0][4] = small/2.0;
      
      off[1][0] = -off[0][1];
      off[2][0] = -off[0][2];
      off[3][0] = -off[0][3];
      off[4][0] = -off[0][4];
      }
    else { /* deltay>0 deltax <0 */
      fprintf(stderr,"ddd deltay is bigger, and positive, deltax is negative  ");
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
      fprintf(stderr,"small is %lf midpoint is %lf\n",small,midpoint);
      off[0][4] = midpoint;
      off[0][1] =  small/2.0;
      
      off[1][0] = -off[0][1];
      off[2][0] = -off[0][2];
      off[3][0] = -off[0][3];
      off[4][0] = -off[0][4];
      }
    } /* if deltay >0 */
  else { /* deltay is <0, */     
    if (deltax>=0.) {
      fprintf(stderr,"deltay is bigger, and negative, deltax is positive  ");

      double big = (deltay);      
      double startpoint= deltay;
      fprintf(stderr,"big offset is %lf startpoint is %lf\n",big,startpoint);
      off[0][4] = (big-deltax)*0.5;
      off[0][3] = (big-deltax)*0.5;
      off[0][1] = big;
      off[0][2] = 0;
      
    
    
      off[1][0] = -off[0][1];
      off[2][0] = -off[0][2];
      off[3][0] = -off[0][3];
      off[4][0] = -off[0][4];
      }  
    else { /* negative geltay, negative deltax */
      fprintf(stderr,"deltay is bigger, and negative, deltax is negative  ");
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
      off[0][3] = midpoint;
      off[0][2] =  (small/2.0);
      
    
      off[1][0] = -off[0][1];
      off[2][0] = -off[0][2];
      off[3][0] = -off[0][3];
      off[4][0] = -off[0][4];
      } /* if negative y, negative x */
    } /* if negative  */
  } /* if deltay is bigger */
else { /* deltax is bigger */
  if (deltay>0.) {
    if (deltax>0.) {
      fprintf(stderr,"deltay is smaller, and positive, deltax is positive  ");
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
      fprintf(stderr,"small is %lf midpoint is %lf\n",small,midpoint);
      off[0][1] = midpoint;
      off[0][2] = (small/2.0);
      
      off[1][0] = -off[0][1];
      off[2][0] = -off[0][2];
      off[3][0] = -off[0][3];
      off[4][0] = -off[0][4];
      }
    else { /* deltay>0 deltax <0 */
      fprintf(stderr,"deltay is smaller, and positive, deltax is negative  ");
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
      fprintf(stderr,"small is %lf midpoint is %lf\n",small,midpoint);
      off[0][4] = midpoint;
      off[0][3] = (small/2.0);
      
      off[1][0] = -off[0][1];
      off[2][0] = -off[0][2];
      off[3][0] = -off[0][3];
      off[4][0] = -off[0][4];
      }
    } /* if deltay >0 */
  else { /* deltay is <0, */     
    if (deltax>0.) {
      fprintf(stderr,"deltay is smaller, and negative, deltax is positive  ");

      double big = (deltax);      
      double startpoint= deltay;
      fprintf(stderr,"big offset is %lf startpoint is %lf\n",big,startpoint);
      off[0][2] = (big-deltay)*0.5;
      off[0][3] = (big-deltay)*0.5;
      off[0][1] = big;
      off[0][4] = 0;
      
    
      off[1][0] = -off[0][1];
      off[2][0] = -off[0][2];
      off[3][0] = -off[0][3];
      off[4][0] = -off[0][4];
      }  
    else { /* negative geltay, negative deltax */
      fprintf(stderr,"deltay is smaller, and negative, deltax is negative  ");
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
      off[0][3] = midpoint;
      off[0][4] = (small/2.0);
      
    
      off[1][0] = -off[0][1];
      off[2][0] = -off[0][2];
      off[3][0] = -off[0][3];
      off[4][0] = -off[0][4];
      } /* if negative y, negative x */
    } /* if negative x */
  } /* if deltay is bigger */


newxx[0] = oldxx[0]+off[0][1] - off[0][4] + off[0][2] - off[0][3];
newyy[0] = oldyy[0]+off[0][1] + off[0][4] - off[0][2] - off[0][3];
if ((newxx[0] != newx)||(newyy[0] != newy)) {
  fprintf(stderr,"errorrrrrrrrrrrrrrrrrrrrrrr\n");
  }

fprintf(stderr,"	4 %lf, 1 %lf\n",off[0][4],off[0][1]);
fprintf(stderr,"	3 %lf, 2 %lf\n",off[0][3],off[0][2]);

newxx[1] = oldxx[1] +off[0][1] - off[0][4] -off[0][3] +off[0][2];
newyy[1] = oldyy[1] -off[0][1] - off[0][4] +off[0][3] +off[0][2];

newxx[2] = oldxx[2] -off[0][2] + off[0][4] - off[0][1] + off[0][3];
newyy[2] = oldyy[2] -off[0][3] - off[0][2] +off[0][4] + off[0][1];

newxx[3] = oldxx[3] -off[0][3] - off[0][2] +off[0][4] + off[0][1];
newyy[3] = oldyy[3] +off[4][0] - off[0][1] + off[0][3] + off[0][2];

newxx[4] = oldxx[4] - off[0][4] + off[0][2] + off[0][3] - off[0][1];
newyy[4] = oldyy[4] + off[0][4] - off[0][2] + off[0][3] - off[0][1];
}



int main () {
double oldx,oldy;
double newx,newy;
oldx=0.;
oldy=0.;
newx=0.;
newy=0.;
double oldxx[5];
double oldyy[5];
double newxx[5];
double newyy[5];

oldxx[0]=oldx;
oldyy[0]=oldy;
newxx[0]=newx;
newyy[0]=newy;


find_pos(oldxx,oldyy,newxx,newyy);
while (1) {
  int x = rand();
  double offx=( ((double)(x % 50)) )-25.;
  int y = rand();
  double offy=( ((double)(y % 50)) )-25.;
  newxx[0] = oldxx[0] + offx;
  newyy[0] = oldyy[0] + offy;
  if ((newxx[0]>100)||(newxx[0]<= -100)) {continue;}
  if ((newyy[0]>100)||(newyy[0]<= -100)) {continue;}
  find_pos(oldxx,oldyy,newxx,newyy);
  fprintf(stderr,
"	%lf,%lf %lf,%lf	 %lf,%lf	%lf,%lf,%lf,%lf -->\n"
,oldxx[0],oldyy[0],oldxx[1],oldyy[1],oldxx[2],oldyy[2],oldxx[3],oldyy[3],oldxx[4],oldyy[4]);
  fprintf(stderr,
"	%lf,%lf %lf,%lf	 %lf,%lf	%lf,%lf,%lf,%lf\n"
,newxx[0],newyy[0],newxx[1],newyy[1],newxx[2],newyy[2],newxx[3],newyy[3],newxx[4],newyy[4]);


int i; for (i=0;i<5;i++) {
  oldxx[i]=newxx[i];
  oldyy[i]=newyy[i];
  }
  } // while
}

