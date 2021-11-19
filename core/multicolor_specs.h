#ifndef MULTICOLOR_SPECS_H
#define MULTICOLOR_SPECS_H 1

#include "hateglue.h"
#include "superpos.h"
#include "mini_not_rl.h"
#include "helper.h"
#include "onecolor_specs.h"  

#define GLFW_INCLUDE_ES2
#include <GLFW/glfw3.h>

	      
typedef struct multicolor_specs {
 unsigned int do_effect;
 unsigned int multi_color_program;
 unsigned int gvPositionHandle;
 
 
 int gvColorHandle;
 int gvElementHandle;
 
 int mMVPMatrixHandle;


 } multicolor_specs;


extern void init_multicolor_specs (multicolor_specs *multic,onecolor_specs *onec);

extern multicolor_specs *multic;

#endif

