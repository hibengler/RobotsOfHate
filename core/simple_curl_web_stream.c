#include "simple_curl_web_stream.h"
#include "simple_seven_eight_decode.h"

#include <curl/curl.h>

/* So, yeah, reading from a standard web page is easy,  but it gets harder and harder because of new protocols, SSL/TLS,
  and who knows what else.   So I am going to take the easy route and use libcurl to add streaming from the web.
  
The streaming file is not "encrypted" and has a 2 byte length prior to each packet.
Also, the stream removes the redundant information, making the file about 30k/s, for just audio, I think.

This should be pretty simple, right?



in the linux server section and the web section there are systems to make this work on web servers, and probably cloud servers.


upload_to_server - is a neat program that uploads files to the upload.pl file (upload.pl is in linux/web).   This makes 10M files of sirad data
from what comes from music_broadcast.
It is multi -treadded, and uploads two files at the same time, for redundancy. :)


on the web side, download.cgi loads the 10M sirad chunks, and seamlessly integrates them into one private stream for the user.

So this will be the third part.  This should allow reading from a streaming web site.
*/


#include "simple_curl_web_stream.h"
#include "simple_processor.h"
#include "simple_decrypt.h"


#ifndef logit
#define logit(...) {fprintf(stderr,__VA_ARGS__);fprintf(stderr,"\n");}
#endif

int curl_already_initted=0;
int we_are_streaming_web=0;
unsigned long bytes_streamed=0;
int curl_reset_url=0;

char *content_type = NULL;
/* ^ that is available by the first read */
/* goto guts to get to the guts of this */


#define protocol_log(stderr,...) 
#define only_protocol(retval) return(retval)
//#define logit(...) {fprintf(stderr,__VA_ARGS__);fprintf(stderr,"\n");}





/* here is the section https://curl.haxx.se/libcurl/c/fopen.html */

/*****************************************************************************
 *
 * This example source code introduces a c library buffered I/O interface to
 * URL reads it supports fopen(), fread(), fgets(), feof(), fclose(),
 * rewind(). Supported functions have identical prototypes to their normal c
 * lib namesakes and are preceaded by url_ .
 *
 * Using this code you can replace your program's fopen() with url_fopen()
 * and fread() with url_fread() and it become possible to read remote streams
 * instead of (only) local files. Local files (ie those that can be directly
 * fopened) will drop back to using the underlying clib implementations
 *
 * See the main() function at the bottom that shows an app that retrieves from
 * a specified url using fgets() and fread() and saves as two output files.
 *
 * Copyright (c) 2003, 2017 Simtec Electronics
 *
 * Re-implemented by Vincent Sanders <vince@kyllikki.org> with extensive
 * reference to original curl example code
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * This example requires libcurl 7.9.7 or later.
 */ 
/* <DESC>
 * implements an fopen() abstraction allowing reading from URLs
 * </DESC>
 */ 
#include <stdio.h>
#include <string.h>
#ifndef _WIN32
#include <sys/time.h>
#endif
#include <stdlib.h>
#include <errno.h>
 
#include <curl/curl.h>
 
enum fcurl_type_e {
  CFTYPE_NONE = 0,
  CFTYPE_FILE = 1,
  CFTYPE_CURL = 2
};
 
struct fcurl_data
{
  enum fcurl_type_e type;     /* type of handle */ 
  union {
    CURL *curl;
    FILE *file;
  } handle;                   /* handle */ 
 
  char *buffer;               /* buffer to store cached data*/ 
  size_t buffer_len;          /* currently allocated buffers length */ 
  size_t buffer_pos;          /* end of data in buffer*/ 
  int still_running;          /* Is background url fetch still in progress */ 
};
 
typedef struct fcurl_data URL_FILE;

  
    
/* exported functions */ 
URL_FILE *url_fopen(const char *url, const char *operation);
int url_fclose(URL_FILE *file);
int url_feof(URL_FILE *file);
size_t url_fread(void *ptr, size_t size, size_t nmemb, URL_FILE *file);
char *url_fgets(char *ptr, size_t size, URL_FILE *file);
void url_rewind(URL_FILE *file);
 
