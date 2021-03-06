
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <unistd.h>
       

#include "superpos.h"
#include "helper.h"
#include "onecolor_specs.h"
#include "multicolor_specs.h"
#include "player2.h"
#include "network2.h"
#include <stdio.h>
#include <stdlib.h>


#define GLFW_INCLUDE_ES2
#include <GLFW/glfw3.h>







// silentradiancesimple:
       #include "silentradiancesimple.h"
        #include "simple_packet_summary.h"
        #include <pthread.h>

        #ifndef logit
            #define logit(...) {fprintf(stderr,__VA_ARGS__);fprintf(stderr,"\n");}
        #endif
        #define PACKETMAXSIZE 4000

        static unsigned char packetbuffer[PACKETMAXSIZE];
        static int recvStringLen;




// hibfonts includes and program and shader and sruff

#include "letters_render_opengles2.h"
static lcontext *lc;
static char *gVertexShaderOneColor =
"uniform mat4 u_MVPMatrix;      \n"             // A constant representing the combined model/view/projection matrix.
                   "attribute vec4 vPosition;     \n"           // Per-vertex position information we will pass in.
                   "void main()                    \n"          // The entry point for our vertex shader.
                   "{                              \n"
                   "   gl_PointSize = 1.;         \n"
                   "   gl_Position = u_MVPMatrix   \n"  // gl_Position is a special variable used to store the final position.
                   "    *   vPosition; \n"  // gl_Position is a special variable used to store the final position.
                   "                 \n"     // Multiply the vertex by the matrix to get the final point in                                                                               $
                   "}                              \n";    // normalized screen coordinates.

static char * gFragmentShaderOneColor =
"precision mediump float;\n"
        "uniform vec4 v_Color;\n"
        "void main() {\n"
        "  gl_FragColor = v_Color;\n"
        "}\n";

// WORLD_INTERNATIONAL off
#ifdef _WIN32
const char gStandard3dVertexVaryingColorShader[] =
        "precision mediump float;"
        "uniform mat4 u_mvpMat;"
        "attribute vec4 a_position;"   
        "attribute vec4 a_color;"
        "varying vec4 v_color;"
        "void main()"
        "{"                                 
        "gl_Position = u_mvpMat * a_position;"
        "v_color = a_color;"
        "}";
const char gStandard3dFragmentVaryingColorShader[] =
        "precision mediump float;"
        "varying vec4 v_color;"
        "void main()"
        "{"
        "    gl_FragColor = v_color;"  
        "}";
#else
const char gStandard3dVertexVaryingColorShader[] =
        "precision mediump float;"
        "uniform mat4 u_mvpMat;"
        "attribute vec4 a_position;"
        "attribute vec4 a_color;"
        "varying lowp vec4 v_color;"
        "void main()"
        "{"
        "gl_Position = u_mvpMat * a_position;"
        "v_color = a_color;"
        "}";
const char gStandard3dFragmentVaryingColorShader[] =
        "varying lowp vec4 v_color;"
        "void main()"
        "{"
        "    gl_FragColor = v_color;"
        "}";
#endif      
// WORLD_INTERNATIONAL on


/* borrowed from http://stackoverflow.com/questions/12943164/replacement-for-gluperspective-with-glfrustrum */
static void perspectiveGL( glMatrix *projection,GLfloat fovY, GLfloat aspect, GLfloat zNear, GLfloat zFar )
{
    const GLfloat pi = 3.1415926535897932384626433832795;
    GLfloat fW, fH;

    //fH = tan( (fovY / 2) / 180 * pi ) * zNear;
    fH = tan( fovY / 360.f * pi ) * zNear;
    fW = fH * aspect;

    //glFrustum( -fW, fW, -fH, fH, zNear, zFar );
    frustumMatrix(projection,-fW, fW, -fH, fH, zNear, zFar );
}


onecolor_specs onecs;
onecolor_specs *onec=&onecs;
multicolor_specs multics;
multicolor_specs *multic=&multics;
hate_game  the_hate_game;








static void set_origin_matrix() {
// no effect origin matrix
glMatrix tempMVMatrix;
multMatrix(&tempMVMatrix,&onec->modelMatrix,&onec->viewMatrix);
multMatrix(&onec->origin_matrix,&tempMVMatrix,&onec->projectionMatrix);
}   



