#ifndef ONECOLOR_SPECS_H
#define ONECOLOR_SPECS_H 1

#include "hateglue.h"
#include "superpos.h"
#include "mini_not_rl.h"
#include "helper.h"

#define GLFW_INCLUDE_ES2
#include <GLFW/glfw3.h>

	      
typedef struct onecolor_specs {
 unsigned int do_effect;
 unsigned int one_color_program;
 unsigned int gvPositionHandle;
 int mPositionHandle;
 int mMVPMatrixHandle;

 glMatrix modelMatrix;
 glMatrix effectMatrix; /* used to do effects like zoom near/zoom away */
 glMatrix viewMatrix;
 glMatrix projectionMatrix;
 glMatrix MVMatrix;
 glMatrix MVPMatrix; // effective matrix for the menu - might be spinning in space.

 glMatrix origin_matrix;  // matrix tha makes x from -1 to 1, y from -1 to 1.  X or y might be bigger depe$

 int colorHandle;
 } onecolor_specs;


extern void init_onecolor_specs (onecolor_specs *onec);


#endif

