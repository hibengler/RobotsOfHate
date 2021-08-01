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






//
// Created by hib on 6/17/16.
//


#include "simple_processor.h"
#include <string.h>

#ifndef PACKETMAXSIZE
#define PACKETMAXSIZE 4000
#endif

#ifndef logit
#define logit(...) {fprintf(stderr,__VA_ARGS__);fprintf(stderr,"\n");}
#endif

OpusDecoder *opusdecoder=NULL;

static int frame=0;
FILE *write_file_spec= NULL;

volatile int soundringhead=0; /* start of the soundringqueue. normally incremented by sound.  But could overflow */
volatile int soundringfirst=-1; /* also incrmented by processor.c - probably the first viable sound in the ring */
volatile int soundringlast=-1; /* also incrmented by processor.c - probably the first viable sound in the ring */
volatile int soundringtail=0; /* the end on the sound ring. this is incremented by processor.c whenever a packet is added */
volatile int soundringsend=-1; /* we are sending this to the sound system  RIGHT NOW: -1 if we are not playing.  This is updated by the sound subsystem */
volatile int soundringnow=-1; /* we are playing this RIGHT NOW: -1 if we are not playing.  This is updated by the sound subsystem */


 
#define DEFAULT_FOR_THE_SOUND_DELAY 31
  
volatile int the_sound_delay=DEFAULT_FOR_THE_SOUND_DELAY; /* this can be set by defaults or something */
    
/*

commands -> the commands active via frame

This is for SR01 and SR00.

soundring - goes to sound.
commandring - list of latest commands - useful for SR01, SR00 not so much
commandring is congruent with soundring.   

            -> sound
*/	    


volatile short soundring[SOUNDRING_COUNT][960];  /* points to soundring_area */




  
  
volatile struct processor_stat soundstat[SOUNDRING_COUNT];

volatile char commandring[SOUNDRING_COUNT][4000]; /* points to commandring_area */
volatile int commandlen[SOUNDRING_COUNT];





/* This handles the multi-packet stuff */
int base_frame=-1;



int command_frame[COLLECTION_COUNT];/* -1 if unknown and need to be processed */








int opus_frame=-1;
float minpcm=0.;
float maxpcm=0.;




void dump_full(int sampleSize,char *packet,int commandstart,int commandend) {
  int j;
  for (j=0;j<sampleSize;j++) {
    fprintf(stderr," %4.4d",j);
    if ((j%50)==49)
           fprintf(stderr,"\\\n");
    }
  fprintf(stderr,"\n");
  
  for (j=0;j<sampleSize;j++) {
    if (j==commandstart) {
      fprintf(stderr,"<%3.3d ",packet[j]);
      }
    else if (j==commandend-1) {
      fprintf(stderr," %3.3d>",packet[j]);
      }
    else {
      fprintf(stderr," %3.3d ",packet[j]);
      }
    if ((j%50)==49)
           fprintf(stderr,"\\\n");
    }
  fprintf(stderr,"\n");
}

void init_processor_buffers() {

{
  int i;
  for (i=0;i<SOUNDRING_COUNT;i++) {
      volatile struct processor_stat *stat;
      stat =soundstat+i;
      stat->len=0;
      stat->frame=-1;
      stat->version=0;


      command_frame[i]=-1;
      }

  }

    soundringhead=0;
    soundringtail=0;
    soundringfirst=-1;
    soundringlast=-1;
    soundringsend=-1;
    soundringnow=-1;
    base_frame=-1;
      

    frame = -1;
}

     




void init_processor() {
opusdecoder = NULL;
frame = 0;

    { /* opus */
        int err;
        opusdecoder = opus_decoder_create((opus_int32)(48000),2,&err);
        if (err != OPUS_OK)
        {
            logit("OPUS decoder fail %d\n",err);
            opusdecoder = NULL; /* error - decode will just be off */
	    return;
        }

    }

init_processor_buffers();
return;
}



void shutdown_processor() {
}