static int start_rand() {
struct timespec a;
clock_gettime(CLOCK_REALTIME_COARSE,(struct timespec *)&a);
srand((unsigned int)(a.tv_sec+a.tv_nsec));
}

 

static void activate_matrix_handle(int matrix_handle) {
    glUniformMatrix4fv(matrix_handle,1,0,(GLfloat *)&onec->MVPMatrix);
    // Apply the projection and view transformation
   checkGlError("setmatrix");

}

static void set_matrix_handle(int matrix_handle) {
      
    glMatrix *xmatrix;
    glMatrix stageMatrix;
    if (onec->do_effect) {
      multMatrix(&stageMatrix,&onec->modelMatrix,&onec->effectMatrix);
      xmatrix = &stageMatrix;
      }
    else {  
      xmatrix = &onec->modelMatrix;
      }
    multMatrix(&onec->MVMatrix,xmatrix,&onec->viewMatrix);
    multMatrix(&onec->MVPMatrix,&onec->MVMatrix,&onec->projectionMatrix);
    activate_matrix_handle(matrix_handle);    
    
}       


void set_matrix_translate_handle(int matrix_handle,GLfloat centerx,GLfloat centery) {
    loadIdentity(&onec->modelMatrix);
    translateMatrix(&onec->modelMatrix,centerx,centery,0.f);
set_matrix_handle(matrix_handle);
}          

void set_matrix_scale_translate_handle(int matrix_handle,GLfloat scale,GLfloat centerx,GLfloat centery) {
    loadIdentity(&onec->modelMatrix);
    scaleMatrix(&onec->modelMatrix,scale,scale,scale);
    translateMatrix(&onec->modelMatrix,centerx,centery,0.f);
set_matrix_handle(matrix_handle);
}          
         


static void set_matrix() {
set_matrix_handle(onec->mMVPMatrixHandle);
}
    
    
void set_matrix_translate(GLfloat centerx,GLfloat centery) {
set_matrix_translate_handle(onec->mMVPMatrixHandle,centerx,centery);
}          


  
extern GLint common_get_shader_program(const char *vertex_shader_source, const char *fragment_shader_source);





	

void printGLString(const char *name, GLenum s) {
    const char *v = (const char *) glGetString(s);
    logit("GL %s = %s\n", name, v);
}