/* we use a global one for convenience */ 
static CURLM *multi_handle=NULL;

 
static sr_simple_seven_eight_decode *sr78dec=NULL;  
static unsigned char eight_bit_buffer[4096];
static unsigned char seven_bit_buffer[7168+8];
int eight_bit_buffer_pos=0;
int eight_bit_buffer_length=0;
int seven_bit_buffer_pos=0;
int seven_bit_buffer_length=0;
int phase7=0;  
int phase8 = 0;
   
int sliding=0;
static int lc=0;
static unsigned char bufferbig[4000];
int captured_so_far=0;
char *the_packet_web_buffer;
int internal_recvStringLen=0;

    
	
	    
void reset_curl_internal_state() {
  eight_bit_buffer_pos=0;
  eight_bit_buffer_length=0;
  seven_bit_buffer_pos=0;
  seven_bit_buffer_length=0;
  phase7=0;
  phase8=0;
  content_type=NULL;
  sliding=0;
  lc=0;
  captured_so_far=0;
  internal_recvStringLen=0;
}
		    
      
/* curl calls this routine to get more data */ 
static size_t write_callback(char *buffer,
                             size_t size,
                             size_t nitems,
                             void *userp)
{
  char *newbuff;
  size_t rembuff;
  if (we_are_streaming_web != 1) {return 0;}
 
  URL_FILE *url = (URL_FILE *)userp;
  size *= nitems;
 
  rembuff = url->buffer_len - url->buffer_pos; /* remaining space in buffer */ 
 
  if(size > rembuff) {
    /* not enough space in buffer */ 
    newbuff = realloc(url->buffer, url->buffer_len + (size - rembuff));
    if(newbuff == NULL) {
      protocol_log(stderr, "callback buffer grow failed\n");
      size = rembuff;
    }
    else {
      /* realloc succeeded increase buffer size*/ 
      url->buffer_len += size - rembuff;
      url->buffer = newbuff;
    }
  }
 
  memcpy(&url->buffer[url->buffer_pos], buffer, size);
  url->buffer_pos += size;
 
  return size;
}
 
