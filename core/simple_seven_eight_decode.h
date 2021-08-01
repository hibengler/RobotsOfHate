#ifndef SIMPLE_SEVEN_EIGHT_DECODE_H
#define SIMPLE_SEVEN_EIGHT_DECODE_H

#ifdef __cplusplus
extern "C" {
#endif

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



#ifdef __cplusplus
}
#endif
#endif