#ifdef NDEBUG
int checkGlErrorReal(const char* op) {
#else
int checkGlError(const char* op) {
#endif
int eflag=0;
    for (GLint error = glGetError(); error; error  = glGetError()) {
        eflag=1;
#define WORLD_display_helper_ ""
        char *s = WORLD_display_helper_;
#define WORLD_GL_INAVLID_ENUM "GL_INAVLID_ENUM"
        if  (error==GL_INVALID_ENUM) {s=WORLD_GL_INAVLID_ENUM;}
#define WORLD_GL_INVALID_VALUE "GL_INVALID_VALUE"
        else if (error==GL_INVALID_VALUE) {s=WORLD_GL_INVALID_VALUE;}
#define WORLD_GL_INVALID_OPERATION "GL_INVALID_OPERATION"
        else if (error==GL_INVALID_OPERATION) {s=WORLD_GL_INVALID_OPERATION;}
#define WORLD_GL_INVALID_FRAMEBUFFER_OPERATION "GL_INVALID_FRAMEBUFFER_OPERATION"
        else if (error==GL_INVALID_FRAMEBUFFER_OPERATION) {s=WORLD_GL_INVALID_FRAMEBUFFER_OPERATION;}
#define WORLD_GL_OUT_OF_MEMORY "GL_OUT_OF_MEMORY"
        else if (error==GL_OUT_OF_MEMORY) {s = WORLD_GL_OUT_OF_MEMORY;}
        logit("after %s() glError %s(0x%x)\n", op, s,error);
    }
return eflag;
}



int checkGlError3(const char *filename,int lineno,const char* op) {
char buf[4000];
sprintf(buf,"%s:%d: %s",filename,lineno,op);
return checkGlError(buf);
}

int setup_graphics_for_letters_and_robots(int participant_id,int w,int h) {
init_onecolor_specs(onec);
init_multicolor_specs(multic,onec);

    loadIdentity(&onec->modelMatrix);
    loadIdentity(&onec->effectMatrix);
    loadIdentity(&onec->viewMatrix);
    loadIdentity(&onec->projectionMatrix);
        
    set_origin_matrix(); // works for onec and multic
    fprintf(stderr,"calling player2_game_init\n");
    player2_game_init(&the_hate_game,participant_id); // that is out player id

    loadIdentity(&onec->projectionMatrix);
    perspectiveGL(&onec->projectionMatrix,45.f, ((GLfloat)w)/((GLfloat)h),  -20.f, 20.f);
    
    
    fprintf(stderr,"done player2_game_init\n");
          
    onec->one_color_program = common_get_shader_program(gVertexShaderOneColor,gFragmentShaderOneColor);
    onec->mMVPMatrixHandle = glGetUniformLocation(onec->one_color_program, "u_MVPMatrix");
      if (checkGlError("glGetAttribLocation")) {
        fprintf(stderr,"Could not create matrix handle.\n");
        return 0;
	 }
    onec->gvPositionHandle = glGetAttribLocation(onec->one_color_program, "vPosition");

      if (    checkGlError("glGetAttribLocation")) {
        fprintf(stderr,"Could not create gv position handle.\n");
        return 0;
    }
    // get handle to fragment shader's v_Color member
    onec->colorHandle = glGetUniformLocation(onec->one_color_program, "v_Color");

    if (checkGlError("glGetAttribLocation")) {
        fprintf(stderr,"Could not locqtuib color handle.\n");
        return 0;
    }
    onec->mPositionHandle = glGetAttribLocation(onec->one_color_program, "vPosition");
    if (checkGlError("glsasGetAttribLocation")) {
        fprintf(stderr,"Could not position inn mPsoitiontable handle.\n");
        return 0;
    }
    glUseProgram(onec->one_color_program);
    checkGlError("useprogram");
    
    // letters init
    set_matrix();    
    lc = linit_context();
    letters_opengles_setgraphics(onec->one_color_program,
                      onec->colorHandle,onec->gvPositionHandle,onec->mPositionHandle); // needs to be set up once, with a single color program
      //once amonst all the drivers
    
    
   // now for multicolor
    multic->multi_color_program = common_get_shader_program(gStandard3dVertexVaryingColorShader,gStandard3dFragmentVaryingColorShader);
    multic->mMVPMatrixHandle = glGetUniformLocation(multic->multi_color_program, "u_mvpMat");
      if (checkGlError("glGetAttribLocation")) {
        fprintf(stderr,"Could not create matrix handle.\n");
        return 0;
	 }
    multic->gvPositionHandle = glGetAttribLocation(multic->multi_color_program, "a_position");

      if (    checkGlError("glGetAttribLocation")) {
        fprintf(stderr,"Could not create gv position handle.\n");
        return 0;
    }
    // get handle to fragment shader's v_Color member
    multic->gvColorHandle = glGetAttribLocation(multic->multi_color_program, "a_color");

    if (checkGlError("glGetAttribLocation")) {
        fprintf(stderr,"Could not locqtuib color attrub handle.\n");
        return 0;
    }
    glUseProgram(multic->multi_color_program);
    checkGlError("useprogram");
    activate_matrix_handle(multic->gvPositionHandle);
    glDisable(GL_CULL_FACE);
   
    glUseProgram(onec->one_color_program);
    glDisable(GL_CULL_FACE);
    checkGlError("useprogram");
    

    return 1;
}
    




// hibfonts stuff ^^^




// Silentradiance simple stuff:
        void *DoSound(void *arg) {
            init_receiver_sound(1);
            while (1) {
                playOneSoundBuffer();
            }
            return NULL;
        }

        void *DoSilentRadiance(void *arg) {
            while (1) {
                if(we_are_streaming_web==-1) {

                } else {
                    if (get_packet_from_web_stream(&recvStringLen,packetbuffer)) {
                        process_packet(recvStringLen,packetbuffer);
                        while (get_packet_from_web_stream(&recvStringLen,packetbuffer)) {
                            process_packet(recvStringLen,packetbuffer);
                        }
                    }
                }
            }
        }

        void InitSilentRadiance(char *url) {
            init_web_stream(url);
            init_processor();
            {
                pthread_t tid[1];
                int err;
                err = pthread_create(tid,NULL,&DoSilentRadiance,NULL);
                if (err != 0) {logit("\ncan't create thread :[%s]", strerror(err))};
            }
            {
                pthread_t tid[1];
                int err;
                err = pthread_create(tid,NULL,&DoSound,NULL);
                if (err != 0) {logit("\ncan't create thread :[%s]", strerror(err))};
            }
        }
// ^^^ silentradiance simple stuff



static const GLfloat svertices[] = {
     0.0f,  0.2f, 0.0f,
     0.2f, -0.2f, 0.0f,
    -0.2f, -0.2f, 0.0f,
     0.0f,  0.2f, 0.0f,
     0.2f, -0.2f, 0.0f,
    -0.2f, -0.2f, 0.0f,
     0.0f,  0.2f, 0.0f,
     0.2f, -0.2f, 0.0f,
    -0.2f, -0.2f, 0.0f,
     0.0f,  0.2f, 0.0f,
     0.2f, -0.2f, 0.0f,
    -0.2f, -0.2f, 0.0f,
     0.0f,  0.2f, 0.0f,
     0.2f, -0.2f, 0.0f,
    -0.2f, -0.2f, 0.0f,
     0.0f,  0.2f, 0.0f,
     0.2f, -0.2f, 0.0f,
    -0.2f, -0.2f, 0.0f,
     0.0f,  0.2f, 0.0f,
     0.2f, -0.2f, 0.0f,
    -0.2f, -0.2f, 0.0f,
     0.0f,  0.2f, 0.0f,
     0.2f, -0.2f, 0.0f,
    -0.2f, -0.2f, 0.0f,
     0.0f,  0.2f, 0.0f,
     0.2f, -0.2f, 0.0f,
    -0.2f, -0.2f, 0.0f,
     0.0f,  0.2f, 0.0f,
     0.2f, -0.2f, 0.0f,
    -0.2f, -0.2f, 0.0f,
};
 static GLfloat vertices[] = {
     0.0f,  0.2f, 0.0f,
     0.2f, -0.2f, 0.0f,
    -0.2f, -0.2f, 0.0f,
     0.0f,  0.2f, 0.0f,
     0.2f, -0.2f, 0.0f,
    -0.2f, -0.2f, 0.0f,
     0.0f,  0.2f, 0.0f,
     0.2f, -0.2f, 0.0f,
    -0.2f, -0.2f, 0.0f,
     0.0f,  0.2f, 0.0f,
     0.2f, -0.2f, 0.0f,
    -0.2f, -0.2f, 0.0f,
     0.0f,  0.2f, 0.0f,
     0.2f, -0.2f, 0.0f,
    -0.2f, -0.2f, 0.0f,
     0.0f,  0.2f, 0.0f,
     0.2f, -0.2f, 0.0f,
    -0.2f, -0.2f, 0.0f,
     0.0f,  0.2f, 0.0f,
     0.2f, -0.2f, 0.0f,
    -0.2f, -0.2f, 0.0f,
     0.0f,  0.2f, 0.0f,
     0.2f, -0.2f, 0.0f,
    -0.2f, -0.2f, 0.0f,
     0.0f,  0.2f, 0.0f,
     0.2f, -0.2f, 0.0f,
    -0.2f, -0.2f, 0.0f,
     0.0f,  0.2f, 0.0f,
     0.2f, -0.2f, 0.0f,
    -0.2f, -0.2f, 0.0f,
};
 static GLfloat vertices2[] = {
     0.0f,  0.2f, 0.0f,
     0.2f, -0.2f, 0.0f,
    -0.2f, -0.2f, 0.0f,
     0.0f,  0.2f, 0.0f,
     0.2f, -0.2f, 0.0f,
    -0.2f, -0.2f, 0.0f,
     0.0f,  0.2f, 0.0f,
     0.2f, -0.2f, 0.0f,
    -0.2f, -0.2f, 0.0f,
     0.0f,  0.2f, 0.0f,
     0.2f, -0.2f, 0.0f,
    -0.2f, -0.2f, 0.0f,
     0.0f,  0.2f, 0.0f,
     0.2f, -0.2f, 0.0f,
    -0.2f, -0.2f, 0.0f,
     0.0f,  0.2f, 0.0f,
     0.2f, -0.2f, 0.0f,
    -0.2f, -0.2f, 0.0f,
     0.0f,  0.2f, 0.0f,
     0.2f, -0.2f, 0.0f,
    -0.2f, -0.2f, 0.0f,
     0.0f,  0.2f, 0.0f,
     0.2f, -0.2f, 0.0f,
    -0.2f, -0.2f, 0.0f,
     0.0f,  0.2f, 0.0f,
     0.2f, -0.2f, 0.0f,
    -0.2f, -0.2f, 0.0f,
     0.0f,  0.2f, 0.0f,
     0.2f, -0.2f, 0.0f,
    -0.2f, -0.2f, 0.0f,
};

const  static GLfloat lines[] = {
     -0.99f,  -0.3333f,0.f,
      0.99f,-0.3333f,0.f,
      0.99f,  -0.99f,0.f,
      0.3333f,-0.99f,0.f,
      0.3333f,  0.99f,0.f,
      0.99f,    0.99f,0.f,
     0.99f,0.3333f,0.f,
     -0.99f,0.3333f,0.f,
     -0.99f,0.99f,0.f,
     -0.3333f,0.99f,0.f,
     -0.3333f,-0.99f,0.f,
     -0.99f,-0.99f,0.f,
     -0.99f,  -0.3333f,0.f,
      0.99f,-0.3333f,0.f,
};

const  static GLfloat boxlines[] = {
     -0.99f,  -0.99f,0.f,
      0.99f,-0.99f,0.f,
      0.99f,  0.99f,0.f,
      -0.99f,0.99f,0.f,
     -0.99f,  -0.99f,0.f,
};

GLint common_get_shader_program(const char *vertex_shader_source, const char *fragment_shader_source) {
    enum Consts {INFOLOG_LEN = 512};
    GLchar infoLog[INFOLOG_LEN];
    GLint fragment_shader;
    GLint shader_program;
    GLint success;
    GLint vertex_shader;

    /* Vertex shader */
    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_source, NULL);
    glCompileShader(vertex_shader);
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertex_shader, INFOLOG_LEN, NULL, infoLog);
        printf("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n%s\n", infoLog);
    }

    /* Fragment shader */
    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_source, NULL);
    glCompileShader(fragment_shader);
    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragment_shader, INFOLOG_LEN, NULL, infoLog);
        printf("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n%s\n", infoLog);
    }

    /* Link shaders */
    shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);
    glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shader_program, INFOLOG_LEN, NULL, infoLog);
        printf("ERROR::SHADER::PROGRAM::LINKING_FAILED\n%s\n", infoLog);
    }

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
    return shader_program;
}

