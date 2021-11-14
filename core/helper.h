#ifndef HELPER_DOT_H
#define HELPER_DOT_H

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>


#include "superpos.h"
#include "larry_harvey_has_a_possee.h"
#include <stdio.h>
#include <stdlib.h>

#define GLFW_INCLUDE_ES2
#include <GLFW/glfw3.h>



typedef struct{
    GLfloat mat[4][4];
    } glMatrix;

#define NDEBUG

#ifdef NDEBUG
#define checkGlError(dfdff) (0)
#define gerr(dsdssd)
extern int checkGlErrorForReal(const char* op);
#else 
extern int checkGlError(const char* op);
//#define gerr(a) checkGlError(__FILE__ ":" __LINE__ ":" a)
#define gerr(a) checkGlError3(__FILE__,__LINE__,a)
#define checkGlErrorForReal(dfdff) checkGlError(dfddf)
#endif

extern int checkGlError3(const char *filename,int lineno,const char* op);

extern void printGLString(const char *name, GLenum s);


void multMatrix(glMatrix *result, glMatrix *srcA, glMatrix *srcB);
void loadIdentity(glMatrix *result);
void translateMatrix(glMatrix *result, GLfloat x,GLfloat y,GLfloat z);
void translateMatrix2(glMatrix *result, GLfloat x,GLfloat y,GLfloat z);
void scaleMatrix(glMatrix *result, GLfloat sx, GLfloat sy, GLfloat sz);
void set_matrix_translate(GLfloat centerx,GLfloat centery);

#endif
