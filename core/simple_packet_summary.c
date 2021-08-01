/*
Silent Radiance wifi broadcast for digital silent disco.
Copyright (C) 2017-2018 Hibbard M. Engler

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


/* differences from silentradiance:
1. credit system "cred" is disabled.  would let you get credit for being tuned in to special events.
2. International languages are disabled.
3. Ip address to physical location is disabled.
4. cant analyze local music 
5. demos/movies cant be done with this - just live
6. command len is compensated for sr01,sr00 end overhead before - where in main code it is done here. -12/-28 something liek that
7. Therea are no area sections - so soundstat is no longer a pointer to a soundstat, it is an array
*/




#include <stdio.h>
#include <sys/time.h>
#include <string.h>
#include <stdlib.h>


#include "silentradiancesimple.h"
#include "simple_packet_summary.h"
#include "simple_processor.h"

#ifndef logit
#define logit(...) {fprintf(stderr,__VA_ARGS__);fprintf(stderr,"\n");}
#endif

struct packet_summary packet_summary;



#ifdef COLLECT_JPG_IMAGES
struct jpg_image *jpg_images[65536];

unsigned short jpg_image_queue[JPG_IMAGE_QUEUE_SIZE];
int jpg_image_queue_head=0;
int jpg_image_queue_tail=0;
#endif


int packet_summary_ignore_queue_size=0;

/* songs have frame numbers that are used for visual voice */
packet_song_frame_to_stream_frame song_frame_to_stream_frame = (packet_song_frame_to_stream_frame){.valid=0,.stream_frame=0.,.song_frame=0l};


volatile packet_measure_chunk packet_measure_chunks[MEASURE_CHUNK_COUNT];
volatile int packet_measurechunkhead=0;
volatile int packet_measurechunktail=0;


char packet_next_beat_buffer[SR_MEASURE_MAX_SIZE];

int int_from_command(unsigned char *l) {
int i1 = l[0] & 0xff;
int i2 = l[1] & 0xff;
int i3 = l[2] & 0xff;
int i4 = l[3] & 0xff;
return i1 + (i2 << 8) + (i3 << 16) + (i4 << 24);
}

extern void dump_full(int sampleSize,char *packet,int commandstart,int commandend);


int float_from_command(unsigned char *l) {
float value;
unsigned char *pt = (unsigned char *)&value;
pt[0]=l[0];
pt[1]=l[1];
pt[2]=l[2];
pt[3]=l[3];
return value;
}


void init_packet(struct packet *packet) {
        packet->header_frame_number = -1; /* find this also */
        packet->has_statistics = 0;
        packet->has_some_flags = 0; /* do forward analysis to see if some flags came through */
        packet->has_music_samples=0;
        packet->pitch=0.f;
        packet->microseconds=-1;
        packet->db_level_int=-11000;
        packet->db_level= packet->db_level_int*0.001;
        packet->flags = 0;
        packet->folded_flags = 0;  /* ^^^ ??? */
        packet->has_beat = 0;
        packet->has_onset = 0;
        packet->snake_segment_around_here = 0;
        packet->more_length = 0;
#define WORLD_packet_summary_SR01 "SR01"
        strcpy(packet->version,WORLD_packet_summary_SR01);
}

/* called when the command has no length */
void more_empty_command(struct packet *packet) {}

void more_null_command(struct packet *packet,unsigned char *command,int length) {}




#ifdef future_use
/* convert hex 1 into a byte */
static int hex1byte(char *one_byte_hex) {
int value=0;
if ((*one_byte_hex>='0')&&(*one_byte_hex<='9')) {
  value += *one_byte_hex-'0';
  }
else if ((*one_byte_hex >='a')&&(*one_byte_hex<='f')) {
  value += *one_byte_hex - 'a' +10;
  }
else if ((*one_byte_hex >='A')&&(*one_byte_hex<='F')) {
  value += *one_byte_hex - 'A' +10;
  }
return value;
}
#endif

/* convert hex 2 into a byte */
static int hex2byte(char *two_byte_hex) {
int value=0;
if ((*two_byte_hex>='0')&&(*two_byte_hex<='9')) {
  value += *two_byte_hex-'0';
  }
else if ((*two_byte_hex >='a')&&(*two_byte_hex<='f')) {
  value += *two_byte_hex - 'a' +10;
  }
else if ((*two_byte_hex >='A')&&(*two_byte_hex<='F')) {
  value += *two_byte_hex - 'A' +10;
  }

value = value*16;
two_byte_hex++;

if ((*two_byte_hex>='0')&&(*two_byte_hex<='9')) {
  value += *two_byte_hex-'0';
  }
else if ((*two_byte_hex >='a')&&(*two_byte_hex<='f')) {
  value += *two_byte_hex - 'a' +10;
  }
else if ((*two_byte_hex >='A')&&(*two_byte_hex<='F')) {
  value += *two_byte_hex - 'A' +10;
  }

return value;
}

/* this command:
C04003F
will set the background color to what is specified.

There will be a way to specify a timefreme as an additional argument
later on.
*/
void C_set_background_color_command(struct packet *packet,unsigned char *command,int length)
{
int r,g,b;
if (length>=7) {
  r = hex2byte((char *)command+1);
  g = hex2byte((char *)command+3);
  b = hex2byte((char *)command+5);
  
  packet_summary.commanded_background_color[0]=((float)(r))/255.f;
  packet_summary.commanded_background_color[1]=((float)(g))/255.f;
  packet_summary.commanded_background_color[2]=((float)(b))/255.f;
  packet_summary.commanded_background_color[3]=((float)(255))/255.f;
  }
}  









/* this command lods a jpg image into the list of images - up to 65536 
however, phones cant handle too many images. so the count will probably go down to save memory 
The image ws sent by sr_send_binary - a server program that injects it into shared memory
*/
void I_set_jpg_image_command(struct packet *packet,unsigned char *command,int length)
{
#ifdef wait_until_1_point_2
#ifdef COLLECT_JPG_IMAGES
if (length<7) {
//  logit("toosmall\n");
  return;
  }
int i=1;
unsigned char subcommand = command[i++];
int image_id = command[i] + (command[i+1]<<8);i += 2; //id numbers
int chunk_number = command[i] + (command[i+1]<<8) + (command[i+2]<<16); i +=3;  // chunk number 1M
/*logit("I %d %x len %d\n",image_id,chunk_number,length);
int j;
for (j=0;j<length;j++) {
//  logit("%2.2x",command[j]);
  if ((j%64)==63) {
//    logit("\n");
    }
  }
//logit("\n");
*/
if (chunk_number != 0xffffff) { /* loading a chunk */
   struct jpg_image *image = jpg_images[image_id];
   if (!image) {
//     logit("no image %d here\n",image_id);
     return;
     }
   image->chunk_map[chunk_number]=1;
   long image_pos = chunk_number*(255-7);
   if (image_pos+length-7>image->file_size) {
//     logit("too big for file %ld to %ld\n",image_pos+length-7,image->file_size);
     return;  
     } 
   memcpy((void *)(image->image+image_pos),command+7,length-7);
//   if (chunk_number==0) {
//     logit("special %lx %lx start: %2.2x%2.2x%2.2x%2.2x\n",(long)(image->image),(long)(image->chunk_map),image->image[0],image->image[1],image->image[2],image->image[3]);
//     }
   return;
   }
/* here we are a new file */
unsigned long file_size = command[i] + (command[i+1]<<8)  + (command[i+2]<<16) + (command[i+3]<<24); i+=4;
unsigned int number_of_frames = command[i] + (command[i+1]<<8);i += 2;

//logit("w %d + %d<<8 = %d\n",command[i] ,(command[i+1]),command[i] + (command[i+1]<<8));
int original_width =  command[i] + (command[i+1]<<8);i += 2;
int original_height =  command[i] + (command[i+1]<<8);i += 2;

unsigned int reserved1 =  command[i] + (command[i+1]<<8);i += 2;
unsigned char *filename= command+i;
while (i<length) {
  if (command[i]=='\0') break;
  i++;
  }
if (i==length) {
//  logit("missing parameters 1\n");
  return; // sanity;
  }
i++;
unsigned char *upc= command+i;
while (i<length) {
  if (command[i]=='\0') break;
  i++;
  }
if (i==length) {
//  logit("missing parameters 2\n");
  return; // sanity;
  }
i++;
if (i+8>length)  {
//  logit("missing parameters 3. should be len %d but is length %d\n",i+8,length);
  return; /* got 8 more reserved */
  }

struct jpg_image *image = (jpg_images[image_id]);
if (image) {
  if ((strcmp(image->filename,filename)==0)) {
     if (strcmp(image->upc,upc)==0) {
//       logit("duplicate image- keep building\n");
       return; /* we are the same one */
       }
     }
  if (image->chunk_map) free(image->chunk_map);
  if (image->image) free(image->image);
  if (image->raw_image) free((void *)(image->raw_image));
  jpg_images[image_id]=NULL;
  free(image);  
  }

image= malloc(sizeof(struct jpg_image));
image->frames = number_of_frames;
image->file_size = file_size;
unsigned long nchunks = (file_size+(255-7)-1)/(255-7);
//logit("NCHUNKS is %d\n",nchunks);
image->chunk_map=calloc(sizeof(char),nchunks);
image->image = malloc(sizeof(char)*file_size);
image->original_width = original_width;
image->original_height = original_height;
image->raw_image = NULL;
image->width=0;
image->height=0;
image->raw_image_size=0;
strcpy(image->filename,filename);
strcpy(image->upc,upc);

jpg_images[image_id] = image;
/* image all set up */
#endif
#endif
}