double dist=2.;

GLfloat *old[5];
GLfloat *neww[5];

GLfloat voo[15][5][5];
GLfloat vnn[15][5][5];

GLfloat deltas[3*5];
double oldx,oldy;
double newx,newy;

GLfloat *deltaz[5]={deltas,deltas+3,deltas+6,deltas+9,deltas+12};

static float changex[5];
static float changey[5];

int step_init () {
dist=2.;

}



static  int count=0;

static  double offsetx=0.;
static double offsety=0.;
static double offsetz=0.;
static  double offx=0.;
static double offy=0.;
static double offz=0.;


int running=1;






  
int step() {      




  player2_game_step(&the_hate_game);
  
      

}



network2_complete network2_abc;
char sent_buffers[NUMBER_OF_NETWORK1_PARTICIPANTS][2000];
char received_buffers[NUMBER_OF_NETWORK1_PARTICIPANTS][2000];
char stdin_buffer[2000];



void received_stdin_in1(struct network2_complete *c,int i, int n) {
volatile struct network2_complete *cv = (struct network2_complete *)c;
if (cv->poll_state[i]==5) {
  for (int j=0;j<cv->buflen[i];j++) {
    int ch = cv->buffers[i][j];
     fprintf(stderr,"   %c\n",ch);
    if ((ch==0) ||(ch == EOF)) {}
    else if (ch==3) running=0;
//    else if (ch=='~') {
//      broadcast = !broadcast;
//      }
    }
  }
cv->buflen[i]=0;
cv->poll_state[i]=3;
cv->call_rounds[i]=0;
}

