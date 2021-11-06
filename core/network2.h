


#ifndef NETWORK2_H 
#define NETWORK2_H 1

#include "network1.h"
#include "player.h"
    
struct network2_complete;	    
  
  
/* There are commands from too - something not c */

  
/* read in 0 6 12 18 24 30 - queue by thread for read thread

  For write -> write 36 queues from read to write
  
read - 256 packets - 6

  read -> received[a]=1
  read -> packert.acknowledged[a] |=1
    if acknowledged changed
      queue to write[a+1]  
      
    
*/      


#define NETWORK2_HIGHEST_COMMAND 5
#define NETWORK2_COMMAND_FULL_LENGTH 2048
#define NETWORK2_COMMAND_MAX_LENGTH 1400
// ^ mtu size pluss ctrld at start


#define NETWORK2_NUMBER_COMMANDS 256  
#define NETWORK2_STATE_LENGTH 18
#define NETWORK2_QUEUE_SIZE 20
#define NETWORK2_LEN 2048


struct network2_packeted_command;

typedef struct network2_queue {
int head;
int tail;
volatile struct network2_packeted_command *packeted_commands[NETWORK2_QUEUE_SIZE];
} network2_queue;


/* each sent packet is a command here
Start with round robin


A coded network command is 
ctrl-c lengthlo lengthhi 

ctrl-c lengthlo lengthhi idlo idhi sendist send2nd send3rd send4 send5 send6 state characters...
ctrl-a lengthlo lengthhi idlo idhi state extra - in ack
ctrl-b lengthlo lengthhi string -- simple string
ctrl-e lengthlo lengthhi idlo idhi  ask for the command 
ctrl-\ null - done

If different character - assume person to person command - and just execute it


ctrlc - we send our first recird 
ctrlc l0 lhi cmdlo cmdhi 6 0 0 1 2 3 4 5 1 hello
*/
typedef void (*network2_complete_round_call)(struct network2_complete *,int,int);


typedef struct network2_internal_command {
unsigned short length; // length of actual command text
unsigned short id;
unsigned char code; // command
unsigned char network_order[NUMBER_OF_NETWORK1_PARTICIPANTS];
unsigned char sent_state[NUMBER_OF_NETWORK1_PARTICIPANTS]; // send sets this after try to send to. receive sets this after received from
unsigned char acknowledged_state[NUMBER_OF_NETWORK1_PARTICIPANTS]; // recv sets this when recevied state
unsigned char executed_state[NUMBER_OF_NETWORK1_PARTICIPANTS]; // recv sets this when recevied state
unsigned char clear_state[NUMBER_OF_NETWORK1_PARTICIPANTS]; // send sets whis when attempt to clear out
unsigned char did_states[NETWORK2_STATE_LENGTH];
} network2_internal_command;



#define NETWORK2_PATH_NOTHING_STATE_NOTHING 0
#define NETWORK2_PATH_SIMPLE_STATE_SIMPLE 1
#define NETWORK2_PATH_BROADCAST_ALLACK_ALL 2 // send 0->1, then 0-2 then 0->3 then 0->4 then 0->5 then 0->6 execute,
                                             //         1->0 ack  2->0 ack 3->0 ack 4->0 ack 5->0 ack 6->0 ack 
#define NETWORK2_PATH_BROADCAST_ALLACK_ALL_ACK 3

					     

typedef struct network2_packeted_command {
network2_internal_command packeted_ic;
unsigned char bind_id;
unsigned char code;
unsigned short length; // length of actual command text
unsigned char the_command[2048];
unsigned char this_state;
} network2_packeted_command;


/* we will have standard send and receive that trumps the other send and receive one time
so if the standard send and receive is filled in, we do that, otherwise, we go through execute, then 0-5 receive actions */


