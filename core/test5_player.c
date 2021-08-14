/* this tests network layer 1 */
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>

#include "network1.h"


char recv_buffers[6][2048];


char send_buffers[6][2048];

char pending_send[6][2048];
char convo_buffers[6][50000];
char sent_buffers[6][50000];
char received_buffers[6][50000];

int running=1;
int to_player=0;

volatile int lock_ch1=0;
volatile int lock_ch2=0;
volatile int lock_ch3=0;
volatile int ch= EOF;


void * readchars(void *x) {
while (running) {
  lock_ch2=1;
  while (lock_ch1) {}


       


  ch=EOF;
  ch=getchar();
  lock_ch2=0;
  while (lock_ch3) {}

  }
}












void do_in1(struct network1_complete *c,int i, int n) {
fprintf(stderr,"calling do_in1  poll state is %d\r\n",c->poll_state[i]);
int communicator = c->communicator[i];
c->buffers[i][c->buflen[i]]='\0';
if (c->poll_state[i]==5) {
  strcat(received_buffers[communicator],c->buffers[i]);
  if (strlen(received_buffers[communicator])>60) {
    strcpy(received_buffers[communicator], received_buffers[communicator] +strlen(received_buffers[communicator])-60);
    }
  c->buflen[i]=0;
  }
fprintf(stderr,"r");  
}








void do_out1(struct network1_complete *c,int i, int n) { 
fprintf(stderr,"calling do_out1  poll state is %d\r\n",c->poll_state[i]);
int communicator = c->communicator[i];
c->buffers[i][c->buflen[i]]='\0';
if (c->poll_state[i]==5) {
  strcat(sent_buffers[communicator],c->buffers[i]);
  if (strlen(sent_buffers[communicator])>60) {
    strcpy(sent_buffers[communicator], sent_buffers[communicator] +strlen(sent_buffers[communicator])-60);
    }
  fprintf(stderr,"w");  
  c->buflen[i]=0;
  }
fprintf(stderr,"x");  



}



void do_in3(struct network1_complete *c,int i, int n) {
//if (c->poll_state[i]==5) {
//  c->poll_state[i]=3;
//  c->buflen[i]=0;
//  }
//if (c->poll_state[i]==3) {
//  c->buflen[i]=0;
//  }
}



void do_out3(struct network1_complete *c,int i, int n) {
fprintf(stderr,"calling do_out3  poll state is %d\r\n",c->poll_state[i]);
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

fprintf(stdout,"%c[2J%c[1;1HPlayer %d\r\nrecv  sent\r\nstatus              Conversation\r\n",(char)27,(char)27,c->participant_number);
for (int i=0;i<12;i++) {
  fprintf(stdout,"%d",c->poll_state[i]);
  }


fprintf(stdout,"   ressult %d\r\n",result);
for (int i=0;i<12;i++) {
  fprintf(stdout,"%4d 	",c->ports[i]);
  }
fprintf(stdout,"\r\n");


for (int i=0;i<6;i++) {
  fprintf(stdout,"	from	%d %s\r\n",i,received_buffers[i]);
  fprintf(stdout,"        to 	%d %s\r\n",i,sent_buffers[i]);
  fprintf(stdout,"	 you 	%d %s\r\n\r\n",i,convo_buffers[i]);
  }
{
  char ds[30];
  char dpr[30];
  char dpl[30];
  char dh[30];
  char d1[30];
  char d2[30];
  char d3[30];
  char dps[30];
  
  char dse[30];
  
  format_difference_date(dse,c->local_check_start_time,c->local_network1_check_end_time);
  format_difference_date(ds,c->local_check_start_time,c->local_poll_predo_start_time[0]);
  format_difference_date(dpr,c->local_poll_predo_start_time[0],c->local_poll_predo_end_time[0]);
  format_difference_date(dpl,c->local_poll_predo_end_time[0],c->local_poll_end_time[0]);
  format_difference_date(dh,c->local_poll_end_time[0],c->local_handle_end_time[0]);
  format_difference_date(d1,c->local_handle_end_time[0],c->local_round1_end_time[0]);
  format_difference_date(d2,c->local_round1_end_time[0],c->local_round2_end_time[0]);
  format_difference_date(d3,c->local_round2_end_time[0],c->local_round3_end_time[0]);
  format_difference_date(dps,c->local_round3_end_time[0],c->local_postdo_end_time[0]);
  fprintf(stdout,"\r\n\r\n	Date %s 	= pre %s	pl %s	ph %s		r1 %s r2 %s	r3 %s ps %s\r\n",dse,dpr,dpl,dh,d1,d2,d3,dps);
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
network1_init(c,participant_number,"255.255.255.255",&(argv[1]),
   NULL,&(do_in1),&(do_out1),NULL,
   NULL,NULL,NULL,NULL,
   NULL,&(do_in3),&(do_out3),NULL,
    NULL,NULL,NULL);
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
//  network1_set_buffer(c,i+6,send_buffers[i],0);
  }          

pthread_t yy;
pthread_create(&yy,NULL,readchars,NULL);
      
to_player = (participant_number+1)%6;
while (running) {
  lock_ch1=1;
  lock_ch3=0;
  if (!lock_ch2) {
    if((ch==0) ||(ch == EOF)) {}
    else if (ch==3) running=0;
    else if ((ch>'0')&&(ch<'6')) {
      if ((ch-'0') != participant_number) {
        to_player=ch-'0';
        }
      }
    else {
      char s[4];
      s[0]=ch;
      s[1]='\0';
      fprintf(stderr, " hey %c\r\n",ch);
      strcat(pending_send[to_player],s);
      
      fprintf(stderr, " We got %s\r\n",pending_send[to_player]);
      }
    
    
    ch = EOF;
    lock_ch1 = 0;        
    fprintf(stderr,"%d %d %d %lx %d\n",to_player,c->poll_state[6+to_player],c->send_buffer_ready[to_player],(long)(c->buffers[to_player+6]),c->buflen[to_player+6]);
    if ( (c->poll_state[6+to_player] <=3)&& (c->send_buffer_ready[to_player]))  {
      
       int l=strlen(pending_send[to_player]);

       if (l) {
          fprintf(stderr, " We trnasfer  to %d %s\r\n",to_player,pending_send[to_player]);
          strcat(convo_buffers[to_player],pending_send[to_player]);
          if (strlen(convo_buffers[to_player])>60) {
            strcpy(convo_buffers[to_player], convo_buffers[to_player] +strlen(convo_buffers[to_player])-60);
            }
//          if (c->poll_state[6+to_player] ==5) {
//	    c->poll_state[6+to_player]=3;
//	    c->buflen[6+to_player]=0;
	  

          strcat(c->buffers[6+to_player],pending_send[to_player]);
	  c->buflen[6+to_player] +=l;
	
	  pending_send[to_player][0]='\0';
	  fprintf(stderr, "we are staged for %d  %s\r\n",6+to_player,send_buffers[6+to_player]);
	  }
       } // if we can send
    }

  lock_ch3=1;
  int result =  network1_poll_check(c);
  output_screen(c,result);
//       {
//        struct timespec thislong;
//             thislong.tv_sec = 0;
//             thislong.tv_nsec = 50000000; /* 50 milliseconds */

//             nanosleep(&thislong, &thislong);
 //      }
  

  }

system("stty sane");
exit(0);
}
