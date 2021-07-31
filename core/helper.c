#include "helper.h"
    
void multMatrix(glMatrix *result, glMatrix *srcA, glMatrix *srcB)
{
    glMatrix    tmp;
    int         i;

    for (i=0; i<4; i++)
    {
        tmp.mat[i][0] = (srcA->mat[i][0] * srcB->mat[0][0]) +
                        (srcA->mat[i][1] * srcB->mat[1][0]) +
                        (srcA->mat[i][2] * srcB->mat[2][0]) +
                        (srcA->mat[i][3] * srcB->mat[3][0]) ;

        tmp.mat[i][1] = (srcA->mat[i][0] * srcB->mat[0][1]) +
                        (srcA->mat[i][1] * srcB->mat[1][1]) +
                        (srcA->mat[i][2] * srcB->mat[2][1]) +
                        (srcA->mat[i][3] * srcB->mat[3][1]) ;

        tmp.mat[i][2] = (srcA->mat[i][0] * srcB->mat[0][2]) +
                        (srcA->mat[i][1] * srcB->mat[1][2]) +
                        (srcA->mat[i][2] * srcB->mat[2][2]) +
                        (srcA->mat[i][3] * srcB->mat[3][2]) ;

        tmp.mat[i][3] = (srcA->mat[i][0] * srcB->mat[0][3]) +
                        (srcA->mat[i][1] * srcB->mat[1][3]) +
                        (srcA->mat[i][2] * srcB->mat[2][3]) +
                        (srcA->mat[i][3] * srcB->mat[3][3]) ;
    }
    memcpy(result, &tmp, sizeof(glMatrix));
}

void loadIdentity(glMatrix *result)
{
    memset(result, 0x0, sizeof(glMatrix));
    result->mat[0][0] = 1.0f;
    result->mat[1][1] = 1.0f;
    result->mat[2][2] = 1.0f;
    result->mat[3][3] = 1.0f;
}
                        
void translateMatrix(glMatrix *result, GLfloat x,GLfloat y,GLfloat z) {
        glMatrix transmat;
    //memset(transmat, 0x0, sizeof(glMatrix));
    loadIdentity(&transmat);

     transmat.mat[3][0] = x;
     transmat.mat[3][1] = y;
     transmat.mat[3][2] = z;
     transmat.mat[3][3] = 1.0f;
/*
      transmat.mat[0][3] = x;
     transmat.mat[1][3] = y;
     transmat.mat[2][3] = z;
     transmat.mat[3][3] = 1.0f;
*/
                        
    multMatrix( result, &transmat, result );
}

void translateMatrix2(glMatrix *result, GLfloat x,GLfloat y,GLfloat z) {
        glMatrix transmat;
    //memset(transmat, 0x0, sizeof(glMatrix));
    loadIdentity(&transmat);
/*
     transmat.mat[3][0] = x;
     transmat.mat[3][1] = y;
     transmat.mat[3][2] = z;
     transmat.mat[3][3] = 1.0f;
*/
      transmat.mat[0][3] = x;
     transmat.mat[1][3] = y;
     transmat.mat[2][3] = z;
     transmat.mat[3][3] = 1.0f;
 
    
    multMatrix( result, &transmat, result );
}

                        
void scaleMatrix(glMatrix *result, GLfloat sx, GLfloat sy, GLfloat sz)
{
    result->mat[0][0] *= sx;
    result->mat[0][1] *= sx;
    result->mat[0][2] *= sx;
    result->mat[0][3] *= sx;

    result->mat[1][0] *= sy;
    result->mat[1][1] *= sy;
    result->mat[1][2] *= sy;
    result->mat[1][3] *= sy;
    
    result->mat[2][0] *= sz;
    result->mat[2][1] *= sz;
    result->mat[2][2] *= sz;
    result->mat[2][3] *= sz;
}
    
