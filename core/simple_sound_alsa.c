/*
Silent Radiance wifi broadcast for digital silent disco.
Copyright (C) 2017 Hibbard M. Engler

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



/*

This example reads standard from input and writes
to the default PCM device for 5 seconds of data.

*/

    

/* Use the newer ALSA API */
#define ALSA_PCM_NEW_HW_PARAMS_API
#include <stdio.h> /* for printf() and fprintf() */
#include <stdlib.h> /* for atoi() and exit() */
#include <string.h> /* for memset() */
#include <unistd.h> /* for close() */
#include <errno.h>

/*#define THREADING_ON 1 
^^^ for threadding example - doesnt help much just burns a cpu */

#include "simple_processor.h"

#include <alsa/asoundlib.h>
#include "simple_processor.h"
#include "simple_sound_alsa.h"
#include "simple_compute_soundringnow.h"

static volatile int wipe_sound=0;  // quick buffer clear

short zero_buffer[480];

short * getNextQueue240() {

if (wipe_sound) { // quick to help clear when changing the channel
  wipe_sound=0;
  soundringlast = soundringhead;
  soundringtail = soundringhead;
  soundringsend = -1;
//  compute_soundringnow();
  }
  
char status;
   status='z';
      short *nextBuffer=zero_buffer;

  
    if (soundringhead == soundringtail) { /* Empty zero it! */
        status='a';
        soundringsend=-1;
    } else {
        if (soundringsend==-1) {  /* was emptied. Check to see if we have enough */
            /* Here we should wait until we got at least 4 or so */
            int queue_size;
            queue_size = soundringtail-soundringhead;
            if (queue_size<0) queue_size += SOUNDRING_COUNT;
	    
            if (queue_size>24) { /* ok, we have 24 consecutive ones. Go at it! */
                soundringfirst=soundringhead;
                soundringsend=soundringhead;
		
		
		status='2';
                }
	    }
        /* OK - here we may or may not be doing real sound.  so do one of the other */
        if (soundringsend==-1) { /* still not good enough */
            /* do nothing here - we are already set to zero_buffer */
        }
        else if (soundringsend==soundringhead) { /* Good enough */
	    if (soundringfirst<0) soundringfirst=soundringhead;
            soundringsend = soundringhead;
            nextBuffer = (short *)soundring[soundringsend];
	    status=' ';
            soundringhead = (soundringhead + 1) % SOUNDRING_COUNT;
        } else { /* soundringsend is pointing ot the last frame. See if the frames are consecutive */
	    if (soundringfirst<0) soundringfirst=soundringhead;
            soundringsend = soundringhead;
            nextBuffer = (short *)soundring[soundringsend];
            soundringhead = (soundringhead + 1) % SOUNDRING_COUNT;
            }
        }
    compute_soundringnow();
    if (nextBuffer==zero_buffer) {fprintf(stderr,"%c",status);}
    return  nextBuffer;

}


  int init_send_sound=1;
  long loops;
  int rc;
  int size;
  snd_pcm_t *handle;
  snd_pcm_hw_params_t *params;
  unsigned int val;
  int dir;
  snd_pcm_uframes_t frames;
  char *buffer;
  struct pollfd *ourpolls;
  int soundpolls_count;
  int socket_poll;
  
  int sound_state = 0;
  
  static int frame=0;
int no_sound = 0;