void i_show_jpg_image_command(struct packet *packet,unsigned char *command,int length)
{
#ifdef wait_till_1_dot_2
#ifdef COLLECT_JPG_IMAGES
if (length<4) return;
int i=1;
unsigned char subcommand = command[i++];
int image_id = command[i] + (command[i+1]<<8);i += 2;
{
   struct jpg_image *image = jpg_images[image_id];
   if (!image) {
//     logit("no image id %d\n",image_id);
     return;
     }
   unsigned long nchunks = (image->file_size+(255-7)-1)/(255-7);
   for (i=0;i<nchunks;i++) {
     if (image->chunk_map[i]==0) {
       logit("missing %d",i);
       return; // no good
       }
     }
   // indicate that this chunk is ready to see
   for (int i=jpg_image_queue_head;i!=jpg_image_queue_tail; i = (i+1)%JPG_IMAGE_QUEUE_SIZE) {
     if (image_id == jpg_image_queue[i]) {
       logit("there");
       return; // already there
       }
     }
//   logit("we got %d\n",image_id);
   jpg_image_queue[jpg_image_queue_tail]=image_id;
   jpg_image_queue_tail = (jpg_image_queue_tail+1)%JPG_IMAGE_QUEUE_SIZE;
   if (jpg_image_queue_head==jpg_image_queue_tail) {
     jpg_image_queue_head = (jpg_image_queue_head+1)%JPG_IMAGE_QUEUE_SIZE;
     }
   return;
   }

#endif
#endif
}


char current_display_title_text[TITLE_TEXT_MAX];
char new_display_title_text[TITLE_TEXT_MAX];
int change_display_title_text=0;


void t_set_text(struct packet *packet,unsigned char *command,int length)
{
//logit("t_set_text %d\n",length);
if ((length>2)&&(length<TITLE_TEXT_MAX-2)) {
  strncpy(new_display_title_text,(char *)command+2,length-2);
  new_display_title_text[length-2]='\0';
  change_display_title_text=1;  
//  logit("new text: %s\n",new_display_title_text);
  }
}  
  
  
unsigned short ip4_last16; /* would have each phone at a concert display different colors for different bits
for wifi broadcast mode.  So the server could read the bits and give every user an x,y coordinate, so that we
could light up all the phones and do like when they put up cards at baseball games to make an image. 
Well, needs some hooks to work, and simple version doesnt handle wifi broadcast */
  
/* Wxx  - xx is the bit of the ip address
the color is read for 0, cyan for 1
Example:   
W00
W01
W02
W03
W04
W05
W060
W071
W082
W090
W0A1
W0B2
W0C0
W0D1
W0E2
W0F2
would be a sequence of commands sent by a remote camera station to the server, and broadcast to the clients in order to get the entier ip address
list.   This camera station will then have an X,Y for each ip address in theory. Quite a fun problem.
*/
void W_show_ip_address(struct packet *packet,unsigned char *command,int length)
{
int bit_number;
if (packet_summary.command_show_ip_address_stuff.show_mode) return;
if (length<3) return;
logit("%s command",command);
if (ip4_last16==0) {
  logit("empty ip address. skip");
  return; /* dont display if we are 0 */
  }
packet_summary.command_show_ip_address_stuff.show_mode=1;
bit_number = hex2byte((char *)command+1);



// for now: (ipv6 or ipv4)
bit_number = bit_number&15;
unsigned int ip_address=ip4_last16;

if (ip_address&((unsigned int)1<<bit_number)) {
  packet_summary.command_show_ip_address_stuff.ourcolor=1;
  }
else {
  packet_summary.command_show_ip_address_stuff.ourcolor=0;
  }
logit ("colors:");
packet_summary.command_show_ip_address_stuff.stop_show_mode_time = packet_summary.current_sample_time;
packet_summary.command_show_ip_address_stuff.stop_show_mode_time.tv_sec+= 2; /* keep on for 2 seconds */

/*packet_summary.where_in_tempo
beat_interval_in_microseconds
*/
}






/* f  <long> 
Note - f might readjust the structure of visual voice 
*/
void f_set_song_frame_number(struct packet *packet,unsigned char *command,int length) {
long tr;
char *pb;
tr =strtol((const char *)(command+1),&pb,10);
if (pb!=(char *)command) { /* only call this command if it is majorly different or invalid */
  if (song_frame_from_stream_frame(packet->frame_number)==-1l) {
    set_packet_song_frame_to_stream_frame(tr,packet->frame_number);
    }
  }
else {
  logit("bad command %s\n",command);
  }
}



/* on full version would learn the x and y coordinates of the phone in the world, so we could do work effects 
i GUESS WE COULD DO A LATITUDE AND LONGITUDE, SO the DJ could give a shoutrout to a countey, and all phones in that country would light up
But that could be done with a table faster.*/
static char last_match[500];
void R_cred_received(struct packet *packet,unsigned char *command,int length)
{
return;
}


static int matches_our_ip_address(char *match_string) {
return(1); /* stub */
}



/* place phone is used to place the phone at a given x,y coordinate (between -1 and 1 in x and y). 
The command is P <match_string> x y z
where <match_string> is a sequence of 0,1, or ? to match with the ip address, from lowest bit to highest bit
x is a floating point x position, basically from a camera that is directly above everybody.
y is a floating point y position, also from a camera that is directly above everybody.
z is just set to 0 for now. but with multiple cameras, thaere could be a z position as well.

Example:

P ?1??0?1?0 0.4 -1.2 0

The question marks can save time, once the vision tool has decided pretty much where eveyone is, it could be that not all the bits are known.
Also, some phones might cut out or be occluded during some transmissions. 
It is a tricky algorithm:

because the points will be something like 
????????????1?? 13 15
?????????????0? 14 15
??????????????1 12 16
????????????1?? 3 15
?????????????1? 4 15
??????????????0 3 16

These have a certain variance, and do not clash, so they are added together.
?????????????101 13 15.6
?????????????110 3.6 15.6
Then there will be a number of groups.  Once 1:
90% of the groups are equal to the number of points found.

and 
2. 
no groups (or almost no groups) have multiple addresses out of the cluster.

Its a tough one.
*/
void P_place_phone(struct packet *packet,unsigned char *command,int length)
{
char junk;
char match_string[300];
float x=0.f;
float y=0.f;
float z=0.f;
match_string[0]=0;

#define WORLD__pct_c__pct_s__pct_f__pct_f__pct_f "%c %s %f %f %f"
sscanf((char *)command,WORLD__pct_c__pct_s__pct_f__pct_f__pct_f,&junk,match_string,&x,&y,&z);

if (matches_our_ip_address(match_string)) {
  logit("Estimate we are at %f,%f,%f",x,y,z);
  packet_summary.our_location.estimated=1;
  packet_summary.our_location.when_estimated = packet_summary.current_sample_time;
  packet_summary.our_location.x = x;
  packet_summary.our_location.x = y;
  packet_summary.our_location.x = z;
  }
}





