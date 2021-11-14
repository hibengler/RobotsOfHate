




#include "video_planets.h"
#include <sys/time.h>


#include "crystals_geo.h"




static hate_rps normalize_rps(hate_rps in_rps) {
float r = in_rps.rps[0]*in_rps.rps[0];
float p = in_rps.rps[1]*in_rps.rps[1];
float s = in_rps.rps[2]*in_rps.rps[2];
float size = r+p+s;
if (size>0.0) {
  float one_over_size = 1.f/size;
  in_rps.rps[0] = in_rps.rps[0] * one_over_size;
  in_rps.rps[1] = in_rps.rps[1] * one_over_size;
  in_rps.rps[2] = in_rps.rps[2] * one_over_size;
  in_rps.rps[3] = in_rps.rps[3] * size;
  }
return in_rps;
}


static void transfer_vertexes_to_indexes(video_planet_data *dat) {
int a=0;
for (int i=0;i<dat->number_indexes;i++) {
  int index = dat->indexes[i];
  dat->final_vertexes[i*4]=dat->vertexes[index*4];
  dat->final_vertexes[i*4+1]=dat->vertexes[index*4+1];
  dat->final_vertexes[i*4+2]=dat->vertexes[index*4+2];
  dat->final_vertexes[i*4+3]=1.f;
  
  
  int ic = i/3;
  dat->final_colors[i*4]=dat->colors[ic*4];
  dat->final_colors[i*4+1]=dat->colors[ic*4+1];
  dat->final_colors[i*4+2]=dat->colors[ic*4+2];
  dat->final_colors[i*4+3]=dat->colors[ic*4+3];
  }
}









static float normal_rock_color[4] = {0.58f,0.294f,0.0f,1.f};
static float normal_paper_color[4] = {0.0f,0.8f,0.0f,1.f};
static float normal_scissors_color[4] = {0.8313f,0.686f,0.215f,1.f};
static float normal_water_color[4] = {0.0f,0.0f,0.9f,1.f};
static float normal_other_color[4] = {0.5f,0.5f,0.5f,1.f};

/* rock - brown.
   paper - green
   scissors -> purple or gold
   water - blue
   Step 1 - figure out the type of each cube
   Step 2 - distribute power - .25 for under water, rest above.
   Step 3 - randomly pick r,p,s distribution, 
   
   Expand - randomly pick subcolor of color.
   RPS ratio is different for each screen, so have each planet warped.
   
   Greyscale and blue until it is contacted by a player.
   Blah Blah Blah
   for now, just default to a default color.
   */
static void generate_colors(video_planet_data *dat,hate_rps actual_rps,int *rps_view,float color_saturation, float *other_color) {

video_planet_color_info *info= &dat->color_info;


hate_rps  normalized_rps = normalize_rps(info->actual_rps);

info->actual_rps= actual_rps;
  
for (int i=0;i<4;i++) {
  info->rps_order[i] = rps_view[i];
  }  

/* compute view_rps */
hate_rps view_rps;
for (int i=0;i<4;i++) {
  view_rps.rps[i] = normalized_rps.rps[rps_view[i]];
  }


// default out rps colors
for (int i=0;i<4;i++) {
   info->rps_colors[0][i] = normal_rock_color[i];
   info->rps_colors[1][i] = normal_paper_color[i];
   info->rps_colors[2][i] = normal_scissors_color[i];
   info->rps_colors[3][i] = normal_water_color[i];
   info->rps_colors[4][i] = other_color[i];
   }

info->color_saturation = color_saturation;

int water_choice_amt = ((rand()) & 32767) + 16384 ; /* 25 to 74% water 1023 */
info->water_ratio = ((float)water_choice_amt) / 65536.f;

int actual_number_water = 0;

for (int i=0;i<info->number_triangles;i++) {
  unsigned int water_choice = (rand()) & 65535;
  if (water_choice < water_choice_amt) {
    info->types[i]=3;
    actual_number_water++;
    }
  else info->types[i]=4;
  }



   
// pick the colors         
int number_above = (info->number_triangles-actual_number_water);
int number_left = number_above;
int number_rock = (int)(  (float)number_left *  view_rps.rps[0]);
int number_paper = (int)(  (float)number_left *  view_rps.rps[1]);
int number_scissors = number_left - number_rock - number_paper;


int i=0;
while (number_left) {  
  if (info->types[i]!=3) {  // if not water
    int level_rock = number_rock;
    int level_paper = number_paper+number_rock;
    unsigned int x = (rand() % number_left);
    if (x<level_rock) {
      info->types[i]=0;
      number_rock--;
      }
    else if (x<level_paper) {
      info->types[i]=1;
      number_paper--;
      }
    else {
      info->types[i]=2;
      number_scissors--;
      }
    number_left--;
    }
  i++;
  }
}













