//




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

int pp=0;



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
    cv2->call_rounds[i]=0;
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
  







void path_finished_sending_acknowlegement(struct network2_complete *c2,volatile network2_packeted_command *pc,volatile network2_internal_command *icv) {
unsigned char this_state = pc->this_state;
if (icv->did_states[this_state] ==0) {
  icv->did_states[this_state] = 1;
  // later have some calls that are per states
  if (this_state==NETWORK2_PATH_NOTHING_STATE_NOTHING) {
    }
  else if (this_state==NETWORK2_PATH_SIMPLE_STATE_SIMPLE) {
    }
  }
}



void path_finished_receiving_acknowlegement(struct network2_complete *c2,volatile network2_packeted_command *pc,volatile network2_internal_command *icv) {
unsigned char this_state = pc->this_state;
if (icv->did_states[this_state] ==0) {
  icv->did_states[this_state] = 1;
  for (int j=0;j<NUMBER_OF_NETWORK1_PARTICIPANTS;j++) {
    icv->acknowledged_state[j] |= pc->packeted_ic.acknowledged_state[j];
    }
  // later have some calls that are per states
  if (this_state==NETWORK2_PATH_NOTHING_STATE_NOTHING) {
    }
  else if (this_state==NETWORK2_PATH_SIMPLE_STATE_SIMPLE) {
    }
  }
}








void path_queue_up(struct network2_complete *c2,volatile network2_packeted_command *pc,int me_bind_id,int to_bind_id) {
volatile network2_complete *cv2 = (volatile network2_complete *)c2;
volatile network2_queue *q =  &(cv2->queues_from_poll_to_send[me_bind_id][to_bind_id]);
while (((q->tail+1)%NETWORK2_QUEUE_SIZE) == q->head) {
  nsleep();
  }
int tail2 =  ((q->tail+1)%NETWORK2_QUEUE_SIZE);
int tail = q->tail;
q->packeted_commands[tail] = (network2_packeted_command *)malloc(sizeof(network2_packeted_command));
memcpy((void *)q->packeted_commands[tail],(void *)pc,sizeof(network2_packeted_command));
q->tail = tail2;
}




// must be run by poll
int network2_get_free_command_number(struct network2_complete *c2,int me_bind_id) {
volatile network2_complete *cv2 = (volatile network2_complete *)c2;
int id=cv2->next_command_id;
int id2= id + NUMBER_OF_NETWORK1_PARTICIPANTS;
if (id2>NETWORK2_NUMBER_COMMANDS) {
  id2 = c2->cv1->participant_number;
  }
cv2->next_command_id=id2;
cv2->internal_commands[id].id = id;
return id;
}





// only call from poll task, preferred_first and next can be -1 if it doesnt matter
// me_bind_id = our thread, or participant_number
int network2_path_new_command(struct network2_complete *c2,
    unsigned char *command,int length, int initial_state, int me_bind_id, int preferred_first,int preferred_next) {
int command_id = network2_get_free_command_number(c2,me_bind_id);
memset((void *)(c2->internal_commands+command_id),0,sizeof(network2_internal_command));
volatile network2_complete *cv2 = (volatile network2_complete *)c2;
network2_packeted_command pcc;
volatile network2_packeted_command *pcv = &pcc;
volatile network2_internal_command *icv = &pcv->packeted_ic;

memset((void *)(pcv),0,sizeof(network2_packeted_command));

icv->length = length;
icv->id = command_id;
icv->code=3;

pcv->code = 3;
pcv->length = length;
pcv->this_state = initial_state;
memcpy((void *)pcv->the_command,(void *)command,length);
memcpy((void *)c2->internal_command_buffers[command_id],(void *)command,length);


for (int j=0;j<NUMBER_OF_NETWORK1_PARTICIPANTS;j++) {
  icv->network_order[j]=-1;
  }
icv->network_order[0] = cv2->cv1->participant_number;
if (preferred_first!=-1) {
  icv->network_order[1] = preferred_first;
  if(preferred_next != -1) {
    icv->network_order[2] = preferred_next;
    }
  }  
  
int  list[NUMBER_OF_NETWORK1_PARTICIPANTS];
int  available[NUMBER_OF_NETWORK1_PARTICIPANTS];
int count_to_do = 0;


for (int j=0;j<NUMBER_OF_NETWORK1_PARTICIPANTS;j++) {
   available[j]=1;
   }
   
for (int j=0;j<NUMBER_OF_NETWORK1_PARTICIPANTS;j++) {
  if (icv->network_order[j]!=255) {
    available[icv->network_order[j]]=0;
    }
  }

// figure out what is left to be done.
for (int j=0;j<NUMBER_OF_NETWORK1_PARTICIPANTS;j++) {
  if (available[j]) {
    list[count_to_do] =j;
    count_to_do++;
    }
  }


int j;
j = NUMBER_OF_NETWORK1_PARTICIPANTS-count_to_do;  
for (;count_to_do>=1;count_to_do--) {
  unsigned int new_index = rand();
  new_index = new_index >>3;
  new_index = new_index % count_to_do;
  icv->network_order[j] = list[new_index];
  list[new_index] = list[count_to_do-1];
  j++;
  }
    
pcv->bind_id = icv->network_order[1]+NUMBER_OF_NETWORK1_PARTICIPANTS;



for (int j=0;j<NUMBER_OF_NETWORK1_PARTICIPANTS;j++) {
  icv->sent_state[j]=0; 
  icv->acknowledged_state[j]=0;
  icv->executed_state[j]=0;
  icv->clear_state[j]=0;
  }
icv->sent_state[icv->network_order[0]]=1;  
icv->acknowledged_state[icv->network_order[0]]=1;  

for (int j=0;j<NETWORK2_STATE_LENGTH;j++) {
  icv->did_states[j]=0;
  }
 


cv2->internal_commands[command_id] = *icv;  
path_queue_up(c2,pcv,me_bind_id,icv->network_order[1]+NUMBER_OF_NETWORK1_PARTICIPANTS);
// queue to send it out;     
}