/* V <long> <long> 
Note - f might readjust the structure of visual voice 
*/
void V_add_visual_voice_command_line(struct packet *packet,unsigned char *command,int length) {
long tr;
long len;
char *pb;
char *pb2;
logit("c	%s",command);

tr =strtol((const char *)(command+1),&pb,10);
if (pb!=(char *)command) {
  len =strtol(pb,&pb2,10);
  if (pb!=pb2) {
    int redo_song_frame = 0;
    
    
    if ((tr==0l)||(song_frame_from_stream_frame(packet->frame_number)==-1l)) { /* bad packet frame number */
      redo_song_frame = 1;
      }
    
    
    /* wipe out packet_measuredheads if our frame number is less then them */
    if (tr==0) { packet_measurechunkhead=packet_measurechunktail;}
    {
      while (packet_measurechunkhead != packet_measurechunktail) {
        volatile packet_measure_chunk *mc = packet_measure_chunks+packet_measurechunkhead;
        if (mc->beat_buffer_now_frame > tr) {
	  logit("GONE %ld %ld\n",mc->beat_buffer_now_frame,tr);
          packet_measurechunkhead = (packet_measurechunkhead+1) % MEASURE_CHUNK_COUNT;
	  redo_song_frame = 1;
	  }
        else if (mc->beat_buffer_now_frame+ mc->beat_buffer_length + 7000 < tr) { // long time ago
	  logit("GONE2 %ld %ld\n",mc->beat_buffer_now_frame,tr);
          packet_measurechunkhead = (packet_measurechunkhead+1) % MEASURE_CHUNK_COUNT;
	  redo_song_frame = 1;
	  }
        else break;
        }
      } /* block to wipe out junky packet measure heads */    
    
    
    if (redo_song_frame) {
      set_packet_song_frame_to_stream_frame(tr,packet->frame_number);
      }
    
    
    
    volatile packet_measure_chunk *mc = packet_measure_chunks+packet_measurechunktail;
    if (len>SR_MEASURE_MAX_SIZE-1) len=SR_MEASURE_MAX_SIZE-1;
    mc->beat_buffer_length = len;
    int i;
    for (i=0;i<len;i++) { mc->beat_buffer[i] = ' ';}
    for (i=0;i<SR_MEASURE_MAX_SIZE;i++) { mc->beat_buffer[i] = '\0';}
    mc->beat_buffer_now_frame = tr;
    mc->number_line_buffers=0;
    
    packet_measurechunktail = (packet_measurechunktail+1) % MEASURE_CHUNK_COUNT;
    if (packet_measurechunktail==packet_measurechunkhead) {
      logit("GONE3 %ld %ld\n",mc->beat_buffer_now_frame,tr);
      packet_measurechunkhead = (packet_measurechunkhead+1) % MEASURE_CHUNK_COUNT;
      }	
    return;
    } /* block where we got a new command line */          
  }
logit("bad command %s\n",command);
}



static void init_visual_voice_line(volatile visual_voice_line *vvl,int line_number) {
vvl->line_number = line_number;
vvl->number_of_symbols = 0;
vvl->computed = 0;
}	  



/* v <long> <int> <int> <string> 
current frame, line number   start_pos  string_to_finish

Set the voice line up - might take multiple tries

*/
void v_add_visual_voice_line(struct packet *packet,unsigned char *command,int length) {
long tr;
int line_no;
int extenstion;
char *pb;
char *pb2;
char *pb3;
logit("c	%s",command);
tr =strtol((const char *)(command+1),&pb,10);
if (pb!=(char *)command) {
  line_no =strtol(pb,&pb2,10);
  if (pb!=pb2) {
    extenstion = strtol(pb2,&pb3,10);
    if (pb2!=pb3) {
      int len;
      if ((*pb3)&&(*pb3<=' ')) pb3++;
      len = strlen(pb3);
      
      int m = packet_measurechunkhead;
      while (m != packet_measurechunktail) {
        volatile packet_measure_chunk *mc = packet_measure_chunks+m;
	if (mc->beat_buffer_now_frame == tr)  {
	  if (line_no>=SR_MEASURE_NUM_LINES) line_no=SR_MEASURE_NUM_LINES-1;
	  if (line_no<0) line_no=0;
	  if (extenstion<0) extenstion=0;
	  
	  if (extenstion+len+1<SR_MEASURE_MAX_SIZE) {
//	    logit("v cool frame %ld line %d ext %d %s\n",tr,line_no,extenstion,pb3);
	    while (mc->number_line_buffers<=line_no) {
	      memset((char *)(mc->line_buffer[mc->number_line_buffers]),0,SR_MEASURE_MAX_SIZE);
	      init_visual_voice_line(mc->visual_voice_lines + mc->number_line_buffers, mc->number_line_buffers);
	      (mc->number_line_buffers)++;
	      }
	    for (int iii=0;iii<mc->number_line_buffers;iii++) {
	      mc->visual_voice_lines[iii].computed = 0;

	      }	    
	    strcpy((char *)(mc->line_buffer[line_no]+extenstion),(char *)pb3); 
	    return;
	    }
	  else {
	    // maxsize too big 
	    logit("cerr: overflow    %s\n",command);
	    return;
	    }
	  } /* if found */
	m = (m+1)  % MEASURE_CHUNK_COUNT;
	} // while search 
      // cant find chunk
      logit("cerr: cant find chunk frame %ld     :%s\n",tr,command);
      return;
      
      }
    // invalud extenstion
    logit("cerr: invalid extenstion:%s\n",command);
    return;
    }
  // invalid line_no
  logit("cerr: invalid line number:%s\n",command);
  return;
  }
// invalid chunk no
logit("cerr: invalid chunk number:%s\n",command);
return;
}
    
   
  





static void determine_final_color() {
if (packet_summary.command_show_ip_address_stuff.show_mode) {
  if (time_difference_in_microseconds(
    packet_summary.current_sample_time,packet_summary.command_show_ip_address_stuff.stop_show_mode_time)>=0) {
    packet_summary.command_show_ip_address_stuff.show_mode=0;
    return;
    } /* if we are done showing the positional color */
//  logit("diff %d",time_difference_in_microseconds(packet_summary.current_sample_time,packet_summary.command_show_ip_address_stuff.stop_show_mode_time));
  /* the final color is picked in default.c display set */    
  } /* if we are changing the screen color to tell the server who we are, and where we are */
}

#define SECONDS_OF_VALID_ESTIMATION 600
static void faze_out_location_estimation_after_a_while() {
if (!packet_summary.our_location.estimated) return;
if ((packet_summary.current_sample_time.tv_sec -packet_summary.our_location.when_estimated.tv_sec) > SECONDS_OF_VALID_ESTIMATION) {
  packet_summary.our_location.estimated=0;
  }
}


/* these commands quickly jump to the parser based on the first character 
This makes it easy for one character commands - like to set the bar number
And this also makes it easy to do word commands quickly - as 
there are less choices for any one character starting up.
Note that additional modules (when we have them) might modify this code
to add custom command handling. YEAH!
*/
void (*more_commands[256])(struct packet *packet,unsigned char *command,int length) = {
more_null_command, /* null */
more_null_command, /* ctrl a */
more_null_command, /* ctrl b */
more_null_command, /* etc... */
more_null_command,
more_null_command,
more_null_command,
more_null_command,
more_null_command,
more_null_command,
more_null_command,
more_null_command,
more_null_command,
more_null_command,
more_null_command,
more_null_command,
more_null_command,
more_null_command,
more_null_command,
more_null_command,
more_null_command,
more_null_command,
more_null_command,
more_null_command,
more_null_command,
more_null_command,
more_null_command,
more_null_command,
more_null_command,
more_null_command,
more_null_command,
more_null_command,
more_null_command, /* space command */
more_null_command, /* ! */
more_null_command, /* " */
more_null_command, /* # */
more_null_command, /* $ */
more_null_command, /* % */
more_null_command, /* & */
more_null_command, /* etc */
more_null_command,
more_null_command,
more_null_command,
more_null_command,
more_null_command,
more_null_command,
more_null_command,
more_null_command,
more_null_command, /* 0 */
more_null_command, /* 1 */
more_null_command, /* 2 */
more_null_command, /* 3 */
more_null_command, /* 4 */
more_null_command, /* 5 */ 
more_null_command, /* 6 */
more_null_command, /* 7 */
more_null_command, /* 8 */
more_null_command, /* 9 */
more_null_command, /* : */
more_null_command, /* ; */
more_null_command, /* < */
more_null_command, /* = */
more_null_command, /* > */
more_null_command, /* ? */
more_null_command, /* @ */
more_null_command, /* A */
more_null_command, /* B */
C_set_background_color_command, /* C */
more_null_command, /* D */
more_null_command, /* E */
more_null_command, /* F */
more_null_command, /* G */
more_null_command, /* H */
I_set_jpg_image_command, /* I */
more_null_command, /* J */
more_null_command, /* K */
more_null_command, /* L */
more_null_command, /* M */
more_null_command, /* N */
more_null_command, /* O */
P_place_phone, /* P */
more_null_command, /* Q */
R_cred_received, /* R */
more_null_command, /* S */
more_null_command, /* T */
more_null_command, /* U */
V_add_visual_voice_command_line, /* V */
W_show_ip_address, /* W */
more_null_command, /* X */
more_null_command, /* Y */
more_null_command, /* Z */
more_null_command, /* [ */
more_null_command, /* \ */
more_null_command, /* ] */
more_null_command, /* ^ */
more_null_command, /* _ */
more_null_command, /* ` */
more_null_command, /* a */
more_null_command, /* b */
more_null_command, /* c */
more_null_command, /* d */
more_null_command, /* e */
f_set_song_frame_number, /* f */
more_null_command, /* g */
more_null_command, /* h */
i_show_jpg_image_command, /* i */
more_null_command, /* j */
more_null_command, /* k */
more_null_command, /* l */
more_null_command, /* m */
more_null_command, /* n */
more_null_command, /* o */
more_null_command, /* p */
more_null_command, /* q */
more_null_command, /* r */
more_null_command, /* s */
t_set_text, /* t */
more_null_command, /* u */
v_add_visual_voice_line, /* v */
more_null_command, /* w */
more_null_command, /* x */
more_null_command, /* y */
more_null_command, /* z */
more_null_command, /* { */
more_null_command, /* | */
more_null_command, /* } */
more_null_command, /* ~ */
more_null_command, /* DEL */
more_null_command,
more_null_command,
more_null_command,
more_null_command,
more_null_command,
more_null_command,
more_null_command,
more_null_command,
more_null_command,
more_null_command,
more_null_command,
more_null_command,
more_null_command,
more_null_command,
more_null_command,
more_null_command,
more_null_command,
more_null_command,
more_null_command,
more_null_command,
more_null_command,
more_null_command,
more_null_command,
more_null_command,
more_null_command,
more_null_command,
more_null_command,
more_null_command,
more_null_command,
more_null_command,
more_null_command,
more_null_command,
more_null_command,
more_null_command,
more_null_command,
more_null_command,
more_null_command,
more_null_command,
more_null_command,
more_null_command,
more_null_command,
more_null_command,
more_null_command,
more_null_command,
more_null_command,
more_null_command,
more_null_command,
more_null_command,
more_null_command,
more_null_command,
more_null_command,
more_null_command,
more_null_command,
more_null_command,
more_null_command,
more_null_command,
more_null_command,
more_null_command,
more_null_command,
more_null_command,
more_null_command,
more_null_command,
more_null_command,
more_null_command,
more_null_command,
more_null_command,
more_null_command,
more_null_command,
more_null_command,
more_null_command,
more_null_command,
more_null_command,
more_null_command,
more_null_command,
more_null_command,
more_null_command,
more_null_command,
more_null_command,
more_null_command,
more_null_command,
more_null_command,
more_null_command,
more_null_command,
more_null_command,
more_null_command,
more_null_command,
more_null_command,
more_null_command,
more_null_command,
more_null_command,
more_null_command,
more_null_command,
more_null_command,
more_null_command,
more_null_command,
more_null_command,
more_null_command,
more_null_command,
more_null_command,
more_null_command,
more_null_command,
more_null_command,
more_null_command,
more_null_command,
more_null_command,
more_null_command,
more_null_command,
more_null_command,
more_null_command,
more_null_command,
more_null_command,
more_null_command,
more_null_command,
more_null_command,
more_null_command,
more_null_command,
more_null_command,
more_null_command,
more_null_command,
more_null_command,
more_null_command,
more_null_command,
more_null_command,
more_null_command,
more_null_command,
more_null_command,
more_null_command,
more_null_command };