/* to expand, we need to know what colors are on each edge, or were  */
static void explode_colors(video_planet_data *new,video_planet_data *old) {

video_planet_color_info *oldinfo= &old->color_info;
video_planet_color_info *newinfo= &new->color_info;

newinfo->actual_rps = oldinfo->actual_rps;

for (int i=0;i<4;i++) {
  newinfo->rps_order[i] =  oldinfo->rps_order[i];
  }  


hate_rps  normalized_rps = normalize_rps(newinfo->actual_rps);

/* compute view_rps */
hate_rps view_rps;


for (int i=0;i<4;i++) {
  view_rps.rps[i] = newinfo->actual_rps.rps[newinfo->rps_order[i]];
  }

// default out rps colors
for (int i=0;i<4;i++) {
   newinfo->rps_colors[0][i] = oldinfo->rps_colors[0][i];
   newinfo->rps_colors[1][i] = oldinfo->rps_colors[1][i];
   newinfo->rps_colors[2][i] = oldinfo->rps_colors[2][i];
   newinfo->rps_colors[3][i] = oldinfo->rps_colors[3][i];
   newinfo->rps_colors[4][i] = oldinfo->rps_colors[4][i];
   }


newinfo->water_ratio = oldinfo->water_ratio;
int water_choice_amt =   (float)newinfo->water_ratio  * (float)newinfo->number_triangles;

int actual_number_rock=0;
int actual_number_paper=0;
int actual_number_scissors=0;
int actual_number_left=0;

int actual_number_water = 0;
for (int i=0;i<newinfo->number_triangles;i++) {
  if (newinfo->types[i]==0) {
    actual_number_rock++;
    }
  else if (newinfo->types[i] == 1) {
    actual_number_paper++;
    }
  else if (newinfo->types[i] == 2) {
    actual_number_scissors++;
    }
  else if (newinfo->types[i] == 3) {
    actual_number_water++;
    }
  else {
    actual_number_left++;  // other
    }
  }


/* every 0,4,8,12, etc vector is a vector of midpoints - so we can assume the midpoint is a good vector to define an Edge! yes!
so we dont need to handle the edges.
for each midpoint vector - we init the count to 0 and init the sums to 0 for each type
Then! for each midpoiint, sum up the colors! Then the midpoint averages - for each triangle gives a good distribution
Lets say there are 2 - which is usual:

We sum up rocks and sum up papers, and sum up scissors and sum up waters - and multiply by number left and then divide by count 
*/

// Init:

for (int i=0;i<new->number_indexes;i++) {
  new->final_vertexes[i*4]=0.f;
  new->final_vertexes[i*4+1]=0.f;
  new->final_vertexes[i*4+2]=0.f;
  new->final_vertexes[i*4+3]=0.f;
  }


// tally the types of rock, paper, scissors, water
int ci=0;
  
for (int i=0;i<new->number_indexes;i+= (4*3)) {
  // Borrow the final_vertexes - which is number of indexes * 4
  float *summary_m1 = &new->final_vertexes[new->indexes[i]*4];
  float *summary_m2 = &new->final_vertexes[new->indexes[i+1]*4];
  float *summary_m3 = &new->final_vertexes[new->indexes[i+2]*4];
  
  if (newinfo->types[ci] <4) { // make sure it is water, land, etc  
    int type = newinfo->types[ci];
    summary_m1[type]++;
    summary_m2[type]++;
    summary_m3[type]++;
    }
  ci++;
  }
 
// so all the midpoints have the tally of the number of colors on each side, and the end points have all zeros 
  


   
    
        
int number_total = newinfo->number_triangles;
int number_above = actual_number_left;
int number_left = number_above;
int number_water = (int)(float)number_total * newinfo->water_ratio - (float)actual_number_water;
if (number_water <0) number_water=0;

int number_rock = (int)(  (float)(number_total- number_water) *  view_rps.rps[0]) - actual_number_rock;
if (number_rock<0) number_rock=0;

int number_paper = (int)(  (float)(number_total-number_water) *  view_rps.rps[1]) - actual_number_paper;
if (number_paper<0) number_paper=0;
int number_scissors = number_left - number_water - number_rock - number_paper;
if (number_scissors<0) number_paper=0;

ci=0;
while (number_left) {  
  // come up with the levels by first averaging the colors on the 2 sides ( by the 2 midpoints)
  int i=ci*3;
  if (newinfo->types[ci] == 4) {
    int type_count[4];
    
    for (int j=0;j<4;j++) {
      type_count[j]=0;
      }
    
    int count=0;
    for (int j=0;j<3;j++) {
      float *summary = &new->final_vertexes[new->indexes[i+j]*4];
      for (int k=0;k<4;k++) {
        type_count[k]+=  (int)summary[k];
	count += (int)summary[k];
	}
      }	
    
    float water_ratio;
    float rock_ratio;
    float paper_ratio;
    float scissors_ratio;
    if (count==0) {
      water_ratio = 0.f;
      rock_ratio = 0.f;
      paper_ratio = 0.f;
      scissors_ratio = 0.f;
      }
    else {
      water_ratio = ((float)type_count[3])/((float)(count));
      rock_ratio =  ((float)type_count[0])/((float)(count));
      paper_ratio = ((float)type_count[1])/((float)(count));
      scissors_ratio = ((float)type_count[2])/((float)(count));
      }

    int level_water = (int)(  water_ratio * (float)(number_water) * 15.f + (float)number_water);
    int level_rock = level_water + (int)(  rock_ratio * (float)(number_rock) * 15.f + (float)number_rock);
    int level_paper = level_rock + (int)(  paper_ratio * (float)(number_paper) * 15.f + (float)number_paper);
    int level_scissors = level_scissors + (int)(  scissors_ratio * (float)(number_scissors) * 15.f + (float)number_scissors);

    unsigned int x = (rand() % level_scissors);
    if (x<level_water) {
      newinfo->types[ci]=3;
      number_water--;
      }
    else if (x<level_rock) {
      newinfo->types[ci]=0;
      number_rock--;
      }
    else if (x<level_paper) {
      newinfo->types[ci]=1;
      number_paper--;
      }
    else {
      newinfo->types[ci]=2;
      number_scissors--;
      }
    number_left--;
    }
  ci++;
  }
}





