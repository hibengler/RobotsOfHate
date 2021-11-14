/* this tests network layer 1 */
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>
#include <fcntl.h>
#include "network1.h"









#include "player_packet_summary.h"







char recv_buffers[6][2048];


char send_buffers[6][2048];

char pending_send[6][2048];
char convo_buffers[6][50000];
char sent_buffers[6][50000];
char received_buffers[6][50000];

int running=1;
int to_player=0;

char stdin_buffer[50000];







void stdin_in1(struct network1_complete *c,int i, int n) {
volatile struct network1_complete *cv = (struct network1_complete *)c;
fprintf(stderr,"stdin_in1 got state %d buflen %d\n",cv->poll_state[i],cv->buflen[i]);
if (cv->poll_state[i]==5) {
  for (int j=0;j<cv->buflen[i];j++) {

    int ch = cv->buffers[i][j];
     fprintf(stderr,"	%c\n",ch);
    if ((ch==0) ||(ch == EOF)) {}
    else if (ch==3) running=0;
    else if ((ch>='0')&&(ch<'6')) {
      if ((ch-'0') != cv->participant_number) {
        to_player=ch-'0';
        }
      }
    else {
      char s[4];
      s[0]=ch;
      s[1]='\0';
      strcat(pending_send[to_player],s);
//      fprintf(stderr,"___pending is %s\n",pending_send[to_player]);
      } 
    }
  }
cv->buflen[i]=0;
cv->poll_state[i]=3;
cv->call_rounds[i]=0;
}

void stdin_in3(struct network1_complete *c,int i, int n) {
fprintf(stderr, "stdin_in3\n");
}



void do_in1(struct network1_complete *c,int i, int n) {
volatile struct network1_complete *cv = (struct network1_complete *)c;
fprintf(stderr,"do_in1 state is %d\b",c->poll_state[i]);
int communicator = c->communicator[i];
if (c->poll_state[i]>=6) {
  fprintf(stderr,"xxx");
}
else if (c->poll_state[i]==5) {
  c->buffers[i][c->buflen[i]]='\0';
  strcat(received_buffers[communicator],c->buffers[i]);
  if (strlen(received_buffers[communicator])>60) {
    strcpy(received_buffers[communicator], received_buffers[communicator] +strlen(received_buffers[communicator])-60);
    }
  cv->buflen[i]=0;
  cv->poll_state[i]=3;
  }
cv->buflen[i]=0;
cv->poll_state[i]=3;
cv->call_rounds[i]=0;
}












void do_out1(struct network1_complete *c,int i, int n) { 
fprintf(stderr,"			out1\n\n");
volatile struct network1_complete *cv = (struct network1_complete *)c;

if (cv->poll_state[i]>=6) {
  fprintf(stderr,"xxx1");
}
else if (cv->poll_state[i]==5) {
int communicator = cv->communicator[i];
  cv->buffers[i][cv->buflen[i]]='\0';
  strcat(sent_buffers[communicator],c->buffers[i]);
  if (strlen(sent_buffers[communicator])>60) {
    strcpy(sent_buffers[communicator], sent_buffers[communicator] +strlen(sent_buffers[communicator])-60);
    }
  fprintf(stderr,"w");  
  }
cv->buflen[i]=0;
cv->buffers[i][c->buflen[i]]='\0';
cv->poll_state[i]=3;
cv->call_rounds[i]=0;
}



void do_in3(struct network1_complete *c,int i, int n) {
}



void do_out3(struct network1_complete *c,int i, int n) {
}

void get_receive_buffer(struct network1_complete *c,int i, int n) {}
void get_new_send_buffer(struct network1_complete *c,int i, int n) {}
void get_next_send_buffer_from_queue (struct network1_complete *c,int i, int n) {}



static void format_difference_date(char *s,struct timeval a,struct timeval b) {
long au = a.tv_usec - b.tv_usec;
long as = a.tv_sec - b.tv_sec;
long am = as *1000 + (au/1000);
sprintf(s,"%4ld",am);
}