void process_commands_for_summary(struct packet *packet) {
unsigned char *more= (unsigned char *)packet->more;
int i=0;
while (i<packet->more_length) {
  int sublength = more[i++];
  if (sublength+i >packet->more_length) {
    /* illegal packer for length - could be additional data I dont know*/
    logit("%d of %d = %d compared to  m %d",sublength,i,sublength+i,packet->more_length);
    {
      int j;
      for (j=0;j<packet->more_length;j++)  {
        fprintf(stderr," %3.3d ",j);
	}
      fprintf(stderr,"\n");
      for (j=0;j<packet->more_length;j++)  {
        fprintf(stderr," %3.3d ",more[j]);
	}
      fprintf(stderr,"\n");
      }
    logit("nope .");
    break;
    }
  char command[300]; /* up to 256 */
  if (sublength) {
    memcpy(command,(char *)(more+i),sublength);
    command[sublength]='\0';
    unsigned int c = ((unsigned char)(command[0]));
//    logit("c %c (%d) command: %s f %d",c,c,command,packet->frame_number);
    more_commands[c](packet,(unsigned char *)command,sublength); /* call quickly */
    i+=sublength;
    }
  else {
    more_empty_command(packet);
    }
  }
}








void collect_a_packet(int current,int current_index) {
/* note - the frame_number is set to the soundframe[current] 
But sometimes, current is -1 = indicating we just know the data, but it is not in soundframe yet.
Sometimes - as in the future. right now, we just do ones that have sounds 
*/
struct packet *packet = packet_summary.packets+current_index;

unsigned char *command = NULL;
int commandSize=0;
unsigned char *e = NULL;
if (current != -1) {
  command = (unsigned char *)commandring[current];
  commandSize=commandlen[current];
  if (commandSize==0) {
    goto no_version;
    }  
  
  e = command+commandSize;
  packet->frame_number = soundstat[current].frame; // redundant - done before the call to collect_a_packet.
//  record_stream_frame(packet->frame_number);      /* goofed up visual. movement by going forward all the time */
  }
else {
  logit("not yet implemented");
  return;
  }

packet->more_length = 0;
int sr01_mode=1;
{
 char *x;
 x = (char *)(&(soundstat[current].version));
 if (x[3]=='1') {
   sr01_mode = 1;
   }
 else if (x[3]=='0') {
   sr01_mode = 0;
   }
 else if (soundstat[current].version==0) {
   no_version:
//          logit("Current %d index %d 0  version",current,current_index);
          /* analysis is not yet complete. */
          packet->header_frame_number = -1; /* find this also */
          packet->has_statistics = 0;
          packet->has_some_flags = 0; /* do forward analysis to see if some flags came through */
          /* has_music_samples done later */
          packet->pitch=0.f;
	  packet->db_level=-11.;
	  packet->db_level_int=-11000;
          packet->microseconds=-1;
          packet->flags = 0;
          packet->folded_flags = 0;  /* ^^^ ??? */
          packet->has_beat = 0;
          packet->has_onset = 0;
          packet->snake_segment_around_here = 0;
          packet->more_length = 0;
#define WORLD_SR_PROTOCOL_SR00 "sr00"
	  strcpy(packet->version,WORLD_SR_PROTOCOL_SR00); // fake it 
	  goto okgood;
	  }
 else {
   logit("packet summary invalid packet version %c%c%c%c",x[0],x[1],x[2],x[3]);
      dump_full(packet->more_length+12,packet->more,packet->more_length,packet->more_length+12) ;
   
   }
 memcpy(packet->version,x,4*sizeof(char));
 packet->version[4]='\0';
 }
 
if (sr01_mode) {
    if (commandSize<40) {
        packet->header_frame_number = -1; /* find this also */
        packet->has_statistics = 0;
        packet->has_some_flags = 0; /* do forward analysis to see if some flags came through */
        /* has_music_samples done later */
        packet->pitch=0.f;
        packet->microseconds=-1;
        packet->db_level_int=-11000;
        packet->db_level= packet->db_level_int*0.001;
        packet->flags = 0;
        packet->folded_flags = 0;  /* ^^^ ??? */
        packet->has_beat = 0;
        packet->has_onset = 0;
        packet->snake_segment_around_here = 0;
        packet->more_length = 0;
    } /* if this packet has no command */
    else {
        packet->header_frame_number=int_from_command(e-20);
        packet->has_statistics = 1;
        int fn = int_from_command(e-28);

        if (fn != packet->frame_number) {
            logit("Stats error frame number thought to be %d but command shows %d",packet->frame_number,fn);
        }
        packet->has_some_flags = 1; /* do forward analysis to see if some flags came through */
        /* has_music_samples done later */
        packet->pitch=float_from_command(e-24);
        packet->microseconds=int_from_command(e-16);
        packet->db_level_int=int_from_command(e-12);
        packet->db_level= packet->db_level_int*0.001;
        packet->flags = int_from_command(e-8);
        packet->folded_flags = packet->flags | (packet->flags>>8) | (packet->flags>>16) | (packet->flags>>24);
        packet->has_beat = packet->folded_flags&2;
        packet->has_onset = packet->folded_flags&1;
        packet->snake_segment_around_here = packet->folded_flags&4;
        packet->more_length=commandSize-40;
        if (packet->more_length>COMMAND_PACKET_MAX_SIZE-1) packet->more_length=COMMAND_PACKET_MAX_SIZE-1;
        memcpy(packet->more,command,packet->more_length);
	
	process_commands_for_summary(packet);
    } /* if packet has a command */
  } /* Sr01 mode */
else { /* SR00 mode */
        if (1==0) { /* impossible to be blank for SR00 */
          } /* if this packet has no command */
        else {
          packet->header_frame_number=int_from_command(e-8);
          packet->has_statistics = 1;

          packet->has_some_flags = 1; /* do forward analysis to see if some flags came through */
          /* has_music_samples done later */
          unsigned char *f=((unsigned char *)(soundstat[current].stats_area))+PER_FRAME_OVERHEAD;
	  packet->pitch=float_from_command(f-16);
	  
//	  logit("p %f\n",packet->pitch);
          packet->microseconds=int_from_command(f-12);
          packet->db_level=float_from_command(f-8);
	  packet->db_level_int = packet->db_level*1000.;
	  if (packet->db_level*1000. < -11000.) {
	    packet->db_level_int = -11000;
	    }
          packet->flags = int_from_command(f-4);
//	  if (packet->flags&3) {logit("f %x",packet->flags);}
          packet->folded_flags = packet->flags | (packet->flags>>8) | (packet->flags>>16) | (packet->flags>>24);
          packet->has_beat = packet->folded_flags&2;
          packet->has_onset = packet->folded_flags&1;
          packet->snake_segment_around_here = packet->folded_flags&4;
	  if (commandSize) {         
         	  packet->more_length=commandSize-12;
		  }
          else {
	    packet->more_length=0;
	    }
 	  }
  if (packet->more_length>0) {
    if (packet->more_length>COMMAND_PACKET_MAX_SIZE-1) packet->more_length=COMMAND_PACKET_MAX_SIZE-1;
      memcpy(packet->more,command,packet->more_length);
//      fprintf(stderr,"sr00 %d (%d) %c\n",packet->more_length,packet->more[1],packet->more[1]);
//    if (packet->more[0]==0) 
//      dump_full(packet->more_length+12,packet->more,packet->more_length,packet->more_length+12) ;
#ifdef judsfndfsjs	  
    {
      int j;
      for (j=0;j<packet->more_length;j++)  {
        fprintf(stderr," %3.3d ",j);
	}
      fprintf(stderr,"\n");
      for (j=0;j<packet->more_length;j++)  {
        fprintf(stderr," %3.3d ",packet->more[j]);
	}
      fprintf(stderr,"\n");
      }
#endif      
   } /* SR00 has a command */
        	  process_commands_for_summary(packet);
} /* SR00  mode*/



okgood:
    /* got command, now do a full sample for drawing stuff */
    {
        packet->has_music_samples=1;
        short *rawsamples = (short *)soundring[current];
        short *samples = packet->sample;
	    memcpy(samples,rawsamples,960);

    }

}