static video_planet_data *allocate_video_data(int vertexes,int indexes) {
video_planet_data *dat = malloc(sizeof(video_planet_data));

memset((void *)dat,0,sizeof(video_planet_data));
dat->number_vertexes = vertexes;
dat->number_indexes = indexes;

dat->indexes = malloc(sizeof(GLshort)*dat->number_indexes);
dat->vertexes = malloc(sizeof(GLfloat)*4*dat->number_vertexes);
dat->colors = malloc(sizeof(GLfloat)*4*(dat->number_indexes/3));
dat->final_vertexes = malloc(sizeof(GLfloat)*4*dat->number_indexes);
dat->final_colors = malloc(sizeof(GLfloat)*4*dat->number_indexes);

video_planet_color_info *info = &dat->color_info;

info->number_triangles = indexes/3;
info->types = malloc(info->number_triangles);

return dat;
}




static void free_video_data(video_planet_data *dat) {
if (!dat) return;

video_planet_color_info *info = &dat->color_info;
if (info->types) free(info->types);
if (dat->indexes) free(dat->indexes);
if (dat->vertexes) free(dat->vertexes);

if (dat->colors) free(dat->colors);
if (dat->final_vertexes) free(dat->final_vertexes);
if (dat->final_colors) free(dat->final_colors);
memset((void *)dat,0,sizeof(video_planet_data));
free(dat);
}



static void utoh () {
fprintf(stderr,"utoh\n");
}

// order of n
static int expand_locate_vertex(float *vertex,int *pnumber_vectors, float *new_vertexes,int max) {
int number_vectors = *pnumber_vectors;
int i;
for (i=0;i<number_vectors;i++) {
  if (i>=max) {
    utoh();
    }
  float *test= &new_vertexes[i*4];
  if (test[0]!=vertex[0]) continue;
  if (test[1]!=vertex[1]) continue;
  if (test[2]!=vertex[2]) continue;
  if (test[3]!=vertex[3]) continue;
  return i;
  }
new_vertexes[number_vectors*4]=vertex[0];
new_vertexes[number_vectors*4+1]=vertex[1];
new_vertexes[number_vectors*4+2]=vertex[2];
new_vertexes[number_vectors*4+3]=vertex[3];
*(pnumber_vectors)= number_vectors+1;
return number_vectors;
}


