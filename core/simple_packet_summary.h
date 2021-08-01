/*
Silent Radiance wifi broadcast for digital silent disco.
Copyright (C) 2017-2019 Hibbard M. Engler

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License   
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/






#ifndef SIMPLE_PACKET_SAMPLE_H
#define SIMPLE_PACKET_SAMPLE_H 1

#ifdef __cplusplus
extern "C" {
#endif


#include <sys/time.h>




//define COLLECT_JPG_IMAGES 1
// ^ set in glue or on make - this makes up collect the jpg images for show 


#define COMMAND_PACKET_MAX_SIZE 780
/* note - this is copied from music_broadcast.c on the server 
This was 524 before -perhaps this was a number by experimentation? I dont know. 
*/

struct packet {
    int frame_number;
    int header_frame_number;
    int has_statistics;
    int has_some_flags;
    int has_music_samples;
    float pitch;
    int microseconds;
    int db_level_int;
    float db_level;
    int flags;
    int folded_flags;
    int has_beat;
    int has_onset;
    int snake_segment_around_here;
    unsigned char more[COMMAND_PACKET_MAX_SIZE];
    int more_length; /* zero if no more extra data */
    short sample[480]; /* left right */
    char version[5]; /* always SR01 for now */
};

#define PACKET_STATE_DEAD -1
/* no packets for 10 seconds */

#define PACKET_STATE_NOTHING  1
/* no packets in queue, no sound in queue */

#define PACKET_STATE_WARMING 2
/* packets in queue, but not enough for sound */

#define PACKET_STATE_WARNING  3
/* low packets in queue, still have sound */


#define PACKET_STATE_GOOD 4
/* packets in queue, sound, and we have packets ahead of the game too */


struct show_ip_address {
  int show_mode;
  struct timeval stop_show_mode_time;
  int ourcolor; /* 0,1 - 0 for red - off, 1 for cyan - on */
  };
  
  
struct location_estimation {
  int estimated;
  struct timeval when_estimated;
  float x;
  float y;
  float z;
  };

#define PACKET_SUMMARY_SIZE 1024
struct packet_summary {
int state; /* the state */
/* vvv only filled in if the state is PACKET_STATE_GOOD or PACKET_STATE_WARNING */
int now_frame;  /* current frame being played (estimated) */
int send_frame;  /* current frame being sent to the play buffer (less estimated */
int start_index; /* current index of start frame - useful to find other frames */
int start_frame; /* start frame we have from all time - might have gaps */
int no_gap_frame; /* start frame we have from all time with no gaps */
int end_frame; /* last frame for which we have data. Warning! this could be in the future!
                  but think - we know the future man! Wow! */
float warming_quality; /* number between 0. and 1. */
float quality; /* number between 0 and 1 - % of packets rolling average from current back 80 that have statistics */
int soundringsend;
int soundringnow;
int soundringtail;
int soundringfirst;
int soundringhead;
struct timeval start_of_nothing_state;
struct timeval current_sample_time;
struct timeval tempo_points[4];
float where_in_tempo;
int beat_interval_in_microseconds; // more accurate
int beat_interval_in_milliseconds;
struct packet packets[PACKET_SUMMARY_SIZE];

float commanded_background_color[4]; /* set by the command parser. */

struct show_ip_address command_show_ip_address_stuff;
struct location_estimation our_location;

char credit_event[256];
char credit_event_month[256];
char credit_dj[256];
int credit_count;
};





/* visual voice stuff: */
/* packet_song_frame_to_stream_frame, if valid, retains the ration from the stream frame number to the song frame number.
If it skips around, then the frame number is probably invalid, 
and we should dump */

typedef struct packet_song_frame_to_stream_frame {
int valid;
int stream_frame;
long song_frame;
} packet_song_frame_to_stream_frame;



/* note - these structures exist in packet_summary (client) and srplayer (server)
with slightly different names.
packet_summary reads what we are writing,  both can print out text 
but from packet summary, we can have the blue bounding ball!
*/