void init_packet_summary() {
#ifdef COLLECT_JPG_IMAGES
memset(jpg_images,0,sizeof(void *));
jpg_image_queue_head=0;
jpg_image_queue_tail=0;
#endif

packet_summary.state=PACKET_STATE_NOTHING;
packet_summary.send_frame=-1;
packet_summary.now_frame=-1;
packet_summary.start_index=0;
packet_summary.start_frame=-1;
packet_summary.no_gap_frame=0;
packet_summary.end_frame=0;
packet_summary.warming_quality=0.;
packet_summary.quality=0.;
packet_summary.soundringsend=-1;
packet_summary.soundringnow=-1;
packet_summary.soundringtail=-1;
packet_summary.soundringfirst=-1;
packet_summary.soundringhead=-1;
sr_gettimeofday(&(packet_summary.start_of_nothing_state),NULL);
packet_summary.current_sample_time=packet_summary.start_of_nothing_state;
struct timeval tv;
tv.tv_sec = 0;
tv.tv_usec = 0;
packet_summary.tempo_points[0] = tv;
packet_summary.tempo_points[1] = tv;
packet_summary.tempo_points[2] = tv;
packet_summary.tempo_points[3] = tv;
packet_summary.where_in_tempo=0.;
packet_summary.beat_interval_in_microseconds=5000000;
packet_summary.beat_interval_in_milliseconds=(packet_summary.beat_interval_in_microseconds+999)/1000;
int i;
for (i=0;i<PACKET_SUMMARY_SIZE;i++) {
  init_packet(packet_summary.packets+i);
  }
  
/* init command stuff */
packet_summary.commanded_background_color[0]=0.f;
packet_summary.commanded_background_color[1]=0.f;
packet_summary.commanded_background_color[2]=0.f;
packet_summary.commanded_background_color[3]=1.f;


packet_summary.command_show_ip_address_stuff.show_mode = 0;

packet_summary.our_location.estimated=0;


packet_summary.credit_event[0]='\0';
packet_summary.credit_event_month[0]='\0';
packet_summary.credit_dj[0]='\0';
packet_summary.credit_count=0;

reset_packet_song_frame_to_stream_frame();
}




long time_difference_in_milliseconds (struct timeval a,struct timeval b) {
long ms = time_difference_in_microseconds(a,b);
if (ms>=0) {
  ms = (ms+999)/1000;
  }
else {
  ms = -ms;
  ms = (ms+999)/1000;
  ms = -ms;
  }  
return ms;
}

struct timeval add_milliseconds_to_time (struct timeval a,long milliseconds) {
long microseconds = milliseconds*1000;
return add_microseconds_to_time(a,microseconds);
}



long time_difference_in_microseconds (struct timeval a,struct timeval b) {
long as = a.tv_sec;
long au = a.tv_usec;
long bs = b.tv_sec;
long bu = b.tv_usec;

long ms;
long mu;
long mt;
ms = (long)(as-bs);
mu = au-bu;
ms = ms * 1000000;
mt = ms + mu;
return mt;
}

struct timeval add_times(struct timeval a,struct timeval b) {
long u = b.tv_usec;
long s = b.tv_sec;
struct timeval c = add_microseconds_to_time(a,u);
long cu = c.tv_usec;

long cs = c.tv_sec;
cs = cs + s;
struct timeval result = (struct timeval){cs,cu};
return result;
}


struct timeval add_microseconds_to_time (struct timeval a,long microseconds) {
long u = a.tv_usec;
long s = a.tv_sec;
u = u+microseconds;
if (u<0L) {
  u=-u;
  s = s - (u/1000000);
  u = (u % 1000000);
  u = -u;
  }
else {
  s = s + (u/1000000);
  u = u % 1000000;
  }
struct timeval result = (struct timeval){s,u};
return result;
}