void received_do_in1(struct network2_complete *c,int i, int n) {
volatile struct network2_complete *cv = (struct network2_complete *)c;
fprintf(stderr,"do_in1 state is %d\b",c->poll_state[i]);
int communicator = c->communicator[i];
if (c->poll_state[i]>=6) {
  fprintf(stderr,"xxx");
}
else if (c->poll_state[i]==5) {
  c->buffers[i][c->buflen[i]]='\0';
//  strcat(received_buffers[communicator],c->buffers[i]);
//  if (strlen(received_buffers[communicator])>60) {
//    strcpy(received_buffers[communicator], received_buffers[communicator] +strlen(received_buffers[communicator])-60);
//    }
//  cv->buflen[i]=0;
//  cv->poll_state[i]=3;
//  }
// act on command 
  }
cv->buflen[i]=0;
cv->poll_state[i]=3;
cv->call_rounds[i]=0;
}



void sent_do_out1(struct network2_complete *c,int i, int n) {
fprintf(stderr,"                        out1\n\n");
volatile struct network2_complete *cv = (struct network2_complete *)c;
 
if (cv->poll_state[i]>=6) {
  fprintf(stderr,"xxx1");
}
else if (cv->poll_state[i]==5) {
int communicator = cv->communicator[i];
//  cv->buffers[i][cv->buflen[i]]='\0';
//  strcat(sent_buffers[communicator],c->buffers[i]);
//  if (strlen(sent_buffers[communicator])>60) {
//    strcpy(sent_buffers[communicator], sent_buffers[communicator] +strlen(sent_buffers[communicator])-60);
//    }
//  fprintf(stderr,"w");
  }
cv->buflen[i]=0;
cv->buffers[i][c->buflen[i]]='\0';
cv->poll_state[i]=3;  
cv->call_rounds[i]=0;
}







