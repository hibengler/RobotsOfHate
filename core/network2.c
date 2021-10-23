




// network2 - send to everybody



// here we go

#include "glue.h"
#include "network2.h"
#include <error.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <pthread.h>
              
//
//









static void nsleep() {
struct timespec req;
req.tv_sec=0;
req.tv_nsec=5000000;
            
nanosleep(&req,NULL);
}     
	      
	      





static void network2_delay_for_milliseconds(network2_complete *c, int bind_id,unsigned int millisecondsa) {
struct timeval a,b;
b=a;
unsigned long milliseconds = millisecondsa;
unsigned long usec = a.tv_usec;
unsigned long asec = milliseconds / 1000l;
unsigned long usecm1 = milliseconds%1000l; 
unsigned long usecm2 = milliseconds *1000l;
unsigned long tusec = usec + usecm2;
if (tusec > 1000000l) {
  asec = asec + 1;
  tusec = tusec - 1000000l;
  }
a.tv_sec += asec;
a.tv_usec = tusec;
fprintf(stderr,"	delay add %d milliseconds to %ld %ld -> %ld %ld\n",millisecondsa,b.tv_sec,b.tv_usec,a.tv_sec,a.tv_usec);
struct timespec ts;
ts.tv_sec = a.tv_sec;
ts.tv_nsec = a.tv_usec*1000l;
nanosleep(&ts,NULL);
}











			

            
	    


  


																      
																																      
																																																      



     
      

void network12_handle_stdin_action_round1_in_poll (struct network1_complete *c1,int i, int n) {
}
  
  
void network12_handle_stdin_action_round2_in_poll (struct network1_complete *c1,int i, int n) {
}
  
void network12_handle_stdin_action_round3_in_poll (struct network1_complete *c1,int i, int n) {
}
  


void network12_handle_action_round1_in (struct network1_complete *c1,int i, int n) {
}
void network12_handle_action_round1_out (struct network1_complete *c1,int i, int n) {
}
void network12_handle_action_round2_in (struct network1_complete *c1,int i, int n) {
}
void network12_handle_action_round2_out (struct network1_complete *c1,int i, int n) {
}
void network12_handle_action_round3_in (struct network1_complete *c1,int i, int n) {
}
void network12_handle_action_round3_out (struct network1_complete *c1,int i, int n) {
}
void network12_handle_action_round3_out (struct network1_complete *c1,int i, int n) {
}


void network12_handle_action_round1_in (struct network1_complete *c1,int i, int n) {
}
void network12_handle_action_round1_out (struct network1_complete *c1,int i, int n) {
}
void network12_handle_action_round2_in (struct network1_complete *c1,int i, int n) {
}
void network12_handle_action_round2_out (struct network1_complete *c1,int i, int n) {
}
void network12_handle_action_round3_in (struct network1_complete *c1,int i, int n) {
}
void network12_handle_action_round3_out (struct network1_complete *c1,int i, int n) {
}

void network12_get_new_receive_buffer (struct network1_complete *c1,int i, int n) {
struct network2_complete *c2 = (struct network2_complete *)(c1);
struct network2_complete *cv2 = (volatile struct network2_complete *)(c1);

int i=
}
void network12_get_new_send_buffer (struct network1_complete *c1,int i, int n) {

}
void network12_pull_next_send_buffer_from_queue (struct network1_complete *c1,int i, int n) {
}



/*
	
		0 ->1 0->2 0->3 0->4
		      1->4 1->2 1->3
		           2->4 2->5
			   								assumed because twice
		0->1			0	01					0	01
		0->3	1->3		0	01		013			03	013		013
		0->2	1->2 3->1	0	013	012	013			0123	0123	0123	0123
		0->5	1->5 3->4 2->4	0	013	012	013	01234 0135
		5->0A   4->0A		01234	0135      	
		0->1A	5->2A 4->2a
		0->3a	1->4A 2->5A   THATS IT
		
		
alternate, and better - because it activates 4 earlier:
		0->1			0	01
		0->3 1->3		0	01		013
		0->2 3->2 1->4		0	01	0123		014
		0->5 2->4 3->5		0	01	0123	013	01234	0135
		5->1a 4->0a		01234	0135	0123	013	01234	0135
		4->1 5->0 0->2 1->3a	012345	012345	01235	013	01234	0135
		5->4 0->3 1->4 2->5a	012345	012345	012345	012345	012345	012345
			
				
*/