void compute_packet_summary() {
int send=soundringsend;
int now=soundringnow;
int tail=soundringtail;
int first=soundringfirst;
int head=soundringhead;
int old_state = packet_summary.state;
int total_queue_size;
sr_gettimeofday(&(packet_summary.current_sample_time),NULL);
total_queue_size  = soundringtail-soundringhead;
if (total_queue_size<0) total_queue_size += SOUNDRING_COUNT;


if(packet_summary_ignore_queue_size) {
  packet_summary.state=PACKET_STATE_GOOD;
  goto yeah;
  }    

  
/* transition states */
if (total_queue_size>24) {
  int previous_frame=soundstat[soundringhead].frame-1;
  int i;
  total_queue_size=0;
  for (i=soundringhead;i!=soundringtail;i = (i+1)%SOUNDRING_COUNT) {
    if ((previous_frame+1)!=soundstat[i].frame) { /* the frames are not sequential in the buffer */
      break;
      }
    total_queue_size++;
    previous_frame = soundstat[i].frame;
    }
  }
if (total_queue_size>=40) {
  packet_summary.state=PACKET_STATE_GOOD;
  }
else if (total_queue_size) {
  packet_summary.state=PACKET_STATE_WARNING;
  }
else {
  /* set the start of nothing state if we are starting the nothing state */
  if ((packet_summary.state!=PACKET_STATE_NOTHING)&&(packet_summary.state!=PACKET_STATE_DEAD)) {
    sr_gettimeofday(&(packet_summary.start_of_nothing_state),NULL);
    }
  packet_summary.state=PACKET_STATE_NOTHING;
  }


yeah:

/* now do the work based on the state */
if (packet_summary.state==PACKET_STATE_NOTHING) {
  packet_summary.send_frame=-1;
  packet_summary.now_frame=-1;
  packet_summary.warming_quality=0;
  packet_summary.quality=0;
  packet_summary.soundringsend=-1;
  packet_summary.soundringnow=-1;
  packet_summary.soundringtail=-1;
  packet_summary.soundringfirst=-1;
  packet_summary.soundringhead=-1;
  goto final;
  }

if (packet_summary.state==PACKET_STATE_WARMING) {
  packet_summary.send_frame=-1;
  packet_summary.now_frame=-1;
  packet_summary.quality=0;
  packet_summary.soundringsend=-1;
  packet_summary.soundringnow=-1;
  packet_summary.soundringtail=-1;
  packet_summary.soundringfirst=-1;
  packet_summary.soundringhead=-1;
  packet_summary.warming_quality= 0.f; // simple does not show that
  goto final;
  }


if (packet_summary.start_frame==-1) {
  if (first != -1) {
    packet_summary.start_frame=soundstat[first].frame;
    packet_summary.end_frame = soundstat[first].frame;
    }
  }

if ((send != -1)&&(now != -1)) {
  packet_summary.send_frame = soundstat[send].frame;
  packet_summary.now_frame = soundstat[now].frame;
  }
    else {
        packet_summary.send_frame=-1;
        packet_summary.now_frame=-1;
	}
    
if (packet_summary.send_frame<=0) {
  packet_summary.send_frame = soundstat[soundringhead].frame;
  }

if (first==-1) {
  packet_summary.send_frame=-1;
  packet_summary.now_frame=-1;
  packet_summary.quality=0;
  packet_summary.soundringsend=-1;
  packet_summary.soundringnow=-1;
  packet_summary.soundringtail=-1;
  packet_summary.soundringfirst=-1;
  packet_summary.soundringhead=-1;
  packet_summary.warming_quality=  0.f; // simple does not show that
  goto final;
  }
/* go back */
{
  int f=soundstat[first].frame;
//  logit("  f is soundstat[%d].frame = %d",first,f);
  if ((packet_summary.start_frame==-1)||(packet_summary.start_frame-f>PACKET_SUMMARY_SIZE)||
    (f-packet_summary.start_frame>PACKET_SUMMARY_SIZE*2)||
    (packet_summary.start_frame-packet_summary.send_frame>PACKET_SUMMARY_SIZE)||
    (packet_summary.send_frame-packet_summary.start_frame>PACKET_SUMMARY_SIZE)    ) {
    packet_summary.start_frame=f;
    packet_summary.start_index=0;
    int i;
    for (i=0;i<PACKET_SUMMARY_SIZE;i++) {
      init_packet(packet_summary.packets+i);
      }
    packet_summary.no_gap_frame=f;
    packet_summary.end_frame=f;
    }
//  logit("  start at %d tp %d",packet_summary.start_frame,f);
  /* go backward if necessary */
  while (packet_summary.start_frame > f) {
    int index = packet_summary.start_index;
    index = index - 1;
    if (index<0) index += PACKET_SUMMARY_SIZE;
    struct packet *packet = packet_summary.packets+index;
    if (packet->frame_number!=packet_summary.start_frame-1) {
      init_packet(packet);
      packet->frame_number = packet_summary.start_frame-1;
      }
    /* clip the end if you have to */
    if ((packet_summary.end_frame-packet_summary.start_frame)>PACKET_SUMMARY_SIZE-2) {
      packet_summary.end_frame = packet_summary.start_frame+(PACKET_SUMMARY_SIZE-2);
      }
    packet_summary.start_index=index;
    packet_summary.start_frame--;
    }
  

  /* go forward */
  int ff=packet_summary.start_frame;
  while (ff < f) {
    int index = (packet_summary.start_index + ff - packet_summary.start_frame) % PACKET_SUMMARY_SIZE;
    if (index<0) index += PACKET_SUMMARY_SIZE;
    struct packet *packet = packet_summary.packets+index;
    if (packet->frame_number!=ff) {
      init_packet(packet);
      packet->frame_number = ff;
      }
    ff++;
    }
  
  }

  /* ok, all frames from start_frame to just before our first frame have frame numbers. So now we can add frames as we go in the correct 
  positions via offset. Later we will do a scan from start to finish and clean it up */

int maxframe=-1;
{
  int i;
  for (i=soundringfirst;i!=soundringtail;i = (i+1)%SOUNDRING_COUNT) {
    int f = soundstat[i].frame;
    if (f>maxframe) maxframe=f;
    int index = (packet_summary.start_index + f - packet_summary.start_frame) % PACKET_SUMMARY_SIZE;
    if (index<0) index += PACKET_SUMMARY_SIZE;
    struct packet *packet = packet_summary.packets+index;
    if ((packet->has_statistics==0)||(packet->frame_number != f)) {
      packet->frame_number = f;
      collect_a_packet(i,index);
      }
    }
  }

/* OK - now lets handle if maxframe is so big, that we need to readjust start_frame */
if ((maxframe-packet_summary.start_frame) > PACKET_SUMMARY_SIZE-2) {
  int offset = (maxframe-packet_summary.start_frame) - (PACKET_SUMMARY_SIZE-2);
  packet_summary.start_frame+=offset;
  packet_summary.start_index = (packet_summary.start_index + offset)%PACKET_SUMMARY_SIZE;
  if (packet_summary.start_index<0) packet_summary.start_index += PACKET_SUMMARY_SIZE;
  }
if (maxframe >packet_summary.end_frame) packet_summary.end_frame=maxframe;

/* OK - we collected all the packets.  Now we scan from start to finish to compute statistics, no_gap_frame, end_frame */


packet_summary.soundringsend = send;
packet_summary.soundringnow = now;
packet_summary.soundringtail = tail;
packet_summary.soundringfirst = first;
packet_summary.soundringhead = head;
packet_summary.warming_quality = ((float)total_queue_size) / 25.0;
if (packet_summary.warming_quality>1.) packet_summary.warming_quality=1.;

{
  int f;
  for (f=packet_summary.start_frame;f<packet_summary.end_frame;f++) {
    int index = (packet_summary.start_index + f - packet_summary.start_frame) % PACKET_SUMMARY_SIZE;
    if (index<0) index += PACKET_SUMMARY_SIZE;
    struct packet *packet = packet_summary.packets+index;  
    if (packet->frame_number!=f) {
      init_packet(packet);
      packet->frame_number = f;
      }
    }
  }



{ /* find no_gap_frame */
  packet_summary.no_gap_frame = packet_summary.now_frame;
  while (packet_summary.no_gap_frame>packet_summary.start_frame) {
    int index = (packet_summary.start_index + (packet_summary.no_gap_frame-1) - packet_summary.start_frame) % PACKET_SUMMARY_SIZE;
    if (index<0) index += PACKET_SUMMARY_SIZE;
    struct packet *packet = packet_summary.packets+index;  
    if (!packet->has_music_samples) {
      break;
      }
    packet_summary.no_gap_frame--;
    }
  }
  
{ /* left over analyzing */
  packet_summary.quality=0.;
  }


/* compute tempo times */
if (packet_summary.now_frame != -1) { 
    int index = (packet_summary.start_index + packet_summary.now_frame - packet_summary.start_frame) % PACKET_SUMMARY_SIZE;
    if (index<0) index += PACKET_SUMMARY_SIZE;
    struct packet *packet = packet_summary.packets+index;  
    if (packet->has_statistics) {
        int good=1;
	int bad=0;
	int difference=time_difference_in_microseconds(packet_summary.current_sample_time,packet_summary.tempo_points[0]);
	if (difference<0) bad=1;
	if (difference<200000) good=0;
	if (difference>1000000) bad=1;
	if (!packet->has_beat) good=0;
	if (bad) good=0;
//	logit("diff %d bad %d good %d",difference,bad,good);
	/* bad means reset tempos */
	if (bad) {
	  struct timeval tv;
	  tv.tv_sec = 0;
	  tv.tv_usec = 0;
	  packet_summary.tempo_points[0] = tv;
	  packet_summary.tempo_points[1] = tv;
	  packet_summary.tempo_points[2] = tv;
	  packet_summary.tempo_points[3] = tv;
	  packet_summary.where_in_tempo=-1.;
	  packet_summary.beat_interval_in_microseconds= 5000000;
          packet_summary.beat_interval_in_milliseconds=(packet_summary.beat_interval_in_microseconds+999)/1000;
	  if (packet->has_beat) {
	    packet_summary.tempo_points[0]=packet_summary.current_sample_time;
	    }
	  }
	else if (good) {
	  packet_summary.tempo_points[3] = packet_summary.tempo_points[2];
	  packet_summary.tempo_points[2] = packet_summary.tempo_points[1];
	  packet_summary.tempo_points[1] = packet_summary.tempo_points[0];
	  packet_summary.tempo_points[0] = packet_summary.current_sample_time;
	  packet_summary.where_in_tempo=0.;
	  packet_summary.beat_interval_in_microseconds= difference;
          }
	else {
	  if (packet_summary.tempo_points[1].tv_sec != 0) {
  	    int difference2=time_difference_in_microseconds(packet_summary.tempo_points[0],packet_summary.tempo_points[1]);
	    packet_summary.where_in_tempo = (float)((double)(difference)/(double)(difference2));
	    packet_summary.beat_interval_in_microseconds= difference2;
            packet_summary.beat_interval_in_milliseconds=(packet_summary.beat_interval_in_microseconds+999)/1000;
	    
	    }
	  else {
	    packet_summary.where_in_tempo = (float)((double)(difference)/(double)(5000000)); /* assume a tempo of 0.5 */
            packet_summary.beat_interval_in_microseconds= 5000000;
            packet_summary.beat_interval_in_milliseconds=(packet_summary.beat_interval_in_microseconds+999)/1000;
	    }
	  }	
        } /* if we have statistics */
    } /* if the current frame is good */

final:
/* if nothing, we might be dead - final transition change */
if (packet_summary.state==PACKET_STATE_NOTHING)
{
  if (packet_summary.current_sample_time.tv_sec-packet_summary.start_of_nothing_state.tv_sec>=10)  /* ten seconds - we are dead */
    packet_summary.state=PACKET_STATE_DEAD;
  }

/* wipe out all the old frames if we are newly dead */
if ((packet_summary.state==PACKET_STATE_DEAD)&&(old_state != packet_summary.state)) {
  packet_summary.start_index=0;
  int i;
  for (i=0;i<PACKET_SUMMARY_SIZE;i++) {
    init_packet(packet_summary.packets+i);
    }
  }


determine_final_color(); /* used to tell where the phones are based on ip address */
faze_out_location_estimation_after_a_while();
output_visual_voice(stderr);
}    
    






