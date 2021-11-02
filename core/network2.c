




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











			

            
	    


  


																      
																																      
																																																      


void network12_action_start_round1_poll (struct network1_complete *c1,int i, int n) {
/* called when we are starting*/
// call_rounds is used to decide wheteher to call the round1-round3 to externally process messages

}
void network12_handle_action_round1_in_poll (struct network1_complete *c1,int i, int n) {
/* called when we are starting*/

}

void network12_handle_action_round1_out_poll (struct network1_complete *c1,int i, int n) {
/* called when we are starting*/
}
void network12_action_finish_round1_poll (struct network1_complete *c1,int i, int n) {
/* called when we are starting*/
}



void network12_action_start_round2_poll (struct network1_complete *c1,int i, int n) {
/* called when we are starting*/
}
void network12_handle_action_round2_in_poll (struct network1_complete *c1,int i, int n) {
/* called when we are starting*/
}
void network12_handle_action_round2_out_poll (struct network1_complete *c1,int i, int n) {
/* called when we are starting*/
}
void network12_action_finish_round2_poll (struct network1_complete *c1,int i, int n) {
/* called when we are starting*/
}



void network12_action_start_round3_poll (struct network1_complete *c1,int i, int n) {
/* called when we are starting*/
}
void network12_handle_action_round3_in_poll (struct network1_complete *c1,int i, int n) {
/* called when we are starting*/
}
void network12_handle_action_round3_out_poll (struct network1_complete *c1,int i, int n) {
/* called when we are starting*/
}
void network12_action_finish_round3_poll (struct network1_complete *c1,int i, int n) {
/* called when we are starting*/
}






  

     
      

void network12_handle_stdin_action_round1_in_poll (struct network1_complete *c1,int i, int n) {
volatile struct network1_complete *cv1 = (volatile struct network1_complete *) c1;
volatile struct network2_complete *cv2 = (volatile struct network2_complete *) c1;
struct network2_complete *c2 = (struct network2_complete *) c1;

/* called when we are starting*/
if (cv1->poll_state[i]==5) {
  if (cv2->poll_state[i]<3) {
    cv2->buflen[i]=0;
    cv2->poll_state[i]=3;
    cv2->call_rounds[i]=1;
    }
  if (cv2->poll_state[i]==5) {
    // need to wait
    cv2->call_rounds[i]=1;
    return;
    }
  else if  (cv2->poll_state[i]>=6) {  
    cv2->buflen[i]=0;
    cv2->poll_state[i]=3;
    cv2->call_rounds[i]=1;
    return;
    }
  else if  (cv2->poll_state[i]==3) {  
    if ((cv2->buffers[i])&&(cv2->buflen[i]==0)) { 
      memcpy((void *)(c2->buffers[i]),(void *)(c1->buffers[i]),cv1->buflen[i]);
      cv2->poll_state[i]=5;
      cv2->buflen[i]=cv1->buflen[i];
      cv2->call_rounds[i]=1;
      cv1->buflen[i]=0;
      cv1->poll_state[i]=3;
      cv1->call_rounds[i]=0;
      return;
      }
    }  
  return;
  }  
else if  (cv1->poll_state[i]>=6) {
  cv1->buflen[i]=0;
  cv1->poll_state[i]=3;
  cv1->call_rounds[i]=0;
  }    
}
   
     
void network12_handle_stdin_action_round2_in_poll (struct network1_complete *c1,int i, int n) {
}
  
void network12_handle_stdin_action_round3_in_poll (struct network1_complete *c1,int i, int n) {
}
  


void network2_handle_receive_buffer_in_receive_thread(struct network2_complete *c2,int i,int n,int round) {
volatile network1_complete *cv1 = c2->cv1;
volatile network2_complete *cv2 = (volatile network2_complete *) c2;

if (cv2->poll_state[i]<3) {
  cv2->poll_state[i]=3;
  }
else if (cv2->poll_state[i]==4) {
  cv2->poll_state[i]=3;
  }
while (cv2->poll_state[i]!=3) {
waitning:
  nsleep();
  }
//assume simple now.
if (cv2->buflen[i]) {
  goto waitning;
  }
cv2->poll_state[i]=4;
if ((cv1->buffers[i])&&(cv1->buflen[i])&&(cv1->buffers[i])) {
  memcpy((void *)c2->buffers[i],(void *)c2->c1->buffers[i],c2->c1->buflen[i]);
  }
cv2->buflen[i] = cv1->buflen[i];
cv2->poll_state[i] = cv1->poll_state[i];
cv2->call_rounds[i]=1;

cv1->buflen[i]=0;
cv1->poll_state[i]=3;
cv1->call_rounds[i]=0;
// call the three cv2nonpoll rounds
  if (cv2->network2_handle_action_round1[i]) {
      (*cv2->network2_handle_action_round1[i])(c2,i,1); 
      }     
  if (cv2->network2_handle_action_round2[i]) {
            (*cv2->network2_handle_action_round2[i])(c2,i,1); 
      }     
  if (cv2->network2_handle_action_round3[i]) {
            (*cv2->network2_handle_action_round3[i])(c2,i,1); 
      }     
// hopefully c2 is back to 3, but we dont know
}






