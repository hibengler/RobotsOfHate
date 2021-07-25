
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



static GLuint do_effect=0;
static GLuint one_color_program=0;
static GLuint gvPositionHandle=0;
static GLint mPositionHandle=0;
static GLint mMVPMatrixHandle=0;

static glMatrix modelMatrix;
static glMatrix effectMatrix; /* used to do effects like zoom near/zoom away */
static glMatrix viewMatrix;
static glMatrix projectionMatrix;
static glMatrix MVMatrix;
static glMatrix MVPMatrix; // effective matrix for the menu - might be spinning in space.

glMatrix origin_matrix;  // matrix tha makes x from -1 to 1, y from -1 to 1.  X or y might be bigger depending on orientation


static int colorHandle;   




static void multMatrix(glMatrix *result, glMatrix *srcA, glMatrix *srcB)
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



static void set_origin_matrix() {
// no effect origin matrix
glMatrix mvmatrix;
multMatrix(&mvmatrix,&modelMatrix,&viewMatrix);
multMatrix(&origin_matrix,&mvmatrix,&projectionMatrix);
}   


static void set_matrix() {
      
    glMatrix *xmatrix;
    glMatrix stageMatrix;
    if (do_effect) {
      multMatrix(&stageMatrix,&modelMatrix,&effectMatrix);
      xmatrix = &stageMatrix;
      }
    else {  
      xmatrix = &modelMatrix;
      }
    multMatrix(&MVMatrix,xmatrix,&viewMatrix);
    multMatrix(&MVPMatrix,&MVMatrix,&projectionMatrix);
    
    
    // Apply the projection and view transformation
    glUniformMatrix4fv(mMVPMatrixHandle, 1, GL_FALSE, (GLfloat *)(&MVPMatrix));
   checkGlError("setmatrix");
}       
    
    
static void loadIdentity(glMatrix *result)
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
    
    
static void set_matrix_translate(GLfloat centerx,GLfloat centery) {
    loadIdentity(&modelMatrix);
    translateMatrix(&modelMatrix,centerx,centery,0.f);
set_matrix();
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

int setup_graphics_for_letters_and_robots() {

    loadIdentity(&modelMatrix);
    loadIdentity(&effectMatrix);
    loadIdentity(&viewMatrix);
    loadIdentity(&projectionMatrix);
        
    set_origin_matrix();
          
    one_color_program = common_get_shader_program(gVertexShaderOneColor,gFragmentShaderOneColor);
    mMVPMatrixHandle = glGetUniformLocation(one_color_program, "u_MVPMatrix");
      if (checkGlError("glGetAttribLocation")) {
        fprintf(stderr,"Could not create matrix handle.\n");
        return 0;
    }
    gvPositionHandle = glGetAttribLocation(one_color_program, "vPosition");

      if (    checkGlError("glGetAttribLocation")) {
        fprintf(stderr,"Could not create gv position handle.\n");
        return 0;
    }
    // get handle to fragment shader's v_Color member
    colorHandle = glGetUniformLocation(one_color_program, "v_Color");

    if (checkGlError("glGetAttribLocation")) {
        fprintf(stderr,"Could not locqtuib color handle.\n");
        return 0;
    }
    mPositionHandle = glGetAttribLocation(one_color_program, "vPosition");
    if (checkGlError("glsasGetAttribLocation")) {
        fprintf(stderr,"Could not position inn mPsoitiontable handle.\n");
        return 0;
    }
    glUseProgram(one_color_program);
    checkGlError("useprogram");
    
    // letters init
    set_matrix();    
    lc = linit_context();
    letters_opengles_setgraphics(one_color_program,colorHandle,gvPositionHandle,mPositionHandle); // needs to be set up once, with a single color program
      //once amonst all the drivers
    
           

    return 1;
}
    



// hibfonts stuff ^^^







static const GLuint WIDTH = 1600;
static const GLuint HEIGHT = 1600;


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

double dist=0.66666666666;

GLfloat *old[5];
GLfloat *neww[5];

GLfloat voo[15][5][5];
GLfloat vnn[15][5][5];

GLfloat deltas[3*5];
double oldx,oldy;
double newx,newy;

GLfloat *deltaz[5]={deltas,deltas+3,deltas+6,deltas+9,deltas+12};


int step_init () {
GLfloat *olds= vertices2;
GLfloat *news= vertices;

unsigned long dfactor;
dfactor=3l;
deltaz[0]=deltas+0l*dfactor;
deltaz[1]=deltas+1l*dfactor;
deltaz[2]=deltas+2l*dfactor;
deltaz[3]=deltas+3l*dfactor;
deltaz[4]=deltas+4l*dfactor;

unsigned long factor = 3l * 3l; /* floats per point * points * number of triangles */
old[0]=olds;
old[1]=olds+1l*factor;
old[2]=olds+2l*factor;
old[3]=olds+3l*factor;
old[4]=olds+4l*factor;
neww[0]=news;
neww[1]=news+1l*factor;
neww[2]=news+2l*factor;
neww[3]=news+3l*factor;
neww[4]=news+4l*factor;
for (int i=0;i<3*3*10;i++) {
  vertices[i]=svertices [i]* 0.1;
  vertices2[i]=svertices [i]* 0.1;
  }
oldx=0.;
oldy=0.;
newx=0.;
newy=0.;


}



static  int count=0;

static  double offsetx=0.;
static double offsety=0.;
static double offsetz=0.;
static  double offx=0.;
static double offy=0.;
static double offz=0.;

  
int step() {      


dist=0.6666666666666666;

int factor=9;

{
  int i,j;
  for (i=0;i<5;i++) {
    for(j=0;j<9;j++) {
      neww[i][j]=old[i][j];
      }
    }
  }


int context=0;    
{ 
  int x,y,z;
  while (1) {
    if ((count&7)==0) {
//      fprintf(stderr,"r");
      x = rand();
      y = rand();
      z = rand();
      offx=( ((double)(x % 50)) )-25.;
      offy=( ((double)(y % 50)) )-25.;
      offz=( ((double)(z % 50)) )-25.;
      offx = offx/1500.;
      offy=offy/1500.;
      offz=offz/1500.;
      offz = 0.;
      }
    count++;
    if ((neww[context][0]+offsetx+offx>0.300)||(neww[context][0]+offsetx+offx<= -0.300)) {count=0;continue;}
    if ((neww[context][1]+offsety+offy>0.300)||(neww[context][1]+offsety+offy<= -0.300)) {count=0;continue;}
    if ((neww[context][2]+offz>0.300)||(neww[context][2]+offz<= -0.300)) {count=0;continue;}
    offsetx += offx;
    offsety += offy;
    offsetz += offz;

    for (unsigned long i=0;i<9;i+=3) {
      neww[context][i+0] +=  offsetx;
      neww[context][i+1] +=  offsety;
      neww[context][i+2] +=  offsetz;
      }
    break;
    }
  
  


  
for (int newi=0;newi<3;newi++) {
  fprintf(stderr,"newi %d\n",newi);
  unsigned long offset = 3*newi;
  
  super_point p = (super_point){xyz:{neww[0][offset],neww[0][offset+1],neww[0][offset+2]}};  
  for (unsigned long i=0;i<5;i++) {
    super_point p1 = p;

    super_point p2 = xyz_from_context_to_context(context,dist,
	                          p1,
				                          i);
    fprintf(stderr,"suerpoint  %f,%f to %f,%f \n\n",p1.xyz[0],p1.xyz[1],p2.xyz[0],p2.xyz[1]);
    p2.xyz[0] += changex[i];							    
    p2.xyz[1] += changey[i];							    
//    fprintf(stderr,"context %ld from %f,%f to %f,%f with changey %f\n",i,p.xyz[0],p.xyz[1],p1.xyz[0],p1.xyz[1],changey[i]);
    
    neww[i][offset] = p2.xyz[0];
    neww[i][offset+1] = p2.xyz[1];
    neww[i][offset+2] = p2.xyz[2];
    }
  }
  
/*

  fprintf(stderr,
"p0	%f,%f %f,%f	 %f,%f	%f,%f	%f,%f -->\n"
,old[0][0],old[0][1],old[1][0],old[1][1],old[2][0],old[2][1],old[3][0],old[3][1],old[4][0],old[4][1]);
  fprintf(stderr,
"p0n	%f,%f %f,%f	 %f,%f	%f,%f	%f,%f\n"
,neww[0][0],neww[0][1],neww[1][0],neww[1][1],neww[2][0],neww[2][1],neww[3][0],neww[3][1],neww[4][0],neww[4][1]);
  fprintf(stderr,
"p1	%f,%f %f,%f	 %f,%f	%f,%f	%f,%f -->\n"
,old[0][3],old[0][4],old[1][3],old[1][4],old[2][3],old[2][4],old[3][3],old[3][4],old[4][3],old[4][4]);
  fprintf(stderr,
"p1n	%f,%f %f,%f	 %f,%f	%f,%f	%f,%f\n"
,neww[0][3],neww[0][4],neww[1][3],neww[1][4],neww[2][3],neww[2][4],neww[3][3],neww[3][4],neww[4][3],neww[4][4]);
  fprintf(stderr,
"p2	%f,%f %f,%f	 %f,%f	%f,%f	%f,%f -->\n"
,old[0][3],old[0][4],old[1][3],old[1][4],old[2][3],old[2][4],old[3][3],old[3][4],old[4][3],old[4][4]);
*/
  
  fprintf(stderr,"\n\n");
  }
}



int main(void) {
    GLuint vbo,vboframe;
    GLFWwindow* window;
    step_init();
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    window = glfwCreateWindow(WIDTH, HEIGHT, __FILE__, NULL, NULL);
    glfwMakeContextCurrent(window);

//    printf("GL_VERSION  : %s\n", glGetString(GL_VERSION) );
//    printf("GL_RENDERER : %s\n", glGetString(GL_RENDERER) );
 

    

    setup_graphics_for_letters_and_robots();	// this has a matrix to do 3d model, effect, etc

    

    glClearColor(0.0f, 0.0f, 0.3f, 1.0f);
    glViewport(0, 0, WIDTH, HEIGHT);
	checkGlError("vp");

    glGenBuffers(1, &vbo);
	checkGlError("a");
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
	checkGlError("b");
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
	checkGlError("c");
    glVertexAttribPointer(mPositionHandle, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
   glEnableVertexAttribArray(mPositionHandle);
	checkGlError("d");
	
    glBindBuffer(GL_ARRAY_BUFFER, 0);
	checkGlError("e");
    
    glGenBuffers(1, &vboframe);
	checkGlError("f");
    glBindBuffer(GL_ARRAY_BUFFER, vboframe);
	checkGlError("g");
    glBufferData(GL_ARRAY_BUFFER, sizeof(lines), lines, GL_STATIC_DRAW);
	checkGlError("h");
    glVertexAttribPointer(mPositionHandle, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
   glEnableVertexAttribArray(mPositionHandle);
	checkGlError("i");
    glBindBuffer(GL_ARRAY_BUFFER, 0);
	checkGlError("j");

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
	step();
        glClear(GL_COLOR_BUFFER_BIT);
	
	checkGlError("clear");
	
	
        glUseProgram(one_color_program);
	checkGlError("useprogram");
        glUniform4f(colorHandle, 0.8f,0.7f,0.3f,1.0f);
    set_matrix();    
	checkGlError("mat2");

	glBindBuffer(GL_ARRAY_BUFFER, vbo);	
        checkGlError("glBundBuffervbo");
    glVertexAttribPointer(mPositionHandle, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
   glEnableVertexAttribArray(mPositionHandle);
        checkGlError("glBundBuffervbovab");
	 /*
	vertices[0]=-1.;
	vertices[1]=1;
	vertices[2]=1;
	vertices[3]=0;
	vertices[4]=2;
	vertices[5]=1.1;
	vertices[6]=0.22;
	vertices[7]=-0,6;
	vertices[8]=-0.4; 
	 */
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
        checkGlError("blbufferdatavbo");
	
        glDrawArrays(GL_TRIANGLES, 0, 9);
        checkGlError("gradtriangles");

	fprintf(stderr,"vbo %f,%f,%f,%f,%f,%f,%f,%f,%f\n",vertices[0],vertices[1],vertices[2],
			vertices[3],vertices[4],vertices[5],
			vertices[6],vertices[7],vertices[8]);
    

        glBindBuffer(GL_ARRAY_BUFFER, vboframe);	
        glUniform4f(colorHandle, 0.8f,1.0f,1.0f,1.0f);
    glVertexAttribPointer(mPositionHandle, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
   glEnableVertexAttribArray(mPositionHandle);
        glBufferData(GL_ARRAY_BUFFER, sizeof(lines), lines, GL_STATIC_DRAW);
        glDrawArrays(GL_LINE_STRIP, 0, 12);

	

        lc->needed_points->line_width=0.0428f;
        glBindBuffer(GL_ARRAY_BUFFER, 0);	
	
        letters_out(lc,0.05f,-0.025f,-0.025f,0.f,"0");
        letters_out(lc,0.05f,0.166666666f-0.025f,0.166666666f-0.025f,0.f,"1");
        letters_out(lc,0.05f,0.166666666f-0.025f,-0.166666666f-0.025f,0.f,"2");
        letters_out(lc,0.05f,-0.166666666f-0.025f,-0.166666666f-0.025f,0.f,"3");
        letters_out(lc,0.05f,-0.166666666f-0.025f,0.166666666f-0.025f,0.f,"4");

	float xx=0.66666;
	float yy=0.66666;
        letters_out(lc,0.05f,xx-0.025f,yy-0.025f,0.f,"1");
        letters_out(lc,0.05f,xx+0.166666666f-0.025f,yy+0.166666666f-0.025f,0.f,"2");
        letters_out(lc,0.05f,xx+0.166666666f-0.025f,yy+-0.166666666f-0.025f,0.f,"3");
        letters_out(lc,0.05f,xx-0.166666666f-0.025f,yy+-0.166666666f-0.025f,0.f,"4");
        letters_out(lc,0.05f,xx-0.166666666f-0.025f,yy+0.166666666f-0.025f,0.f,"0");
			
	 xx=0.66666;
	 yy=-0.66666;
        letters_out(lc,0.05f,xx-0.025f,yy-0.025f,0.f,"2");
        letters_out(lc,0.05f,xx+0.166666666f-0.025f,yy+0.166666666f-0.025f,0.f,"3");
        letters_out(lc,0.05f,xx+0.166666666f-0.025f,yy+-0.166666666f-0.025f,0.f,"4");
        letters_out(lc,0.05f,xx-0.166666666f-0.025f,yy+-0.166666666f-0.025f,0.f,"0");
        letters_out(lc,0.05f,xx-0.166666666f-0.025f,yy+0.166666666f-0.025f,0.f,"1");
			
	 xx=-0.66666;
	 yy=-0.66666;
        letters_out(lc,0.05f,xx-0.025f,yy-0.025f,0.f,"3");
        letters_out(lc,0.05f,xx+0.166666666f-0.025f,yy+0.166666666f-0.025f,0.f,"4");
        letters_out(lc,0.05f,xx+0.166666666f-0.025f,yy+-0.166666666f-0.025f,0.f,"0");
        letters_out(lc,0.05f,xx-0.166666666f-0.025f,yy+-0.166666666f-0.025f,0.f,"1");
        letters_out(lc,0.05f,xx-0.166666666f-0.025f,yy+0.166666666f-0.025f,0.f,"2");
	
	 xx=-0.66666;
	 yy=0.66666;
        letters_out(lc,0.05f,xx-0.025f,yy-0.025f,0.f,"4");
        letters_out(lc,0.05f,xx+0.166666666f-0.025f,yy+0.166666666f-0.025f,0.f,"0");
        letters_out(lc,0.05f,xx+0.166666666f-0.025f,yy+-0.166666666f-0.025f,0.f,"1");
        letters_out(lc,0.05f,xx-0.166666666f-0.025f,yy+-0.166666666f-0.025f,0.f,"2");
        letters_out(lc,0.05f,xx-0.166666666f-0.025f,yy+0.166666666f-0.025f,0.f,"3");
			
	
	
        glfwSwapBuffers(window);
    }
    glDeleteBuffers(1, &vbo);
    glfwTerminate();
    return EXIT_SUCCESS;
}







