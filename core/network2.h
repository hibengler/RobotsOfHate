


#ifndef NETWORK2_H 
#define NETWORK2_H 1

#include "network1.h"
#include "player.h"
    
struct network2_complete;	    
  

#define NETWORK2_QUEUE_SIZE 20
#define NETWORK2_QUEUE_LEN 2048
typedef struct network2_buffer_queue {
int head;
int tail;
unsigned char buffer[NETWORK2_QUEUE_SIZE][NETWORK2_QUEUE_LEN];
} network2_buffer_queue;


typedef void (*network2_complete_round_call)(struct network2_complete *,int,int);

typedef struct network2_hate_rps { // rock, paper,scissors
float rps[3];
} hate_rps;

typedef struct network2_hate_robot {
  uchar mode; // attack, defend, neutral
  uchar current_planet;
  uchar destination_planet;
  uchar position;  // position to destination, if on destination, that planet controlls the batt
  hate_rps strength;
  } network2_hate_robot;
	  

typedef struct network2_hate_planet {
  uchar mode; // disconnected, playing, lobby
  uchar player_id; /* 0-5 */
  struct network2_hate_robot robots[HATE_NUMBER_ROBOTS];
  } network2_hate_player;
      
typedef struct network2_hate_player {  // players from a network2 point of view.
  uchar mode; // disconnected, playing, lobby
  uchar player_id; /* 0-5 */
  } network2_hate_player;
      

struct network2_hate_planet_scene { // part of the info for the hate scene - each planet is statted out
unsigned int global_frame;
unsigned int player_frame;
unsigned int local_frame;
unsigned char broadcast_count; /* 0 - original, 1 - 1 frame away, 2 - 2 frame away, 3 - 3 frame away, etc */
unsigned char from_player;
unsigned char to1;
unsigned char to2;
unsigned char to3;
unsigned char seenby;
network2_hate_planet planet;
} network2_hate_player_scene;




typedef struct network2_hate_scene { 
unsigned short lenth;
unsigned char type; /* s - scene */
unsigned int global_frame;
unsigned int player_frame;
unsigned int local_frame;
unsigned char broadcast_count; /* 0 - original, 1 - 1 frame away, 2 - 2 frame away, 3 - 3 frame away, etc -1, not useful */
unsigned char to1;
unsigned char to2;
unsigned char to3;
unsigned char seenby;
network2_hate_player players[NUMBER_OF_NETWORK1_PARTICIPANTS];
network2_hate_planet_scene scenes[NUMBER_OF_NETWORK1_PARTICIPANTS];
} network2_hate_scene;



typedef struct network2_command {
unsigned short lenth;
unsigned char type; /* c - command */
unsigned char extra; 
unsigned int global_frame;
unsigned int player_frame;
unsigned int local_frame;
unsigned char broadcast_count; /* 0 - original, 1 - 1 frame away, 2 - 2 frame away, 3 - 3 frame away, etc -1, not useful */
unsigned char to1;
unsigned char to2;
unsigned char to3;
unsigned char seenby;
unsigned char completeby;
} network2_command_status;


typedef struct network2_command_status {

} network2_command_status;


typedef struct network2_complete {
  network1_complete complete1;
  network1_complete *c1;
  volatile network1_complete *cv1;
  int call_rounds[MAX_NUMBER_OF_POLLS];
    
  int number_of_active_players;
  network2_buffer_queue receive_queues[NUMBER_OF_NETWORK1_PARTICIPANTS];
  unsigned char player_is_active[NUMBER_OF_NETWORK1_PARTICIPANTS];  
  struct network2_hate_scene hate_scenes[NUMBER_OF_NETWORK1_PARTICIPANTS];
  struct network2_hate_scene previous_hate_scene; /* built by averaging the others, controls the display somewhat */
  struct network2_hate_scene latest_hate_scene; /* built by averaging the others, controls the display somewhat */
 } network2_complete;





/* this initializes the network1_complete structure - so it is set up to start going.
No network calls are run, but evetything is structures 

This can be adjusted after the fact-- in the smae thread
*/
extern int network2_init (network2_complete *c,int participant_number,char *broadcast_ip,char *ips[],
     network2_complete_round_call network2_action_start_round1,
     network2_complete_round_call network2_handle_action_round1_in, network2_complete_round_call network2_handle_action_round1_out,
     network2_complete_round_call network2_action_end_round1,
     network2_complete_round_call network2_action_start_round2,
     network2_complete_round_call network2_handle_action_round2_in, network2_complete_round_call network2_handle_action_round2_out,
     network2_complete_round_call network2_action_end_round2,
     network2_complete_round_call network2_action_start_round3,
     network2_complete_round_call network2_handle_action_round3_in, network2_complete_round_call network2_handle_action_round3_out,
     network2_complete_round_call network2_action_end_round3,
     network2_complete_round_call network2_get_new_receive_buffer, /* get a new receive buffer for this poll - could be broadcast in */
     network2_complete_round_call network2_get_new_send_buffer, /* get a new send buffer - 0 length  - just to fill up  - why? */
     network2_complete_round_call network2_pull_next_send_buffer_from_queue /* get a semd buffer off the queue for this polling address - returns queue length */
     ) ;



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



extern int network2_send_command(network2_complete *c,char *command,int need_everyone_to_confirm);

	  
		    
extern int network2_set_buffer(network2_complete *c,int bind_id,char *buffer, int buflen,int force_flag);
// returns the command number, or -1 if no can do
			                
extern int network2_add_standard_input_fd(network2_complete *c,int fd,
     network2_complete_round_call network2_handle_action_round1_in,
     network2_complete_round_call network2_handle_action_round2_in,
     network2_complete_round_call network2_handle_action_round3_in,
     network2_complete_round_call network2_get_new_receive_buffer);

int rehearse_network(network2_complete *c);  /* find the other ones */

#endif