void network2_handle_done_send_buffer_thread(struct network2_complete *c2,int i,int n,int round) {  // handles all packets as received commands , will call round 1,2,3 here, round2 in will call round 2,3
struct network1_complete *c1 = c2->c1;
volatile struct network1_complete *cv1 = (volatile struct network1_complete *) c2->cv1;
volatile struct network2_complete *cv2 = (volatile struct network2_complete *) c2;


if (cv1->buffers[i][0] <= NETWORK2_HIGHEST_COMMAND ) {
  cv2->internal_position[i]=0;
  while (cv2->internal_position[i] < cv1->buflen[i]) {
    char code = cv1->buffers[i][cv2->internal_position[i]];
    if ((code!=3) ||(code != 5) ||(code !=1)||(code !=2)) {
      fprintf(stderr,"invalid code %d position %d writer %d\n",code,cv2->internal_position[i],i);
      bummerparse:
      if (cv2->poll_state[i]==4) {
        cv2->poll_state[i]=3;
	cv2->buflen[i]=0;
//        cv2->send_buffer_ready[i]=1;
	}
      cv1->poll_state[i]=3;
      cv1->buflen[i]=0;
      return;
      }
      
    unsigned short buflen =    (unsigned short) (cv1->buffers[i][cv2->internal_position[i]+1]) | (unsigned short)(cv1->buffers[i][cv2->internal_position[i]+2]);
    
    if (buflen+ cv2->internal_position[i] >cv1->buflen[i]) {
      fprintf(stderr,"invalid buffer length %d position %d writer %d\n",buflen,cv2->internal_position[i],i);
      goto bummerparse;
      }
      
      
    if (code==1) { // ctrla -- who cares
      cv2->internal_position[i] += buflen;
      continue;
      }

    else if (code==3) { // ctrlc -- who cares
      cv2->internal_position[i] += buflen;
      continue;
      }
    else if (code==2) { // ctrlb -- who cares
      cv2->internal_position[i] += buflen;
      continue;
      }
    else if (code==5) { // ctrle -- who cares
      cv2->internal_position[i] += buflen;
      continue;
      }
    } // while have space
  }  // if highest command
else {
  if (cv2->poll_state[i]<3) {
    cv2->poll_state[i]=3;
//    cv2->send_buffer_ready[i]=1;
    }
  else if (cv2->poll_state[i]==4) {
    cv2->poll_state[i]=3;   
    }
  while (cv2->poll_state[i]!=3) {
    nsleep();
    }
  /* set t2 to be like c1 */
  cv2->call_rounds[i]=cv1->call_rounds[i];
//  cv2->send_buffer_ready[i] = cv1->send_buffer_ready[i];
  if ((cv1->buffers[i])&&(cv1->buflen[i])&&(cv2->buffers[i])) {
    memcpy((void *)c2->buffers[i],(void *)c1->buffers[i],c1->buflen[i]);
    }
  cv2->buflen[i] = cv1->buflen[i];
  cv2->poll_state[i] = cv1->poll_state[i];
  if (round<=1) {
    if (c2->network2_handle_action_round1[i]) {
      (*c2->network2_handle_action_round1[i])(c2,i,1); 
      }
    }
  if (round<=2) {
    if (c2->network2_handle_action_round2[i]) {
      (*c2->network2_handle_action_round2[i])(c2,i,1); 
      }
    }
  if (round<=3) {
    if (c2->network2_handle_action_round3[i]) {
      (*c2->network2_handle_action_round3[i])(c2,i,1); 
      }
    }
  cv1->call_rounds[i] = cv2->call_rounds[i];
//  cv1->send_buffer_ready[i] = cv2->send_buffer_ready[i];
  cv1->poll_state[i] = cv2->poll_state[i];    
  cv1->buflen[i] = cv2->buflen[i];
//  if ((cv2->poll_state[i]==3)&&(cv2->buflen[i]==0)) {
//    cv2->send_buffer_ready[i]=1;
//    }
   
  }
  


}




