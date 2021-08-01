#ifndef SIMPLE_CURL_WEB_STREAM_H
#define SIMPLE_CURL_WEB_STREAM_H

#include <stdio.h>



#include <stdlib.h> /* for atoi() and exit() */
#include <string.h> /* for memset() */
#include <unistd.h> /* for close() */
#include <errno.h>



extern int we_are_streaming_web;

void init_web_stream(char *url);
int get_packet_from_web_stream();
void shutdown_web_stream();




#endif
