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
#ifndef SILENTRADIANCESIMPLE_H
#define SILENTRADIANCESIMPLE_H

#include <stdio.h>
#include <stdlib.h> /* for atoi() and exit() */
#include <string.h> /* for memset() */
#include <unistd.h> /* for close() */
#include <errno.h>



#ifdef __cplusplus
extern "C" {
#endif








/* This is a simple silent radiance set of routines which will allow access to streaming silent radiance radio, wither in 8 bit or 7 bit format.

The idea here is to have this code incorporated in video games to provide ear candy while saving the universe, or killing 3 million monsters.  

This does not handle the commands being passed ( like t command to draw text for the song title).

This does not handle wireless broadcast.

This does use globals.






Generally, there are three different main sections of code:

1. Collecting the web based stream information.  Currenly, this is done with libcurl.  However, there are other 
modules for other systems that will be added.

    This information might be in 8 bit format, or 7 bit format.   The 7 bit format is designed for EMScripten, which cannot stream clean 8 bit ongoing
because UTF8 is lame.
    Also, there is an encryption on the information.   So a decrypt1 function is part of this section.



2. Processing the packets.   This is done with simple_processor.c - This processor is Much simpler than the main Silent Radiance code, as it eskews
   commands, multi-opus-packet packets, and statistics gathering.   The code is similar to processor.c in the main SilentRadiance branch.
   
   
   
   
3. Outputting the packets to an audio device.   Currently this is done in alsa.   However, I will port other modules for other systems as well, (SDL 1.2, SDL 2.0, OpenAL, etc).




The configuration will have various ways to mix and match these.






So using this with your own radio station is free, but there are radio stations (ps.silentradince.com for psytrance and ds.silentradiance.com for dubstep) that 
stream copyrighted material - via direct copyright, not statuatory streaming.  This means that these stations can be used in commercial games, provided there is added functionality to 
buy music and or other media that have recently been played.  

TL;DR - the music is free in your games, if you prop up a store for the music in your game.



I will be cleaning up the globals and making everything sorta C-like,  with some of these changes going back into the original Silent Radiance code.

Enjoy






*/




/*----------------------------------------------------
globals

----------------------------------------------------*/
#define SOUNDRING_COUNT 400  
/* ^^^ the number of sound buffers (1/200th of a second 240 sample chunks) that we fill. 
  This can be adjusted, but works pretty well.
  When the SOUNDRING_COUNT buffer is getting full, the streaming code generally will stop reading packets while waiting for the sound to be played.
  */
#define COLLECTION_COUNT 400

#ifndef PER_FRAME_OVERHEAD
#define PER_FRAME_OVERHEAD 16
#endif








/*----------------------------------------------------
Stream

----------------------------------------------------*/

extern int we_are_streaming_web;  // flag - set to 1 if we are currently streaming and 0 if not.
// init_web_stream starts it, shutdown_web_stream can set it to 0.  
// Also, channel changing (currenntly not there) will futz with it.



void init_web_stream(char *url);    // Initialize and open the stream
int get_packet_from_web_stream(int *precvStringLen,unsigned char *packetbuffer);   // Get the packet from the web stream.  7 bit and 8 bit formats are both supported.
void shutdown_web_stream();         // disconnect the web stream, and reset all that.  


/*
stream flows packets into the sound ring buffer
*/




/* 7/8 bit decode is used by the stream processor.  
By using 0-127, and 255 codes, the 7 bits available in UTF8, and the one error bit can come over a javascript web site.

Generally:   http://radiowherever/download.cgi - is an 8 bit download
and          http://radiowherever/download7.cgi - is the 7 bit download, which is 12% bigger


This code is pretty comlex. Be glad its all written. :)
*/

typedef struct sr_simple_seven_eight_decode {
  unsigned char seven_to_eight_last_buf[8]; /* the codec cannot always complet the task, so we might buffer up to 8 chars
    It is just because 8 to the hotdog and "7" to the bun */
    
  int seven_to_eight_res_count; /* default to = 0;*/
     /* if using this, be sure to reset this if there is a disription in the byte stream */
  int seven_eight_locked_eight; /* default =0;   used to determine if the stream is eight bit or seven bit and lock it in on the stream */
  int seven_eight_locked_seven; /* default =0;  used to determine if the stream is eight bit or seven bit and lock it in on the stream */
  
  
  int seven_to_8_broken;

  } sr_simple_seven_eight_decode;


extern sr_simple_seven_eight_decode *new_sr78dec();
extern void init_sr78dec(sr_simple_seven_eight_decode  * sr78dec);
extern int sr78dec_do_we_know_if_we_are_7_bit(sr_simple_seven_eight_decode  * sr78dec); /* called deep in the web get process */
extern int sr78dec_we_are_7_bit(sr_simple_seven_eight_decode * sr78dec,unsigned char *data,int len,char *optional_content_type);  /*an initial cheat so we can detect 7 bit or 8 bit automatically */
/* this code is very tricky. ha! */
extern int sr78dec_seven_to_8(sr_simple_seven_eight_decode * sr78dec,unsigned char *destination,unsigned char *osource,int len7);
/* it remembers state because you cannot always do a multiple of 7 buffers */
/* destination and source can be the same buffer *IF - destination is source-8.  */

extern int sr78dec_seven_to_8_break(sr_simple_seven_eight_decode * sr78dec);

extern void sr78dec_reset_seven_eight_low_level(sr_simple_seven_eight_decode *sr78dec);













/*---------------------------------------------
processor 
-----------------------------------------------*/
/* The processor reads and evaluates a packet and then calls opus to decode the packet to the sound ring buffer.
The sound is added to the tail, unless the head if full, which might cause dome data to be clipped off for stragglers.
Currently, the buffer size is 2 seconds.
*/

extern void init_processor(void);
extern void process_packet(int recvStringLen, unsigned char *packetbuffer);
extern void shutdown_processor(void);







/*--------------------------------------
Sound
---------------------------------------*/

/* these ring variables are controlled by both the 
extern volatile int soundringfirst; /* first one on record - we can go back that far */
extern volatile int soundringcurrent; /* we are playing this RIGHT NOW: -1 if we are not playing.  This i*/

extern volatile int soundringhead;
extern volatile int soundringtail;
extern volatile short soundring[SOUNDRING_COUNT][960];  /* points to soundring_area */


#define NUMBER_OF_PACKETS_BEFORE_ACTIVATING_SOUND 80
/* ^^^ not sure if theis is used anymore */











extern int init_receiver_sound(int block); /* calls the sound init stuff. This plays sterer 48000 hz, no exceptions. */

extern int playOneSoundBuffer(); /* meant to be called from a separate thread, or a fast non-blocking loop, or callback. Whatever*/
extern void finish_stream_sound(); /* hush hush! */









#ifdef __cplusplus
}
#endif



#endif
/* ^^^ to s3e dont include twice */