void path_queue_execute_on_poll(struct network2_complete *c2,volatile network2_packeted_command *pc,volatile network2_internal_command *icv) {
int pn = c2->cv1->participant_number;
if ((icv->executed_state[pn])==0) {
  path_queue_up(c2,pc,pc->bind_id,pn);
  }
}




void sync_states(struct network2_complete *c2,volatile network2_packeted_command *pcv,volatile network2_internal_command *icv) {
for (int j=0;j<NUMBER_OF_NETWORK1_PARTICIPANTS;j++) {
  icv->sent_state[j] |= pcv->packeted_ic.sent_state[j];
  icv->acknowledged_state[j] |= pcv->packeted_ic.acknowledged_state[j];
  icv->executed_state[j] |= pcv->packeted_ic.acknowledged_state[j];
  icv->clear_state[j] |= pcv->packeted_ic.clear_state[j];
  }
}


void path_possibly_execute(struct network2_complete *c2,volatile network2_packeted_command *pcv,volatile network2_internal_command *icv) {
int j;
int pn = c2->cv1->participant_number;


if((icv->acknowledged_state[pn])&&(icv->executed_state[pn]==0)) { // if we got the command
  path_queue_up(c2,pcv,icv->network_order[0],pn);
  icv->executed_state[pn]|=1;
  pcv->packeted_ic.executed_state[pn]|=1;
  }
}


void path_finished_sending_command(struct network2_complete *c2,volatile network2_packeted_command *pc,volatile network2_internal_command *icv) {
unsigned char this_state = pc->this_state;
int pn = c2->cv1->participant_number;
int i=pc->bind_id;
if (icv->did_states[this_state] ==0) {
  icv->did_states[this_state] = 1;
  // later have some calls that are per states
  if (this_state==NETWORK2_PATH_NOTHING_STATE_NOTHING) {
    pc->packeted_ic.sent_state[i] = 1;
    sync_states(c2,pc,icv);
    path_possibly_execute(c2,pc,icv);
    }
  else if (this_state==NETWORK2_PATH_SIMPLE_STATE_SIMPLE) {
    pc->packeted_ic.sent_state[i] = 1;
    sync_states(c2,pc,icv);
    path_possibly_execute(c2,pc,icv);
    }
  }
}



void path_finished_receiving_command(struct network2_complete *c2,volatile network2_packeted_command *pcv,volatile network2_internal_command *icv) {
volatile network2_complete *cv2 = (volatile network2_complete *)c2;
unsigned char this_state = pcv->this_state;
int pn = c2->cv1->participant_number;

// block to set the command
int flush=0;
if (pcv->packeted_ic.id != icv->id) {
  flush=1;
  }
else if (icv->length != pcv->length) {
  flush=1;
  }
else if (memcmp((void *)pcv->the_command,(void *)c2->internal_command_buffers[pcv->packeted_ic.id],pcv->length) !=0) {
  flush=1;
  }

if (flush) {
  *icv = pcv->packeted_ic;
  memcpy((void *)pcv->the_command,(void *)c2->internal_command_buffers[pcv->packeted_ic.id],pcv->length);
  }




if (!flush) {
  for (int j=0;j<NUMBER_OF_NETWORK1_PARTICIPANTS;j++) {
    icv->network_order[j] = pcv->packeted_ic.network_order[j];
    }
  for (int j=0;j<NUMBER_OF_NETWORK1_PARTICIPANTS;j++) {
    if (j != pn) {
      icv->sent_state[j] |= pcv->packeted_ic.sent_state[pn];
      icv->acknowledged_state[j] |= pcv->packeted_ic.acknowledged_state[j];
      icv->executed_state[j] |= pcv->packeted_ic.acknowledged_state[j];
      icv->clear_state[j] |= pcv->packeted_ic.clear_state[j];
      }
    }
  }

int acknowledged_before = (icv->acknowledged_state[pn]==1);

// well we received it


// havent transferred the states yet
if (icv->did_states[this_state] ==0) {
  icv->did_states[this_state] = 1;
  
  pcv->packeted_ic.acknowledged_state[pn] = 1;
  icv->acknowledged_state[pn] = 1;
  pcv->packeted_ic.sent_state[pn] = 1;
  icv->sent_state[pn] = 1;
  // later have some calls that are per states
  if (this_state==NETWORK2_PATH_NOTHING_STATE_NOTHING) {
    path_possibly_execute(c2,pcv,icv);
    }
  else if (this_state==NETWORK2_PATH_SIMPLE_STATE_SIMPLE) {
    path_possibly_execute(c2,pcv,icv);
    }
  }
}






void path_finished_sending_redo(struct network2_complete *c2,volatile network2_packeted_command *pcv,volatile network2_internal_command *icv) {
unsigned char this_state = pcv->this_state;
if (icv->did_states[this_state] ==0) {
  icv->did_states[this_state] = 1;
  // later have some calls that are per states
  if (this_state==NETWORK2_PATH_NOTHING_STATE_NOTHING) {
    }
  else if (this_state==NETWORK2_PATH_SIMPLE_STATE_SIMPLE) {
    }
  }
}



void path_finished_receiving_redo(struct network2_complete *c2,volatile network2_packeted_command *pc,volatile network2_internal_command *icv) {
unsigned char this_state = pc->this_state;
if (icv->did_states[this_state] ==0) {
  icv->did_states[this_state] = 1;
  // later have some calls that are per states
  if (this_state==NETWORK2_PATH_NOTHING_STATE_NOTHING) {
    }
  else if (this_state==NETWORK2_PATH_SIMPLE_STATE_SIMPLE) {
    }
  }
}





