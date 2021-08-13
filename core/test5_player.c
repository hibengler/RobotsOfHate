/* this tests network layer 1 */
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

#include "network1.h"


char recv_buffers[6][2048];


char send_buffers[6][2048];

char pending_send[6][2048];
char sent_buffers[6][50000];
char received_buffers[6][50000];

int running=1;
int to_player=0;
int participant_number;

volatile int lock_ch1=0;
volatile int lock_ch2=0;
volatile int lock_ch3=0;
volatile int ch='\0';


void * readchars(void *x) {
while (running) {
  lock_ch2=1;
  while (lock_ch1) {}
  ch=getchar();
  lock_ch2=0;
  while (lock_ch3) {}
  }
}












void do_in1(struct network1_complete *c,int i, int n) {
int communicator = c->communicator[i];
c->buffers[i][c->buflen[i]]='\0';


strcat(received_buffers[communicator],c->buffers[i]);
if (strlen(received_buffers[communicator])>60) {
  strcpy(received_buffers[communicator], received_buffers[communicator] +strlen(received_buffers[communicator])-60);
  }
c->buflen[i]=0;
}





void do_out1(struct network1_complete *c,int i, int n) { 
int communicator = c->communicator[i];
c->buffers[i][c->buflen[i]]='\0';
c->buflen[i]=0;
c->buffers[i][0]='\0';
strcat(sent_buffers[communicator],send_buffers[communicator]);
if (strlen(sent_buffers[communicator])>60) {
  strcpy(sent_buffers[communicator], sent_buffers[communicator] +strlen(sent_buffers[communicator])-60);
  }
}



void do_in3(struct network1_complete *c,int i, int n) {}
void do_out3(struct network1_complete *c,int i, int n) {}

void get_receive_buffer(struct network1_complete *c,int i, int n) {}
void get_new_send_buffer(struct network1_complete *c,int i, int n) {}
void get_next_send_buffer_from_queue (struct network1_complete *c,int i, int n) {}





int output_screen(network1_complete *c) {

fprintf(stdout,"%c[2JPlayer %d\nrecv  sent\nstatus              Conversation\n",(char)27,participant_number);
for (int i=0;i<12;i++) {
  fprintf(stdout,"%d",c->poll_state[i]);
  }
  
for (int i=0;i<6;i++) {
  fprintf(stdout," from %d %s\n",i,received_buffers[i]);

  fprintf(stdout,"             to %d   %s\n\n",i,sent_buffers[i]);
  }
fflush(stdout);
}



void usage() {
  fprintf(stderr," Usage: %s ip0 ip1 ip2 ip3 ip4 ip5 player_id\n","test5_player");
  }

int main(int argc,char *argv[]) {
system("stty -raw");

if (argc!=8) {
  usage();
  exit(-1);
  }
int participant_number = argv[7][0]-'0';
participant_number = participant_number % 6;
network1_complete xc;
network1_complete *c = &xc;
network1_init(c,participant_number,"255.255.255.255",&(argv[1]),
   NULL,&(do_in1),&(do_out1),NULL,
   NULL,NULL,NULL,NULL,
   NULL,&(do_in3),&(do_out3),NULL,
   &get_receive_buffer,
   &get_new_send_buffer,
   &get_next_send_buffer_from_queue);
   
 

for (int i=0;i<6;i++) {
  received_buffers[i][0]='\0';
  sent_buffers[i][0]='\0';
  send_buffers[i][0]='\0';  
  recv_buffers[i][0]='\0';  
  pending_send[i][0]='\0';  
  network1_set_buffer(c,i,recv_buffers[i],0);
  network1_set_buffer(c,i+6,send_buffers[i],0);
  }          

pthread_t yy;
pthread_create(&yy,NULL,readchars,NULL);
      
to_player = (participant_number+1)%6;
while (running) {
  lock_ch1=1;
  lock_ch3=0;
  if (!lock_ch2) {
    if (ch==3) running=0;
    else if ((ch>'0')&&(ch<'6')) {
      if ((ch-'0') != participant_number) {to_player=ch-'0';}
      }
    else {
      char s[4];
      s[0]=ch;
      s[1]='\0';
      strcat(pending_send[to_player],s);
      }
        
    if (c->poll_state[6+to_player] !=4) {
      int l=strlen(pending_send[to_player]);
      if (l) {
        if (c->poll_state[6+to_player] ==5) {
	  c->poll_state[6+to_player]=3;
	  c->buflen[6+to_player]=0;
	  }
        strcat(send_buffers[6+to_player],pending_send[to_player]);
	c->buflen[6+to_player] +=l;
	pending_send[to_player][0]='\0';;
	}
      }
    } // if we can send


  lock_ch3=1;
  network1_poll_check(c);
  output_screen(c);
  }


system("stty sane");
exit(0);
}