void network12_pull_next_send_buffer_from_queue (struct network1_complete *c1,int i, int n) {
struct network2_complete *c2 = (struct network2_complete *) c1;
volatile struct network1_complete *cv1 = (volatile struct network1_complete *) c1;
volatile struct network2_complete *cv2 = (volatile struct network2_complete *) c2;
int bind_id=i;
if (cv2->poll_state[i]<3) {
    cv2->poll_state[i]=3;
    }
int cv2_send=0;
if (cv2->poll_state[i]==3) {
  int flag=0;
  if (cv2->buffers[i]){
    if (cv2->buflen[bind_id]) {
      flag=1;
      }
    else {
      if (cv2->network2_pull_next_send_buffer_from_queue[bind_id]) {
          (*cv2->network2_pull_next_send_buffer_from_queue[bind_id])(c2,bind_id,0);
          }
      }
    }
  else {
    if (cv2->network2_pull_next_send_buffer_from_queue[bind_id]) {
      (*cv2->network2_pull_next_send_buffer_from_queue[bind_id])(c2,bind_id,0);
      }
    }
    
  if ((cv2->buffers[bind_id]!=NULL)&&(cv2->buflen[bind_id])) {
    flag=1;
    }
  else {
    if (cv2->buffers[bind_id]==NULL) {
      if (cv2->network2_get_new_send_buffer[bind_id]) {
        (*cv2->network2_get_new_send_buffer[bind_id])(c2,bind_id,0);
        }
      }
    if (cv2->buffers[bind_id]) {
      if (cv2->buflen[bind_id]) {
        flag=1;
        }
      }
    }
  if (flag) {
    cv2->poll_state[i]=4;
    cv2_send = 1;
    }
  }
  
    
  

int other_send=0;

for (int j=0;j<NUMBER_OF_NETWORK1_PARTICIPANTS_TIMES_2;j++) {
  volatile network2_queue *q = &(cv2->queues_from_poll_to_send[j][i]); // one for each read thread, and write thread, as the write thread can invoke sends after certain task numbers have been done to other write threads.

  if (q->head != q->tail) {
    other_send = 1;
    break;
    }
  }

if (cv2_send && !other_send) { // simple
  memcpy((void *)c1->buffers[i],(void *)c2->buffers[i],c2->buflen[i]);
  cv1->buflen[i]=cv2->buflen[i];
  return; // all done simple
  }
else if ((!cv2_send)&&(!other_send)) { // nothing to do
  return;
  }
else {
  fprintf(stderr,"	n2 sending complex\n");
  cv2->internal_position[i] = 0;
  
  
  if (cv2_send) {
     unsigned short buflen =  cv2->buflen[i] + 1 + 2; // ctrlb then length low hi then characters - len includes all
    if (cv2->buflen[i] + cv2->internal_position[i] + buflen > NETWORK2_COMMAND_MAX_LENGTH) {
      cv2->buflen[i]=0;
      cv2->poll_state[i]=3; // cant do it
      }
    else {
      cv1->buffers[i][cv2->internal_position[i]] = 2; // ctrlb
      cv1->buffers[i][cv2->internal_position[i]+1] = (buflen&0xff); // lenght low
      cv1->buffers[i][cv2->internal_position[i]+2] = ((buflen>>8)&0xff); // lenght high
      memcpy((void *)(c1->buffers[i]+cv2->internal_position[i]+3),(void *)c2->buffers[i],c2->buflen[i]);
      cv2->internal_position[i]+=buflen;
      }
    
    for (int j=0;j<NUMBER_OF_NETWORK1_PARTICIPANTS_TIMES_2;j++) {
      volatile network2_queue *q = &(cv2->queues_from_poll_to_send[j][i]);
      while (q->head != q->tail) {
        int head = q->head;
	int command_id = q->command_id[head];
	volatile network2_internal_command *ic = &(cv2->internal_commands[command_id]);
	volatile unsigned char * internal_command_buffers = &(cv2->internal_command_buffers[command_id][0]);
	volatile char command_code = q->command_code[head];
	
	int do_it=0;
	
	unsigned short buflen = ic->length;
	if (command_code==3) { buflen += 1 + 2 + 2  + 6 + 4 +1; } // ctrl-d len cmdnum 0 1 2 3 4 5 order of sending send ack executed delete state_number command }
	else if (command_code == 1) { buflen = 1 +  2 +  2 + 1 + 1 + 1 + 1; } // ctrla len2 cmdnum2 sent ack exe clear 
	else if (command_code == 5) { buflen = 1 +  2 +  2; } // ctrle len2 cmdnum 
	else { // unknown command code - erase it 
	  head = (head+1) % NETWORK2_QUEUE_SIZE;
	  q->head = head;
	  continue;
	  }
	if (buflen + cv2->internal_position[i] > NETWORK2_COMMAND_MAX_LENGTH) {
          break; // try the next queue
	  }
        cv1->buffers[i][cv2->internal_position[i]] = command_code; // ctrlb
        cv1->buffers[i][cv2->internal_position[i]+1] = (buflen&0xff); // lenght low
        cv1->buffers[i][cv2->internal_position[i]+2] = ((buflen>>8)&0xff); // lenght high
        cv1->buffers[i][cv2->internal_position[i]+3] = (command_id&0xff); // lenght low
        cv1->buffers[i][cv2->internal_position[i]+4] = ((command_id>>8)&0xff); // lenght high
        if (command_code==3) { 
          cv1->buffers[i][cv2->internal_position[i]+5] = ic->network_order[0];
          cv1->buffers[i][cv2->internal_position[i]+6] = ic->network_order[1];
          cv1->buffers[i][cv2->internal_position[i]+7] = ic->network_order[2];
          cv1->buffers[i][cv2->internal_position[i]+8] = ic->network_order[3];
          cv1->buffers[i][cv2->internal_position[i]+9] = ic->network_order[4];
          cv1->buffers[i][cv2->internal_position[i]+10] = ic->network_order[5];
	  unsigned char send=0;
	  if (ic->sent_state[0]) send |=1;
	  if (ic->sent_state[1]) send |=2;
	  if (ic->sent_state[2]) send |=4;
	  if (ic->sent_state[3]) send |=8;
	  if (ic->sent_state[4]) send |=16;
	  if (ic->sent_state[5]) send |=32;
          cv1->buffers[i][cv2->internal_position[i]+11] = send;
	  unsigned char ack=0;
	  if (ic->acknowledged_state[0]) ack |=1;
	  if (ic->acknowledged_state[1]) ack |=2;
	  if (ic->acknowledged_state[2]) ack |=4;
	  if (ic->acknowledged_state[3]) ack |=8;
	  if (ic->acknowledged_state[4]) ack |=16;
	  if (ic->acknowledged_state[5]) ack |=32;
          cv1->buffers[i][cv2->internal_position[i]+12] = ack;
	  unsigned char executed=0;
	  if (ic->executed_state[0]) executed |=1;
	  if (ic->executed_state[1]) executed |=2;
	  if (ic->executed_state[2]) executed |=4;
	  if (ic->executed_state[3]) executed |=8;
	  if (ic->executed_state[4]) executed |=16;
	  if (ic->executed_state[5]) executed |=32;
          cv1->buffers[i][cv2->internal_position[i]+13] = executed;
          unsigned char cleared=0;
	  if (ic->clear_state[0]) cleared |=1;
	  if (ic->clear_state[1]) cleared |=2;
	  if (ic->clear_state[2]) cleared |=4;
	  if (ic->clear_state[3]) cleared |=8;
	  if (ic->clear_state[4]) cleared |=16;
	  if (ic->clear_state[5]) cleared |=32;
          cv1->buffers[i][cv2->internal_position[i]+14] = cleared;
	  cv1->buffers[i][cv2->internal_position[i]+15] = q->state_number[q->head];
	  memcpy((void *)(c1->buffers[i]+cv2->internal_position[i]+16),((void *)internal_command_buffers),ic->length);
          cv2->internal_position[i]+=buflen;
          } // if we are ctrlc
	else if (command_code==1) {  // ctrla
	  unsigned char send=0;
	  if (ic->sent_state[0]) send |=1;
	  if (ic->sent_state[1]) send |=2;
	  if (ic->sent_state[2]) send |=4;
	  if (ic->sent_state[3]) send |=8;
	  if (ic->sent_state[4]) send |=16;
	  if (ic->sent_state[5]) send |=32;
          cv1->buffers[i][cv2->internal_position[i]+5] = send;
	  unsigned char ack=0;
	  if (ic->acknowledged_state[0]) ack |=1;
	  if (ic->acknowledged_state[1]) ack |=2;
	  if (ic->acknowledged_state[2]) ack |=4;
	  if (ic->acknowledged_state[3]) ack |=8;
	  if (ic->acknowledged_state[4]) ack |=16;
	  if (ic->acknowledged_state[5]) ack |=32;
          cv1->buffers[i][cv2->internal_position[i]+6] = ack;
	  unsigned char executed=0;
	  if (ic->executed_state[0]) executed |=1;
	  if (ic->executed_state[1]) executed |=2;
	  if (ic->executed_state[2]) executed |=4;
	  if (ic->executed_state[3]) executed |=8;
	  if (ic->executed_state[4]) executed |=16;
	  if (ic->executed_state[5]) executed |=32;
          cv1->buffers[i][cv2->internal_position[i]+7] = executed;
          unsigned char cleared=0;
	  if (ic->clear_state[0]) cleared |=1;
	  if (ic->clear_state[1]) cleared |=2;
	  if (ic->clear_state[2]) cleared |=4;
	  if (ic->clear_state[3]) cleared |=8;
	  if (ic->clear_state[4]) cleared |=16;
	  if (ic->clear_state[5]) cleared |=32;
          cv1->buffers[i][cv2->internal_position[i]+8] = cleared;
	  cv1->buffers[i][cv2->internal_position[i]+9] = q->state_number[q->head];
          cv2->internal_position[i]+=buflen;
          }
	else if (command_code==5) {  // ctrle
          cv2->internal_position[i]+=buflen;
          }
	head = (head+1) % NETWORK2_QUEUE_SIZE;
	q->head = head;
	} // while entries in this queue to do
      } // for all queues
    }
  if (cv2->internal_position[i]) {
    cv1->buflen[i]=cv2->buflen[i];
    }
  return;
  }

}