int process_packet_main(int flag_override,int recvStringLen,unsigned char *packet) {

int justwrite=0;
short *soundBuffer;
int soundBufferSize = 0;
//fprintf(stderr,"process_packet_main override %d sampleSize %d\n",flag_override,sampleSize);
if (!opusdecoder) return -1;
    
int sampleSize=recvStringLen;




if (sampleSize<12+2) return -1;
if (sampleSize >PACKETMAXSIZE-2) {
//  logit("sampleSize too big");
  return -1;
  }

// should handle sr00 and sr01 - just reads the one packet    



unsigned int flag=0;
{ /* compute flag */
        int l = packet[0]+packet[1]*256+PER_FRAME_OVERHEAD;
	if (l<=PER_FRAME_OVERHEAD) {
	  logit("bad collection no first frame");
	  return -2;
	  }
	if (l>sampleSize) {
	  logit("bad collection first length too big: %d",l);
	  return -3;
	  }
}

    
    
int framestart;
int framesize;
/* figure out how many opus packets are there. */
if ((packet [0])||(packet[1]) ) {
  framestart=2;
  unsigned int pl1 = packet[0];
  unsigned int pl2 = packet[1]; 
  pl2 = pl2 <<8;

  framesize= pl1+pl2;
  }
else return (-5);

int opus_result;
float pcm[480];
int tail = soundringtail;
soundBuffer = (short *)soundring[tail];
soundBufferSize = 0;
opus_result = opus_decode_float(opusdecoder,((unsigned char *)(packet))+2,
                                            (opus_int32)(framesize),pcm,(int)240,(int)0);
  int i;
  if (opus_result != 240) {
    logit("opus_Result is %d not 240\n",opus_result);
    return(-6);
    }
  for (i=0;i<480;i++) {
    short s;
                    //     if (i<3)   	        logit("test %d %f,%f\n",i,pcm[i+i],pcm[i+i+1]);
    float ma;
    float m;
    ma=pcm[i];
    m=ma*32767.;
    if (m>32767.) {
      s=32767;
      }
    else if (m<-32767.) {
      s=-32767;
      }
    else {
      s=(int)m;
      }
    soundBuffer[soundBufferSize++]=s;
    } /* for each sample * 2 channels */

if ((packet[sampleSize-1])=='1') { /* SR01 older transmission */
    int l = sampleSize-20;
    unsigned int i1=packet[l]&0xff;
    unsigned int i2=packet[l+1]&0xff;
    unsigned int i3=packet[l+2]&0xff;   
    unsigned int i4=packet[l+3]&0xff;

    frame = i1+(i2<<8)+(i3<<16)+(i4<<24);
  }
else   {/* SR00 */
    int l = sampleSize-8;
    unsigned int i1=packet[l]&0xff;
    unsigned int i2=packet[l+1]&0xff;
    unsigned int i3=packet[l+2]&0xff;   
    unsigned int i4=packet[l+3]&0xff;

    frame = i1+(i2<<8)+(i3<<16)+(i4<<24);
  }

memcpy((void *)(soundstat[tail].stats_area),
        ((unsigned char *)(packet))+2+framesize,PER_FRAME_OVERHEAD);
soundstat[tail].frame = frame;
soundstat[tail].len = 480;
memcpy((void *)(&soundstat[tail].version),(void *)(packet+sampleSize-4),4*sizeof(char));


/* find the command start end end */
int commandend;
int commandstart;

if ((packet[sampleSize-1])=='1') { /* SR01 older transmission */
  commandend=sampleSize; /* 40 bytes overhead */
  }
else { // sr00 older transmission
  commandend =sampleSize; /* 20 bytes overhead */
  }



int look;
look=0;
/* figure out how many opus packets are there. */
int opus_frames = 0;
while ( ( (packet [look])||(packet[look+1]) )&&(opus_frames<12)&&(look<sampleSize)) {
            /* found a frame. */   
            unsigned int pl1 = packet[look];  
            unsigned int pl2 = packet[look+1];
            pl2 = pl2 <<8;
 
            int framesize= pl1+pl2;
//          fprintf(stderr,"framestart %d opus_frames %d framesize[opus_frames] %d\n",
//              framestart[opus_frames],opus_frames,framesize[opus_frames]);
            // if framesize is too big, we will skip this frame
            if ( framesize +look+2+PER_FRAME_OVERHEAD >sampleSize) {
	      commandlen[tail] = 0;
              goto ohwell;
              }
            

            look=look+2+framesize+PER_FRAME_OVERHEAD;
            opus_frames++;
            

            }
// we ignore multi-frame packets in simple - but need to find the command start anyways.

commandstart = look+2;

/* place the last chunk into collectioncommands - collectioncommandcount - is a modulo counter */
commandlen[tail]=commandend-commandstart;
if (commandlen[tail]<0) {
  commandlen[tail] = 0;
  goto ohwell;
  }
memcpy(commandring[tail],(packet+commandstart),commandlen[tail]);
if ((packet[sampleSize-1])!='0') { /* SR01 older transmission */
  fprintf(stderr,"aha bad 08\n");
  dump_full(sampleSize,packet,commandstart,commandend);
  }

#ifdef safhdjkasdfhjfasdkasdjkf
if ((commandlen[tail])&&(commandlen[tail]!=12)) {
  fprintf(stderr,"bad - packet %lx commandstart %d commandend %d samplesize %d\n",(long)packet,commandstart,commandend,sampleSize);
  int j;
  for (j=0;j<sampleSize;j++) {
    fprintf(stderr," %4.4d",j);
    }
  fprintf(stderr,"\n");
  
  for (j=0;j<sampleSize;j++) {
    if (j==commandstart) {
      fprintf(stderr,"<%3.3d ",packet[j]);
      }
    else if (j==commandend-1) {
      fprintf(stderr," %3.3d>",packet[j]);
      }
    else {
      fprintf(stderr," %3.3d ",packet[j]);
      }
    }
  fprintf(stderr,"\n");
      
    
  } 
#endif
ohwell:

command_frame[tail]=frame;


soundringtail= (tail+1) % SOUNDRING_COUNT;
if (soundringhead==soundringtail) {
  soundringhead = (soundringhead+1) % SOUNDRING_COUNT;
  }





return 0;
} /* process_packet_main */








void process_packet_ignore_framestart(int recvStringLen, unsigned char *packet)
{
int t=process_packet_main(4,recvStringLen,packet);
if (t) fprintf(stderr,"e%d.",t);
}

void process_packet(int recvStringLen, unsigned char *packet) 
{
int t=process_packet_main(0,recvStringLen,packet);
if (t) fprintf(stderr,"e%d.",t);
}
