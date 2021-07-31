#include "onecolor_specs.h"

void init_onecolor_specs (onecolor_specs *onec) {
onec->do_effect=0;
onec->one_color_program=0;
onec->gvPositionHandle=0; 
onec->mPositionHandle=0;  
onec->mMVPMatrixHandle=0; 
loadIdentity(&(onec->modelMatrix));
loadIdentity(&(onec->effectMatrix));
loadIdentity(&(onec->viewMatrix));  
loadIdentity(&(onec->projectionMatrix));
loadIdentity(&(onec->origin_matrix));   
}