/* use to attempt to fill the read buffer up to requested number of bytes */ 
static int fill_buffer(URL_FILE *file, size_t want)
{
  fd_set fdread;
  fd_set fdwrite;
  fd_set fdexcep;
  struct timeval timeout;
  int rc;
  CURLMcode mc; /* curl_multi_fdset() return code */ 
 
  /* only attempt to fill buffer if transactions still running and buffer
   * doesn't exceed required size already
   */ 
  if((!file->still_running) || (file->buffer_pos > want))
    return 0;
 
  /* attempt to fill buffer */ 
  do {
    int maxfd = -1;
    long curl_timeo = -1;
 
    FD_ZERO(&fdread);
    FD_ZERO(&fdwrite);
    FD_ZERO(&fdexcep);
 
    /* set a suitable timeout to fail on */ 
    timeout.tv_sec = 7; /* 1 minute */ 
    timeout.tv_usec = 0;
 
    curl_multi_timeout(multi_handle, &curl_timeo);
    if(curl_timeo >= 0) {
      timeout.tv_sec = curl_timeo / 1000;
      if(timeout.tv_sec > 1)
        timeout.tv_sec = 1;
      else
        timeout.tv_usec = (curl_timeo % 1000) * 1000;
    }
 
    /* get file descriptors from the transfers */ 
    mc = curl_multi_fdset(multi_handle, &fdread, &fdwrite, &fdexcep, &maxfd);
 
    if(mc != CURLM_OK) {
      protocol_log(stderr, "curl_multi_fdset() failed, code %d.\n", mc);
      break;
    }
 
    /* On success the value of maxfd is guaranteed to be >= -1. We call
       select(maxfd + 1, ...); specially in case of (maxfd == -1) there are
       no fds ready yet so we call select(0, ...) --or Sleep() on Windows--
       to sleep 100ms, which is the minimum suggested value in the
       curl_multi_fdset() doc. */ 
 
    if(maxfd == -1) {
#ifdef _WIN32
//      Sleep(100);
      rc = 0;
#else
      /* Portable sleep for platforms other than Windows. */ 
      struct timeval wait = { 0, 100 * 1000 }; /* 100ms */ 
      rc = select(0, NULL, NULL, NULL, &wait);
#endif
    }
    else {
      /* Note that on some platforms 'timeout' may be modified by select().
         If you need access to the original value save a copy beforehand. */ 
      rc = select(maxfd + 1, &fdread, &fdwrite, &fdexcep, &timeout);
    }
 
    switch(rc) {
    case -1:
      /* select error */ 
      break;
 
    case 0:
    default:
      /* timeout or readable/writable sockets */ 
      curl_multi_perform(multi_handle, &file->still_running);
      break;
    }
  } while(file->still_running && (file->buffer_pos < want));
  return 1;
}
 
 
/* use to remove want bytes from the front of a files buffer */ 
static int use_buffer(URL_FILE *file, size_t want)
{
  /* sort out buffer */ 
  if((file->buffer_pos - want) <= 0) {
    /* ditch buffer - write will recreate */ 
    free(file->buffer);
    file->buffer = NULL;
    file->buffer_pos = 0;
    file->buffer_len = 0;
  }
  else {
    /* move rest down make it available for later */ 
    memmove(file->buffer,
            &file->buffer[want],
            (file->buffer_pos - want));
 
    file->buffer_pos -= want;
  }
  return 0;
}

 
URL_FILE *url_fopen(const char *url, const char *operation)
{
  /* this code could check for URLs or types in the 'url' and
     basically use the real fopen() for standard files */ 
 
  URL_FILE *file;
  (void)operation;
 
  file = malloc(sizeof(URL_FILE));
  if(!file)
    return NULL;
 
  memset(file, 0, sizeof(URL_FILE));
 
  file->handle.file = fopen(url, operation);
  if(file->handle.file)
    file->type = CFTYPE_FILE; /* marked as URL */ 
 
  else {
    file->type = CFTYPE_CURL; /* marked as URL */ 
    file->handle.curl = curl_easy_init();
 
#ifdef _WIN32
    curl_easy_setopt(file->handle.curl, CURLOPT_SSL_VERIFYPEER, FALSE);
#endif
    curl_easy_setopt(file->handle.curl, CURLOPT_TCP_KEEPALIVE, 1L);
    curl_easy_setopt(file->handle.curl, CURLOPT_URL, url);
    curl_easy_setopt(file->handle.curl, CURLOPT_WRITEDATA, file);
    curl_easy_setopt(file->handle.curl, CURLOPT_VERBOSE, 1L); //a
    curl_easy_setopt(file->handle.curl, CURLOPT_STDERR, stderr);
    curl_easy_setopt(file->handle.curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(file->handle.curl, CURLOPT_NOPROGRESS, 1L);
    curl_easy_setopt(file->handle.curl, CURLOPT_FAILONERROR, 1L);
    curl_easy_setopt(file->handle.curl, CURLOPT_IGNORE_CONTENT_LENGTH , 1L);
//    curl_easy_setopt(file->handle.curl, CURLOPT_TIMEOUT , 10L);
    
    if(!multi_handle)
      multi_handle = curl_multi_init();
 
    curl_multi_add_handle(multi_handle, file->handle.curl);
 
    /* lets start the fetch */ 
    CURLcode multi = curl_multi_perform(multi_handle, &file->still_running);


    if((file->buffer_pos == 0) && (!file->still_running)) {
      /* if still_running is 0 now, we should return NULL */ 
 
      /* make sure the easy handle is not in the multi handle anymore */ 
      curl_multi_remove_handle(multi_handle, file->handle.curl);
 
      /* cleanup */ 
      curl_easy_cleanup(file->handle.curl);
 
      free(file);
 
      file = NULL;
    }
  }
return(file);
}

  
int url_fclose(URL_FILE *file)
{
if (!file) return 0;
  int ret = 0;/* default is good return */ 
 
  switch(file->type) {
  case CFTYPE_FILE:
    ret = fclose(file->handle.file); /* passthrough */ 
    break;
 
  case CFTYPE_CURL:
  
    if (multi_handle) {
      /* make sure the easy handle is not in the multi handle anymore */ 
      curl_multi_remove_handle(multi_handle, file->handle.curl);
      }
 
    /* cleanup */ 
    curl_easy_cleanup(file->handle.curl);
    
//    if (multi_handle) {
//      curl_multi_cleanup(multi_handle);
//      multi_handle = NULL;
//      }
    
    break;
 
  default: /* unknown or supported type - oh dear */ 
    ret = EOF;
    errno = EBADF;
    break;
  }
 
//  free(file->buffer);/* free any allocated buffer space */ 
// ^^^ handle multiple threads, etc.

//  free(file);
// ^^^ handle multiple threads, etc.
 
  return ret;
}
 
int url_feof(URL_FILE *file)
{
  int ret = 0;
  if (!file) return(0);
  switch(file->type) {
  case CFTYPE_FILE:
    ret = feof(file->handle.file);
    break;
 
  case CFTYPE_CURL:
    if((file->buffer_pos == 0) && (!file->still_running))
      ret = 1;
    break;
 
  default: /* unknown or supported type - oh dear */ 
    ret = -1;
    errno = EBADF;
    break;
  }
  return ret;
}
 
 
 

    
int dump7(unsigned char *label,unsigned char *x,int len,int phase) {
only_protocol(phase);
int originalphase = phase;
int newphase = phase;
protocol_log(stderr,"%s: \n",label);
int i;
for (i=0;i<phase;i++) {
  protocol_log(stderr,"__ ");
  }
for (i=0;i<len;i++) { 
  if (x[i]==255) {
    protocol_log(stderr,"FF|");
    }
  else {
    protocol_log(stderr,"%2.2x ",x[i]);
    }
  
  if (((phase+i)%56)==55) {
    protocol_log(stderr,"\n");
    }
  else if (((phase+i)&7)==7) {
    protocol_log(stderr," ");
    }
  }
  
if (((phase+i-1)%56)!=55) {
  protocol_log(stderr,"\n");
  }
protocol_log(stderr,"\n");
newphase = (phase+i)%8;
return newphase;
}

  
    
      
int dump8(unsigned char *label,unsigned char *x,int len,int phase,int wanted_len) {
only_protocol(phase);
int originalphase = phase;
int newphase = phase;
if (wanted_len > len) {
  protocol_log(stderr,"warning, incomplete packet\n   wanted %d got %d     hex: wanted %x got %x\n",wanted_len,len,wanted_len,len);
  }
else if (wanted_len < len) {
  protocol_log(stderr,"ERROR, to many  packet   wanted %d got %d     hex: wanted %x got %x\n",wanted_len,len,wanted_len,len);
  }
protocol_log(stderr,"%s: \n",label);
int i;
for (i=0;i<phase;i++) {
  protocol_log(stderr,"__ ");
  }
for (i=0;i<len;i++) { 
  protocol_log(stderr,"%2.2x ",x[i]);

  if (((phase+i)%56)==55) {
    protocol_log(stderr,"\n");
    }
  else if (((phase+i)&7)==6) {
    protocol_log(stderr," ");
    }
  }
  
if (((phase+i-1)%56)!=55) {
  protocol_log(stderr,"\n");
  }
protocol_log(stderr,"\n");
return (phase+i)%7;
}

  
    
      
int dump(unsigned char *label,unsigned char *x,int len) {
only_protocol(0);
protocol_log(stderr,"%s: \n",label);
int i;
for (i=0;i<len;i++) { 
  protocol_log(stderr,"%2.2x ",x[i]);

  if (((i)%56)==55) {
    protocol_log(stderr,"\n");
    }
  else if (((i)&7)==6) {
    protocol_log(stderr," ");
    }
  }
  
if (((i-1)%56)!=55) {
  protocol_log(stderr,"\n");
  }
protocol_log(stderr,"\n");
}
        
	  
  
size_t url_fread8(void *ptr, size_t size, size_t nmemb, URL_FILE *file)
{
  size_t want;
  if (!file) return 0;
  switch(file->type) {
  case CFTYPE_FILE:
    want = fread(ptr, size, nmemb, file->handle.file);
    break;

  case CFTYPE_CURL:
    want = nmemb * size;
    fill_buffer(file, want);
  
    /* check if there's data in the buffer - if not fill_buffer()
     * either errored or EOF */
    if(!file->buffer_pos)
      return 0;  

      CURLcode res;

      if (!content_type) {/* ask for the content-type */ 
        res = curl_easy_getinfo(file->handle.curl, CURLINFO_CONTENT_TYPE, &content_type);
 
        if((CURLE_OK == res) && content_type)
          content_type = strdup(content_type);
	  protocol_log(stderr,"We received Content-Type: %s\n", content_type);
	}
	
    /* ensure only available data is considered */
    if(file->buffer_pos < want)
      want = file->buffer_pos;
    
    /* xfer data to caller */
    memcpy(ptr, file->buffer, want);
      
    use_buffer(file, want);
    
    want = want / size;     /* number of items */
    
    if (sr78dec_do_we_know_if_we_are_7_bit(sr78dec)) {
      if (sr78dec_we_are_7_bit(sr78dec,seven_bit_buffer,0,content_type)) {
        phase7 = dump7("read7",ptr,want,phase7);
	}
      else {
        dump8("usual_read",ptr,want,0,want); 
	}
      }
    else {
      dump("First test read ",ptr,want);
      }
  
    break;
      
  default: /* unknown or supported type - oh dear */
    want = 0;
    errno = EBADF;
    break;
  
  }
  return want;
}


    
size_t url_fread7(void *ptr, size_t size, size_t nmemb, URL_FILE *file) {
if (!file) return 0;
protocol_log(stderr,"fread7 some pointer size %ld nmemb %ld file %lx\n",size,nmemb,(long) file);
int want = nmemb * size;
int want_done = 0;
int want_left = want;

    while (want_left) {
      int want2=want_left;
      
      if (eight_bit_buffer_length) {
        if (want2>eight_bit_buffer_length) {
	  want2 = eight_bit_buffer_length;
	  }
	memcpy(ptr+want_done,eight_bit_buffer+eight_bit_buffer_pos,want2);
	want_done += want2;
	want_left -= want2;
	eight_bit_buffer_pos += want2;
	eight_bit_buffer_length -=want2;
	} /* if we have some 8 bit to do */
      
      /* might still have more */
      if (want_left) {
        if (want_left < 7) { /* buffer the small one byte and two byte reads */
          eight_bit_buffer_length = url_fread8(seven_bit_buffer,1,8,file);
	  eight_bit_buffer_pos=0;
	  if (eight_bit_buffer_length==0) {
	    protocol_log(stderr,"CAnd get eight fucking bytes? WTF???\n");
            want_left=0; /* cant get it */
            continue; /* go back and use our 8 bit buffer */
            }
          eight_bit_buffer_length = sr78dec_seven_to_8(sr78dec,eight_bit_buffer,seven_bit_buffer,8);
	  if (eight_bit_buffer_length==0) {
	    protocol_log(stderr,"CAnd decode eight fucking bytes? WTF???\n");
            want_left=0; /* cant get it */
            continue; /* go back and use our 8 bit buffer */
            }
	  continue;
          } /* if it was small */        
        int want7 = ((want_left+6)/7)*8; /* a bit more */
        if (want7 >7168) want7=7168;
        int l = url_fread8(seven_bit_buffer,1,want7,file);
        if (l==0) {
	  protocol_log(stderr,"seven cant perform.\n");
          want_left=0;
          continue;
          }
	eight_bit_buffer_pos=0;
        eight_bit_buffer_length = sr78dec_seven_to_8(sr78dec,eight_bit_buffer,seven_bit_buffer,l);
	if (eight_bit_buffer_length==0) {
          want_left=0; /* cant get it */
          continue; /* go back and use our 8 bit buffer */
          }
	continue;
        } /* if there was some left over after we cleared the 8 bit buffer */
      } /* while we havent tried everything without a failure from below */
        
phase8 = dump8("fread7",ptr,want_done,phase8,want);
want_done = want_done / size;     /* number of items */ 
return want_done;
}
    


size_t url_fread(void *ptr, size_t size, size_t nmemb, URL_FILE *file)
{
//return url_fread8(ptr,size,nmemb,file)  ;
if (!file) return 0;
  size_t want;
  if (!(sr78dec_do_we_know_if_we_are_7_bit(sr78dec))) {  /* first time - figure out if 7 or 8 bit */
  	protocol_log(stderr,"url_fread_first_Time\n");
    eight_bit_buffer_pos=0;
    eight_bit_buffer_length=0;
    seven_bit_buffer_pos=0;
    seven_bit_buffer_length=0;
    /* ^^^ new, or something reset . so we will too */    
    
    int l=url_fread8(seven_bit_buffer,1,896,file);
    if (!l) return l;
//      dump("read ",ptr,want_done);
    
    if(sr78dec_we_are_7_bit(sr78dec,seven_bit_buffer,l,content_type)) {
      phase7 = dump7("firstconv7",seven_bit_buffer+seven_bit_buffer_pos,l,phase7);
      eight_bit_buffer_length = sr78dec_seven_to_8(sr78dec,eight_bit_buffer,seven_bit_buffer,l);
      /* ??? check to see that the seven bit internal buffer is clear - should be */
      phase8 = dump8("firstconv",eight_bit_buffer+eight_bit_buffer_pos,eight_bit_buffer_length,phase8,eight_bit_buffer_length);
      protocol_log(stderr,"\n\n\nfirst day 7 bit jitters oh no!\n\n\n");
      }
    else {
      memcpy(eight_bit_buffer,seven_bit_buffer,l);
      eight_bit_buffer_length = l;
      eight_bit_buffer_pos = 0;
      protocol_log(stderr,"special_first_day_jitters\n");
      goto special_first_day_jitters;
      }
    } /* if we are new */
  if (sr78dec_we_are_7_bit(sr78dec,seven_bit_buffer,0,content_type)) {
    return url_fread7(ptr,size,nmemb,file);
    } /* if we are 7 bit */
  else {
  
    special_first_day_jitters:
    
    if (eight_bit_buffer_length) {
      int want = nmemb * size;
      int want_done = 0;
      int want_left = want;
      protocol_log(stderr,"eithg biut buffer code\n");
      while (want_left) {
        if (eight_bit_buffer_length) {
          int want2=want_left;
          if (want2>eight_bit_buffer_length) {
              want2 = eight_bit_buffer_length;
	      }
  	  memcpy(ptr+want_done,eight_bit_buffer+eight_bit_buffer_pos,want2);
	  want_done += want2;
	  want_left -= want2;
	  eight_bit_buffer_pos += want2;
	  eight_bit_buffer_length -=want2;
	  } /* if we have some 8 bit to do */
	if (want_left) { /* implies eight_bit_buffer_length is 0 */
	  int want8 = want_left;
	  int l = url_fread8(ptr+want_done,1,want8,file);
	  want_done += l;
	  want_left -= l;
	  if (l==0) {
	    want_left=0; /* cant get it */
	    continue; /* go back and use our 8 bit buffer */
	    }
	  }
	} /* while wanting something in this time of transition */					  
      dump("read ",ptr,want_done);
      want_done = want_done / size;
      
      return want_done;
      } /* if we have the buffer - should go away pretty quickly */
    else {
      return url_fread8(ptr, size, nmemb, file);
      }
    } /* we are confirmed 8 bit */
    

} 
     
/* end of https://curl.haxx.se/libcurl/c/fopen.html */






/*---------------------------------------------------------------------------------------------------*/
/* guts */
URL_FILE *url_file;



/* note - this might be re-called by the receive loop based on various errors
because we sometimes get a recvfrrom failed error on android, when switching
networks */
void init_web_stream(char *url) {
if (!curl_already_initted) {
  sr78dec = new_sr78dec();
  curl_global_init(CURL_GLOBAL_ALL);
  curl_already_initted=1;
  }
#define HEY_NONBLOCK 00004000
//the_network_block=block;
if (url_file) {
  logit("hey url file still openQQQQQQQ!\n");
  }
logit("open %s\n",url); 
  url_file = url_fopen(url,"r");
logit("done\n");
  if (!url_file) {
    logit("error\n");
    protocol_log(stderr,"cant open the url %s\n",url);
    we_are_streaming_web=-1; 
    bytes_streamed=0;
    return;
    }
  reset_curl_internal_state();
  
  curl_reset_url = 1;
  

we_are_streaming_web=1; 
bytes_streamed=0;

return;
}



int stream_slide_until_sr_indicator(char *startbuf,int number_in_buf)
{
int i=0;
int state = 0;
char ch;
while (1) {
  if (i<number_in_buf) {
    ch=startbuf[i];
    }
  else {
    int f=url_fread((void *)startbuf,1,1,url_file);
    if (!f) return 0;
    bytes_streamed++;
    ch = startbuf[0];
    }
  if (state==0) {
    if (ch=='s') state = 1; else state=0;
    }
  else if (state==1) {
    if (ch=='r') state = 2; else state=0;
    }
  else if (state==2) {
    if (ch=='0') state = 3; else state=0;
    }
  else if (state==3) {
    if ((ch=='0')||(ch=='1')) return(1); else state=0;
    }
  i++;
  }
}





int get_packet_from_web_stream(int *precvStringLen,unsigned char *packetbuffer) {
protocol_log(stderr,"lc %d sliding %d captured_so_far %d\n",lc,sliding,captured_so_far);
/* here we know that we will get one packet - because the polls told us */

/* Receive a single datagram from the server */
{
  /* wait for processor.c */
  do {
    int queue_size = soundringtail-soundringhead;
    if (queue_size<0) queue_size += SOUNDRING_COUNT;
    protocol_log(stderr,"%d\n",queue_size);
    if (queue_size >275) {
      struct timespec thislong;
      thislong.tv_sec = 0;
      thislong.tv_nsec = 10000000; /* 10 milliseconds */
      nanosleep(&thislong, &thislong);
      return (0);
      }
    else {
      break;
      }
    } while (1);
  } 
  
if (!url_file) return(0);
  
  
if (captured_so_far==0) {

  if (lc!=2) {

reread:
    if (!url_file) return 0;
    int f=url_fread((void *)bufferbig+lc,1,2-lc,url_file);
    bytes_streamed += 1;
//  protocol_log(stderr,"f is %d\n",f);
    if (!f) {
      if (url_feof(url_file)) {
        url_fclose(url_file);
        url_file=NULL;
        we_are_streaming_web = -1;
	sr78dec_reset_seven_eight_low_level(sr78dec);
        }
      return(0);
      }
    if (lc <2) {
      lc +=f;
      if (lc<2) { // big speedup
        return(0); 
	}
      }   
    } /* if lcd is not set */


  if (!sliding) {
    captured_so_far=0;
    internal_recvStringLen = bufferbig[0] + (bufferbig[1]<<8);
      protocol_log(stderr,"recvStringLen %d\n",internal_recvStringLen);
    if ((internal_recvStringLen<1)||(internal_recvStringLen>2000)) sliding=1;
    }
  if (sliding) {
    int f=stream_slide_until_sr_indicator(bufferbig,2);
    if (!f) {
      if (url_feof(url_file)) {
        url_fclose(url_file);
        url_file=NULL;
        we_are_streaming_web = -1;
	sr78dec_reset_seven_eight_low_level(sr78dec);
        }
      return 0;  
      }
//  protocol_log(stderr,"error %d   total: %ld\n",errno,bytes_streamed); 
    lc=0;
    sliding=0;
    goto reread;
    }
    
    
    
    
    
  the_packet_web_buffer= (char *)packetbuffer;  
  } /* if not partially captured */



int f = url_fread((void *)the_packet_web_buffer+captured_so_far,1,internal_recvStringLen-captured_so_far,url_file);
protocol_log(stderr,"f is %d\n",f);
if (f==0) {
  if (url_feof(url_file)) {
    logit("error2 %d %ld total",errno,bytes_streamed);
    url_fclose(url_file);
    url_file=NULL;
    we_are_streaming_web = -1;
	sr78dec_reset_seven_eight_low_level(sr78dec);
    }
  return(0);
  }
captured_so_far +=f;
bytes_streamed += f;

if (captured_so_far>internal_recvStringLen) {
  fprintf( stderr, "oops - too much\n");
  dump8("encoded frame",the_packet_web_buffer,internal_recvStringLen,2,captured_so_far);  
  }
else if (captured_so_far<internal_recvStringLen) {
  return(0);
  }
/* here we have a match */
captured_so_far=0;

if (internal_recvStringLen>4) {
  decrypt1((unsigned char *)the_packet_web_buffer,internal_recvStringLen-4,internal_recvStringLen);
  *precvStringLen=internal_recvStringLen;
  dump8("decoded frame",the_packet_web_buffer,internal_recvStringLen,2,internal_recvStringLen);
  }
lc=0;
return(1);
}



void shutdown_web_stream() {
if (url_file) {
  url_fclose(url_file);
  sr78dec_reset_seven_eight_low_level(sr78dec);
  url_file=NULL;
//  curl_global_cleanup();
//  logit("closed_it\n");
  }
reset_curl_internal_state();
sr78dec_reset_seven_eight_low_level(sr78dec);
we_are_streaming_web=0;
}