static void find_midpoint(float *out,float *a,float *b) {
out[0] = (a[0]+b[0]) * 0.5;
out[1] = (a[1]+b[1]) * 0.5;
out[2] = (a[2]+b[2]) * 0.5;
out[3] = (a[3]+b[3]) * 0.5;
}



static video_planet_data * expand_detail_video_data(video_planet_data *outdat,video_planet_data *dat) {
int new_indexes = dat->number_indexes*4;
int new_vertexes = dat->number_indexes*3;  // max max
video_planet_data *out = outdat;
if ((!out)||(out==dat)||(out->number_indexes != new_indexes)||(out->number_vertexes != new_vertexes)) {
  out = allocate_video_data(new_vertexes,new_indexes);
  }

video_planet_color_info *oldinfo = &dat->color_info;  
video_planet_color_info *newinfo = &out->color_info;  
  
int total_new_vectors = 0;
int total_new_indexes = 0;
int new_ci=0;
int old_ci=0;
for (int i=0;i<dat->number_indexes;i+=3) {
  int j = (i+1);
  int k = (i+2);
  int old_current_v = dat->indexes[i];
  int old_prev_v = dat->indexes[k];
  int old_next_v = dat->indexes[j];
  
  int new_current_v = expand_locate_vertex(&dat->vertexes[old_current_v*4],&total_new_vectors,&out->vertexes[0],out->number_vertexes);
  int new_next_v    = expand_locate_vertex(&dat->vertexes[old_next_v*4],&total_new_vectors,&out->vertexes[0],out->number_vertexes);
  int new_prev_v    = expand_locate_vertex(&dat->vertexes[old_prev_v*4],&total_new_vectors,&out->vertexes[0],out->number_vertexes);
  
  float m1[4];
  find_midpoint(m1,&dat->vertexes[old_current_v*4],&dat->vertexes[old_next_v*4]);
  int new_m1 = expand_locate_vertex(m1,&total_new_vectors,&out->vertexes[0],out->number_vertexes);
  
  float m2[4];
  find_midpoint(m2,&dat->vertexes[old_next_v*4],&dat->vertexes[old_prev_v*4]);
  int new_m2 = expand_locate_vertex(m2,&total_new_vectors,&out->vertexes[0],out->number_vertexes);
  
  float m3[4];
  find_midpoint(m3,&dat->vertexes[old_prev_v*4],&dat->vertexes[old_current_v*4]);
  int new_m3 = expand_locate_vertex(m3,&total_new_vectors,&out->vertexes[0],out->number_vertexes);

  // middle triangle
  int c=total_new_indexes/3;
  out->indexes[total_new_indexes++] = new_m1;
  out->indexes[total_new_indexes++] = new_m2;
  out->indexes[total_new_indexes++] = new_m3;
  newinfo->types[new_ci] = oldinfo->types[old_ci];   // set as before.
  new_ci++;
  
  // top triangle
  out->indexes[total_new_indexes++] = new_m3;
  out->indexes[total_new_indexes++] = new_current_v;
  out->indexes[total_new_indexes++] = new_m1;
  newinfo->types[new_ci] = 4;   // do this later
  new_ci++;

  
  // lower right triangle
  out->indexes[total_new_indexes++] = new_m1;
  out->indexes[total_new_indexes++] = new_next_v;
  out->indexes[total_new_indexes++] = new_m2;
  newinfo->types[new_ci] = 4;   // do this later
  new_ci++;

  
  // lower left  triangle
  out->indexes[total_new_indexes++] = new_m2;
  out->indexes[total_new_indexes++] = new_prev_v;
  out->indexes[total_new_indexes++] = new_m3;
  newinfo->types[new_ci] = 4;   // do this later
  new_ci++;
  
  old_ci++;
  };		

out->number_vertexes = total_new_vectors;
			
explode_colors(out,dat);	
return out;

}