/* call after network2_init to add a file or non udp socket port */
int network2_add_standard_input_fd(network2_complete *c2,int fd,  
     network2_complete_round_call network2_handle_action_round1_in_poll,  
     network2_complete_round_call network2_handle_action_round2_in_poll,
     network2_complete_round_call network2_handle_action_round3_in_poll,   
     network2_complete_round_call network2_get_new_receive_buffer) {
network1_complete *c1 = c->c1;
return network1_add_standard_input_fd(c1,fd,network12_handle_action_round1_in_poll,
network12_handle_action_round2_in_poll,
network12_handle_action_round3_in_poll,
network12_get_new_receive_buffer

);
}





int network2_init (network2_complete *c2,int participant_number,char *broadcast_ip,char *ips[],
     network2_complete_round_call network2_action_start_round1_poll,
     network2_complete_round_call network2_handle_action_round1_in_poll, 
     network2_complete_round_call network2_handle_action_round1_out_poll,
     network2_complete_round_call network2_action_finish_round1_poll,
     network2_complete_round_call network2_action_start_round2_poll,
     network2_complete_round_call network2_handle_action_round2_in_poll, 
     network2_complete_round_call network2_handle_action_round2_out_poll,
     network2_complete_round_call network2_action_finish_round2_poll,
     network2_complete_round_call network2_action_start_round3_poll,
     network2_complete_round_call network2_handle_action_round3_in_poll, 
     network2_complete_round_call network2_handle_action_round3_out_poll,
     network2_complete_round_call network2_action_finish_round3_poll,
     network2_complete_round_call network2_handle_action_round1_in, network2_complete_round_call network2_handle_action_round1_out,
     network2_complete_round_call network2_handle_action_round2_in, network2_complete_round_call network2_handle_action_round2_out,
     network2_complete_round_call network2_handle_action_round3_in, network2_complete_round_call network2_handle_action_round3_out,
     network2_complete_round_call network2_get_new_receive_buffer, /* get a new receive buffer for this poll - could be broadcast in */                                       
     network2_complete_round_call network2_get_new_send_buffer, /* get a new send buffer - 0 length  - just to fill up  - why? */                                 
     network2_complete_round_call network2_pull_next_send_buffer_from_queue /* get a semd buffer off the queue for this polling address - returns queue length */          
     )  
{


memset(c,0,sizeof(network2_complete));
c2->c1 = &(c2.c);
c2->cv1 = (volatile network1_complete *) &(c2.c);
network1_init(c2->c1,participant_number,broadcast_ip,ips,
  NULL,NULL,NULL,NULL,
  NULL,NULL,NULL,NULL,
  NULL,NULL,NULL,NULL,
  network12_handle_action_round1_in,
  network12_handle_action_round1_out,
  network12_handle_action_round2_in,
  network12_handle_action_round2_out,
  network12_handle_action_round3_in,
  network12_handle_action_round3_out,
  network12_get_new_receive_buffer,
  network12_get_new_send_buffer,
  network12_pull_next_send_buffer_from_queue);
  
  
/*
c->network1_action_start_round1_poll = network2_action_start_round1_poll;
c->network1_action_finish_round1_poll = network2_action_finish_round1_poll;
c->network1_action_start_round2_poll = network2_action_start_round2_poll;
c->network1_action_finish_round2_poll = network2_action_finish_round2_poll;
c->network1_action_start_round3_poll = network2_action_start_round3_poll;
c->network1_action_finish_round3_poll = network2_action_finish_round3_poll;
*/

/* set up input ports */
for (int i=0;i<NUMBER_OF_NETWORK1_PARTICIPANTS;i++) {
  int communicator = i;
  c1->buflen[i]= 0;
  c1->buffers[i]= NULL;
  }


for (int o=NUMBER_OF_network2_PARTICIPANTS;o<NUMBER_OF_network2_PARTICIPANTS_TIMES_2;o++) {
  int communicator = o-NUMBER_OF_NETWORK1_PARTICIPANTS;
  c1->buflen[o]= 0;
  c1->buffers[o]= NULL;
    
/*  
  c->network2_handle_action_round1_poll[o] = network2_handle_action_round1_out_poll;
  c->network2_handle_action_round2_poll[o] = network2_handle_action_round2_out_poll;
  c->network2_handle_action_round3_poll[o] = network2_handle_action_round3_out_poll;
  c->network2_handle_action_round1[o] = network2_handle_action_round1_out;
  c->network2_handle_action_round2[o] = network2_handle_action_round2_out;
  c->network2_handle_action_round3[o] = network2_handle_action_round3_out;
  c->network2_get_new_send_buffer[o] = network2_get_new_send_buffer;
  c->network2_pull_next_send_buffer_from_queue[o] = network2_pull_next_send_buffer_from_queue;
  */
  }
  

  
return 1;
}