typedef struct network2_complete {
  network1_complete complete1;
  network1_complete *c1;
  volatile network1_complete *cv1;
  int call_rounds[MAX_NUMBER_OF_POLLS];

  int next_command_id;
  int buflen[MAX_NUMBER_OF_POLLS]; // should work the same
  char *buffers[MAX_NUMBER_OF_POLLS]; // send it  or receive it - ctrl b or normal just add send quick. set the state as per for polling

  int poll_state[MAX_NUMBER_OF_POLLS];       // pulled from network1 poll state/ except we can add and modify this one also
  int participant_number;                   // pulled from network1->participant_nmbver
  int communicator[MAX_NUMBER_OF_POLLS];    // pulled from network1->communicatore
    
  int number_of_active_players;

  network2_internal_command internal_commands[NETWORK2_NUMBER_COMMANDS];
  unsigned char internal_command_buffers[NETWORK2_NUMBER_COMMANDS][NETWORK2_COMMAND_FULL_LENGTH];

  network2_queue queues_from_poll_to_send[NUMBER_OF_NETWORK1_PARTICIPANTS_TIMES_2][NUMBER_OF_NETWORK1_PARTICIPANTS_TIMES_2];  //send reads each one - the receive thread that
  // is the same as the current player -- that is the queu used by the poll process.  and the buffer,buflen, and state is used by any thread at level 2.
  // level1 is built up by the send stuff
  char internal_buffer[MAX_NUMBER_OF_POLLS][NETWORK2_COMMAND_FULL_LENGTH]; // these point to network1 buffers and polls - except maybe the read characters
  char internal_position[MAX_NUMBER_OF_POLLS]; // internal position - essentially buflen when sending, and next position to read from when receivieng



  network2_complete_round_call network2_handle_action_round1[MAX_NUMBER_OF_POLLS]; /* action right after got a packet */
  network2_complete_round_call network2_handle_action_round2[MAX_NUMBER_OF_POLLS]; /* action after first set complete run */
  network2_complete_round_call network2_handle_action_round3[MAX_NUMBER_OF_POLLS]; /* action after first set complete run */

  network2_complete_round_call network2_get_new_receive_buffer[MAX_NUMBER_OF_POLLS]; /* get a new receive buffer for this poll -  */
  network2_complete_round_call network2_get_new_send_buffer[MAX_NUMBER_OF_POLLS]; /* get a new send buffer - 0 length  - just to fill up  - why? */
  network2_complete_round_call network2_pull_next_send_buffer_from_queue[MAX_NUMBER_OF_POLLS]; /* get a semd buffer off the queue for this polling address - returns queue length somehow */

  /* the above callbacks are in the reader/writer threads.  The below calls are done by the poll thread */


  network2_complete_round_call network2_action_start_round1_poll; /* we are to start a round */
  network2_complete_round_call network2_handle_action_round1_poll[MAX_NUMBER_OF_POLLS]; /* action right after got a packet */
  network2_complete_round_call network2_action_finish_round1_poll; /* we are to finish a round */

  network2_complete_round_call network2_action_start_round2_poll; /* we are to start a round */
  network2_complete_round_call network2_handle_action_round2_poll[MAX_NUMBER_OF_POLLS]; /* action after first set complete run */
  network2_complete_round_call network2_action_finish_round2_poll; /* we are to finish a round */

  network2_complete_round_call network2_action_start_round3_poll; /* we are to start a round */
  network2_complete_round_call network2_handle_action_round3_poll[MAX_NUMBER_OF_POLLS]; /* action after first set complete run */
  network2_complete_round_call network2_action_finish_round3_poll; /* we are to finish a round */

  
  
 } network2_complete;





/* this initializes the network2_complete structure - so it is set up to start going.
No network calls are run, but evetything is structures 

This can be adjusted after the fact-- in the smae thread
*/
extern int network2_init (network2_complete *c2,int participant_number,char *broadcast_ip,char *ips[],
     network2_complete_round_call network2_action_start_round1_poll,
     network2_complete_round_call network2_handle_action_round1_in_poll, network2_complete_round_call network2_handle_action_round1_out_poll,
     network2_complete_round_call network2_action_end_round1_poll,
     network2_complete_round_call network2_action_start_round2_in_poll,
     network2_complete_round_call network2_handle_action_round2_in_poll, network2_complete_round_call network2_handle_action_round2_out_poll,
     network2_complete_round_call network2_action_end_round2_poll,
     network2_complete_round_call network2_action_start_round3_in_poll,
     network2_complete_round_call network2_handle_action_round3_in_poll, network2_complete_round_call network2_handle_action_round3_out_poll,
     network2_complete_round_call network2_action_end_round3_poll,
     network2_complete_round_call network2_action_round1_in,  network2_complete_round_call network2_action_round1_out,
     network2_complete_round_call network2_action_round2_in,  network2_complete_round_call network2_action_round2_out,
     network2_complete_round_call network2_action_round3_in,  network2_complete_round_call network2_action_round3_out,
     network2_complete_round_call network2_get_new_receive_buffer, /* get a new receive buffer for this poll - could be broadcast in */
     network2_complete_round_call network2_get_new_send_buffer, /* get a new send buffer - 0 length  - just to fill up  - why? */
     network2_complete_round_call network2_pull_next_send_buffer_from_queue /* get a semd buffer off the queue for this polling address - returns queue length */
     );


extern int network2_poll_check(network2_complete *c); /* returns number of polls done
This is the big one
It makes states from
0 - no socket, no bind
1 - bound
2 - pending connect to ohter peer
3 - ready to call send, or recive if the buffers are ready
4 - sending or receiving in progress. please wait
5 - sent or receipt is done, but the buffers are not changed


5 should return to 3 right after the 3 rounds of processing are done.

This assumes 6 (well, 5) inputs and 6 (well 5) outputs. We do not send to ourselves

*/




	  
		    
extern int network2_set_buffer(network2_complete *c,int bind_id,char *buffer, int buflen,int force_flag);
// returns the command number, or -1 if no can do
			                
extern int network2_add_standard_input_fd(network2_complete *c,int fd,
     network2_complete_round_call network2_handle_action_round1_in,
     network2_complete_round_call network2_handle_action_round2_in,
     network2_complete_round_call network2_handle_action_round3_in,
     network2_complete_round_call network2_get_new_receive_buffer);

int rehearse_network(network2_complete *c);  /* find the other ones */


extern int network2_path_new_command(struct network2_complete *c2,
    unsigned char *command,int length, int initial_state, int me_bind_id, int preferred_first,int preferred_next) ;
    

#endif