// This is inside the reading thread
void network12_handle_action_round1_in (struct network1_complete *c1,int i, int n) {
struct network2_complete *c2 = (struct network2_complete *) c1;
volatile struct network1_complete *cv1 = (volatile struct network1_complete *) c1;
volatile struct network2_complete *cv2 = (volatile struct network2_complete *) c2;

if (cv1->type[i]==2) {
  if (c2->network2_handle_action_round1[i]) { 
    if (cv1->poll_state[i]>=6) {
      // ignore error 
      cv1->buflen[0];
      cv1->poll_state[i]=3;
      cv1->call_rounds[i]=0;
      }
    else if (cv1->poll_state[i]==5) {
      if (cv2->poll_state[i]<3) {
        cv2->poll_state[i]=3;
        }
      else if (cv2->poll_state[i]==4) {
        cv2->poll_state[i]=3;
	}	
      while (cv2->poll_state[i]!=3) {
	nsleep();
	}
    /* set t2 to be like c1 */
      cv2->call_rounds[i]=cv1->call_rounds[i];
 //     cv2->send_buffer_ready[i] = cv1->send_buffer_ready[i];
      if ((cv1->buffers[i])&&(cv1->buflen[i])&&(cv2->buffers[i])) {
        memcpy((void *)c2->buffers[i],(void *)c1->buffers[i],c1->buflen[i]);
        }
      cv2->buflen[i] = cv1->buflen[i];
      cv2->poll_state[i] = cv1->poll_state[i];
      (*c2->network2_handle_action_round1[i])(c2,i,1); 
      cv1->call_rounds[i] = cv2->call_rounds[i];
//      cv1->send_buffer_ready[i] = cv2->send_buffer_ready[i];
      cv1->poll_state[i] = cv2->poll_state[i];    
      cv1->buflen[i] = cv2->buflen[i];
      return; 
      }
    else {
      cv1->buflen[0];
      cv1->poll_state[i]=3;
      cv1->call_rounds[i]=0;
      }
    }
  }
else if (cv1->type[i]==0) {
  if (cv1->poll_state[i]>=6) {
    // ignore error 
    cv1->buflen[0];
    cv1->poll_state[i]=3;
    cv1->call_rounds[i]=0;
    }
  else if (cv1->poll_state[i]==5) {
    network2_handle_receive_buffer_in_receive_thread(c2, i, n,1);  // handles all packets as received commands , will call round 1,2,3 here, round2 in will call round 2,3
    /* this could block the receive thread until the poll handles things, which could be multiple times , or maybe this is also handled in the receive thread */
    }
  }
cv1->buflen[0];
cv1->poll_state[i]=3;
cv1->call_rounds[i]=0;
}


