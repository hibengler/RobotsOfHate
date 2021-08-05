
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
#include "player.h"
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


onecolor_specs onecs;
onecolor_specs *onec=&onecs;
hate_game  the_hate_game;




static void set_origin_matrix() {
// no effect origin matrix
glMatrix tempMVMatrix;
multMatrix(&tempMVMatrix,&onec->modelMatrix,&onec->viewMatrix);
multMatrix(&onec->origin_matrix,&tempMVMatrix,&onec->projectionMatrix);
}   


static void set_matrix() {
      
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
    
    
    // Apply the projection and view transformation
    glUniformMatrix4fv(onec->
   mMVPMatrixHandle, 1, GL_FALSE, (GLfloat *)(&onec->MVPMatrix));
   checkGlError("setmatrix");
}       
    
    
void set_matrix_translate(GLfloat centerx,GLfloat centery) {
    loadIdentity(&onec->modelMatrix);
    translateMatrix(&onec->modelMatrix,centerx,centery,0.f);
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
init_onecolor_specs(onec);
    loadIdentity(&onec->modelMatrix);
    loadIdentity(&onec->effectMatrix);
    loadIdentity(&onec->viewMatrix);
    loadIdentity(&onec->projectionMatrix);
        
    set_origin_matrix();
    game_init(&the_hate_game,0); // 0 is our player id
          
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

static const GLuint WIDTH = 2100;
static const GLuint HEIGHT = 2100;


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









  
int step() {      




  game_step(&the_hate_game);
  
      

}



int main(void) {
    GLuint vbo,vboframe;
    GLFWwindow* window;
        InitSilentRadiance("https://pt.silentradiance.com/download.cgi");
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
                        
            if (p)
            {
                if (p->has_beat)
                {
                    //thisBack = BLUE;  
                }
                if (p->has_onset)
                {
                    //thisBack = BLUE;
                }
                // pitch is p->pitch
                // db is a rough db level - just 4 levels
                // p->folded flas is flags like beat, onset, but folded over a few packets to better match with the video frame rate
                //radianceData.beatScale = 1 + p->pitch/10000;
            }
#ifdef sadsasdaasddsdddsdsdfdfs            
            if ((packet_summary.commanded_background_color[0] != 0.f)  ||
                (packet_summary.commanded_background_color[1] != 0.f)  ||
                (packet_summary.commanded_background_color[2] != 0.f))
            {  if the dj sets a color, use that as a background
             
                this_background = (Color){(unsigned char)(packet_summary.commanded_background_color[0]*255.f),
                            (unsigned char)(packet_summary.commanded_background_color[1]*255.f),
                        (unsigned char)(packet_summary.commanded_background_color[2]*255.f),255};
            }
            else
            {
                this_background = RAYWHITE;
            }
#endif


// silentradiance ^^^^^

	step();
        glClear(GL_COLOR_BUFFER_BIT);
	
	checkGlError("clear");
	
	
        glUseProgram(onec->one_color_program);
	checkGlError("useprogram");
        glUniform4f(onec->colorHandle, 0.8f,0.7f,0.3f,1.0f);
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
			
        glUseProgram(onec->one_color_program);
	checkGlError("useprogram");
	game_draw(onec,&the_hate_game);
	
        glfwSwapBuffers(window);
    }
    glDeleteBuffers(1, &vbo);
    glfwTerminate();
    return EXIT_SUCCESS;
}







