

#include <stdio.h>
#include <sys/time.h>
#include <string.h>
#include <stdlib.h>


#include "silentradiancesimple.h"
#include "simple_packet_summary.h"
#include "simple_processor.h"
#include "player2.h"

#ifndef logit
#define logit(...) {fprintf(stderr,__VA_ARGS__);fprintf(stderr,"\n");}
#endif


int sent_ready=0;





int send_eveybody_action_ready(volatile hate_game *game) { // called when inside event loop
if (sent_ready==0) {
  if ((packet_summary.now_frame !=0)&&(packet_summary.now_frame != -1)) {
    int planet_id = game->my_player_id;   // could be different if the player id is not known
    for (i=0;i<6;i++) {
      volatile hate_frame *frame = game->hate_frame[game->current_frame];
      int action_id = player2_get_free_action(game,frame);
      volatile hate_action *action = frame->hate_action[action_id];
      action->length = sizeof(struct hate_action);
      action->hate_action_code = HATE_ACTION_READY_PLAYER;
      action->now_frame = packet_summary.now_frame;
      clock_gettime(CLOCK_REALTIME_COARSE,(struct timespec *)(&action->sent_local_ts));
      action->sender_time_bias = frame->current_time_bias_to_global[game->my_player_id];
      
      float beat_interval = (float)(packet_summary.beat_interval_in_microseconds)*0.000001f;
      if (beat_interval<=0.f) {beat_interval=60.f/138.f;}
      
      float start_time_min=0.f;
      
      if (frame->stats_are_useful) {
        start_time_min = frame->time_spread + beat_interval;
	}
	
      float send_to_receive_time=0.150;
      if (frame->stats_are_useful) {
        send_to_receive_time = frame->send_to_receive_time[i];
	if (send_to_reveive_time <=0.f) {
	  if ((i!=game->my_player_id)) {
            send_to_receive_time=0.150;
	    }
	  else {
	    send_to_receive_time = 0.f;
	    }
          }
      float start_time =  packet_summary->where_in_tempo + beat_interval;
      if (start_time < send_to_receive_time) {
        start_time = send_to_receive_time;
	}	
      action->asked_for_start_time = start_time;
      action->try_again_offset = beat_interval;
      action->start_time = action->asked_for_start_time;
      action->end_time = action->start_time+100000f;
      action->now_time = frame->current_time_bias_to_global[game->my_player_id];
      action->derived_received_time_bias= -frame->current_time_bias_to_global[game->my_player_id]+frame->current_time_bias_to_global[i] + send_to_receive_time;
      action->global_ts_from_receivers_view = player2_add_seconds_to_timestamp(action->sent_local_ts,(double)(action->derived_received_time_bias));

      volatile hate_thing *proposed_action_thing = &(action->proposed_action_thing);
      proposed_action_thing->global_ts = player2_add_seconds_to_timestamp(action->sent_local_ts,(double)frame->current_time_bias_to_global[game->my_player_id];
      proposed_hate_thing->controlling_player_id=game->my_player_id;
      proposed_hate_thing->controlling_planet_id=planet_id;
      
      proposed_hate_thing->subject_player_id=game->my_player_id;
      proposed_hate_thing->subject_planet_id=planet_id;
      proposed_hate_thing->subject_me_id=player2_planet_id_to_thing_id(planet_id);
      proposed_hate_thing->object_planet_id = -1;
      proposed_hate_thing->object_player_id = i;
      proposed_hate_thing->object_me_id = player2_player_id_to_thing_id(i);
      proposed_hate_thing->strength = frame->things[player2_planet_id_to_thing_id(planet_id)]->strength;
      proposed_hate_thing->action_id = action_id;
      
      if (i==game->my_player_id) {
        // execute it now for me
	action->hate_action_state = HATE_ACTION_ME;
	frame->things[player2_planet_id_to_thing_id(planet_id)]=*proposed_hate_thing;
	}
      else {
	action->hate_action_state = HATE_ACTION_NEW;
	}   
      }
    sent_ready = 1;
      
      
    }
  }
return sent_ready;
}



