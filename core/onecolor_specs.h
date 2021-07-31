#ifndef ONECOLOR_SPECS_H
#define ONECOLOR_SPECS_H 1

	      
typedef struct onecolor_specs {
 GLuint do_effect,
 GLuint one_color_program,
 GLuint gvPositionHandle,
 GLint mPositionHandle,
 GLint mMVPMatrixHandle,

 glMatrix modelMatrix,
 glMatrix effectMatrix, /* used to do effects like zoom near/zoom away */
 glMatrix viewMatrix,
 glMatrix projectionMatrix,
 glMatrix MVMatrix,
 glMatrix MVPMatrix, // effective matrix for the menu - might be spinning in space.

 glMatrix origin_matrix,  // matrix tha makes x from -1 to 1, y from -1 to 1.  X or y might be bigger depe$

 int colorHandle;
 } onecolor_specs;


extern void init_onecolor_specs (omecolor_specs *onec);


#endif