static video_planet_color_info * copy_video_color_info(video_planet_color_info *out,video_planet_color_info *in) {

out->actual_rps = in->actual_rps;
for (int i=0;i<4;i++) {
  out->rps_order[i] = in->rps_order[i];
  for (int j=0;j<5;j++) {
    out->rps_colors[j][i] = in->rps_colors[j][i];
    }
  }

out->color_saturation = in->color_saturation;
out->number_triangles = in->number_triangles;
out->water_ratio = in->water_ratio;

for (int i=0;i<out->number_triangles;i++) {
  out->types[i] = in->types[i];
  }

return out;

}

static void rotation_step(hate_game *game,int planet_id,video_planet *planetg) {
planetg->rotation[0] += planetg->rotation_delta[0];
planetg->rotation[1] += planetg->rotation_delta[1];
planetg->rotation[2] += planetg->rotation_delta[2];

if (planetg->rotation[0]<0.f) {planetg->rotation[0] += 6.283185307f;}
else if (planetg->rotation[0]>6.283185307f) {planetg->rotation[0] -= 6.283185307f;}
if (planetg->rotation[1]<0.f) {planetg->rotation[1] += 6.283185307f;}
else if (planetg->rotation[1]>6.283185307f) {planetg->rotation[1] -= 6.283185307f;}
if (planetg->rotation[2]<0.f) {planetg->rotation[2] += 6.283185307f;}
else if (planetg->rotation[2]>6.283185307f) {planetg->rotation[2] -= 6.283185307f;}

}


static video_planet_data * copy_video_data(video_planet_data *dat) {
int new_indexes = dat->number_indexes;
int new_vertexes = dat->number_vertexes;
video_planet_data *out = allocate_video_data(new_vertexes,new_indexes);

copy_video_color_info(&out->color_info,&dat->color_info);
int i;
for (i=0;i<new_indexes;i++) {
  out->indexes[i] = dat->indexes[i];
  }

fprintf(stderr,"A %d\n",i);
for (i=0;i<new_vertexes*4;i++) {
  out->vertexes[i] = dat->vertexes[i];
  }
fprintf(stderr,"B %d\n",i);

for (i=0;i<new_indexes/3*4;i++) {
  out->colors[i] = dat->colors[i];
  }
fprintf(stderr,"C %d\n",i);

for (i=0;i<new_indexes*4;i++) {
  out->final_vertexes[i] = dat->final_vertexes[i];
  }
fprintf(stderr,"D %d\n",i);

for (i=0;i<new_indexes*4;i++) {
  out->final_colors[i] = dat->final_colors[i];
  }
fprintf(stderr,"E %d\n",i);
  
  

out->expansion1 = dat->expansion1;
out->expansion2 = dat->expansion2;

return out;

}


static void init_rotation(video_planet *planetg) {
planetg->rotation[0] = (float)((unsigned int) (rand()&65535) )*6.28f/65535.f;
planetg->rotation[1] = (float)((unsigned int)(rand()&65535)  )*6.28f/65535.f;
planetg->rotation[2] = (float)((unsigned int)(rand()&65535)  )*6.28f/65535.f;

int first_pick = (rand() % 3);
int second_pick = (rand() &1) + 1;
int third_pick = (first_pick + second_pick + 1) %3;
second_pick = (first_pick + second_pick) %3;

planetg->rotation_delta[first_pick] = (float)( (unsigned int)(rand()&65535+8000))*0.0314f/65535.f + 0.011;
planetg->rotation_delta[second_pick] = planetg->rotation_delta[first_pick] * (0.15 +  (float)((unsigned int)(rand()&65535)))*0.50/65536.f;
planetg->rotation_delta[third_pick] = planetg->rotation_delta[second_pick] * (0.15 +  (float)((unsigned int)(rand()&65535)))*0.50/65536.f;

if (rand()&1) {
  planetg->rotation_delta[first_pick] = -planetg->rotation_delta[first_pick];
  }

  
if (rand()&1) {
  planetg->rotation_delta[second_pick] = -planetg->rotation_delta[second_pick];
  }
if (rand()&1) {
  planetg->rotation_delta[third_pick] = -planetg->rotation_delta[third_pick];
  }

}



//vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvtetrahedron




video_planet the_video_planet_tetrahedron;





