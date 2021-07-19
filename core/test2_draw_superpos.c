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

static const GLuint WIDTH = 1600;
static const GLuint HEIGHT = 1600;
static const GLchar* vertex_shader_source =
    "#version 100\n"
    "attribute vec3 position;\n"
    "void main() {\n"
    "   gl_Position = vec4(position, 1.0);\n"
    "}\n";
static const GLchar* fragment_shader_source =
    "#version 100\n"
    "void main() {\n"
    "   gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);\n"
    "}\n";
static const GLchar* fragment_shader_source2 =
    "#version 100\n"
    "void main() {\n"
    "   gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0);\n"
    "}\n";
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



GLfloat *old[5];
GLfloat *neww[5];


GLfloat deltas[3*5];
double oldx,oldy;
double newx,newy;
double dist=0.5;

GLfloat *deltaz[5]={deltas,deltas+3,deltas+6,deltas+9,deltas+12};

int step_init () {
GLfloat *olds= vertices;
GLfloat *news= vertices+(3l*3l*5l); /* points per pixel *  number of points per triangle  Times number of triangle repeats */

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
  }
oldx=0.;
oldy=0.;
newx=0.;
newy=0.;
dist=0.6666666;
{  int i;
  for (i=0;i<3;i++) {
    unsigned long offset = i*3;
    GLfloat *pold = old[0];
    pold[0+offset]+=oldx;
    pold[1+offset]+=oldy;
    pold[2+offset]+=0.;

    pold = old[1];
    pold[0+offset]+=-dist;  
    pold[1+offset]+=dist; 
    pold[2+offset]+=dist;
   
    pold=old[2];
    pold[0+offset]+=-dist;
    pold[1+offset]+=-dist;
    pold[2+offset]+=0.;
   
    pold = old[3];
    pold[0+offset]+=dist; 
    pold[1+offset]+=-dist;
    pold[2+offset]+=0.;
     
    pold = old[4];   
    pold[0+offset]+=-dist;
    pold[1+offset]+=-dist;
    pold[2+offset]+=0.;
    }     
  }       

{
  int i,j;
  for (i=0;i<5;i++) {
    for (j=0;j<factor;j++) {
      neww[i][j]=old[i][j];
      }
    }
  }
}

  
int step() {      
int i; for (i=0;i<5;i++) {
  for (int j=0;j<9;j++) {
    old[i][j]=neww[i][j];
    }
  }
  int count=0;
  int x,y;
  double offx;
  double offy;
  while (1) {
      fprintf(stderr,"%d\n",count);
    if ((count&7)==0) {
      fprintf(stderr,"r");
      x = rand();
      y = rand();
      offx=( ((double)(x % 50)) )-25.;
      offy=( ((double)(y % 50)) )-25.;
      offx = offx/800.;
      offy=offy/800.;
      }
    count++;
    if ((neww[0][0]+offx>0.300)||(neww[0][0]+offx<= -0.300)) {count=0;continue;}
    if ((neww[0][1]+offy>0.300)||(neww[0][1]+offy<= -0.300)) {count=0;continue;}
    
    for (unsigned long i=0;i<9;i+=3) {
      neww[0][i+0] +=  offx;
      neww[0][i+1] +=  offy;
      }
    break;
    }
  compute_superpos_vertices(old,neww,3,deltaz);
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
  fprintf(stderr,
"p2n	%f,%f %f,%f	 %f,%f	%f,%f	%f,%f\n"
,neww[0][6],neww[0][7],neww[1][6],neww[1][7],neww[2][6],neww[2][7],neww[3][6],neww[3][7],neww[4][6],neww[4][7]);

  
fprintf(stderr,"\n\n");
}



int main(void) {
    GLuint shader_program,shader_program2,vbo,vboframe;
    GLint pos,pos2;
    GLFWwindow* window;
    step_init();
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    window = glfwCreateWindow(WIDTH, HEIGHT, __FILE__, NULL, NULL);
    glfwMakeContextCurrent(window);

    printf("GL_VERSION  : %s\n", glGetString(GL_VERSION) );
    printf("GL_RENDERER : %s\n", glGetString(GL_RENDERER) );

    shader_program = common_get_shader_program(vertex_shader_source, fragment_shader_source);
    pos = glGetAttribLocation(shader_program, "position");
    shader_program2 = common_get_shader_program(vertex_shader_source, fragment_shader_source2);
    pos2 = glGetAttribLocation(shader_program2, "position");

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glViewport(0, 0, WIDTH, HEIGHT);


    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(pos);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    glGenBuffers(1, &vboframe);
    glBindBuffer(GL_ARRAY_BUFFER, vboframe);
    glBufferData(GL_ARRAY_BUFFER, sizeof(lines)-4, lines, GL_STATIC_DRAW);
        glVertexAttribPointer(pos2, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
        glEnableVertexAttribArray(pos2);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
	step();
        glClear(GL_COLOR_BUFFER_BIT);
	
	
	
        glUseProgram(shader_program);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);	
        glVertexAttribPointer(pos, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
	
        glDrawArrays(GL_TRIANGLES, 0, 6);

	glFlush();
			
    

        glBindBuffer(GL_ARRAY_BUFFER, vboframe);	
        glUseProgram(shader_program2);
        glBufferData(GL_ARRAY_BUFFER, sizeof(lines), lines, GL_STATIC_DRAW);
        glVertexAttribPointer(pos2, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
        glDrawArrays(GL_LINE_STRIP, 0, 12);
	
	glFlush();	
        glfwSwapBuffers(window);
    }
    glDeleteBuffers(1, &vbo);
    glfwTerminate();
    return EXIT_SUCCESS;
}
