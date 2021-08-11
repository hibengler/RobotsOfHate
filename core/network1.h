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


struct network1_messages {
  int message_id;
  int message_known[NUMBER_OF_NETWORK1_PARTICIPANTS];
  int message_bad[NUMBER_OF_NETWORK1_PARTICIPANTS];
  int message_sent[NUMBER_OF_NETWORK1_PARTICIPANTS];
    
struct network1_complete;
  
typedef network1_complete {
  int participant_number;/* 0-4 for players, 5 for an overseer */
  int current_number_of_polls;

  char ip_addresses_string[NUMBER_OF_NETWORK1_PARTICIPANTS][20];
  struct sockaddr_in sending_to[NUMBER_OF_NETWORK1_PARTICIPANTS];
  int sent_to_ports[NUMBER_OF_NETWORK1_PARTICIPANTS];
			     
  int receiving_poll[NUMBER_OF_NETWORK1_PARTICIPANTS];  // redundant // !*
  int sending_poll[NUMBER_OF_NETWORK1_PARTICIPANTS];    // redundant // !*

  
    
        
  int buflen[MAX_NUMBER_OF_POLLS];
  char *buffers[MAX_NUMBER_OF_POLLS];



  
  
  struct timeval local_network1_check_start_time;  // time when poll_ckeck start time first called
  
  int metwork1_check_poll_runs_in_call; // 0 to  NETWORK1_POLL_CHECK_MAX_POLL_CALLS
  struct timeval local_poll_check_poll_start_time[NETWORK1_POLL_CHECK_MAX_POLL_CALLS];  // time poll was last called
  struct timeval local_poll_end_time[NETWORK1_POLL_CHECK_MAX_POLL_CALLS];  // time poll was last ended  struct timeval local_round1_end_time; // later time after round 1
  struct timeval local_round1_end_time[NETWORK1_POLL_CHECK_MAX_POLL_CALLS]; // time after round 1
  struct timeval local_round2_end_time[NETWORK1_POLL_CHECK_MAX_POLL_CALLS]; // time after round 2
  struct timeval local_round3_end_time[NETWORK1_POLL_CHECK_MAX_POLL_CALLS]; // time after round 3
  
  struct timeval local_network1_check_end_time;  // time after the entire call
  
  int broadcast_permission[MAX_NUMBER_OF_POLLS];
      
  int poll_state[MAX_NUMBER_OF_POLLS]; // 0 - not set up 1 - set up and bound , 2 -> waiting for connect or listen, 3 -> connect or listen, 4-> try to read or send*/ 
  int communicator[MAX_NUMBER_OF_POLLS]; /* player number this is from, or the monitor */ // !*
  int direction[MAX_NUMBER_OF_POLLS];  //  computed to see what the dircetion is for receiving --- then we swap it for output  // *-
  int ports[MAX_NUMBER_OF_POLLS]; // yes, this is stored here, and also in poll_addresses// !*
  int sockets[MAX_NUMBER_OF_POLLS]; // each socket for each side of a poll 
  struct sockaddr_in poll_addresses[MAX_NUMBER_OF_POLLS]; // virtual addresses through nebula - that we send or receive. has the addresses
  struct pollfd pollfds[MAX_NUMBER_OF_POLLS];
  void (*network1_action_start_round1)(struct network1_complete *,int); /* we are to start a round */
  void (*network1_handle_action_round1)(struct network1_complete *,int,int)[MAX_NUMBER_OF_POLLS]; /* action right after got a packet */
  void (*network1_action_finish_round1)(struct network1_complete *,int,int); /* we are to start a round */
  void (*network1_action_start_round2)(struct network1_complete *,int,int)[MAX_NUMBER_OF_POLLS]; /* we are to start a round */
  void (*network1_handle_action_round2)(struct network1_complete *,int,int); /* action after first set complete run */
  void (*network1_action_finish_round1)(struct network1_complete *,int,int); /* we are to start a round */
  void (*network1_action_start_round3)(struct network1_complete *,int,int); /* we are to start a round */
  void (*network1_handle_action_round3)(struct network1_complete *,int,short)[MAX_NUMBER_OF_POLLS]; /* action after second set processed */
  void (*network1_action_finish_round3)(struct network1_complete *,int); /* we are to start a round */
  void (*network1_get_new_receive_buffer)(struct network1_complete *,int); /* get a new receive buffer for this poll -  */
  void (*network1_get_new_send_buffer)(struct network1_complete *,int); /* get a new send buffer - 0 length  - just to fill up  - why? */
  int (*network1_pull_next_send_buffer_from_queue)(struct network1_complete *,int); /* get a semd buffer off the queue for this polling address - returns queue length */
  
} network1_complete;



int network1_poll_check(network1_complete *c); /* returns number of polls */



int setup_network_complete(network_complete *c,int participant_number,
     (*network1_action_start_round1)(struct network1_complete *,int),
     (*network1_handle_action_round1_in)(struct network1_complete *,int,int),(*network1_handle_action_round1_out)(struct network1_complete *,int,int),
     (*network1_action_end_round1)(struct network1_complete *,int),
     (*network1_action_start_round2)(struct network1_complete *,int),
     (*network1_handle_action_round2_in)(struct network1_complete *,int,int),(*network1_handle_action_round2_out)(struct network1_complete *,int,int),
     (*network1_action_end_round2)(struct network1_complete *,int),
     (*network1_action_start_round3)(struct network1_complete *,int),
     (*network1_handle_action_round3_in)(struct network1_complete *,int,int),(*network1_handle_action_round3_out)(struct network1_complete *,int,int),
     (*network1_action_end_round3)(struct network1_complete *,int)
     );

     
int rehearse_network(network_complete *c);  /* find the other ones */

#endif
