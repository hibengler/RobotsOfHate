#ifndef PLAYER2_NETWORK1_H

#include "player2.h"
#include "network1.h"



#define PLAYER2_NETWORK1_MAX_PACKET 1500



typedef struct p2n1_action_broadcast {
char order[6];
unsigned short action_id;
unsigned char mode_sent[18];
unsigned char mode_received[18];
unsigned char mode_acknowledged[18];

struct timespec local_ts local_sents[18];
struct timespec local_ts local_receives[18];
float local_to_global_offsets[6];

typedef struct p2n1_network_game {
  struct network1_complete *c;
  volatile struct network1_complete *cv;
  hater_game *game;
  
  char recv_buffers[6][2048];
  char send_buffers[6][2048];
  unsigned short recv_working_size[6];
  unsigned short send_working_size[6];
  
  
  
  
  } p2n1_network_game;
  

typedef struct p2n1_buffer {
int 



/* receive section - until action is in,  the action is not looked at.
derive section copied from receive section. then receive section is freed from derived.
Then clear out in derived, and process the forms that are new. , and derived.
Then build send v1, send v2, for those that can be sent, if pending, queue up on next one.


	mode	PREREQ	from	to	expect			actual
mode	0		0	1	0	
	1		0	2	0 1 	
	2	0	1	3	0 1 2			0
	3	1	2	3	0 2 3			0 2 
	4	2 3	
	5
	6
	7
	8
	9
	10
	11
	12
	13
	14
	15
	16
	17
	18



*/