// repeated in video_planets
/* screen number*5+real_player_number */
static int placement[] = {0,1,2,3,4,
                 4,0,1,2,3,
                 3,4,0,1,2,
                 2,3,4,0,1,
                 1,2,3,4,0};
static float pos_translate[] = {0.f,0.f,0.f,
                     1.f,1.f,0.f,
                     1.f,-1.f,0.f,
                     -1.f,-1.f,0.f,
                     -1.f,1.f,0.f};


void draw_multiscreen_grid_and_planets(GLuint vboframe,float *this_foreground) {
hate_game *game = &the_hate_game;

        glUseProgram(onec->one_color_program);
	checkGlError("useprogram");
	
	
        glUniform4f(onec->colorHandle, this_foreground[0],this_foreground[1],this_foreground[2],this_foreground[3]);
    loadIdentity(&onec->viewMatrix);
    loadIdentity(&onec->modelMatrix);
	
    set_matrix();    
	checkGlError("mat2");


 

        glBindBuffer(GL_ARRAY_BUFFER, vboframe);	
        glUniform4f(onec->colorHandle, 0.8f,1.0f,1.0f,1.0f);
    glVertexAttribPointer(onec->mPositionHandle, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
   glEnableVertexAttribArray(onec->mPositionHandle);
        glBufferData(GL_ARRAY_BUFFER, sizeof(lines), lines, GL_STATIC_DRAW);
        glDrawArrays(GL_LINE_STRIP, 0, 12);

	

        lc->needed_points->line_width=0.0428f;
        glBindBuffer(GL_ARRAY_BUFFER, 0);	
	
 
	for (int screen_numberx=0;screen_numberx<5;screen_numberx++) {
          hate_screen *screen = &game->screens.screens[screen_numberx];
	  
	  
	    onec->viewMatrix = screen->screenViewMatrix;


         multMatrix(&onec->MVPMatrix,&onec->viewMatrix,&onec->projectionMatrix);
         glUniformMatrix4fv(onec->mMVPMatrixHandle, 1, GL_FALSE, (GLfloat *)(&onec->MVPMatrix));

	  
  	  for (int planet_id=0;planet_id<5;planet_id++) {
 	    int central_planet_id = screen->central_planet_id;
	    int  plx = placement[central_planet_id*5+planet_id];
	
	    float d=1.3333333333f;
	    float o=0.2f;
	    float w=0.16f;
  	    float zdiff = 0.0f;
	    char xx[2];
	    xx[0]='0' + planet_id;
	    xx[1]='\0';
            letters_out(lc,w,d*pos_translate[plx*3]-o,
	                   d*pos_translate[plx*3+1]-o,zdiff,xx);
            }
	  }
	
        glUseProgram(onec->one_color_program);
	checkGlError("useprogram");
}



void draw_singlescreen_grid_and_planets(GLuint vboboxframe,float *this_foreground) {
hate_game *game = &the_hate_game;
hate_screen *screen = &game->screens.screens[0];
        glUseProgram(onec->one_color_program);
	checkGlError("useprogram");
        glUniform4f(onec->colorHandle, this_foreground[0],this_foreground[1],this_foreground[2],this_foreground[3]);
    loadIdentity(&onec->viewMatrix);
    loadIdentity(&onec->modelMatrix);
	
    set_matrix();    
	checkGlError("mat2");



 

        glBindBuffer(GL_ARRAY_BUFFER, vboboxframe);	
        glUniform4f(onec->colorHandle, 0.8f,1.0f,1.0f,1.0f);
    glVertexAttribPointer(onec->mPositionHandle, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
   glEnableVertexAttribArray(onec->mPositionHandle);
        glBufferData(GL_ARRAY_BUFFER, sizeof(boxlines), boxlines, GL_STATIC_DRAW);
        glDrawArrays(GL_LINE_STRIP, 0, 5);

	

        lc->needed_points->line_width=0.0428f*3.5f;
        glBindBuffer(GL_ARRAY_BUFFER, 0);	
	
	for (int planet_id=0;planet_id<5;planet_id++) {
 	  int central_planet_id = screen->central_planet_id;
	  int  plx = placement[central_planet_id*5+planet_id];
	
	  float d=0.6666666666666666f;
	  float o=0.1f;
	  float w=0.16f;
	  float zdiff = 0.0f;
	  char xx[2];
	  xx[0]='0' + planet_id;
	  xx[1]='\0';
          letters_out(lc,w,d*pos_translate[plx*3]-o,
	                   d*pos_translate[plx*3+1]-o,zdiff,xx);
	  zdiff = -zdiff;
          }
        glUseProgram(onec->one_color_program);
	checkGlError("useprogram");
return;
}




void usage(int argc,char *argv[]) {
  fprintf(stderr," Usage: %s ip0 ip1 ip2 ip3 ip4 ip5 player_id\r\n",argv[0]);
  }

int main(int argc,char *argv[]) {
    GLuint vbo,vboframe,vboboxframe;
    
    if (argc!=8) {
      usage(argc,argv);
        exit(-1);
	  }



//???system("stty raw -echo");


start_rand();

int participant_number = participant_number % 6;
participant_number = argv[7][0]-'0';
GLuint  screen_width = 2100;
GLuint  screen_height = 2100;

	  
if (participant_number==5) {
  screen_width = 2100;
  screen_height = 2100;
  }
else {
  screen_width = 1024;
  screen_height = 1024;
  }

    GLFWwindow* window;
        InitSilentRadiance("https://pt.silentradiance.com/download.cgi");
    step_init();
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    window = glfwCreateWindow(screen_width, screen_height, __FILE__, NULL, NULL);
    glfwMakeContextCurrent(window);

//    printf("GL_VERSION  : %s\n", glGetString(GL_VERSION) );
//    printf("GL_RENDERER : %s\n", glGetString(GL_RENDERER) );
 

    

    setup_graphics_for_letters_and_robots(participant_number, screen_width, screen_height);	// this has a matrix to do 3d model, effect, etc

//    glEnable(GL_DEPTH_TEST);

    glClearColor(0.0f, 0.0f, 0.2f, 1.0f);
 //   glClear( GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    
    glViewport(0, 0, screen_width, screen_height);
	checkGlError("vp");

    glGenBuffers(1, &vbo);
	checkGlError("a");
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
	checkGlError("b");
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
	checkGlError("c");
    glVertexAttribPointer(onec->mPositionHandle, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
   glEnableVertexAttribArray(onec->mPositionHandle);
	checkGlError("d");
	
    glBindBuffer(GL_ARRAY_BUFFER, 0);
	checkGlError("e");
    
    glGenBuffers(1, &vboframe);
	checkGlError("f");
    glBindBuffer(GL_ARRAY_BUFFER, vboframe);
	checkGlError("g");
    glBufferData(GL_ARRAY_BUFFER, sizeof(lines), lines, GL_STATIC_DRAW);
	checkGlError("h");
    glVertexAttribPointer(onec->mPositionHandle, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
   glEnableVertexAttribArray(onec->mPositionHandle);
	checkGlError("i");
    glBindBuffer(GL_ARRAY_BUFFER, 0);
	checkGlError("j");

    glGenBuffers(1, &vboboxframe);
	checkGlError("f");
    glBindBuffer(GL_ARRAY_BUFFER, vboboxframe);
	checkGlError("g");
    glBufferData(GL_ARRAY_BUFFER, sizeof(lines), lines, GL_STATIC_DRAW);
	checkGlError("h");
    glVertexAttribPointer(onec->mPositionHandle, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
   glEnableVertexAttribArray(onec->mPositionHandle);
	checkGlError("i");
    glBindBuffer(GL_ARRAY_BUFFER, 0);
	checkGlError("j");


    player2_init_graphics(&the_hate_game);

    network2_complete *c2 = &network2_abc;
    network2_init(c2,participant_number,"255.255.255.255",&(argv[1]),
       NULL,&(received_do_in1),&(sent_do_out1),NULL,
          NULL,NULL,NULL,NULL,
	     NULL,NULL,NULL,NULL,
	        NULL,NULL,NULL,
		   NULL,NULL,NULL,
		       NULL,NULL,NULL);
		       
    
   int stdin_port = network2_add_standard_input_fd(c2,fileno(stdin),&(received_stdin_in1),NULL,NULL,NULL);






while (!glfwWindowShouldClose(window)) {
      glfwPollEvents();
        


// silentradiance vvv
      compute_packet_summary();
      struct packet *p = NULL; // pointer to packet
      // find current packet
      if (packet_summary.now_frame != 1)
        {
        int index = (packet_summary.start_index + packet_summary.now_frame - packet_summary.start_frame) % PACKET_SUMMARY_SIZE;
        if (index<0) index += PACKET_SUMMARY_SIZE;
        p = packet_summary.packets+index;
        if (p->has_statistics == 0) p = NULL;
        }

			
    float this_foreground[4];
    if ((packet_summary.commanded_background_color[0] != 0.f)  ||
                (packet_summary.commanded_background_color[1] != 0.f)  ||
                (packet_summary.commanded_background_color[2] != 0.f))
            {  // if the dj sets a color, use that as a foreground

                this_foreground[0] =(packet_summary.commanded_background_color[0]);
                this_foreground[1] = (packet_summary.commanded_background_color[1]);
                this_foreground[2] = (packet_summary.commanded_background_color[2]);
                this_foreground[3] = 1.0f;
            }
            else
            {
                this_foreground[0] =0.8f;
                this_foreground[1] =0.7f;
                this_foreground[2] =0.3f;
                this_foreground[3] =1.0f;
		
            }




									                        
      if (p) {


	
        if (p->has_beat)
          {
                this_foreground[0] =0.8f;
                this_foreground[1] =0.8f;
                this_foreground[2] =0.8f;
                this_foreground[3] =1.0f;  
		fprintf(stderr,"X");
          }
        else if (p->has_onset)
          {
                this_foreground[0] =0.9f;
                this_foreground[1] =0.8f;
                this_foreground[2] =0.5f;
                this_foreground[3] =1.0f;
		fprintf(stderr,".");
           }
                // pitch is p->pitch
                // db is a rough db level - just 4 levels
                // p->folded flas is flags like beat, onset, but folded over a few packets to better match with the video frame rate
                //radianceData.beatScale = 1 + p->pitch/10000;
        } // if p


      int result =  network2_poll_check(c2);
      if (result==0) {
        }

// silentradiance ^^^^^

	step();

	
		
	checkGlError("v3clear");
        glUseProgram(onec->one_color_program);
			
        glClear(GL_COLOR_BUFFER_BIT);
	
	checkGlError("clear");
	
/*
          network2_path_new_command(
              c,pending_send[to_player],l,NETWORK2_PATH_BROADCAST_ALLACK_ALL,c->participant_number,to_player,-1);
           pending_send[to_player][0]='\0';


*/

	
  if (participant_number==5) {
    draw_multiscreen_grid_and_planets(vboframe,this_foreground);	
    }
  else {
    draw_singlescreen_grid_and_planets(vboboxframe,this_foreground);
    }	    
	    
             
	player2_game_draw(&the_hate_game);
	
        glfwSwapBuffers(window);
    }
    glDeleteBuffers(1, &vbo);
    glfwTerminate();
    
    
    
    
system("stty sane");
    
    return EXIT_SUCCESS;
}