void finish_packet_summary() {
#ifdef COLLECT_JPG_IMAGES
int i;
for (i=0;i<65536;i++) {
  struct jpg_image *image = jpg_images[i];
  if (image) {
    if (image->chunk_map) free(image->chunk_map);
    if (image->image) free(image->image);
    free(image);
    jpg_images[i]=NULL;
    }
  }
jpg_image_queue_head=0;
jpg_image_queue_tail=0;
#endif
}




long song_frame_from_stream_frame(int stream_frame) {
if (song_frame_to_stream_frame.valid==0) {return -1l;}
int distance = stream_frame - song_frame_to_stream_frame.stream_frame;
if (distance<0) distance = - distance;
if (distance > 2048) {return -1l;}
return (song_frame_to_stream_frame.song_frame + (long)(stream_frame - song_frame_to_stream_frame.stream_frame));
}

void reset_packet_song_frame_to_stream_frame() {
 /* sets to invalid */
song_frame_to_stream_frame = (packet_song_frame_to_stream_frame){.valid=0,.stream_frame=0.,.song_frame=0l};
}

void set_packet_song_frame_to_stream_frame(long song_frame,int stream_frame) {
 /* links it up */
if ((song_frame == -1l)||(stream_frame == -1l)) {
  reset_packet_song_frame_to_stream_frame();
  return;
  }
//logit("		set %ld to %d ps %d\n",song_frame,stream_frame,packet_summary.now_frame);
song_frame_to_stream_frame = (packet_song_frame_to_stream_frame){.valid=1,.stream_frame=stream_frame,.song_frame=song_frame}; 
}


int record_stream_frame(int stream_frame) {
if (!song_frame_to_stream_frame.valid) return(-1);
/* moves the frame along, but resets if frame is bogus */
int distance = stream_frame - song_frame_to_stream_frame.stream_frame;
if (distance<0) distance = - distance;
if (distance > 2048) {
  logit("??? stream_frame from %d to %d\n",song_frame_to_stream_frame.stream_frame,distance);
  reset_packet_song_frame_to_stream_frame();
  return -1;
  }
distance = stream_frame - song_frame_to_stream_frame.stream_frame;
//logit("		change %ld to %ld\n",song_frame_to_stream_frame.song_frame,
//             song_frame_to_stream_frame.song_frame+distance);
song_frame_to_stream_frame.stream_frame += distance;
song_frame_to_stream_frame.song_frame += (long)(distance);
return(0);
}



void output_visual_voice2(FILE *des,long the_frame) {
int m=packet_measurechunkhead;
while  (packet_measurechunktail!=m) {
  packet_measure_chunk *mc = (packet_measure_chunk *)(packet_measure_chunks+m);
  if (the_frame > (mc->beat_buffer_now_frame+mc->beat_buffer_length)) {
    m = (m+1) % MEASURE_CHUNK_COUNT;
    }
  else if (the_frame < mc->beat_buffer_now_frame) {
//     logit("\nframe %ld < %ld\n",the_frame,mc->beat_buffer_now_frame);
     return;
     }
  else {
    long indent = (the_frame-mc->beat_buffer_now_frame);
    int i;
//    logit("v2 %ld p %ld %d\n",the_frame,indent,mc->number_line_buffers);
//    logit("f %ld pos %ld\n",the_frame,indent);
    for (i=0;i<mc->number_line_buffers;i++) {
      char *buf = mc->line_buffer[i];
//      logit("%d ",buf[indent]);
      if ( (buf[indent])&&(buf[indent] != ' ')&&((indent==0)||(buf[indent-1] == ' ')) ) { /* if we found a new word */
        int indent2=indent;
        if (buf[indent2]== '_') { indent2++;} 
        else if (indent2) fputc(' ',des);
        char ch;
        while ((ch=buf[indent2])&&(ch != ' ')) { if (ch != '_')  putc(ch,des); indent2++; }
        }
      }
    if (mc->number_line_buffers==0) {
      fputc(mc->beat_buffer[indent],des);
      }
    return;
    }
  }
return;	
}


int visual_voice_output_for_real=0;
// for reak does the work



void build_visual_voice_lines(packet_measure_chunk *mc) {
for (int i=0;i<mc->number_line_buffers;i++) {
  visual_voice_line *vvl=mc->visual_voice_lines+i;
  vvl->computed = 0;
  vvl->number_of_symbols = 0;
  vvl->done_by=0;
  vvl->current_symbol=-1;
  vvl->symbol_passed_by=0;
  vvl->line_number = i;  
  mc->line_buffer_len[i] = strlen(mc->line_buffer[i]);
  }

/* find the max buffer length */
int max_buffer_length = mc->beat_buffer_length;
for (int i=0;i<mc->number_line_buffers;i++) {
  if (mc->line_buffer_len[i]>max_buffer_length) max_buffer_length = (mc->line_buffer_len[i]);
  }
  
  
long indent;
long indents[SR_MEASURE_NUM_LINES];
long indent2s[SR_MEASURE_NUM_LINES];
for (int i=0;i<mc->number_line_buffers;i++) {
  indents[i]=0;
  }

for (indent=0;indent<max_buffer_length;indent++) {
  int new_word=0;
  int new_words[SR_MEASURE_NUM_LINES];
  for (int i=0;i<mc->number_line_buffers;i++) {
    new_words[i]=0;
    if (indent>indents[i]) indents[i] = indent; // some lines might be mid-word
    }
  
  for (int i=0;i<mc->number_line_buffers;i++) {
    if (( indent <mc->line_buffer_len[i])&& (indents[i]==indent)) { // if there are characters and we do not skip them becaue we are mid word
      char *buf = mc->line_buffer[i];
      if ( (buf[indent])&&(buf[indent] > ' ')&&((indent==0)||(buf[indent-1] <= ' ')) ) { /* if we found a new word */
        new_words[i]=1;
	new_word=1;
	}
      }

    } /* for each line */
  
  if (new_word) {
    for (int i=0;i<mc->number_line_buffers;i++) {
      indent2s[i] = indent;
      }
    for (int i=0;i<mc->number_line_buffers;i++) {
      char *buf = mc->line_buffer[i];
      visual_voice_line *vvl=mc->visual_voice_lines+i;
      if (new_words[i]) {
        vvl->symbol_indent[vvl->number_of_symbols]=indent;
	vvl->on_this_line[vvl->number_of_symbols]=1;
        char *destbuf=vvl->symbols[vvl->number_of_symbols];
        int desti=0;
        int space=1;
        if (buf[indent2s[i]]== '_') { (indent2s[i])++;
           space=0;}
        else if (indent2s[i]==0) {
          space=0;
          }
        vvl->space_prefix[vvl->number_of_symbols] = space;
    
        while((indent2s[i]<mc->line_buffer_len[i]) &&
	      (buf[indent2s[i]]) &&
	      (buf[indent2s[i]] >' ') &&
	      (desti+1<VISUAL_SYMBOL_MAX_SIZE) ) {
          destbuf[desti] = buf[indent2s[i]];
          desti++;
          (indent2s[i])++;
          }
        destbuf[desti]='\0';
	indents[i]=indent2s[i];
        if (!desti) { // weird case no chars
          vvl->symbol_indent[vvl->number_of_symbols]=-1l;
  	  vvl->on_this_line[vvl->number_of_symbols]=0;
          vvl->space_prefix[vvl->number_of_symbols] = 0;
	  vvl->symbols[vvl->number_of_symbols][0]='\0';
          }
	}
      else { /* this is a fake symbol as a placeholder */
        vvl->symbol_indent[vvl->number_of_symbols]=-1l;
	vvl->on_this_line[vvl->number_of_symbols]=0;
        vvl->space_prefix[vvl->number_of_symbols] = 0;
	vvl->symbols[vvl->number_of_symbols][0]='\0';
        }
      (vvl->number_of_symbols)++;
      } /* for each indent */        
    } /* if we found a new word somewhere */      
  } // while going through each character

for (int i=0;i<mc->number_line_buffers;i++) {

  visual_voice_line *vvl=mc->visual_voice_lines+i;

  vvl->computed = 1;
  }
} // build_visual_voice_lines




