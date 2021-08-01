#include "simple_seven_eight_decode.h"
#include <stdio.h>
#include <string.h>
#include <malloc.h>
/* just decode */


void init_sr78dec(sr_simple_seven_eight_decode  * sr78dec) {
sr78dec->seven_to_eight_res_count=0;



sr78dec->seven_eight_locked_eight=0;
sr78dec->seven_eight_locked_seven=0;

sr78dec->seven_to_8_broken=0;

}


sr_simple_seven_eight_decode *new_sr78dec() {
sr_simple_seven_eight_decode * sr78dec = malloc(sizeof(sr_simple_seven_eight_decode));
init_sr78dec(sr78dec);
return sr78dec;
}




/* this is a guru function.  I uses a block of data to determine if it is a snake using 0-127 ascii, or a standard 8 bit binary.
For it to decide, one must bring offerings to the table, then it decides.*/

int sr78dec_we_are_7_bit(sr_simple_seven_eight_decode * sr78dec,unsigned char *data,int len7,char *optional_content_type) {
/* this works once and thats in - unless a new file is opened. */
if (sr78dec->seven_eight_locked_seven) return(1);
if (sr78dec->seven_eight_locked_eight) return(0);


if (optional_content_type) { /* this is the best way */
  const char *silent_radiance_application_name = "application/silentradiance";
  const char *silent_radiance_sneaky_seven_sname = "application/silentradiancesneakysevensnake";
  int offset;
  for (offset=0;offset<1;offset++) { /* just like &anchor = 1 from snobol days */
    if (strncmp(optional_content_type+offset,silent_radiance_sneaky_seven_sname,strlen(silent_radiance_sneaky_seven_sname)) ==0) {
//      fprintf(stderr,"hell yeah. Its a snake. can tell by the content type\n");
      sr78dec->seven_eight_locked_seven = 1;
      return 1;
      }
    else if (strncmp(optional_content_type+offset,silent_radiance_application_name,strlen(silent_radiance_application_name)) ==0) {
//      fprintf(stderr,"Its the 8 bit. No javascript is going to stream that stuff, no way.\n");
      sr78dec->seven_eight_locked_eight=1;
      return 0;
      }
    }
  }
  
  
  
  
  
/* ok do some analysis on the read file. */
    
int i;
if (len7>1000) len7=1000;

int high_count=0;
for (i=0;i<len7;i++) {
  if ((data[i] > 127) &&(data[i] != 255)) {
    high_count++;
    }
  }


if (high_count>60) {
//    fprintf(stderr,"high count is %d\n",high_count);
    sr78dec->seven_eight_locked_eight=1;
    return 0;
    }
//fprintf(stderr,"we are SEVEN!!!!!\n");
sr78dec->seven_eight_locked_seven = 1;
return 1;
}  




/* yes, the guru might require data, and some things can t read till they know the codec.
So we ask if it has been locked in some sections to be polite */
int sr78dec_do_we_know_if_we_are_7_bit(sr_simple_seven_eight_decode * sr78dec) {
if ((sr78dec->seven_eight_locked_eight)||(sr78dec->seven_eight_locked_seven)) return(1);
return 0;
}




