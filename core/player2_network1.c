/* this tests network layer 1 */
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>
#include <fcntl.h>
#include "network1.h"


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



int parse_buffer(volatile hate_frame *frame,volatile unsigned char buffer[],int index,int buflen) {
if (index>=buflen)  {
  return 0;
  }
  
unsigned int a = buffer[index]*buffer[index+1];
if (a+index >buflen) {
  return 0;
  }
struct hate_action this_action;
unsigned char *act = (unsigned char *)( &this_action);

for (i=0;i<a;i++) {
  act[i] = buffer[index+i];
  }

unsigned short action_id = this_action.action_id;
if (action_id > HATE_NUMBER_ACTIONS) {
  return(0);
  }
  

// test it.
volatile hate_action *ha = &(hate_frame->hate_actions[action_id]);
int action_state = ha->hate_action_state;
if (action_state==HATE_ACTION_NEW) {




void do_in1(struct network1_complete *c,int i, int n) {
volatile struct network1_complete *cv = (struct network1_complete *)c;
fprintf(stderr,"do_in1 state is %d\b",c->poll_state[i]);
int communicator = c->communicator[i];
if (c->poll_state[i]>=6) {
  fprintf(stderr,"xxx");
}
else if (c->poll_state[i]==5) {
  c->buffers[i][c->buflen[i]]='\0';
  c->buffers[i][c->buflen[i+1]]='\0';
  int index=0;
  while (parse_buffer(c->buffers[i]),

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