static void video_planet_init_tetrahedron(hate_rps actual_rps,int *rps_view,float color_saturation, float *other_color) {
video_planet *planetg = &the_video_planet_tetrahedron;

glGenBuffers(1,&planetg->gl_vertex_buf);
checkGlError("getvertex");
glBindBuffer(GL_ARRAY_BUFFER,planetg->gl_vertex_buf);

planetg->base = allocate_video_data(TETRAHEDRON_VERTEXES,TETRAHEDRON_INDEXES);

for (int i=0;i<planetg->base->number_indexes;i++) {
  planetg->base->indexes[i] = tetrahedronIndices[i];
  }
for (int i=0;i<planetg->base->number_vertexes;i++) {
  planetg->base->vertexes[i*4] = tetrahedronVerts[i][0];
  planetg->base->vertexes[i*4+1] = tetrahedronVerts[i][1];
  planetg->base->vertexes[i*4+2] = tetrahedronVerts[i][2];
  planetg->base->vertexes[i*4+3] = 1.f;
  }

generate_colors(planetg->base,actual_rps,rps_view,color_saturation,other_color);

transfer_vertexes_to_indexes(planetg->base);

video_planet_data *intermediate = expand_detail_video_data(NULL,planetg->base);
video_planet_data *intermediate2 = expand_detail_video_data(NULL,intermediate);
video_planet_data *intermediate3 = expand_detail_video_data(NULL,intermediate2);

planetg->expanded = expand_detail_video_data(NULL,intermediate3);

transfer_vertexes_to_indexes(planetg->expanded);

free_video_data(intermediate);
free_video_data(intermediate2);
free_video_data(intermediate3);

planetg->final = copy_video_data(planetg->expanded);

init_rotation(planetg);

      

glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*4*planetg->final->number_indexes, planetg->final->final_vertexes, GL_DYNAMIC_DRAW);
checkGlError("c");
glVertexAttribPointer(onec->mPositionHandle, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
glEnableVertexAttribArray(onec->mPositionHandle);
checkGlError("d"); 
glBindBuffer(GL_ARRAY_BUFFER, 0);
checkGlError("e");

}




























static void video_planet_step0_tetrahedron(hate_game *game,int planet_id) {
	    
		  
			      
video_planet *planetg = &the_video_planet_tetrahedron;
rotation_step(game,planet_id,planetg);
}