int network2_poll_check(network2_complete *c) {
int i;
volatile network2_complete *cv=c;
int number_to_round=0;

// call_rounds is used to decide wheteher to call the round1-round3 to externally process messages
//for (int i=0;i<c->current_number_of_polls;i++) {
//  c->call_rounds[i]=0;
//  c->temp_dont_poll_yet[i]=0;
//  }
network2_reset_sendables_and_stuff(c);



compute_sendable_recieveables(c,1);
process_poll_buffer_statuses(c,0);


for (i=0;i<cv->current_number_of_polls;i++) {
  if (cv->call_rounds[i]) number_to_round++;
  }  
      
if (!number_to_round) return 0;

compute_sendable_recieveables(c,0);
process_poll_buffer_statuses(c,1);

{ // Block to process events round 1
  
  if (number_to_round &&(cv->network2_action_start_round1_poll)) {
    (*cv->network2_action_start_round1_poll)(c,number_to_round,number_to_round);
    }
    
  for (int i=0;i<cv->current_number_of_polls;i++) {
    if (cv->call_rounds[i]) {    
      if  (cv->network2_handle_action_round1_poll[i]) { (*cv->network2_handle_action_round1_poll[i])(c,i,number_to_round);  }
      }
    } /* while loopint through all events */
  
  if ((number_to_round) &&(cv->network2_action_finish_round1_poll)) {
    (*cv->network2_action_finish_round1_poll)(c,number_to_round,number_to_round);
    }


  } // block to process events round 1


// recompute number_to_round after round 1
number_to_round=0;
for (int i=0;i<cv->current_number_of_polls;i++) {
  if (cv->call_rounds[i]) number_to_round++;
  }

process_poll_buffer_statuses(c,0);  


if (!number_to_round) { 
  goto full_check_poll_loop;
  }
  


{ // Block to process events round 2

  if (number_to_round &&(cv->network2_action_start_round2_poll)) {
    (*cv->network2_action_start_round2_poll)(c,number_to_round,number_to_round);
    }
    
  for (int i=0;i<cv->current_number_of_polls;i++) {
    if (cv->call_rounds[i]) {    
      if  (cv->network2_handle_action_round2_poll[i]) { (*cv->network2_handle_action_round2_poll[i])(c,i,number_to_round);  }
      }
    } /* while loopint through all events */
  
  if ((number_to_round) &&(cv->network2_action_finish_round2_poll)) {
    (*cv->network2_action_finish_round2_poll)(c,number_to_round,number_to_round);
    }


  } // block to process events round 2
  
  

// recompute number_to_round after round 2
number_to_round=0;
for (int i=0;i<cv->current_number_of_polls;i++) {
  if (cv->call_rounds[i]) number_to_round++;
  }
process_poll_buffer_statuses(c,0);  


if (!number_to_round) {
  goto full_check_poll_loop;
  }
  
  


{ // Block to process events round 3
  
  if (number_to_round &&(cv->network2_action_start_round3_poll)) {
    (*cv->network2_action_start_round3_poll)(c,number_to_round,number_to_round);
    }
    
  for (int i=0;i<cv->current_number_of_polls;i++) {
    if (cv->call_rounds[i]) {    
      if  (cv->network2_handle_action_round3_poll[i]) { (*cv->network2_handle_action_round3_poll[i])(c,i,number_to_round);  }
      }
    } /* while loopint through all events */
  
  if ((number_to_round) &&(cv->network2_action_finish_round3_poll)) {
    (*cv->network2_action_finish_round3_poll)(c,number_to_round,number_to_round);
    }


  } // block to process events round 3


// recompute number_to_round after round 3 - to use later
number_to_round=0;
for (int i=0;i<cv->current_number_of_polls;i++) {
  if (cv->call_rounds[i]) number_to_round++;
  }
process_poll_buffer_statuses(c,0);  


if (!number_to_round) {
  goto full_check_poll_loop;
  }
  
full_check_poll_loop:

network2_reset_sendables_and_stuff(c);
compute_sendable_recieveables(c,1);
process_poll_buffer_statuses(c,0);
   
if (!number_to_round) nsleep();
   
return 0;
}


/* end of network2.c */

																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																														      																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																														