/* this code is very tricky. ha! */
int sr78dec_seven_to_8(sr_simple_seven_eight_decode * sr78dec,unsigned char *destination,unsigned char *osource,int len7) {
int i;
//fprintf(stderr,"7_to_8 len7 is %d   last_res_count is %d\n",len7,sr78dec->seven_to_eight_res_count);
for (i=0;i<sr78dec->seven_to_eight_res_count;i++) {
  destination[i]=sr78dec->seven_to_eight_last_buf[i];
  }
unsigned char *source=osource;  /* sourcee changes as it starts in destination, we work in that area for the first one.  Then move to source proper */
len7 += sr78dec->seven_to_eight_res_count;
if (sr78dec->seven_to_eight_res_count) {
  int max=8;
  if (max>len7) max=len7;
  for (;i<max;i++) {
    destination[i]=source[i-sr78dec->seven_to_eight_res_count];
    }
  source = destination;
  }
else {
  source = osource;
  }
  
sr78dec->seven_to_8_broken=0;  
i=0;
int len8=0;
while ((i<len7-7)) {
  int c0 = *source++;
  int c1 = *source++;
  int c2 = *source++;
  int c3 = *source++;
  int c4 = *source++;
  int c5 = *source++;
  int c6 = *source++;
  int c7 = *source++;
  
  if (c0&128) {source -=7;i+=1;sr78dec->seven_to_8_broken++;continue;}
  if (c1&128) {source -=6;i+=2;sr78dec->seven_to_8_broken++;continue;}
  
  int d0 = (c0 & 127)       + ((c1 & 1)<<7);
  destination[len8++]=d0;
  
  if (c2&128) {source -=5;i+=3;sr78dec->seven_to_8_broken++;continue;}

  int d1 = ((c1&126)>>1) + ((c2 & 3)<<6);
  destination[len8++]=d1;

  if (c3&128) {source -=4;i+=4;sr78dec->seven_to_8_broken++;continue;}

  int d2 = ((c2&124)>>2) + ((c3 & 7)<<5);
  destination[len8++]=d2;
  
  if (c4&128) {source -=3;i+=5;sr78dec->seven_to_8_broken++;continue;}
  
  int d3 = ((c3&120)>>3) + ((c4 & 15)<<4);
  destination[len8++]=d3;
  
  if (c5&128) {source -=2;i+=6;sr78dec->seven_to_8_broken++;continue;}
  
  int d4 = ((c4&112)>>4) + ((c5 & 31)<<3);
  destination[len8++]=d4;

  if (c6&128) {source -=1;i+=7;sr78dec->seven_to_8_broken++;continue;}
  
  int d5 = ((c5&96)>>5) +  ((c6 & 63)<<2);
  destination[len8++]=d5;

  if (c7&128) {source -=0;i+=8;sr78dec->seven_to_8_broken++;continue;}
  
  int d6 = ((c6&64)>>6) + ((c7 & 127)<<1);
  destination[len8++]=d6;
  
  i+=8;
  source = osource- sr78dec->seven_to_eight_res_count+i;
  }
sr78dec->seven_to_eight_res_count = len7-i;
for (i=0;i<sr78dec->seven_to_eight_res_count;i++) {
  sr78dec->seven_to_eight_last_buf[i] = source[i];
  }
return(len8);
}




int seven_to_8_break(sr_simple_seven_eight_decode  * sr78dec,char *outbuffer) {
if (sr78dec->seven_to_eight_res_count== 0) { return(0);}

int len=0;
int undo=0;
int first=1;
while (sr78dec->seven_to_eight_res_count) {
//  fprintf(stderr,"seven_to_eight_res_count %d one more added\n",seven_to_eight_res_count);
  unsigned char source_all_zeroes[16]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
  int j=sr78dec_seven_to_8(sr78dec,outbuffer,source_all_zeroes,1);
  if ((j!=0) && (sr78dec->seven_to_eight_res_count!=0) && (first!=0)) {
      undo--; 
      first=0;
      }
      
  undo = undo + 1;
  len+=j;
//  fprintf(stderr,"got %d value %x now seven_to_eight_res_count %d\n",j,outbuffer[0],seven_to_eight_res_count);
  outbuffer += j;
  }
if (sr78dec->seven_to_eight_res_count==0) {
   len = len - undo;
   if (len<0) len=0;
   }
return (len);
}



void sr78dec_reset_seven_eight_low_level(sr_simple_seven_eight_decode  * sr78dec) {
sr78dec->seven_eight_locked_eight=0;
sr78dec->seven_eight_locked_seven=0;
sr78dec->seven_to_eight_res_count=0;
}