int init_receiver_sound(int block) {
  
  /* Open PCM device for playback. */
  if (!block) {
#define WORLD_simple_sound_alsa_default "default"
    rc = snd_pcm_open(&handle, WORLD_simple_sound_alsa_default,1,
                    SND_PCM_STREAM_PLAYBACK);
    }
  else {
#define WORLD_simple_sound_alsa_default "default"
    rc = snd_pcm_open(&handle, WORLD_simple_sound_alsa_default,0,
                    SND_PCM_STREAM_PLAYBACK);
    }
  
  if (rc < 0) {
    fprintf(stderr,
#define WORLD_unable_to_open_pcm_device_colon___pct_s_lf_ "unable to open pcm device: %s\n"
            WORLD_unable_to_open_pcm_device_colon___pct_s_lf_,
            snd_strerror(rc));
    exit(1);
  }
  /* Allocate a hardware parameters object. */
  snd_pcm_hw_params_alloca(&params);
    

  /* Fill it in with default values. */
  snd_pcm_hw_params_any(handle, params);

  /* Set the desired hardware parameters. */

  /* Interleaved mode */
  snd_pcm_hw_params_set_access(handle, params,
                      SND_PCM_ACCESS_RW_INTERLEAVED);

  /* Signed 16-bit little-endian format */
  snd_pcm_hw_params_set_format(handle, params,
                              SND_PCM_FORMAT_S16_LE);

  /* Two channels (stereo) */
  snd_pcm_hw_params_set_channels(handle, params, 2);

  val = 48000;
  snd_pcm_hw_params_set_rate_near(handle, params,
                                  &val, &dir);

int err;

  /* Set period size to 32 frames. */
  frames = 240;
  rc=snd_pcm_hw_params_set_period_size_near(handle,
                              params, &frames, &dir);
			      
  snd_pcm_uframes_t f;
  rc = snd_pcm_hw_params_get_period_size(params, &f, &dir);
          if (rc < 0) {
	                  fprintf(stderr,"Unable to get period size for playback: %s\n", snd_strerror(err));
         exit(-1);
         }
  fprintf(stderr,"period size is %d\n",(int)f);




       /* set the buffer time */
	unsigned int time_buffer = 250000;
        err = snd_pcm_hw_params_set_buffer_time(handle, params,time_buffer,dir);
        if (err < 0) {
                fprintf(stderr,"Unable to set buffer size: %s\n",snd_strerror(err));
		
        //        exit(-1);
        }

snd_pcm_uframes_t acsize;
        err = snd_pcm_hw_params_get_buffer_size(params, &acsize);
        if (err < 0) {
                fprintf(stderr,"Unable to get buffer size for playback: %s\n", snd_strerror(err));
            exit(-1);
        }
  fprintf(stderr,"buffer size is %d\n",(int)acsize);

  
						  

if (rc<0) 			      
    fprintf(stderr,
#define WORLD_unable_to_set_hw_parameters_colon___pct_s_lf_ "unable to set hw parameters: %s\n"
            WORLD_unable_to_set_hw_parameters_colon___pct_s_lf_,
            snd_strerror(rc));

  /* Write the parameters to the driver */
  rc = snd_pcm_hw_params(handle, params);
  if (rc < 0) {
    fprintf(stderr,
#define WORLD_unable_to_set_hw_parameters_colon___pct_s_lf_ "unable to set hw parameters: %s\n"
            WORLD_unable_to_set_hw_parameters_colon___pct_s_lf_,
            snd_strerror(rc));
    exit(1);
  }

  /* Use a buffer large enough to hold one period */
  snd_pcm_hw_params_get_period_size(params, &frames,
                                    &dir);
				    
    snd_pcm_prepare(handle);
    
{ /* get the poll descriptiors */
  int err;
  soundpolls_count = snd_pcm_poll_descriptors_count(handle);
  fprintf(stderr,"count is %d\n",soundpolls_count);
  ourpolls = malloc(sizeof(struct pollfd)*(soundpolls_count+1)); /* extra one for listening to the socket. */
  socket_poll = soundpolls_count;
  if ((err = snd_pcm_poll_descriptors(handle,ourpolls,soundpolls_count))<0) {
    fprintf(stderr,"unable to obtain poll descriptors for playback: %s\n",snd_strerror(err));
    exit(-1);
    }
  int i;
  for (i=0;i<soundpolls_count;i++) {
    ourpolls[i].events = POLLERR|POLLOUT;
    }
  }

  size = 4000;
  buffer = (char *) malloc(size);
  

sound_state=0;
{
  int i;
  for (i=0;i<480;i++) {
//    zero_buffer[i]=(i*80)%32768;   /* for debugging */
    zero_buffer[i]=0;
    }
  }

return 0;
}


int finish_play_sound() {
  snd_pcm_drain(handle);
  snd_pcm_close(handle);
  free(buffer);
return(0);
}


void DieWithError(char *errorMessage); /* External error handling function */

void DieWithError(char *errorMessage)
{
perror(errorMessage);
exit(1);
}


void finish_stream_sound() {
wipe_sound=1;
soundringhead = soundringtail; // was -1 everywhere
//if (soundringhead<0) soundringhead +=SOUNDRING_COUNT;
}






static int xrun_recovery(snd_pcm_t *handle, int err)
{
        if (err == -EPIPE) {    /* under-run */
                err = snd_pcm_prepare(handle);
                if (err < 0)
#define WORLD_Can_qt_t_recovery_from_underrun_comma__prepare_failed_colon___pct_s_lf_ "Can't recovery from underrun, prepare failed: %s\n"
                        printf(WORLD_Can_qt_t_recovery_from_underrun_comma__prepare_failed_colon___pct_s_lf_, snd_strerror(err));
                return 0;
        } else if (err == -ESTRPIPE) {
                while ((err = snd_pcm_resume(handle)) == -EAGAIN)
                        fprintf(stderr,"sleep????\n");
                        sleep(1);       /* wait until the suspend flag is released */
                if (err < 0) {
                        err = snd_pcm_prepare(handle);
                        if (err < 0)
#define WORLD_Can_qt_t_recovery_from_suspend_comma__prepare_failed_colon___pct_s_lf_ "Can't recovery from suspend, prepare failed: %s\n"
                                printf(WORLD_Can_qt_t_recovery_from_suspend_comma__prepare_failed_colon___pct_s_lf_, snd_strerror(err));
                }
                return 0;
        }
        return err;
}



		
		    
			
			        



  

 
  
 


int playOneSoundBuffer() {
char *soundBuffer;
soundBuffer = (char *)getNextQueue240();
int soundBufferSize = 960;
rewriteit:
rc = snd_pcm_writei(handle, soundBuffer, (soundBufferSize/4));
if (rc<0) {
    if (rc==-11) {
      fprintf(stderr,"??");
      return 0;
      }
  fprintf(stderr,"!");
  if (xrun_recovery(handle, rc) < 0) {
#define WORLD_Write_error_colon___pct_d__pct_s_lf_ "Write error: %d %s\n"
                                        printf(WORLD_Write_error_colon___pct_d__pct_s_lf_,rc, snd_strerror(rc));
                                        exit(EXIT_FAILURE);
                                }
  goto rewriteit;
  }
else if (rc != (soundBufferSize/4)) {
    fprintf(stderr,"playonesoundbuffer failed when it shhouldn't %d\n",rc);
    }
else {   /* AOK */
    soundBufferSize=0;
    }
}



