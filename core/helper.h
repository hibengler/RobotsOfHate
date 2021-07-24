#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>


#include "superpos.h"

#include <stdio.h>
#include <stdlib.h>

#define GLFW_INCLUDE_ES2
#include <GLFW/glfw3.h>



typedef struct{
    GLfloat mat[4][4];
    } glMatrix;


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