int output_screen(network1_complete *c,int result) {

//fprintf(stdout,"%c[2J%c[1;1HPlayer %d\r\nrecv  sent\r\nstatus              Conversation\r\n",(char)27,(char)27,c->participant_number);
fprintf(stdout,"%c[2J%c[1;1HPlayer %d\r\nrecv  sent\r\nstatus              Conversation\r\n",(char)27,(char)27,c->participant_number);
for (int i=0;i<12;i++) {
  fprintf(stdout,"%d",c->poll_state[i]);
  }


fprintf(stdout,"   result %d\r\n",result);
for (int i=0;i<12;i++) {
  fprintf(stdout,"%4d 	",c->ports[i]);
  }
fprintf(stdout,"\r\n");
for (int i=0;i<12;i++) {
  fprintf(stdout,"%4d 	",c->sent_to_ports[i]);
  }

fprintf(stdout,"\r\n");

for (int i=0;i<6;i++) {
  char *me;
  if (to_player==i) me="-->"; else me = "";
  fprintf(stdout,"	from	%d %s\r\n",i,received_buffers[i]);
  fprintf(stdout,"	  to 	%d %s\r\n",i,sent_buffers[i]);
  fprintf(stdout,"%s	 you 	%d %s\r\n\r\n",me,i,convo_buffers[i]);
  }
  
fflush(stdout);
}


void usage() {
  fprintf(stderr," Usage: %s ip0 ip1 ip2 ip3 ip4 ip5 player_id\r\n","test5_player");
  }


int main(int argc,char *argv[]) {
system("stty raw -echo");

if (argc!=8) {
  usage();
  exit(-1);
  }
int participant_number = argv[7][0]-'0';
participant_number = participant_number % 6;
network1_complete xc;
network1_complete *c = &xc;
volatile network1_complete *cv = (volatile network1_complete *)&xc;


network1_init(c,participant_number,"255.255.255.255",&(argv[1]),
   NULL,&(do_in1),&(do_out1),NULL,
   NULL,NULL,NULL,NULL,
   NULL,&(do_in3),&(do_out3),NULL,
   NULL,NULL,NULL,
   NULL,NULL,NULL,
    NULL,NULL,NULL);

//int r=fcntl(fileno(stdin),F_SETFL,O_NONBLOCK);
//fprintf(stderr,"fcntl r is %d\n",r);
int stdin_port = network1_add_standard_input_fd(c,fileno(stdin),&(stdin_in1),NULL,&(stdin_in3),NULL);

/*
   &get_receive_buffer,
   &get_new_send_buffer,
   &get_next_send_buffer_from_queue);
   
 */

for (int i=0;i<6;i++) {
  received_buffers[i][0]='\0';
  sent_buffers[i][0]='\0';
  send_buffers[i][0]='\0';  
  recv_buffers[i][0]='\0';  
  convo_buffers[i][0]='\0';  
  pending_send[i][0]='\0'; 
  c->buffers[i]=&(recv_buffers[i][0]);
  c->buffers[i+6]=&(send_buffers[i][0]);
  c->buflen[i]=0;
  c->buflen[i+6]=0;
//  network1_set_buffer(c,i+6,send_buffers[i],0);
  }          

stdin_buffer[0]='\0';
cv->buflen[stdin_port] = 0;
cv->buffers[stdin_port] = stdin_buffer;

to_player = (participant_number+1)%6;
while (running) {
//    fprintf(stderr,"polling for player %d    outstate  %d  send_buffer_ready %d\n",to_player,c->poll_state[6+to_player],c->send_buffer_ready[to_player]);
    if (cv->poll_state[6+to_player] ==5) { // waiting for us to clear the sent
      cv->buflen[6+to_player]=0; // clear it
      cv->poll_state[6+to_player]=3;
      }
    if ( (cv->poll_state[6+to_player] ==3)&& (cv->send_buffer_ready[to_player]))  {
       int l=strlen(pending_send[to_player]);
//       fprintf(stderr,"add %d\n",l);
       if (l) {
         strcat(convo_buffers[to_player],pending_send[to_player]);
         if (strlen(convo_buffers[to_player])>60) {
           strcpy(convo_buffers[to_player], convo_buffers[to_player] +strlen(convo_buffers[to_player])-60);
           }
	 if(cv->buflen[6+to_player]==0) { // not waiting for a send
           strcat(cv->buffers[6+to_player],pending_send[to_player]);
	   cv->buflen[6+to_player] = strlen(cv->buffers[6+to_player]);
   	   pending_send[to_player][0]='\0';
 // 	   fprintf(stderr,"pending send filled len %d\n", c->buflen[6+to_player]);
	   }
	 else {
  	   fprintf(stderr,"pending send waiting for packet to be sent, len %d\n", c->buflen[6+to_player]);
	   }
	 }
       
       } // if we can send
    
//  fprintf(stderr,"poll check\n");
  int result =  network1_poll_check(c);
//  fprintf(stderr,"poll checked %d\n",result);
  if (result==-1) {
    running=0;
    }
//  if (result) {
    output_screen(c,result);
       {
        struct timespec thislong;
             thislong.tv_sec = 0;
             thislong.tv_nsec = 5000000; /* 5 milliseconds */

             nanosleep(&thislong, &thislong);
      }
//    }

  }

system("stty sane");
exit(0);
}

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