void network12_handle_action_round1_out (struct network1_complete *c1,int i, int n) {
struct network2_complete *c2 = (struct network2_complete *) c1;
volatile struct network1_complete *cv1 = (volatile struct network1_complete *) c1;
volatile struct network2_complete *cv2 = (volatile struct network2_complete *) c2;
// we just sent one out.  build from in list, current out, and outlist into c1 until full
// then compute the initial buffer.
// then send out and after complete - possible set the sent id
// i guess round1 should read, round2 should do execute and round3 should do write, aagggh, lets do round1 and round3
// also should be able to send buffer as is, which is what we are going to start to do - so this is compatible with the test5_player code.
if (cv1->type[i]==3) {
  if (c2->network2_handle_action_round1[i]) { 
    if (cv1->poll_state[i]>=6) {
      // ignore error 
      cv1->buflen[0];
      cv1->poll_state[i]=3;
      cv1->call_rounds[i]=0;
      }
    else if (cv1->poll_state[i]==5) {
      if (cv2->poll_state[i]<3) {
        cv2->poll_state[i]=3;
        }
      else if (cv2->poll_state[i]==4) {
        cv2->poll_state[i]=3;
	}
      
      while ((cv2->poll_state[i]!=3)||(cv2->buflen==0)) {
	nsleep();
	}
      /* set t2 to be like c1 */
      cv2->call_rounds[i]=cv1->call_rounds[i];
//      cv2->send_buffer_ready[i] = cv1->send_buffer_ready[i];
      if ((cv1->buffers[i])&&(cv1->buflen[i])&&(cv2->buffers[i])) {
        memcpy((void *)c2->buffers[i],(void *)c1->buffers[i],c1->buflen[i]);
        }
      cv2->buflen[i] = cv2->buflen[i];
      cv2->poll_state[i] = cv1->poll_state[i];
      (*c2->network2_handle_action_round1[i])(c2,i,1); 
      cv1->call_rounds[i] = cv2->call_rounds[i];
//      cv1->send_buffer_ready[i] = cv2->send_buffer_ready[i];
      cv1->poll_state[i] = cv2->poll_state[i];    
      cv1->buflen[i] = cv2->buflen[i];
      }
    else {
      cv1->buflen[0];
      cv1->poll_state[i]=3;
      cv1->call_rounds[i]=0;
      }          
    return; 
    }
  }
else if (cv1->type[i]==1) {
  if (cv1->poll_state[i]>=6) {
    // ignore error 
    cv1->buflen[0];
    cv1->poll_state[i]=3;
    cv1->call_rounds[i]=0;
    }
  else if (cv1->poll_state[i]==5) {
    network2_handle_done_send_buffer_thread(c2,i,n,1);  // the top level send is changed from 4 to 3, and we set cv2->poll state to 5 if it was 4
    /* but for all the send records in this send (via parsing just like the recv parser, except that we also might parse the status record also makk the sent field to the five writer
         so if cv2->status was 4, set cv2 status to 5 or 6 or whatever.  for now, we can probably skip it. */
    /* this could block the receive thread until the poll handles things, which could be multiple times , or maybe this is also handled in the receive thread */
    }
  }
cv1->buflen[i]=0;
cv1->poll_state[i]=3;
cv1->call_rounds[i]=0;
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
volatile struct network2_complete *cv2 = (volatile struct network2_complete *)(c1);

}