void video_planet_draw0_tetrahedron(hate_game *game,hate_screen *screen,int planet_id) {
int show_planet = screen->planet_choices[planet_id];
video_planet *planetg = &the_video_planet_tetrahedron;
// assuming using onecolor porgaam


        glBindBuffer(GL_ARRAY_BUFFER, planetg->gl_vertex_buf);
        glUniform4f(onec->colorHandle, 0.8f,0.0f,0.0f,1.0f);
    glVertexAttribPointer(onec->mPositionHandle, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
    glEnableVertexAttribArray(onec->mPositionHandle);
    glBufferData(GL_ARRAY_BUFFER, planetg->final->number_indexes*4*sizeof(GLfloat), planetg->final->final_vertexes, GL_STATIC_DRAW);
    glDrawArrays(GL_LINE_STRIP, 0, planetg->final->number_indexes);

    glBindBuffer(GL_ARRAY_BUFFER, 0);


}










video_planet the_video_planet_cube;





void video_planet_init_cube(hate_rps actual_rps,int *rps_view,float color_saturation, float *other_color) {

video_planet *planetg = &the_video_planet_cube;


memset((void *)planetg,0,sizeof(video_planet));

planetg->base = allocate_video_data(CUBE_VERTEXES,CUBE_INDEXES);



glGenBuffers(1,&planetg->gl_vertex_buf);
checkGlError("getvertex");
glBindBuffer(GL_ARRAY_BUFFER,planetg->gl_vertex_buf);


for (int i=0;i<planetg->base->number_indexes;i++) {
  planetg->base->indexes[i] = cubeIndices[i];
  }
for (int i=0;i<planetg->base->number_vertexes;i++) {
  planetg->base->vertexes[i*4] = cubeVerts[i][0];
  planetg->base->vertexes[i*4+1] = cubeVerts[i][1];
  planetg->base->vertexes[i*4+2] = cubeVerts[i][2];
  planetg->base->vertexes[i*4+3] = 1.f;
  }
    

generate_colors(planetg->base,actual_rps,rps_view,color_saturation,other_color);

transfer_vertexes_to_indexes(planetg->base);

video_planet_data *intermediate = expand_detail_video_data(NULL,planetg->base);
video_planet_data *intermediate2 = expand_detail_video_data(NULL,intermediate);
video_planet_data *intermediate3 = expand_detail_video_data(NULL,intermediate2);
planetg->expanded = expand_detail_video_data(NULL,intermediate3);
transfer_vertexes_to_indexes(planetg->expanded);

free_video_data(intermediate);
free_video_data(intermediate2);
free_video_data(intermediate3);

planetg->final = copy_video_data(planetg->expanded);

init_rotation(planetg);

glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*4*planetg->final->number_indexes, planetg->final->final_vertexes, GL_DYNAMIC_DRAW);
checkGlError("c");
glVertexAttribPointer(onec->mPositionHandle, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
glEnableVertexAttribArray(onec->mPositionHandle);
checkGlError("d"); 
glBindBuffer(GL_ARRAY_BUFFER, 0);
checkGlError("e");

}




static void video_planet_step1_cube(hate_game *game,int planet_id) {
video_planet *planetg = &the_video_planet_cube;
rotation_step(game,planet_id,planetg);

}


static void video_planet_draw1_cube(hate_game *game,hate_screen *screen,int planet_id) {
int show_planet = screen->planet_choices[planet_id];
video_planet *planetg = &the_video_planet_cube;
video_planet_data *dat = planetg->final;
// assuming using onecolor porgaam


        glBindBuffer(GL_ARRAY_BUFFER, planetg->gl_vertex_buf);
        glUniform4f(onec->colorHandle, 0.0f,0.8f,0.0f,1.0f);
    glVertexAttribPointer(onec->mPositionHandle, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
    glEnableVertexAttribArray(onec->mPositionHandle);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*4*dat->number_indexes, dat->final_vertexes, GL_DYNAMIC_DRAW);
    glDrawArrays(GL_LINE_STRIP, 0, dat->number_indexes);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
























video_planet the_video_planet_diamond;





void video_planet_init_diamond(hate_rps actual_rps,int *rps_view,float color_saturation, float *other_color) {

video_planet *planetg = &the_video_planet_diamond;


memset((void *)planetg,0,sizeof(video_planet));

planetg->base = allocate_video_data(DIAMOND_VERTEXES,DIAMOND_INDEXES);



glGenBuffers(1,&planetg->gl_vertex_buf);
checkGlError("getvertex");
glBindBuffer(GL_ARRAY_BUFFER,planetg->gl_vertex_buf);


for (int i=0;i<planetg->base->number_indexes;i++) {
  planetg->base->indexes[i] = octahedronIndices[i];
  }
for (int i=0;i<planetg->base->number_vertexes;i++) {
  planetg->base->vertexes[i*4] = octahedronVerts[i][0];
  planetg->base->vertexes[i*4+1] = octahedronVerts[i][1];
  planetg->base->vertexes[i*4+2] = octahedronVerts[i][2];
  planetg->base->vertexes[i*4+3] = 1.f;
  }
    

generate_colors(planetg->base,actual_rps,rps_view,color_saturation,other_color);

transfer_vertexes_to_indexes(planetg->base);

video_planet_data *intermediate = expand_detail_video_data(NULL,planetg->base);
video_planet_data *intermediate2 = expand_detail_video_data(NULL,intermediate);
video_planet_data *intermediate3 = expand_detail_video_data(NULL,intermediate2);
planetg->expanded = expand_detail_video_data(NULL,intermediate3);
transfer_vertexes_to_indexes(planetg->expanded);

free_video_data(intermediate);
free_video_data(intermediate2);
free_video_data(intermediate3);

planetg->final = copy_video_data(planetg->expanded);

init_rotation(planetg);

glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*4*planetg->final->number_indexes, planetg->final->final_vertexes, GL_DYNAMIC_DRAW);
checkGlError("c");
glVertexAttribPointer(onec->mPositionHandle, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
glEnableVertexAttribArray(onec->mPositionHandle);
checkGlError("d"); 
glBindBuffer(GL_ARRAY_BUFFER, 0);
checkGlError("e");

}




static void video_planet_step2_diamond(hate_game *game,int planet_id) {
video_planet *planetg = &the_video_planet_diamond;
rotation_step(game,planet_id,planetg);

}


static void video_planet_draw2_diamond(hate_game *game,hate_screen *screen,int planet_id) {
int show_planet = screen->planet_choices[planet_id];
video_planet *planetg = &the_video_planet_diamond;
video_planet_data *dat = planetg->final;
// assuming using onecolor porgaam


        glBindBuffer(GL_ARRAY_BUFFER, planetg->gl_vertex_buf);
        glUniform4f(onec->colorHandle, 0.0f,0.0f,1.0f,1.0f);
    glVertexAttribPointer(onec->mPositionHandle, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
    glEnableVertexAttribArray(onec->mPositionHandle);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*4*dat->number_indexes, dat->final_vertexes, GL_DYNAMIC_DRAW);
    glDrawArrays(GL_LINE_STRIP, 0, dat->number_indexes);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}



















static video_planet *all_video_planets[5];




void video_planet_init() {
hate_rps actual_rps;
actual_rps.rps[0]=1.f;
actual_rps.rps[1]=1.f;
actual_rps.rps[2]=1.f;
actual_rps.rps[3]=1.f/20.f;
int rps_view[4];
rps_view[0]=0;
rps_view[1]=1;
rps_view[2]=2;
rps_view[3]=3;

float color_saturation=0.f;

float other_color[4];
other_color[0]=1.f;
other_color[1]=1.f;
other_color[2]=0.f;
other_color[3]=1.f;

video_planet_init_tetrahedron(actual_rps,rps_view,color_saturation, other_color);
video_planet_init_cube(actual_rps,rps_view,color_saturation, other_color);
video_planet_init_diamond(actual_rps,rps_view,color_saturation, other_color);


all_video_planets[0] = &the_video_planet_tetrahedron; 
all_video_planets[1] = &the_video_planet_cube; 
all_video_planets[2] = &the_video_planet_diamond; 
all_video_planets[3] = NULL;
all_video_planets[4] = NULL;

}





																			


/* screen number*5+real_player_number */
static int placement[] = {0,1,2,3,4,
                 4,0,1,2,3,
                 3,4,0,1,2,
                 2,3,4,0,1, 
                 1,2,3,4,0};
static float placement_scale = 0.4f;
static float translate_size = 1.33333333f;
static float pos_translate[] = {0.f,0.f,0.f,
                     1.f,1.f,0.f,
                     1.f,-1.f,0.f,
                     -1.f,-1.f,0.f,
                     -1.f,1.f,0.f};



void video_planet_draw(hate_game *game,hate_screen *screen,int planet_id) {
int screen_number = screen->player_id;

//fprintf(stderr,"screen %d	planet %d   plx %d	show %d\n",screen_number,planet_id, placement[screen_number*5+planet_id],screen->planet_choices[planet_id]);
glMatrix r;
loadIdentity(&r);
int show_planet = screen->planet_choices[planet_id];


int  plx = placement[screen_number*5+planet_id];

translateMatrix(&r,translate_size*pos_translate[plx*3],
  translate_size*pos_translate[plx*3+1],
  translate_size*pos_translate[plx*3+2]);
scaleMatrix(&r,placement_scale,placement_scale,placement_scale);
video_planet *pl = all_video_planets[show_planet];
if (pl) {
//  fprintf(stderr,"rotayion %f,%f,%f\n",pl->rotation[0]*360./6.28318530718f,pl->rotation[1]*360./6.28318530718f,pl->rotation[2]*360./6.28318530718f);
  rotationMatrix(&r,pl->rotation[0]*360./6.28318530718f,1.f,0.f,0.f);
  rotationMatrix(&r,pl->rotation[1]*360./6.28318530718f,0.f,1.f,0.f);
  rotationMatrix(&r,pl->rotation[2]*360./6.28318530718f,0.f,0.f,1.f);
  
  }
glMatrix f;
multMatrix(&f,&r,(&onec->MVPMatrix));
glUniformMatrix4fv(onec->mMVPMatrixHandle, 1, GL_FALSE, (GLfloat *)(&f));

if (show_planet==0) {
  video_planet_draw0_tetrahedron(game,screen,planet_id);
  }
else if (show_planet==1) {
  video_planet_draw1_cube(game,screen,planet_id);
  }
else {
  video_planet_draw2_diamond(game,screen,planet_id);
  }
}






void video_planet_step(hate_game *game) {
  video_planet_step0_tetrahedron(game,0);
  video_planet_step1_cube(game,1);
  video_planet_step2_diamond(game,2);
}








