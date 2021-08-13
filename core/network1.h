 #ifndef NETWORK1_H 
#define NETWORK1_H 1


#include <sys/poll.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <netdb.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <errno.h>
#include <time.h>
#include <linux/tcp.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>


#define NUMBER_OF_NETWORK1_PARTICIPANTS 6
#define NUMBER_OF_NETWORK1_PARTICIPANTS_TIMES_2 12
#define NETWORK1_START_PORT 5169
#define MAX_NUMBER_OF_POLLS 50
#define NETWORK1_POLL_CHECK_MAX_POLL_CALLS 10
#define NETWORK1_MAX_BUFFER_SIZE 2048

    
struct network1_complete;	    
  



typedef void (*network1_complete_round_call)(struct network1_complete *,int,int);


typedef struct network1_complete {
  int participant_number;/* 0-4 for players, 5 for an overseer */
  int current_number_of_polls;

  char ip_addresses_string[NUMBER_OF_NETWORK1_PARTICIPANTS][20];
  struct sockaddr_in sending_to[NUMBER_OF_NETWORK1_PARTICIPANTS];
  int sent_to_ports[NUMBER_OF_NETWORK1_PARTICIPANTS];
			     
  int receiving_poll[NUMBER_OF_NETWORK1_PARTICIPANTS];  // redundant // !*
  int sending_poll[NUMBER_OF_NETWORK1_PARTICIPANTS];    // redundant // !*

  
  int call_rounds[MAX_NUMBER_OF_POLLS];  
        
  int buflen[MAX_NUMBER_OF_POLLS];
  char *buffers[MAX_NUMBER_OF_POLLS];



  struct timeval local_delay_work[MAX_NUMBER_OF_POLLS];  // wait until this time is passed
  
  
  struct timeval local_network1_check_start_time;  // time when poll_ckeck start time first called
  
  int metwork1_check_poll_runs_in_call; // 0 to  NETWORK1_POLL_CHECK_MAX_POLL_CALLS
  struct timeval local_poll_check_poll_start_time[NETWORK1_POLL_CHECK_MAX_POLL_CALLS];  // time poll was last called
  struct timeval local_poll_end_time[NETWORK1_POLL_CHECK_MAX_POLL_CALLS];  // time poll was last ended  struct timeval local_round1_end_time; // later time after round 1
  struct timeval local_handle_poll_end_time[NETWORK1_POLL_CHECK_MAX_POLL_CALLS];  // time poll was last ended  struct timeval local_round1_end_time; // later time after round 1
  
  struct timeval local_round1_end_time[NETWORK1_POLL_CHECK_MAX_POLL_CALLS]; // time after round 1
  struct timeval local_round2_end_time[NETWORK1_POLL_CHECK_MAX_POLL_CALLS]; // time after round 2
  struct timeval local_round3_end_time[NETWORK1_POLL_CHECK_MAX_POLL_CALLS]; // time after round 3
  
  struct timeval local_network1_check_end_time;  // time after the entire call
  
  int broadcast_permission[MAX_NUMBER_OF_POLLS];
      
  int poll_state[MAX_NUMBER_OF_POLLS]; // 0 - not set up 1 - set up and bound , 2 -> waiting for connect or listen, 3 -> connect or listen, 4-> try to read or send 5 ->  5 will clear out new send buffer if it is full after the rounds, then sets to 3
                                       // so if we call set_send_outout or something like that, the calling stuff will force 3 to 5, ohterwise 5 will clear.  This allows multiple sends, well at least 2
                                       
  int communicator[MAX_NUMBER_OF_POLLS]; /* player number this is from, or the monitor */ // !*
  int direction[MAX_NUMBER_OF_POLLS];  //  computed to see what the dircetion is for receiving --- then we swap it for output  // *-
  int ports[MAX_NUMBER_OF_POLLS]; // yes, this is stored here, and also in poll_addresses// !*
  int sockets[MAX_NUMBER_OF_POLLS]; // each socket for each side of a poll 
  struct sockaddr_in poll_addresses[MAX_NUMBER_OF_POLLS]; // virtual addresses through nebula - that we send or receive. has the addresses
  struct pollfd pollfds[MAX_NUMBER_OF_POLLS];

  
  network1_complete_round_call network1_action_start_round1; /* we are to start a round */
  network1_complete_round_call network1_handle_action_round1[MAX_NUMBER_OF_POLLS]; /* action right after got a packet */
  network1_complete_round_call network1_action_finish_round1; /* we are to finish a round */
  
  network1_complete_round_call network1_action_start_round2; /* we are to start a round */
  network1_complete_round_call network1_handle_action_round2[MAX_NUMBER_OF_POLLS]; /* action after first set complete run */
  network1_complete_round_call network1_action_finish_round2; /* we are to finish a round */
  
  network1_complete_round_call network1_action_start_round3; /* we are to start a round */
  network1_complete_round_call network1_handle_action_round3[MAX_NUMBER_OF_POLLS]; /* action after first set complete run */
  network1_complete_round_call network1_action_finish_round3; /* we are to finish a round */
  
  network1_complete_round_call network1_get_new_receive_buffer; /* get a new receive buffer for this poll -  */
  network1_complete_round_call network1_get_new_end_buffer; /* get a new send buffer - 0 length  - just to fill up  - why? */
  network1_complete_round_call network1_pull_next_send_buffer_from_queue; /* get a semd buffer off the queue for this polling address - returns queue length somehow */
  
} network1_complete;







/* this initializes the network1_complete structure - so it is set up to start going.
No network calls are run, but evetything is structures 

This can be adjusted after the fact-- in the smae thread
*/
extern int network1_init (network1_complete *c,int participant_number,char *broadcast_ip,char *ips[],
     network1_complete_round_call network1_action_start_round1,
     network1_complete_round_call network1_handle_action_round1_in, network1_complete_round_call network1_handle_action_round1_out,
     network1_complete_round_call network1_action_end_round1,
     network1_complete_round_call network1_action_start_round2,
     network1_complete_round_call network1_handle_action_round2_in, network1_complete_round_call network1_handle_action_round2_out,
     network1_complete_round_call network1_action_end_round2,
     network1_complete_round_call network1_action_start_round3,
     network1_complete_round_call network1_handle_action_round3_in, network1_complete_round_call network1_handle_action_round3_out,
     network1_complete_round_call network1_action_end_round3,
     network1_complete_round_call network1_get_new_receive_buffer, /* get a new receive buffer for this poll - could be broadcast in */
     network1_complete_round_call network1_get_new_send_buffer, /* get a new send buffer - 0 length  - just to fill up  - why? */
     network1_complete_round_call network1_pull_next_send_buffer_from_queue /* get a semd buffer off the queue for this polling address - returns queue length */
     ) ;



extern int network1_poll_check(network1_complete *c); /* returns number of polls done
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



extern int network1_set_buffer(network1_complete *c,int bind_id,char *buffer, int buflen);


          
int rehearse_network(network1_complete *c);  /* find the other ones */

#endif