void network12_get_new_send_buffer(struct network1_complete *c1,int i, int n) {
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
network1_complete *c1 = c2->c1;
volatile network1_complete *cv1 = c2->cv1;
volatile network2_complete *cv2 = (volatile network2_complete *)c2;
int i=cv1->current_number_of_polls;
cv1->buflen[i]= 0;
cv1->buffers[i]= &c2->internal_buffer[i][0];
cv2->buflen[i]= 0;

cv2->network2_handle_action_round1_poll[i] = network2_handle_action_round1_in_poll;
cv2->network2_handle_action_round2_poll[i] = network2_handle_action_round2_in_poll;
cv2->network2_handle_action_round3_poll[i] = network2_handle_action_round3_in_poll;

int result=network1_add_standard_input_fd(c1,fd,network12_handle_stdin_action_round1_in_poll,
network12_handle_stdin_action_round2_in_poll,
network12_handle_stdin_action_round3_in_poll,
network12_get_new_receive_buffer
);
return result;

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


memset(c2,0,sizeof(network2_complete));
c2->c1 = &(c2->complete1);
c2->cv1 = (volatile network1_complete *) &(c2->complete1);
network1_complete *c1 = c2->c1;;
volatile network1_complete *cv1 = c2->cv1;

network1_init(c2->c1,participant_number,broadcast_ip,ips,
  network12_action_start_round1_poll,
  network12_handle_action_round1_in_poll,
  network12_handle_action_round1_out_poll,
  network12_action_finish_round1_poll,
  network12_action_start_round2_poll,
  network12_handle_action_round2_in_poll,
  network12_handle_action_round2_out_poll,
  network12_action_finish_round2_poll,
  network12_action_start_round3_poll,
  network12_handle_action_round3_in_poll,
  network12_handle_action_round3_out_poll,
  network12_action_finish_round3_poll,
  network12_handle_action_round1_in, 
  network12_handle_action_round1_out,
  network12_handle_action_round2_in,
  network12_handle_action_round2_out,
  network12_handle_action_round3_in,
  network12_handle_action_round3_out,
  network12_get_new_receive_buffer,
  network12_get_new_send_buffer,
  network12_pull_next_send_buffer_from_queue);
  
  

c2->network2_action_start_round1_poll = network2_action_start_round1_poll;
c2->network2_action_finish_round1_poll = network2_action_finish_round1_poll;
c2->network2_action_start_round2_poll = network2_action_start_round2_poll;
c2->network2_action_finish_round2_poll = network2_action_finish_round2_poll;
c2->network2_action_start_round3_poll = network2_action_start_round3_poll;
c2->network2_action_finish_round3_poll = network2_action_finish_round3_poll;


/* set up input ports */
for (int i=0;i<NUMBER_OF_NETWORK1_PARTICIPANTS;i++) {
  int communicator = i;
  c1->buflen[i]= 0;
  c1->buffers[i]= &c2->internal_buffer[i][0];

  c2->call_rounds[i]= cv1->call_rounds[i];
  c2->poll_state[i] = cv1->poll_state[i];
  c2->communicator[i] = cv1->communicator[i];
  c2->buflen[i]= 0;
  c2->buffers[i]= NULL;
  
  
  c2->network2_handle_action_round1_poll[i] = network2_handle_action_round1_in_poll;
  c2->network2_handle_action_round2_poll[i] = network2_handle_action_round2_in_poll;
  c2->network2_handle_action_round3_poll[i] = network2_handle_action_round3_in_poll;
  c2->network2_handle_action_round1[i] = network2_handle_action_round1_in;
  c2->network2_handle_action_round2[i] = network2_handle_action_round2_in;
  c2->network2_handle_action_round3[i] = network2_handle_action_round3_in;
  c2->network2_get_new_send_buffer[i] = network2_get_new_send_buffer;
  c2->network2_pull_next_send_buffer_from_queue[i] = network2_pull_next_send_buffer_from_queue;
  c2->next_command_id[i]=i;
  
  c2->poll_state[i]=3; 
  }


for (int o=NUMBER_OF_NETWORK1_PARTICIPANTS;o<NUMBER_OF_NETWORK1_PARTICIPANTS_TIMES_2;o++) {
  int communicator = o-NUMBER_OF_NETWORK1_PARTICIPANTS;
  c1->buflen[o]= 0;
  c1->buffers[o]= &c2->internal_buffer[o][0];
    
  c2->communicator[o] = communicator;  // where we send from
  c2->buflen[o]= 0;
  c2->buffers[o]= NULL;
  
  c2->network2_handle_action_round1_poll[o] = network2_handle_action_round1_out_poll;
  c2->network2_handle_action_round2_poll[o] = network2_handle_action_round2_out_poll;
  c2->network2_handle_action_round3_poll[o] = network2_handle_action_round3_out_poll;
  c2->network2_handle_action_round1[o] = network2_handle_action_round1_out;
  c2->network2_handle_action_round2[o] = network2_handle_action_round2_out;
  c2->network2_handle_action_round3[o] = network2_handle_action_round3_out;
  c2->network2_get_new_send_buffer[o] = network2_get_new_send_buffer;
  c2->network2_pull_next_send_buffer_from_queue[o] = network2_pull_next_send_buffer_from_queue;

  c2->call_rounds[o]=0;
  c2->poll_state[o] = 0;

  
  c2->poll_state[o]=3; 
  
  }
c2->participant_number = c1->participant_number;

  

c2->network2_action_start_round1_poll = network2_action_start_round1_poll;
c2->network2_action_finish_round1_poll = network2_action_finish_round1_poll;
c2->network2_action_start_round2_poll = network2_action_start_round2_poll;
c2->network2_action_finish_round2_poll = network2_action_finish_round2_poll;
c2->network2_action_start_round3_poll = network2_action_start_round3_poll;
c2->network2_action_finish_round3_poll = network2_action_finish_round3_poll;
return 1;
}