#define VISUAL_SYMBOLS_PER_LINE 60
#define VISUAL_SYMBOL_MAX_SIZE 60
/* measure chunks gets converted to visual voice lines */
typedef struct visual_voice_line {
  int line_number;
  int number_of_symbols;
  int symbol_indent[VISUAL_SYMBOLS_PER_LINE];
  char symbols[VISUAL_SYMBOLS_PER_LINE][VISUAL_SYMBOL_MAX_SIZE];
  char space_prefix[VISUAL_SYMBOLS_PER_LINE]; /* if this symbol is a _ symbol */
  char on_this_line[VISUAL_SYMBOLS_PER_LINE]; /* if the symbol is on this line - or added because of another line */
        /* This       for       case               having                    like
	         is         a                you   have     multiple               this
		      for              when        have               lines */
  int computed;
  int done_by;
  int current_symbol;
  int symbol_passed_by;
  float x_ul[VISUAL_SYMBOLS_PER_LINE];  // upper left x when rendering x position - relative to 0,0
  float y_ul[VISUAL_SYMBOLS_PER_LINE];  // upper left y - relative to 0,0
  float width[VISUAL_SYMBOLS_PER_LINE]; // width to get to lower eight
  float height[VISUAL_SYMBOLS_PER_LINE]; // height to get to lower right
  float x[VISUAL_SYMBOLS_PER_LINE]; // x position that is typeset for this symbol. 
  float y[VISUAL_SYMBOLS_PER_LINE]; // y position that is typeset for this symbol
           // x + width is the width of the symbol itself.
	   // but because of multiline, the next symbol x might be larger.
  } visual_voice_line;


/* size of text */
#define SR_MEASURE_MAX_SIZE 1600
#define SR_MEASURE_NUM_LINES 4

typedef struct packet_measure_chunk {
  long beat_buffer_now_frame;
  char beat_buffer[SR_MEASURE_MAX_SIZE];
  long beat_buffer_length; // length in 1/200ths of a second
  int number_line_buffers;
  char line_buffer[SR_MEASURE_NUM_LINES][SR_MEASURE_MAX_SIZE];
  int line_buffer_len[SR_MEASURE_NUM_LINES];
  struct visual_voice_line visual_voice_lines[SR_MEASURE_NUM_LINES];
  } packet_measure_chunk;




#define MEASURE_CHUNK_COUNT 20
extern volatile packet_measure_chunk packet_measure_chunks[MEASURE_CHUNK_COUNT];
extern volatile int packet_measurechunkhead;
extern volatile int packet_measurechunktail;



  
#define FRAMES_WHERE_DONE_VISUAL_BECOMES_GONE_VISUAL 48      


extern int packet_summary_ignore_queue_size; // notmally 0



#define TITLE_TEXT_MAX 3000

extern char current_display_title_text[TITLE_TEXT_MAX];
extern char new_display_title_text[TITLE_TEXT_MAX];
extern int change_display_title_text;   // =0;
extern packet_song_frame_to_stream_frame song_frame_to_stream_frame;

void output_visual_voice(FILE *des);


#ifdef COLLECT_JPG_IMAGES
struct jpg_image {
  char filename[256];
  char upc[40];
  int frames;
  unsigned long file_size;
  unsigned char *chunk_map;
  unsigned char *image; /* original bgp image */
  int original_width;
  int original_height;
  
  volatile int rawthread;
  
  /* rest is for the translated raw image */
  int width;
  int height;
  int gl_color_format;
  int raw_image_size;
  volatile unsigned char *raw_image;
  
  /* rest is the texture */
  unsigned int texture_object_id;
  unsigned int buffer_id;
  unsigned int program_id;
  
  };
  
extern struct jpg_image *jpg_images[65536];

#define JPG_IMAGE_QUEUE_SIZE 10
extern unsigned short jpg_image_queue[JPG_IMAGE_QUEUE_SIZE];
extern int jpg_image_queue_head;
extern int jpg_image_queue_tail;


#endif



extern struct packet_summary packet_summary;

extern void (*more_commands[256])(struct packet *packet,unsigned char *command,int length);
/* ^^^ command statements.  packet_summary will handle some, but modules can add more */

extern void collect_a_packet(int current,int current_index);

extern void init_packet_summary(void);
extern void compute_packet_summary(void);

extern long time_difference_in_milliseconds(struct timeval a,struct timeval b);
extern long time_difference_in_microseconds(struct timeval a,struct timeval b);

extern struct timeval add_milliseconds_to_time (struct timeval a,
  long milliseconds);
extern struct timeval add_microseconds_to_time (struct timeval a,
  long microseconds);
extern void finish_packet_summary(void);

/* stream frame stuff */
extern long song_frame_from_stream_frame(int stream_frame); /* returns -1l if not valid */
extern void reset_packet_song_frame_to_stream_frame(void); /* sets to invalid */
extern void set_packet_song_frame_to_stream_frame(long song_frame,int stream_frame); /* links it up */
extern int record_stream_frame(int stream_frame); /* moves the frame along, but resets if frame is bogus */

#include <sys/time.h>
extern int sr_gettimeofday(struct timeval *tv, struct timezone *tz);
#ifndef __APPLE__
extern int make_demo_mode;
extern volatile struct timeval demo_time_of_day;
#endif
extern void increase_demo_time(int microseconds);
extern int sr_gettimeofday(struct timeval *tv, struct timezone *tz); 

struct timeval add_times(struct timeval a,struct timeval b);



#ifdef __cplusplus
}
#endif

#endif