void parse_internal_command_state(volatile network2_internal_command *icv,volatile unsigned char *x) {
unsigned char send = *x;
if (send &1) icv->sent_state[0] = 1;
if (send &2) icv->sent_state[1] = 1;
if (send &4) icv->sent_state[2] = 1;
if (send &8) icv->sent_state[3] = 1;
if (send &16) icv->sent_state[4] = 1;
if (send &32) icv->sent_state[5] = 1;
x++;

unsigned char ack = *x;
if (ack &1) icv->acknowledged_state[0] = 1;
if (ack &2) icv->acknowledged_state[1] = 1;
if (ack &4) icv->acknowledged_state[2] = 1;
if (ack &8) icv->acknowledged_state[3] = 1;
if (ack &16) icv->acknowledged_state[4] = 1;
if (ack &32) icv->acknowledged_state[5] = 1;
x++;

unsigned char executed = *x;
if (executed &1) icv->executed_state[0] = 1;
if (executed &2) icv->executed_state[1] = 1;
if (executed &4) icv->executed_state[2] = 1;
if (executed &8) icv->executed_state[3] = 1;
if (executed &16) icv->executed_state[4] = 1;
if (executed &32) icv->executed_state[5] = 1;
x++;

unsigned char clear = *x;
if (clear &1) icv->clear_state[0] = 1;
if (clear &2) icv->clear_state[1] = 1;
if (clear &4) icv->clear_state[2] = 1;
if (clear &8) icv->clear_state[3] = 1;
if (clear &16) icv->clear_state[4] = 1;
if (clear &32) icv->clear_state[5] = 1;
x++;


}




static int getshort(volatile unsigned char *x) {
unsigned int a=*x;
unsigned int b=x[1];
unsigned int c=b*256 + a;
return c;
}