int network2_poll_check_nosleep(network2_complete *c2) {
int i;
volatile network2_complete *cv2=c2;
network1_complete *c1 = c2->c1;
volatile network1_complete *cv1 = c2->cv1;

//network2_reset_sendables_and_stuff(c);

int number_to_round = network1_poll_check_nosleep(c1);




//compute_sendable_recieveables(c,1);
//process_poll_buffer_statuses(c,0);

number_to_round = 0;
for (i=0;i<cv1->current_number_of_polls;i++) {
  if (cv2->call_rounds[i]) number_to_round++;
  }  

      
if (!number_to_round) return 0;

//compute_sendable_recieveables(c,0);
//process_poll_buffer_statuses(c,1);

{ // Block to process events round 1
  
  if (number_to_round &&(cv2->network2_action_start_round1_poll)) {
    (*cv2->network2_action_start_round1_poll)(c2,number_to_round,number_to_round);
    }
    
  for (int i=0;i<cv1->current_number_of_polls;i++) {
    if (cv2->call_rounds[i]) {    
      if  (cv2->network2_handle_action_round1_poll[i]) { (*c2->network2_handle_action_round1_poll[i])(c2,i,number_to_round);  }
      }
    } /* while loopint through all events */
  
  if ((number_to_round) &&(cv2->network2_action_finish_round1_poll)) {
    (*cv2->network2_action_finish_round1_poll)(c2,number_to_round,number_to_round);
    }


  } // block to process events round 1


// recompute number_to_round after round 1
number_to_round=0;
for (int i=0;i<cv1->current_number_of_polls;i++) {
  if (cv2->call_rounds[i]) number_to_round++;
  }

//process_poll_buffer_statuses(c,0);  


if (!number_to_round) { 
  goto full_check_poll_loop;
  }
  


{ // Block to process events round 2

  if (number_to_round &&(cv2->network2_action_start_round2_poll)) {
    (*cv2->network2_action_start_round2_poll)(c2,number_to_round,number_to_round);
    }
    
  for (int i=0;i<cv1->current_number_of_polls;i++) {
    if (cv2->call_rounds[i]) {    
      if  (cv2->network2_handle_action_round2_poll[i]) { (*cv2->network2_handle_action_round2_poll[i])(c2,i,number_to_round);  }
      }
    } /* while loopint through all events */
  
  if ((number_to_round) &&(cv2->network2_action_finish_round2_poll)) {
    (*cv2->network2_action_finish_round2_poll)(c2,number_to_round,number_to_round);
    }


  } // block to process events round 2
  
  

// recompute number_to_round after round 2
number_to_round=0;
for (int i=0;i<cv1->current_number_of_polls;i++) {
  if (cv2->call_rounds[i]) number_to_round++;
  }
  
if (!number_to_round) {
  goto full_check_poll_loop;
  }
  
  


{ // Block to process events round 3
  
  if (number_to_round &&(cv2->network2_action_start_round3_poll)) {
    (*cv2->network2_action_start_round3_poll)(c2,number_to_round,number_to_round);
    }
    
  for (int i=0;i<cv1->current_number_of_polls;i++) {
    if (cv2->call_rounds[i]) {    
      if  (cv2->network2_handle_action_round3_poll[i]) { (*cv2->network2_handle_action_round3_poll[i])(c2,i,number_to_round);  }
      }
    } /* while loopint through all events */
  
  if ((number_to_round) &&(cv2->network2_action_finish_round3_poll)) {
    (*cv2->network2_action_finish_round3_poll)(c2,number_to_round,number_to_round);
    }


  } // block to process events round 3


// recompute number_to_round after round 3 - to use later
number_to_round=0;
for (int i=0;i<cv1->current_number_of_polls;i++) {
  if (cv2->call_rounds[i]) number_to_round++;
  }
//process_poll_buffer_statuses(c,0);  


if (!number_to_round) {
  goto full_check_poll_loop;
  }
  
full_check_poll_loop:

//network2_reset_sendables_and_stuff(c);
//compute_sendable_recieveables(c,1);
//process_poll_buffer_statuses(c,0);
   
if (!number_to_round) nsleep();
   
return number_to_round;
}




int network2_poll_check(network2_complete *c2) {
int result = network2_poll_check_nosleep(c2);
if (!result) nsleep();
return result;
}

/* end of network2.c */

																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																														      																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																														