void visual_voice_lines_done(int m,packet_measure_chunk *mc,
             long old_the_frame,long the_frame) {
int i;
for (i=0;i<mc->number_line_buffers;i++) {
  visual_voice_line *vvl = mc->visual_voice_lines+i;
  vvl->current_symbol=-1;
  vvl->done_by++;
  }
}


void visual_voice_lines_ready(int m,packet_measure_chunk *mc,long old_the_frame,long the_frame) {
int i;
for (i=0;i<mc->number_line_buffers;i++) {
  visual_voice_line *vvl = mc->visual_voice_lines+i;
  if (vvl->computed==0) {
    build_visual_voice_lines(mc);
    return;
    }
  }
}



void visual_voice_lines_run(int m,packet_measure_chunk *mc,long old_the_frame,long the_frame) {
int i;
for (i=0;i<mc->number_line_buffers;i++) {
  visual_voice_line *vvl = mc->visual_voice_lines+i;
  if (vvl->computed==0) {
    build_visual_voice_lines(mc);
    break;
    }
  }

for (i=0;i<mc->number_line_buffers;i++) {
  visual_voice_line *vvl = mc->visual_voice_lines+i;
  
  long indent = the_frame - mc->beat_buffer_now_frame;
  char *buf = mc->line_buffer[i];
  if ( (buf[indent])&&(buf[indent] != ' ')&&((indent==0)||(buf[indent-1] == ' ')) ) { /* if we found a new word */
    /* find the symbol */
    int j;
    for (j=0;j<vvl->number_of_symbols;j++) {
      if (vvl->symbol_indent[j] == indent) {
        vvl->current_symbol=j;
	vvl->symbol_passed_by=0;
	break;
	}
      }
    }
  vvl->symbol_passed_by++;
  } /* for each line */
}



void setup_visual_voice_for_screen(long old_the_frame,long the_frame,
   int visual_voice_output_for_real) {

int m=packet_measurechunkhead;

while  (packet_measurechunktail!=m) {
  packet_measure_chunk *mc = (packet_measure_chunk *)(packet_measure_chunks+m);
  visual_voice_lines_ready(m,mc,old_the_frame,the_frame);
  
  if (the_frame > (mc->beat_buffer_now_frame+mc->beat_buffer_length)) {
    visual_voice_lines_done(m,mc,old_the_frame,the_frame);
    m = (m+1) % MEASURE_CHUNK_COUNT;
    }
  else if (the_frame < mc->beat_buffer_now_frame) {
//     logit("\nframe %ld < %ld\n",the_frame,mc->beat_buffer_now_frame);
    visual_voice_lines_ready(m,mc,old_the_frame,the_frame);

    m = (m+1) % MEASURE_CHUNK_COUNT;
    }
  else {
    long indent = (the_frame-mc->beat_buffer_now_frame);
    visual_voice_lines_run(m,mc,old_the_frame,the_frame);
    m = (m+1) % MEASURE_CHUNK_COUNT;
    }
  }
return;	
}




/**/
void output_visual_voice(FILE *des) {
}




int sr_gettimeofday(struct timeval *tv, struct timezone *tz) {
  return gettimeofday(tv,tz);
}
       


/* 
$Log: packet_summary.c,v $
Revision 1.56  2020/02/07 18:52:13  hib
Drop the libbpg stuff

Revision 1.55  2020/02/02 13:02:03  hib
got silentradiance to compile on ios

Revision 1.54  2019/12/06 12:20:18  hib
Got it to sync up in linux pretty well with soundringnow - but sometimes it goofs up
So still debugging

Revision 1.52  2019/11/30 15:35:45  hib
making the timing right

Revision 1.51  2019/11/25 17:26:54  hib
Fixed bug in processor. minor

Revision 1.50  2019/09/18 04:19:37  hib
wip - got it actually going the mtv demo size

Revision 1.49  2019/09/17 04:09:41  hib
wip on mtv demo maker

Revision 1.48  2019/09/16 14:44:56  hib
work in progress for video creation

Revision 1.47  2019/05/18 21:24:40  hib
minor changes for internationalization

Revision 1.46  2019/05/15 00:28:07  hib
Got resplandor separated
got demos separated - and yes, they compile - new frame logic for video frames untested
Did not finish the mtv like - first make replandor for android
and very first - add nova effect behind GG

Revision 1.45  2019/04/15 11:29:48  hib
OK. NOW this is the correct version for internationalization on linux!

Revision 1.44  2019/04/15 10:40:50  hib
WORLD_ macros have now set up silent radiance to be internationalized.
It is easier to implement a system where there are separate builds,
but I could handle a weird multi-languge on one build with more trickiness.

The world_internatiolization.h file sets it up
Currently just english

Revision 1.43  2019/03/31 19:16:54  hib
nting on the apple
VS: ----------------------------------------------------------------------

Revision 1.42  2018/12/25 17:09:52  hib
Made sound_to_file use the current to point tot he current + next

Revision 1.41  2018/12/22 15:39:11  hib
Well, silent radiance still works.  the demo maker - got it to dump some files upside down.
Switched sr_demo to run the file reader in the background, and the sound in the foreground-  switching abck and forth from sound_to_file.c and sound_alsa.c - well sound_alsa.c usually works but doesn t close the app. and the other one freezes.
Also the video stopped being outputted.
Need to take a break from it.

Revision 1.40  2018/12/22 07:26:17  hib
the demo silent radiance is not processing files for some reason.  but we are progressing further along making a video for a song.

Revision 1.39  2018/09/30 18:50:45  hib
getting there

Revision 1.38  2018/09/28 17:47:20  hib
After apple 1.2, before android 1.2
Linting the files

Revision 1.37  2018/09/22 17:45:36  hib
wip

Revision 1.36  2018/09/19 15:07:54  hib
ready for production apple

Revision 1.35  2018/09/19 00:09:28  hib
Fixed the problem
Didnt show up in linux, because memory was zeroed.

Revision 1.34  2018/09/18 02:38:19  hib
Much better - still need to rotate, and multi color for multi line

Revision 1.33  2018/09/18 00:19:31  hib
Rewrote the visual voice stuff - more stable - but it doesnt work yet.
Also, it      handle     lines      are     the        segment
         can         the lines that     on      same

Revision 1.32  2018/09/17 19:09:43  hib
did some cleanup for visual voice - still needs to rotate with apple, and still not displaying correctly.

Revision 1.31  2018/08/25 10:53:31  hib
made it do more lines - still goofs up.

Revision 1.30  2018/07/29 20:44:08  hib
wip

Revision 1.29  2018/07/26 10:29:03  hib
nailed it.

Revision 1.26  2018/07/25 22:57:36  hib
wip

Revision 1.25  2018/07/25 22:48:59  hib
wip

Revision 1.24  2018/07/25 07:56:30  hib
cleanup for the ios

Revision 1.23  2018/07/25 00:38:32  hib
wip

Revision 1.22  2018/07/22 13:33:21  hib
Got it working well.   The delays are handled on both sides

Revision 1.21  2018/07/22 08:22:00  hib
kinda works. Seems to be missing frame 0 for some reason

Revision 1.20  2018/07/22 04:15:02  hib
We got the frame number, now to add the vocals voice (command v and V

Revision 1.19  2018/05/03 19:08:33  hib
Work in progress

Revision 1.18  2018/01/20 22:13:38  hib
Gor rid of fprintfs

Revision 1.17  2018/01/20 22:10:07  hib
added 2018

Revision 1.16  2018/01/09 10:46:35  hib
Got top menu tpo work on android! Real tough!

Revision 1.15  2017/12/30 04:29:49  hib
better

Revision 1.14  2017/12/30 02:00:42  hib
work in progress

Revision 1.13  2017/12/12 03:27:52  hib
wip

Revision 1.12  2017/12/01 04:45:48  hib
Made all the graphical apps check to see if ran before so we don't recreate all the programs, etc.
just before we did it, images was close to working on android.
Thats our main focus right now.

Revision 1.11  2017/11/15 11:57:23  hib
The image display is working pretty good on ios.  now for the other platforms.

Revision 1.10  2017/11/08 00:21:24  hib
Launched the visualizer V1

Revision 1.9  2017/11/02 04:00:59  hib
causing problem on win32 - lots of bad packets - dont yet know why.

Revision 1.8  2017/10/29 10:06:11  hib
got an image through!

Revision 1.7  2017/10/29 03:17:55  hib
its a big deal to get a picture in.  work in progress. the send command looks good, but something happens
to the server afterwards or on processor or something.

Revision 1.6  2017/10/25 22:28:09  hib
fixing the transmitter

Revision 1.5  2017/07/05 12:40:10  hib
Added copyleft

Revision 1.4  2016/12/22 00:44:03  hib
*** empty log message ***

Revision 1.3  2016/12/21 01:16:01  hib
consolidating

Revision 1.2  2016/12/20 22:24:44  hib
got rid of some messages

*/