int parse_next_packeted_command(
 struct network2_complete *c2,int bind_id,volatile unsigned char *x,int *ppos, int maxlen, volatile network2_packeted_command *pcv)  {


   
memset((void *)pcv,0,sizeof(network2_packeted_command));
pcv->bind_id = bind_id;

volatile network2_internal_command *icv = &(pcv->packeted_ic);

if (*ppos >=maxlen) {
  return 0;
  }

int pn = c2->cv1->participant_number;

if ( (x[*ppos] > NETWORK2_HIGHEST_COMMAND) ) {


  if (1==0) {
    badlength: fprintf(stderr,"bad length on a packet\n");
     goto simpleq;
    badheader: fprintf(stderr,"bad hwader witht he length too small\n");
      goto simpleq;
    badnetworkorder: fprintf(stderr,"bad network order\n");
      goto simpleq;
    badstate: fprintf(stderr,"bad assigned state out of bounds\n");
      goto simpleq;
    badcommandid: fprintf(stderr,"bad command id\n");
      goto simpleq;

    simpleq: 
    goto simple;
      }

simple:
  icv->length = maxlen-*ppos +3;
  icv->id = -1;
  pcv->code = 2;
  icv->code = 2;
  
  int no=0;
  int receiving=0;
  int sending=0;
  if (bind_id < NUMBER_OF_NETWORK1_PARTICIPANTS) {
    icv->network_order[no++]=bind_id;
    icv->network_order[no++]=pn;
    receiving=1;
    sending=0;
    } 
  else {
    icv->network_order[no++]=pn;
    icv->network_order[no++]=bind_id - NUMBER_OF_NETWORK1_PARTICIPANTS;
    sending=1;
    receiving=0;
    }
  for (int j=no;j<NUMBER_OF_NETWORK1_PARTICIPANTS;j++) {
    icv->network_order[j]=-1;
    }
    
  for (int j=0;j<NUMBER_OF_NETWORK1_PARTICIPANTS;j++) {
    icv->sent_state[j]=0;
    icv->acknowledged_state[j]=0;
    icv->executed_state[j]=0;
    icv->clear_state[j]=0;
    }
  for (int j=0;j<NETWORK2_STATE_LENGTH;j++) {
    icv->did_states[j]=0;
    }
  
  pcv->length = maxlen - *ppos;
  pcv->this_state = NETWORK2_PATH_SIMPLE_STATE_SIMPLE;
  memcpy((void *)pcv->the_command,(void *)(x+*ppos),pcv->length);
  *ppos += pcv->length;
  return 1;
  }
 

icv->code = x[*ppos];
pcv->code = icv->code;
  
if (icv->code == 1) { // ctrl-a
  unsigned int intlen;
  if (*ppos+2 > maxlen) {goto badheader;}
    intlen = getshort(x+*ppos+1);
  if ((intlen + *ppos) > maxlen) {goto badlength;}
  if (intlen < 10) {goto badheader;}
  icv->length = intlen-10;
  
  unsigned int  command_id = getshort(x+*ppos+3);
  if ((command_id < 0)||(command_id>=NETWORK2_NUMBER_COMMANDS)) {goto badcommandid;}
  icv->id = command_id;
  
  


  // estimate network order  
  int no=0;
  int receiving=0;
  int sending=0;
  if (bind_id < NUMBER_OF_NETWORK1_PARTICIPANTS) {
    icv->network_order[no++]=bind_id;
    icv->network_order[no++]=pn;
    receiving=1;
    sending=0;
    } 
  else {
    icv->network_order[no++]=pn;
    icv->network_order[no++]=bind_id - NUMBER_OF_NETWORK1_PARTICIPANTS;
    sending=1;
    receiving=0;
    }
  for (int j=no;j<NUMBER_OF_NETWORK1_PARTICIPANTS;j++) {
    icv->network_order[j]=-1;
    }
  
  

  parse_internal_command_state(icv,(volatile unsigned char *) (x+*ppos+5));
  
  // estimate
  for (int j=0;j<NETWORK2_STATE_LENGTH;j++) {
    icv->did_states[j]=0;
    }
    
  unsigned char current_position = x[*ppos+9];
  if (current_position>=NETWORK2_STATE_LENGTH) { goto badstate;}
  
  icv->did_states[current_position]=1;
  
  pcv->length = icv->length;
  pcv->this_state = current_position;
  memcpy((void *)pcv->the_command,(void *)(x+*ppos+10),pcv->length);
  *ppos += icv->length+10;
  return 1;
  } // ctrl-a acknowledge




if (icv->code == 2) { // ctrl-b
  unsigned int intlen;
  if (*ppos+2 > maxlen) {goto badheader;}
    intlen = getshort(x+*ppos+1);
  if ((intlen + *ppos) > maxlen) {goto badlength;}
  if (intlen < 3) {goto badheader;}
  icv->length = intlen-3;
  
  int no=0;
  int receiving=0;
  int sending=0;
  if (bind_id < NUMBER_OF_NETWORK1_PARTICIPANTS) {
    icv->network_order[no++]=bind_id;
    icv->network_order[no++]=pn;
    receiving=1;
    sending=0;
    } 
  else {
    icv->network_order[no++]=pn;
    icv->network_order[no++]=bind_id - NUMBER_OF_NETWORK1_PARTICIPANTS;
    sending=1;
    receiving=0;
    }
  for (int j=no;j<NUMBER_OF_NETWORK1_PARTICIPANTS;j++) {
    icv->network_order[j]=-1;
    }
    
        
  for (int j=0;j<NUMBER_OF_NETWORK1_PARTICIPANTS;j++) {
    icv->sent_state[j]=0;
    icv->acknowledged_state[j]=0;
    icv->executed_state[j]=0;
    icv->clear_state[j]=0;
    }
  for (int j=0;j<NETWORK2_STATE_LENGTH;j++) {
    icv->did_states[j]=0;
    }
  
  pcv->length = icv->length;
  pcv->this_state = NETWORK2_PATH_SIMPLE_STATE_SIMPLE;
  memcpy((void *)pcv->the_command,(void *)(x+*ppos+3),pcv->length);
  *ppos += icv->length+3;
  return 1;
  }





  
if (icv->code == 3) { // ctrl-c
  unsigned int intlen;
  if (*ppos+2 > maxlen) {goto badheader;}
    intlen = getshort(x+*ppos+1);
  if ((intlen + *ppos) > maxlen) {goto badlength;}
  if (intlen < 16) {goto badheader;}
  icv->length = intlen-16;

    
  unsigned int  command_id = getshort(x+*ppos+3);
  if ((command_id < 0)||(command_id>=NETWORK2_NUMBER_COMMANDS)) {goto badcommandid;}
  icv->id = command_id;
  
  for (int j=0;j<NUMBER_OF_NETWORK1_PARTICIPANTS;j++) {
    icv->network_order[j]=x[*ppos+5+j];
    if ((icv->network_order[j]==255)||
         ((icv->network_order[j]>=0)&&
          (icv->network_order[j]<NUMBER_OF_NETWORK1_PARTICIPANTS)))  {
      }
    else { goto badnetworkorder;}
    }
	
  parse_internal_command_state(icv,(volatile unsigned char *) (x+*ppos+5));
  
  // estimate
  for (int j=0;j<NETWORK2_STATE_LENGTH;j++) {
    icv->did_states[j]=0;
    }
    
  unsigned char current_position = x[*ppos+15];
  if (current_position>=NETWORK2_STATE_LENGTH) { goto badstate;}
  
  icv->did_states[current_position]=1;
        
  pcv->length = icv->length;
  pcv->this_state = current_position;
  memcpy((void *)pcv->the_command,(void *)(x+*ppos+16),pcv->length);
  *ppos += icv->length+16;
  return 1;
  }




if (icv->code == 5) { // ctrl-e
  unsigned int intlen;
  if (*ppos+2 > maxlen) {goto badheader;}
    intlen = getshort(x+*ppos+1);
  if ((intlen + *ppos) > maxlen) {goto badlength;}
  if (intlen < 5) {goto badheader;}
  icv->length = intlen-5;
  
  unsigned int  command_id = getshort(x+*ppos+3);
  if ((command_id < 0)||(command_id>=NETWORK2_NUMBER_COMMANDS)) {goto badcommandid;}
  icv->id = command_id;
  


  // estimate network order  
  int no=0;
  int receiving=0;
  int sending=0;
  if (bind_id < NUMBER_OF_NETWORK1_PARTICIPANTS) {
    icv->network_order[no++]=bind_id;
    icv->network_order[no++]=pn;
    receiving=1;
    sending=0;
    } 
  else {
    icv->network_order[no++]=pn;
    icv->network_order[no++]=bind_id - NUMBER_OF_NETWORK1_PARTICIPANTS;
    sending=1;
    receiving=0;
    }
  for (int j=no;j<NUMBER_OF_NETWORK1_PARTICIPANTS;j++) {
    icv->network_order[j]=-1;
    }
  
  
  // estimate
  for (int j=0;j<NETWORK2_STATE_LENGTH;j++) {
    icv->did_states[j]=0;
    }
    
  unsigned char current_position = x[*ppos+5];
  if (current_position>=NETWORK2_STATE_LENGTH) { goto badstate;}
  
  icv->did_states[current_position]=1;
  
  pcv->length = icv->length;
  pcv->this_state = current_position;
  memcpy((void *)pcv->the_command,(void *)(x+*ppos+5),pcv->length);
  *ppos += icv->length+5;
  return 1;
  } // ctrl-e acknowledge



}













