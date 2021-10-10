


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
#include <netinet/in.h>
#include <arpa/inet.h>
		     

#define NUMBER_OF_NETWORK1_PARTICIPANTS 6
#define NUMBER_OF_NETWORK1_PARTICIPANTS_TIMES_2 12
#define NETWORK1_START_PORT 7000
#define MAX_NUMBER_OF_POLLS 50
#define NETWORK1_POLL_CHECK_MAX_POLL_CALLS 10
#define NETWORK1_MAX_BUFFER_SIZE 2048

    
struct network1_complete;	    
  



typedef void (*network1_complete_round_call)(struct network1_complete *,int,int);


typedef struct network1_pthread_parameters {
  volatile struct network1_complete *c;
  int bind_id;
  } network1_pthread_parameters;

typedef struct network1_complete {
  int participant_number;/* 0-4 for players, 5 for an overseer */
  int current_number_of_polls;

  char ip_addresses_string[NUMBER_OF_NETWORK1_PARTICIPANTS][20];
			     
  
  
  
  int sendable[NUMBER_OF_NETWORK1_PARTICIPANTS];  // makes it easier to know id sendable or receiveable
  int recieveable[NUMBER_OF_NETWORK1_PARTICIPANTS];  // makes it easier to know is receiveable
  int responsive[NUMBER_OF_NETWORK1_PARTICIPANTS];  // sendable & receiveable
  int countdown[NUMBER_OF_NETWORK1_PARTICIPANTS];  // if it is sendable + receiveablem there is a counddown
  int alive[NUMBER_OF_NETWORK1_PARTICIPANTS];  // if the person is alive - 
  
  int send_buffer_full[NUMBER_OF_NETWORK1_PARTICIPANTS]; // cause a delay
  int send_buffer_ready[NUMBER_OF_NETWORK1_PARTICIPANTS]; // ready for wrriting to. Just set the address
  int send_buffer_missing[NUMBER_OF_NETWORK1_PARTICIPANTS]; // no problem really.  If missing, state 3 waits until  
  int recv_buffer_full[NUMBER_OF_NETWORK1_PARTICIPANTS]; // cause a delay
  int recv_buffer_ready[NUMBER_OF_NETWORK1_PARTICIPANTS]; // ready for wrriting to. Just set the address
  int recv_buffer_missing[NUMBER_OF_NETWORK1_PARTICIPANTS]; // no problem really.  If missing, state 3 waits until

  // one shows up or calls the defalt callbacks every time just in case
  
  /* to load a certain buffer, do this:
    if send_buffer_ready - copy it, and set the lenght - good to go. - can do this early
    if send_buffer_full - got to wait
    if send_buffer_missing - bneed to set up a buffer
    
    for static:
    if(send_buffer_missing - set it then write
    if (send_buffer_ready) - write
    
    else  send_buffer+full 0 tough titties
    
    
    for dynamic:
    if (send_buffer_ready - copy or wait
    else if send+buffer_missing - set it
    else send_buffer_ready
    
    by sending the buffer in dynamic mode, it will be the first on the list
    but also you can run queues and then its cool 0 thats for network2.h
     will need things random, otherwise we are round robinning too much
    sendable, receiveable, responsive is whether or not to do network2 stuff
    
    dynamic CAN process cia mode 3 checks with the callbacks,
    or you can do things in the triggers in the rounds
    or you can do thins inbetween poll calls.
    
    
    receiver buffer is similar
    the automated system loads receiver buffers  when the receiver is fulll, by calling the triffer.
    the 5 logic does its best to assume static, and just clead the id -- only with an empty receiver will it run
    
    for sendongg - sends only when its full.,  if enpty len will try ot
    if empty - will try to pull another empty one
    
networjk_attempt_send needs it full
    
    */

      


  
        
  pthread_t network_thread[MAX_NUMBER_OF_POLLS];  // redundant // !*
  pthread_attr_t pthread_attrubutes[MAX_NUMBER_OF_POLLS];  // currently null
  struct network1_pthread_parameters pthread_parameters[MAX_NUMBER_OF_POLLS];
  
  
  int buflen[MAX_NUMBER_OF_POLLS];
  char *buffers[MAX_NUMBER_OF_POLLS];
  int temp_dont_poll_yet[MAX_NUMBER_OF_POLLS];

  int change_buffer_flag[MAX_NUMBER_OF_POLLS]; // true if the buffer is being changed by out calls, false otherwise
  int type[MAX_NUMBER_OF_POLLS]; // 0 - read from sender, 1 - send to receiver, 2 - read from standard file, 3 - write from standard file
      
  int poll_state[MAX_NUMBER_OF_POLLS]; // 0 - not set up 1 - set up and bound , 2 -> waiting for connect or listen, 3 -> connect or listen, 4-> try to read or send 
       // 5 ->  5 will clear out new send buffer if it is full after the rounds, then sets to 3 and gets a new address
       // 6 -> we just disconnected, one packet was lost - the one before from what I can see.  -- so we should wait for a bit before pumping extras down the pike
       // 7 -> we are in the 10 count. about to quit
       // 8 -> we  will reestablish or allow a new player to come in? maybe - will ping once a second
       // 9 -> ???
       // anything above 6 sets sendable to 0
       // also - any delay sets sendable to 0 -- also if 4 or 2 or 1 or 0 seet sendable to 0
       // if there are buffers to get on 3, we are good to go, otherwise set sendable to 0
       // receiveable is also 0 - these are done for players, not per poll
       
                                       // so if we call set_send_outout or something like that, the calling stuff will force 3 to 5, ohterwise 5 will clear.  This allows multiple sends, well at least 2
                                       
  int communicator[MAX_NUMBER_OF_POLLS]; /* player number this is from, or the monitor */ // !*
  int ports[MAX_NUMBER_OF_POLLS]; // yes, this is stored here, and also in poll_addresses 
  
  struct sockaddr_in sending_to[MAX_NUMBER_OF_POLLS];
  int sent_to_ports[MAX_NUMBER_OF_POLLS];
  
  
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
  
  network1_complete_round_call network1_get_new_receive_buffer[MAX_NUMBER_OF_POLLS]; /* get a new receive buffer for this poll -  */
  network1_complete_round_call network1_get_new_send_buffer[MAX_NUMBER_OF_POLLS]; /* get a new send buffer - 0 length  - just to fill up  - why? */
  network1_complete_round_call network1_pull_next_send_buffer_from_queue[MAX_NUMBER_OF_POLLS]; /* get a semd buffer off the queue for this polling address - returns queue length somehow */
  
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



extern int network1_set_buffer(network1_complete *c,int bind_id,char *buffer, int buflen,int force_flag);


          
extern int network1_add_standard_input_fd(network1_complete *c,int fd,
     network1_complete_round_call network1_handle_action_round1_in,
     network1_complete_round_call network1_handle_action_round2_in,
     network1_complete_round_call network1_handle_action_round3_in,
     network1_complete_round_call network1_get_new_receive_buffer);

int rehearse_network(network1_complete *c);  /* find the other ones */

#endif