void network2_handle_receive_buffer_in_receive_thread(struct network2_complete *c2,int i,int n,int round) {
volatile network1_complete *cv1 = c2->cv1;
volatile network2_complete *cv2 = (volatile network2_complete *) c2;




if (cv1->buffers[i][0] <= NETWORK2_HIGHEST_COMMAND ) {
  network2_packeted_command ppc;
  network2_packeted_command *pc=&ppc;
  network2_internal_command *this_ic = &pc->packeted_ic;
  
  int pos=0;
  int maxlen = cv1->buflen[i];
  
  while (parse_next_packeted_command(c2,i,(volatile unsigned char *)cv1->buffers[i],&pos,maxlen,&ppc)) {
    if (this_ic->id == 255) {
      path_queue_up(c2,(volatile network2_packeted_command *)pc,i,c2->participant_number);
      }
    else {
      volatile network2_internal_command *icv=c2->internal_commands + this_ic->id;
      if (pc->code==1) { // send acknowledgement
        path_finished_receiving_acknowlegement(c2,pc,icv);
        }
      else if (pc->code==3) { // send acknowledgement
        path_finished_receiving_command(c2,pc,icv);
        }
      else if (pc->code==5) { // send acknowledgement
        path_finished_receiving_redo(c2,pc,icv);
        }
      }
    } // while parsing
  }												
else {
  network2_packeted_command ppc;
  network2_packeted_command *pc=&ppc;
  network2_internal_command *this_ic = &pc->packeted_ic;
  memset((void *)(pc),0,sizeof(network2_packeted_command));
  this_ic->length= cv1->buflen[i];
  this_ic->id=255;
  this_ic->code=2;
  pc->length = this_ic->length;
  pc->this_state = NETWORK2_PATH_NOTHING_STATE_NOTHING;
  memcpy((void *)pc->the_command,(void *)cv1->buffers[i],pc->length);
  pc->bind_id = i;
  
  path_queue_up(c2,(volatile network2_packeted_command *)pc,i,c2->participant_number);
  }

#ifdef oldway
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
cv2->poll_state[i] = 5;

cv1->buflen[i]=0;
cv1->poll_state[i]=3;
cv1->call_rounds[i]=0;
// call the three cv2nonpoll rounds
int called=0;
  if (cv2->network2_handle_action_round1[i]) {
      (*cv2->network2_handle_action_round1[i])(c2,i,1); 
      called=1;
      }     
  if (cv2->network2_handle_action_round2[i]) {
            (*cv2->network2_handle_action_round2[i])(c2,i,1); 
      called=1;
      }     
  if (cv2->network2_handle_action_round3[i]) {
            (*cv2->network2_handle_action_round3[i])(c2,i,1); 
      called=1;
      }     
//if (!called) cv2->call_rounds[i]=1;
#endif
// hopefully c2 is back to 3, but we dont know
}







void network2_handle_done_send_buffer_thread(struct network2_complete *c2,int i,int n,int round) {  // handles all packets as received commands , will call round 1,2,3 here, round2 in will call round 2,3
struct network1_complete *c1 = c2->c1;
volatile struct network1_complete *cv1 = (volatile struct network1_complete *) c2->cv1;
volatile struct network2_complete *cv2 = (volatile struct network2_complete *) c2;


if (cv1->buffers[i][0] <= NETWORK2_HIGHEST_COMMAND ) {
  network2_packeted_command ppc;
  network2_packeted_command *pc=&ppc;
  network2_internal_command *this_ic = &pc->packeted_ic;
  
  int pos=0;
  int maxlen = cv1->buflen[i];
  
  while (parse_next_packeted_command(c2,i,(volatile unsigned char *)cv1->buffers[i],&pos,maxlen,&ppc)) {
    if (this_ic->id == 255) {
      path_queue_up(c2,(volatile network2_packeted_command *)pc,i,c2->participant_number+NUMBER_OF_NETWORK1_PARTICIPANTS);
    
#ifdef oldway
      if (cv2->poll_state[i]<3) {
        cv2->poll_state[i]=3;
        }
      else if (cv2->poll_state[i]==4) {
        goto allrighti;
        }
      while (cv2->poll_state[i]!=3) {
        nsleep();
        }
      /* set t2 to be like c1 */
      allrighti:
  
      if ((cv2->buffers[i])) {
        memcpy((void *)c2->buffers[i],(void *)pc->the_command,pc->length);
        cv2->buflen[i] = pc->length;
        cv2->poll_state[i]=5;
        int called=0;    
        if (c2->network2_handle_action_round1[i]) {
          (*c2->network2_handle_action_round1[i])(c2,i,1); 
          called=1;
	  }
        if (c2->network2_handle_action_round2[i]) {
          (*c2->network2_handle_action_round2[i])(c2,i,1); 
	  called=1;
          }
        if (c2->network2_handle_action_round3[i]) {
          (*c2->network2_handle_action_round3[i])(c2,i,1); 
	  called=1;
          }
        while (cv2->poll_state[i]!=3) {
          nsleep();
          }
        } // should be ab allocation
#endif	
      }
    else {
      volatile network2_internal_command *icv=c2->internal_commands + this_ic->id;
      if (pc->code==1) { // send acknowledgement
        path_finished_sending_acknowlegement(c2,pc,icv);
        }
      else if (pc->code==3) { // send acknowledgement
        path_finished_sending_command(c2,pc,icv);
        }
      else if (pc->code==5) { // send acknowledgement
        path_finished_sending_redo(c2,pc,icv);
        }
      }
    } // while parsing
  }												
else {
  network2_packeted_command ppc;
  network2_packeted_command *pc=&ppc;
  network2_internal_command *this_ic = &pc->packeted_ic;
  memset((void *)(pc),0,sizeof(network2_packeted_command));
  this_ic->length= cv1->buflen[i];
  this_ic->id=255;
  this_ic->code=2;
  this_ic->code=2;
  pc->length = this_ic->length;
  pc->this_state = NETWORK2_PATH_NOTHING_STATE_NOTHING;
  memcpy((void *)pc->the_command,(void *)c1->buffers[i],pc->length);
  pc->bind_id = i;
  
  int pos=0;
  int maxlen = cv1->buflen[i];
  path_queue_up(c2,(volatile network2_packeted_command *)pc,i,c2->participant_number+NUMBER_OF_NETWORK1_PARTICIPANTS);

#ifdef oldway
  if (cv2->poll_state[i]<3) {
    cv2->poll_state[i]=3;
//    cv2->send_buffer_ready[i]=1;
    }
  else if (cv2->poll_state[i]==4) {
    goto allright;
    }
  while (cv2->poll_state[i]!=3) {
    nsleep();
    }
  /* set t2 to be like c1 */
  allright:
  
  if ((cv1->buffers[i])&&(cv1->buflen[i])&&(cv2->buffers[i])) {
    memcpy((void *)c2->buffers[i],(void *)c1->buffers[i],c1->buflen[i]);
    }
  cv2->buflen[i] = cv1->buflen[i];
  cv2->poll_state[i]=5;
  int called=0;
    if (c2->network2_handle_action_round1[i]) {
      (*c2->network2_handle_action_round1[i])(c2,i,1); 
      called=1;
      }
    if (c2->network2_handle_action_round2[i]) {
      (*c2->network2_handle_action_round2[i])(c2,i,1); 
      called=1;
      }
    if (c2->network2_handle_action_round3[i]) {
      (*c2->network2_handle_action_round3[i])(c2,i,1); 
      called=1;
      }
//  if (!called) cv2->call_rounds[i]=1;
  while (cv2->poll_state[i]!=3) {
    nsleep();
    }
  }
#endif
  }

// done  
cv1->call_rounds[i] = 0;
cv1->poll_state[i] = 3;  
cv1->buflen[i] = 0;

}





static void add_internal_command_state(volatile unsigned char *x,volatile network2_internal_command *ic) {
	  unsigned char send=0;
	  if (ic->sent_state[0]) send |=1;
	  if (ic->sent_state[1]) send |=2;
	  if (ic->sent_state[2]) send |=4;
	  if (ic->sent_state[3]) send |=8;
	  if (ic->sent_state[4]) send |=16;
	  if (ic->sent_state[5]) send |=32;
          *x = send;
	  x++;
	  unsigned char ack=0;
	  if (ic->acknowledged_state[0]) ack |=1;
	  if (ic->acknowledged_state[1]) ack |=2;
	  if (ic->acknowledged_state[2]) ack |=4;
	  if (ic->acknowledged_state[3]) ack |=8;
	  if (ic->acknowledged_state[4]) ack |=16;
	  if (ic->acknowledged_state[5]) ack |=32;
          *x = ack;
	  x++;
	  unsigned char executed=0;
	  if (ic->executed_state[0]) executed |=1;
	  if (ic->executed_state[1]) executed |=2;
	  if (ic->executed_state[2]) executed |=4;
	  if (ic->executed_state[3]) executed |=8;
	  if (ic->executed_state[4]) executed |=16;
	  if (ic->executed_state[5]) executed |=32;
          *x = executed;
	  x++;
          unsigned char cleared=0;
	  if (ic->clear_state[0]) cleared |=1;
	  if (ic->clear_state[1]) cleared |=2;
	  if (ic->clear_state[2]) cleared |=4;
	  if (ic->clear_state[3]) cleared |=8;
	  if (ic->clear_state[4]) cleared |=16;
	  if (ic->clear_state[5]) cleared |=32;
          *x = executed;
	  x++;
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
 
if (cv2_send) {   
  cv2->poll_state[i]=4;
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
  // cv2 is tied to this and is at 4 
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
    if (cv2->internal_position[i] + buflen > NETWORK2_COMMAND_MAX_LENGTH) {
      cv2->poll_state[i]=6; // cant do it
      return;
      }
    else {
      cv1->buffers[i][cv2->internal_position[i]] = 2; // ctrlb
      cv1->buffers[i][cv2->internal_position[i]+1] = (buflen&0xff); // lenght low
      cv1->buffers[i][cv2->internal_position[i]+2] = ((buflen>>8)&0xff); // lenght high
      memcpy((void *)(c1->buffers[i]+cv2->internal_position[i]+3),(void *)c2->buffers[i],c2->buflen[i]);
      cv2->internal_position[i]+=buflen;
      }
    }
    
    {
    for (int j=0;j<NUMBER_OF_NETWORK1_PARTICIPANTS_TIMES_2;j++) {
      volatile network2_queue *q = &(cv2->queues_from_poll_to_send[j][i]);
      while (q->head != q->tail) {
        int head = q->head;
        volatile network2_packeted_command *pcv = q->packeted_commands[head];
	int command_id = pcv->packeted_ic.id;
	
	if (command_id==-1) {
          unsigned short buflen =  pcv->length + 1 + 2; // ctrlb then length low hi then characters - len includes all
          if (cv2->internal_position[i] + buflen > NETWORK2_COMMAND_MAX_LENGTH) {
            break;
  	    }
 	  /* need to send it as a 2 command */
          cv1->buffers[i][cv2->internal_position[i]] = 2; // ctrlb
          cv1->buffers[i][cv2->internal_position[i]+1] = (buflen&0xff); // lenght low
          cv1->buffers[i][cv2->internal_position[i]+2] = ((buflen>>8)&0xff); // lenght high
          memcpy((void *)(c1->buffers[i]+cv2->internal_position[i]+3),(void *)pcv->the_command,pcv->length);
          cv2->internal_position[i]+=buflen;
	  }
	
	
	volatile network2_internal_command *ic = &(cv2->internal_commands[command_id]);
	volatile unsigned char * internal_command_buffers = &(cv2->internal_command_buffers[command_id][0]);
	volatile char command_code = pcv->packeted_ic.code;
	
	int do_it=0;
	
	unsigned short buflen = ic->length;
	if (command_code==3) { buflen += 1 + 2 + 2  + 6 + 4 +1; } // ctrl-d len cmdnum 0 1 2 3 4 5 order of sending send ack executed delete state_number command }
	else if (command_code == 1) { buflen = 1 +  2 +  2 + 1 + 1 + 1 + 1; } // ctrla len2 cmdnum2 sent ack exe clear 
	else if (command_code == 5) { buflen = 1 +  2 +  2; } // ctrle len2 cmdnum 
	else { // unknown command code - erase it 
	  int oldhead = head;
	  head = (head+1) % NETWORK2_QUEUE_SIZE;
	  q->head = head;
	  free((void *)q->packeted_commands[oldhead]);
	  q->packeted_commands[oldhead]=NULL;
	  continue;
	  }
	if (buflen + cv2->internal_position[i] > NETWORK2_COMMAND_MAX_LENGTH) {
          break; // try the next queue
	  }
        cv1->buffers[i][cv2->internal_position[i]] = command_code; // ctrla or ctrlb or ctrle
        cv1->buffers[i][cv2->internal_position[i]+1] = (buflen&0xff); // lenght low
        cv1->buffers[i][cv2->internal_position[i]+2] = ((buflen>>8)&0xff); // lenght high
        cv1->buffers[i][cv2->internal_position[i]+3] = (command_id&0xff); // command id low
        cv1->buffers[i][cv2->internal_position[i]+4] = ((command_id>>8)&0xff); // command id high
        if (command_code==3) { // ctrlc
          cv1->buffers[i][cv2->internal_position[i]+5] = ic->network_order[0];
          cv1->buffers[i][cv2->internal_position[i]+6] = ic->network_order[1];
          cv1->buffers[i][cv2->internal_position[i]+7] = ic->network_order[2];
          cv1->buffers[i][cv2->internal_position[i]+8] = ic->network_order[3];
          cv1->buffers[i][cv2->internal_position[i]+9] = ic->network_order[4];
          cv1->buffers[i][cv2->internal_position[i]+10] = ic->network_order[5];
	  add_internal_command_state(cv1->buffers[i]+cv2->internal_position[i]+11,ic);
	  cv1->buffers[i][cv2->internal_position[i]+15] = pcv->this_state;
	  memcpy((void *)(c1->buffers[i]+cv2->internal_position[i]+16),((void *)internal_command_buffers),ic->length);
          cv2->internal_position[i]+=buflen;
          } // if we are ctrlc
	else if (command_code==1) {  // ctrla
	  add_internal_command_state(cv1->buffers[i]+cv2->internal_position[i]+5,ic);
	  cv1->buffers[i][cv2->internal_position[i]+9] = pcv->this_state;
          cv2->internal_position[i]+=buflen;
          }
	else if (command_code==5) {  // ctrle
          cv2->internal_position[i]+=buflen;
          }
	int oldhead = head;
	head = (head+1) % NETWORK2_QUEUE_SIZE;
	q->head = head;
	free((void *)q->packeted_commands[oldhead]);
	q->packeted_commands[oldhead]=NULL;
	} // while entries in this queue to do
      } // for all queues
    }
  if (cv2->internal_position[i]) {
    cv1->buflen[i]=cv2->internal_position[i];
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
 //     cv2->send_buffer_ready[i] = cv1->send_buffer_ready[i];
      if ((cv1->buffers[i])&&(cv1->buflen[i])&&(cv2->buffers[i])) {
        memcpy((void *)c2->buffers[i],(void *)c1->buffers[i],c1->buflen[i]);
        }
      cv2->buflen[i] = cv1->buflen[i];
      cv2->poll_state[i] = 5;
//      cv2->call_rounds[i]=1;
      (*c2->network2_handle_action_round1[i])(c2,i,1); 
//      cv1->call_rounds[i] = cv2->call_rounds[i];
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
  {
      if (cv1->poll_state[i]>=6) {
      // ignore error 
      cv1->buflen[0];
      cv1->poll_state[i]=3;
      cv1->call_rounds[i]=0;
      }
    else if (cv1->poll_state[i]==5) {
      cv1->call_rounds[i]=0; // dont look at it
      if (cv2->poll_state[i]<3) {
        cv2->poll_state[i]=3;
        }
      else if (cv2->poll_state[i]==4) {
        cv2->poll_state[i]=5;
	goto okgo;
	}
      
      while ((cv2->poll_state[i]!=3)||(cv2->buflen==0)) {
	nsleep();
	}
      /* set t2 to be like c1 */
      okgo:
      
      
      
//      cv2->send_buffer_ready[i] = cv1->send_buffer_ready[i];
      if ((cv1->buffers[i])&&(cv1->buflen[i])&&(cv2->buffers[i])) {
        memcpy((void *)c2->buffers[i],(void *)c1->buffers[i],c1->buflen[i]);
        }
      cv2->poll_state[i] = 5;
      cv2->buflen[i] = cv1->buflen[i];
      
      int called=0;             
      if (c2->network2_handle_action_round1[i]) { 
        (*c2->network2_handle_action_round1[i])(c2,i,1);
	called=1;
	} 
      if (c2->network2_handle_action_round2[i]) { 
        (*c2->network2_handle_action_round2[i])(c2,i,1);
	called=1;
	} 
      if (c2->network2_handle_action_round3[i]) { 
        (*c2->network2_handle_action_round3[i])(c2,i,1);
	called=1;
	} 
//      if (!called) cv2->call_rounds[i]=1;
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
    cv1->call_rounds[i]=0; // we are in stasis
    network2_handle_done_send_buffer_thread(c2,i,n,1);  // the top level send is changed from 4 to 3, and we set cv2->poll state to 5 if it was 4
    /* but for all the send records in this send (via parsing just like the recv parser, except that we also might parse the status record also makk the sent field to the five writer
         so if cv2->status was 4, set cv2 status to 5 or 6 or whatever.  for now, we can probably skip it. */
    /* this could block the receive thread until the poll handles things, which could be multiple times , or maybe this is also handled in the receive thread */
    }
  }
//aacv1->buflen[i]=0;
//aacv1->poll_state[i]=3;
//aacv1->call_rounds[i]=0;
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

  c2->call_rounds[i]= 0;
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
  c2->next_command_id=c1->participant_number;
  
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
int ff=0;



//network2_reset_sendables_and_stuff(c);

int number_to_round = network1_poll_check_nosleep(c1);


//compute_sendable_recieveables(c,1);
//process_poll_buffer_statuses(c,0);

number_to_round = 0;

for (i=0;i<cv1->current_number_of_polls;i++) {
  cv2->call_rounds[i]=0;
  if (cv2->poll_state[i]<3) {
    cv2->poll_state[i]=3;
    }
  if (cv2->poll_state[i]>=5) {
    cv2->call_rounds[i]=1;
    }
  if (cv2->call_rounds[i]) number_to_round++;
  }  

      

for (i=0;i<cv1->current_number_of_polls;i++) {
  if (cv2->call_rounds[i]) {    
    if  (cv2->network2_handle_action_round1_poll[i]) { (*c2->network2_handle_action_round1_poll[i])(c2,i,number_to_round);  }
    }
  } /* while loopint through all events */
  
int pn = c2->cv1->participant_number; 

for (int j=0;j<NUMBER_OF_NETWORK1_PARTICIPANTS_TIMES_2;j++) {
  for (i=pn;i<NUMBER_OF_NETWORK1_PARTICIPANTS_TIMES_2;i+=NUMBER_OF_NETWORK1_PARTICIPANTS) {
    volatile network2_queue *q = &(cv2->queues_from_poll_to_send[j][i]);
    while (q->head != q->tail) {
      int head = q->head;
      volatile network2_packeted_command *pcv = q->packeted_commands[head];
      int bind_id = pcv->bind_id;
      if (cv2->poll_state[bind_id]==3) {
        int command_id = pcv->packeted_ic.id;
        if (cv2->buffers[bind_id]) {
          memcpy((void *)cv2->buffers[bind_id],(void *)pcv->the_command,pcv->length);
          cv2->buflen[bind_id] = pcv->length;
          cv2->poll_state[bind_id]=5;
          if  (cv2->network2_handle_action_round1_poll[bind_id]) { 
            (*c2->network2_handle_action_round1_poll[bind_id])(c2,bind_id,number_to_round);  
            number_to_round++; 
	    int oldhead = head;
	    head = (head+1) % NETWORK2_QUEUE_SIZE;
	    q->head = head;
	    free((void *)q->packeted_commands[oldhead]);
	    q->packeted_commands[oldhead]=NULL;
	    continue;
            }  // if we called the function - maybe ignore if there is no function
          } // of we have buffers -  
        } // if we have poll state on bind id 3
      // here we cant continue on the queue, so we will break and try another queue
      } // while we should continue on the queue
    } // checking the inut and output queues to me, or from me. from me means we got a actual command, to me means we are done sending command
  } // checking each queue
#ifdef oldstuff	
	if (command_id==-1) {
          unsigned short buflen =  pcv->length + 1 + 2; // ctrlb then length low hi then characters - len includes all
          if (cv2->internal_position[i] + buflen > NETWORK2_COMMAND_MAX_LENGTH) {
            break;
  	    }
 	  /* need to send it as a 2 command */
          cv1->buffers[i][cv2->internal_position[i]] = 2; // ctrlb
          cv1->buffers[i][cv2->internal_position[i]+1] = (buflen&0xff); // lenght low
          cv1->buffers[i][cv2->internal_position[i]+2] = ((buflen>>8)&0xff); // lenght high
          memcpy((void *)(c1->buffers[i]+cv2->internal_position[i]+3),(void *)pcv->the_command,pcv->length);
          cv2->internal_position[i]+=buflen;
	  }
	
	
	volatile network2_internal_command *ic = &(cv2->internal_commands[command_id]);
	volatile unsigned char * internal_command_buffers = &(cv2->internal_command_buffers[command_id][0]);
	volatile char command_code = pcv->packeted_ic.code;
	
	int do_it=0;
	
	unsigned short buflen = ic->length;
	if (command_code==3) { buflen += 1 + 2 + 2  + 6 + 4 +1; } // ctrl-d len cmdnum 0 1 2 3 4 5 order of sending send ack executed delete state_number command }
	else if (command_code == 1) { buflen = 1 +  2 +  2 + 1 + 1 + 1 + 1; } // ctrla len2 cmdnum2 sent ack exe clear 
	else if (command_code == 5) { buflen = 1 +  2 +  2; } // ctrle len2 cmdnum 
	else { // unknown command code - erase it 
	  int oldhead = head;
	  head = (head+1) % NETWORK2_QUEUE_SIZE;
	  q->head = head;
	  free((void *)q->packeted_commands[oldhead]);
	  q->packeted_commands[oldhead]=NULL;
	  continue;
	  }
	if (buflen + cv2->internal_position[i] > NETWORK2_COMMAND_MAX_LENGTH) {
          break; // try the next queue
	  }
        cv1->buffers[i][cv2->internal_position[i]] = command_code; // ctrla or ctrlb or ctrle
        cv1->buffers[i][cv2->internal_position[i]+1] = (buflen&0xff); // lenght low
        cv1->buffers[i][cv2->internal_position[i]+2] = ((buflen>>8)&0xff); // lenght high
        cv1->buffers[i][cv2->internal_position[i]+3] = (command_id&0xff); // command id low
        cv1->buffers[i][cv2->internal_position[i]+4] = ((command_id>>8)&0xff); // command id high
        if (command_code==3) { // ctrlc
          cv1->buffers[i][cv2->internal_position[i]+5] = ic->network_order[0];
#endif

   
return number_to_round;
}




int network2_poll_check(network2_complete *c2) {
int result = network2_poll_check_nosleep(c2);
if (!result) nsleep();
return result;
}

/* end of network2.c */

																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																														      																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																														
