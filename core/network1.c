#include "network1.h"


int network1_poll_check(network1_complete *c) {
c->metwork1_check_poll_runs_in_call = 0;

sigset_t origmask;
sigset_t set;
int s;
struct timespec ts;
ts.tv_sec=0;
ts.tv_nsec=0;
/* from man pthread_sigmask abd poll */
sigemptyset(&set);
sigaddset(&set, SIGQUIT);
sigaddset(&set, SIGUSR1);

sr_gettimeofday(&(c->poll_check_start_time);

/* clear out revents, and turn off write polls if not necessary. read polls are necessary */
for (int i=0;i<c->current_number_of_polls;i++) {
  if (c->poll_state[i]==0) {
    network1_setup_and_bind(c,i);
    }
  if (c->poll_state[i]==1) {
    if (i<NUMBER_OF_NETWORK1_PARTICIPANTS) {
      network1_listen_to_poll(c,i);
      }
    else if (i<NUMBER_OF_NETWORK1_PARTICIPANTS_TIMES_2) {
      network1_connect_to_poll(c,i);
      }
    }
  if ((c->poll_state[i]==3) {
    if (i<NUMBER_OF_NETWORK1_PARTICIPANTS) {
      network1_set_buffer_if_necessary_and_receivefrom_poll(c,i);
      }
    else if (i<NUMBER_OF_NETWORK1_PARTICIPANTS_TIMES_2) {
      network1_send_if_can(c,i);
      }
  if ((c->poll_state[i]==2)||(c->poll_state[i]==4) {
    c->pollfds[i].fd=c->sockets[i];
    }
  else {
    c->pollfds[i].fd=-1; // dont poll 
    }   
     
  
  /* now clean revents */
  c->pollfds[i].revents=0;
  
  }
  



sigset_t origmask;
int timeout;
struce timevspec two_p;
two_p.tv_sec=0;
two_p.tv_nsec=0;
sigget_t sigmask = 0;



sr_gettimeofday(&c->local_poll_check_poll_start_time);
int number_of_events = ppoll(&(c->pollfds[0]), c->current_number_of_polls,&two_p,&sigmask);		  
sr_gettimeofday(&(c->local_poll_end_time);

if (!number_of_events) {
  c->local_round1_end_time=c->local_poll_end_time;
  c->local_round2_end_time=c->local_poll_end_time;
  c->local_round3_end_time=c->local_poll_end_time;
  c->local_network1_check_end_time = c->local_poll_end_time;  
}
  


{ // Block to process events round 1
  int i=0;
  int no_events = number_of_events;
  
  if ((no_events) &&(c->network1_action_start_round1)) {
    (*c->network1_action_start_round1)(c,i,number_of_events);
    }
    
  while ((no_events)&&(i<c->current_number_of_polls)) {
    if (poll_fds[i].revents) {
      if  (c->network1_handle_action_round1[i]) { (*c->network1_handle_action_round1[i])(c,i,no_events);  }
      i++;
      no_events--;      // we found an event - drop it
      } /* if this had revents */
    else { /* no event found. Check the next one */
      i++;
      }
    } /* while loopint through all events */
  
  if ((number_of_events) &&(c->network1_action_finish_round1)) {
    (*c->network1_action_finish_round1)(c,number_of_events);
    }



  } // block to process events





{ // Block to process events round 2
  int i=0;
  int no_events = number_of_events;
  
  if ((no_events) &&(c->network1_action_start_round2)) {
    (*c->network1_action_start_round2)(c,i,number_of_events);
    }
    
  while ((no_events)&&(i<c->current_number_of_polls)) {
    if (poll_fds[i].revents) {
      if  (c->network1_handle_action_round2[i]) { (*c->network1_handle_action_round2[i])(c,i,no_events);  }
      i++;
      no_events--;      // we found an event - drop it
      } /* if this had revents */
    else { /* no event found. Check the next one */
      i++;
      }
    } /* while loopint through all events */
  
  if ((number_of_events) &&(c->network1_action_finish_round2)) {
    (*c->network1_action_finish_round2)(c,number_of_events);
    }

  /* -- will have to do for round 2, and round 3 */

  } // block to process events round 2





{ // Block to process events round 3
  int i=0;
  int no_events = number_of_events;
  
  if ((no_events) &&(c->network1_action_start_round3)) {
    (*c->network1_action_start_round1)(c,i,number_of_events);
    }
    
  while ((no_events)&&(i<c->current_number_of_polls)) {
    if (poll_fds[i].revents) {
      if  (c->network1_handle_action_round1[i]) { (*c->network1_handle_action_round3[i])(c,i,no_events);  }
      i++;
      no_events--;      // we found an event - drop it
      } /* if this had revents */
    else { /* no event found. Check the next one */
      i++;
      }
    } /* while loopint through all events */
  
  if ((number_of_events) &&(c->network1_action_finish_round3)) {
    (*c->network1_action_finish_round3)(c,number_of_events);
    }

  } // block to process events
return number_of_events;
}





    if (!(c->buffer[i]) {
      c->pollfds[i].fd=-1;
      }
    else {
      (c->pollfds[i].fd=c->sockets[i];);      
    }

int init (network1_complete *c,int participant_number,char *broadcast_ip,char *ips[],
     void (*network1_action_start_round1)(struct network1_complete *,int),
     void (*network1_handle_action_round1_in)(struct network1_complete *,int,int),(*network1_handle_action_round1_out)(struct network1_complete *,int,int),
     void (*network1_action_end_round1)(struct network1_complete *,int),
     void (*network1_action_start_round2)(struct network1_complete *,int),
     void (*network1_handle_action_round2_in)(struct network1_complete *,int,int),(*network1_handle_action_round2_out)(struct network1_complete *,int,int),
     void (*network1_action_end_round2)(struct network1_complete *,int),
     void (*network1_action_start_round3)(struct network1_complete *,int),
     void (*network1_handle_action_round3_in)(struct network1_complete *,int,int),(*network1_handle_action_round3_out)(struct network1_complete *,int,int),
     void (*network1_action_end_round3)(struct network1_complete *,int),
     void (*network1_get_new_receive_buffer)(struct network1_complete *,int), /* get a new receive buffer for this poll - could be broadcast in */
     void (*network1_get_new_send_buffer)(struct network1_complete *,int), /* get a new send buffer - 0 length  - just to fill up  - why? */
     int (*network1_pull_next_send_buffer_from_queue)(struct network1_complete *,int) /* get a semd buffer off the queue for this polling address - returns queue length */
     ) {

memset(c,0,sizeof(network1_complete));

c->current_number_of_polls=UMBER_OF_NETWORK1_PARTICIPANTS*NUMBER_OF_NETWORK1_PARTICIPANTS_TIMES_2; // +2 for broadcast things

c->network1_action_start_round1 = network1_action_start_round1;
c->network1_action_end_round1 = network1_action_end_round1;
c->network1_action_start_round2 = network1_action_start_round2;
c->network1_action_end_round2 = network1_action_end_round2;
c->network1_action_start_round3 = network1_action_start_round3;
c->network1_action_end_round3 = network1_action_end_round3;
c->network1_get_new_receive_buffer = network1_get_new_receive_buffer;
c->network1_get_new_send_buffer = network1_get_new_send_buffer;
c->network1_pull_next_send_buffer_from_queue = network1_pull_next_send_buffer_from_queue;


c->participant_number = participant_number;

for (i=0;i<NUMBER_OF_NETWORK1_PARTICIPANTS;i++) {
  int communicator = i;
  c->poll_state[i] = 0;
  c->communicator[i] = i;
  c->direction[i] = 1;  // will have to do that later
  c->ports[i] = NETWORK1_START_PORT + c->participant_number * NUMBER_OF_NETWORK1_PARTICIPANTS_TIMES_2  + communicator;
  c->sockets[i]=-1;
  strncpy[&(c->ip_addresses[i][0]),ips[i],19);
  c->ip_addresses[i][19]='\0';

  
  struct sockaddr_in inAddr;
  memset ((char *)(& inAddr,0, sizeof(struct sockaddr_in));
  inAddr.sin_family = AF_INET;                 /* Internet address family */
  inAddr.sin_addr.s_addr = inet_addr(ips[participant_number]); /* all inqddr go to me. address is the output address*/
//inAddr.sin_addr.s_addr = htonl(INADDR_ANY);
  inAddr.sin_port = htons(c->ports[i]);         /* listen to our ip address on the given port */
  c->poll_addresses[i] = inAddr;

  c->sent_to_ports[communicator] =  NETWORK1_START_PORT + communicator * NUMBER_OF_NETWORK1_PARTICIPANTS_TIMES_2  +  NETWORK1_START_PORT c->participant_number; // this is the port we send to from the sender to us receiver

  struct sockaddr sendToAddr;
  memset ((char *)(& inAddr,0, sizeof(struct sockaddr_in));
  inAddr.sin_family = AF_INET;                 /* Internet address family */
  inAddr.sin_addr.s_addr = inet_addr(ips[connection]); /* all inqddr go to me. address is the output address*/
//inAddr.sin_addr.s_addr = htonl(INADDR_ANY);
  inAddr.sin_port = htons(c->sent_to_port[connection]);         /* listen to our ip address on the given port */
  c->sending_to[i] = sendToAddr;  /* we filter to see if it is from the same thing */

  c->receiving_poll[communicator] = i;

  c->pullfds = (struct pullfd) fd:-1,events:=POLLIN|POLLPRI|POLLERR,revents:0};

  c->buflen[i]= 0;
  c->buffer[i]= NULL;
    
  c->broadcast_permission[i] = 0;
  c->network1_handle_action_round1[i] = network1_handle_action_round1_in;
  c->network1_handle_action_round2[i] = network1_handle_action_round2_in;
  c->network1_handle_action_round3[i] = network1_handle_action_round3_in;
  }




for (int o=NUMBER_OF_NETWORK1_PARTICIPANTS;<o<NUMBER_OF_NETWORK1_PARTICIPANTS_TIMES_2;o++) {
  int communicator = o-NUMBER_OF_NETWORK1_PARTICIPANTS;
  c->poll_state[o] = 0;
  c->communicator[o] = communicator;
  c->direction[o] = 1;  // will have to do that later
  c->ports[o] = NETWORK1_START_PORT + NUMBER_OF_NETWORK1_PARTICIPANTS + c->participant_number * NUMBER_OF_NETWORK1_PARTICIPANTS_TIMES_2  + communicator;
  c->sockets[o]=-1;
//  strncpy[&(c->ip_addresses[o][0]),ips[communicator],19);
//  c->ip_addresses[o][19]='\0';

  c->sending_poll[communicator] = o;
  
  memset ((char *)(& inAddr,0, sizeof(struct sockaddr_in));
  inAddr.sin_family = AF_INET;                 /* Internet address family */
  inAddr.sin_addr.s_addr = inet_addr(ips[participant_number]); /* all inqddr go to me. address is the output address*/
//inAddr.sin_addr.s_addr = htonl(INADDR_ANY);
  inAddr.sin_port = htons(c->ports[o]);         /* listen to our ip address on the given port */
  c->poll_addresses[o] = inAddr;

  c->sent_to_ports[o] =   communicator * NUMBER_OF_NETWORK1_PARTICIPANTS_TIMES_2  +  c->participant_number;  /* this is what we semd tp from us to a receiver*/ 

  struct sockaddr sendToAddr;
  memset ((char *)(& inAddr,0, sizeof(struct sockaddr_in));
  inAddr.sin_family = AF_INET;                 /* Internet address family */
  inAddr.sin_addr.s_addr = inet_addr(ips[connection]); /* all inqddr go to me. address is the output address*/
//inAddr.sin_addr.s_addr = htonl(INADDR_ANY);
  inAddr.sin_port = htons(c->sent_to_port[o]);         /* listen to our ip address on the given port */
  c->sending_to[o] = sendToAddr;                      
  c->pullfds = (struct pullfd) fd:-1,events:=POLLOUT|POLLERR,revents:0};

  c->buflen[o]= 0;
  c->buffer[o]= NULL;
    
  c->broadcast_permission[o] = 0;
  c->network1_handle_action_round1[o] = network1_handle_action_round1_in;
  c->network1_handle_action_round2[o] = network1_handle_action_round2_in;
  c->network1_handle_action_round3[o] = network1_handle_action_round3_in;
  }

// broadcast in
{
  int i = NUMBER_OF_NETWORK1_PARTICIPANTS_TIMES_2;

  c->poll_state[i] = 0;
  c->communicator[i] = i;
  c->direction[i] = 1;  // will have to do that later
  c->ports[i] = NETWORK1_START_PORT + c->participant_number * NUMBER_OF_NETWORK1_PARTICIPANTS_TIMES_2  + communicator;
  c->sockets[i]=-1;
  strncpy[&(c->ip_addresses[i][0]),ips[i],19);
  c->ip_addresses[i][19]='\0';

  
  struct sockaddr_in inAddr;
  memset ((char *)(& inAddr,0, sizeof(struct sockaddr_in));
  inAddr.sin_family = AF_INET;                 /* Internet address family */
  inAddr.sin_addr.s_addr = inet_addr(ips[participant_number]); /* all inqddr go to me. address is the output address*/
//inAddr.sin_addr.s_addr = htonl(INADDR_ANY);
  inAddr.sin_port = htons(c->ports[i]);         /* listen to our ip address on the given port */
  c->poll_addresses[i] = inAddr;

  c->sent_to_ports[communicator] =  NETWORK1_START_PORT + communicator * NUMBER_OF_NETWORK1_PARTICIPANTS_TIMES_2  +  NETWORK1_START_PORT c->participant_number; // this is the port we send to from the sender to us receiver

  struct sockaddr sendToAddr;
  memset ((char *)(& inAddr,0, sizeof(struct sockaddr_in));
  inAddr.sin_family = AF_INET;                 /* Internet address family */
  inAddr.sin_addr.s_addr = inet_addr(ips[connection]); /* all inqddr go to me. address is the output address*/
//inAddr.sin_addr.s_addr = htonl(INADDR_ANY);
  inAddr.sin_port = htons(c->sent_to_port[connection]);         /* listen to our ip address on the given port */
  c->sending_to[i] = sendToAddr;  /* we filter to see if it is from the same thing */

  c->receiving_poll[communicator] = i;

  c->pullfds = (struct pullfd) fd:-1,events:=POLLIN|POLLPRI|POLLERR,revents:0};

  c->buflen[i]= 0;
  c->buffer[i]= NULL;
    
  c->broadcast_permission[i] = 0;
  c->network1_handle_action_round1[i] = network1_handle_action_round1_in;
  c->network1_handle_action_round2[i] = network1_handle_action_round2_in;
  c->network1_handle_action_round3[i] = network1_handle_action_round3_in;
  }

[i][i][i]
// 
  strncpy[c->broadcast_ip,broadcast_ip,19);
  c->broadcast_ip[19]='\0';

/* from me to you */
int binport,boutport,broadcast_in,broadcast_out;
{ 
  struct sockaddr_in broadcastAddr;
  broadcast_in =  NUMBER_OF_NETWORK1_PARTICIPANTS*NUMBER_OF_NETWORK1_PARTICIPANTS_TIMES_2;
  broadcast_out++;
  binport = NETWORK1_START_PORT + NUMBER_OF_NETWORK1_PARTICIPANTS*NUMBER_OF_NETWORK1_PARTICIPANTS_TIMES_2; // That is the inport and the outport
  boutport = NETWORK1_START_PORT + NUMBER_OF_NETWORK1_PARTICIPANTS*NUMBER_OF_NETWORK1_PARTICIPANTS_TIMES_2; // That is also inport and the outport
  int s;
  if ((s=socket(AF_INET, SOCK_DGRAM|SOCK_NONBLOCK, IPPROTO_UDP)) == -1) {
    fprintf(stderr,"no socket available\n");
    return(0);
    }
  c->sockets[broadcast_in] = s;
  c->broadcast_in = broadcast_in;
  c->broadcast_permission[broadcast_in] = 0;


  memset ((char *)(&broadcastAddr,0, sizeof(struct sockaddr_in));
  broadcastAddr.sin_family = AF_INET;                 /* Internet address family */
//  broadcastAddr.sin_addr.s_addr = inet_addr(BroadcastIP);/* Broadcast IP address  bit it doesnt work, in nebula, so we will use our own */
//  broadcastAddr.sin_addr.s_addr = ();/* Broadcast IP address  bit it doesnt work, in nebula, so we will use our own */
  broadcastAddr.sin_addr.s_addr = inet_addr(ips[participant_number]);/* Broadcast IP address  bit it doesnt work, in nebula, so we will use our own */
  broadcastAddr.sin_port = htons(binport);         /* Broadcast output port */
  c->poll_addresses[broadcast_in] = broadcastAddr;
  
  
  
  if( bind(s , (struct sockaddr*)&(address_recv[broadcast_in]), sizeof(struct sockaddr_in) ) == -1)
    {
    fprintf(stderr,"error bind socket %d id %d port %d\n",s,broadcast_in,binport);
    }
    
  c->network1_handle_action_round1[broadcast_in] = network1_handle_action_round1_in;
  c->network1_handle_action_round2[broadcast_in] = network1_handle_action_round2_in;
  c->network1_handle_action_round3[broadcast_in] = network1_handle_action_round3_in;
  
  c->pollfd[broadcast_in] = (struct pollfd){fd = c->sockets[broadcast_in],events=POLLIN|POLLPRI|POLLERR,revents=0);
  
  
  
  
  
  
  
  /* now for output socket - a different one */

  if ((s=socket(AF_INET, SOCK_DGRAM|SOCK_NOBLOCK, IPPROTO_UDP)) == -1) {
    fprintf(stderr,"no socket available\n");
    return(0);
    }
  c->sockets[broadcast_out]= s;
  c->broadcast_out = broadcast_out;
  c->broadcast_permission[broadcast_out] = 1;
  


/* for ipv6 is IP_MULTICAST_TTL  and other stuff */
  setsockopt(s, SOL_SOCKET, SO_BROADCAST, (void *) &( c->broadcast_permission[broadcast_out]),
          sizeof(int)) < 0) { 
     fprintf(stderr,"cannot set permission);
     };
     
  memset ((char *)(& broadcastAddr,0, sizeof(struct sockaddr_in));
  broadcastAddr.sin_family = AF_INET;                 /* Internet address family */
//  broadcastAddr.sin_addr.s_addr = inet_addr(broadcastIP);/* Broadcast IP address */
  broadcastAddr.sin_addr.s_addr = htonl(INADDR_ANY);
  broadcastAddr.sin_port = htons(broadcastPort);         /* Broadcast port */



  c->address_send[broadcast_out] = broadcastAddr;
  if( bind(s , (struct sockaddr*)&(address_send[broadcast_out]), sizeof(struct sockaddr_in) ) == -1)
    {
    fprintf(stderr,"error bind socket %d id %d port %d\n",s,broadcast_out,binport);
    }
    
  c->network1_handle_action_round1[broadcast_out] = network1_handle_action_round1_out;
  c->network1_handle_action_round2[broadcast_out] = network1_handle_action_round2_out;
  c->network1_handle_action_round3[broadcast_out] = network1_handle_action_round3_out;
  
  c->pollfd[broadcast_out] = (struct pollfd){fd = c->sockets[broadcast_out],events=POLLOUT|POLLERR,revents=POLLOUT); // pollout because we are ready to send to output 
  
 }   // set up the broadcast sender and receiver



struct sockaddr_in inAddr;
memset ((char *)(& inAddr,0, sizeof(struct sockaddr_in));
inAddr.sin_family = AF_INET;                 /* Internet address family */
inAddr.sin_addr.s_addr = inet_addr(ips[participant_number]); /* all inqddr go to me. address is the output address*/
//inAddr.sin_addr.s_addr = htonl(INADDR_ANY);
inAddr.sin_port = htons(0);         /* set for each i */

for (int i=0;i<NUMBER_OF_NETWORK1_PARTICIPANTS;i++) {
  int o = NUMBER_OF_NETWORK1_PARTICIPANTS  + i;
    
  
  int inport;
  inport  = NETWORK1_START_PORT + c->participant_number*NUMBER_OF_NETWORK1_PARTICIPANTS_TIMES_2 + i; 
  inAddr.sin_port = htons(0);  
  address[i] = 
  
  memset ((char *)(&outAddr,0, sizeof(struct sockaddr));

  
  int inport;



  int s;
  if ((s=socket(AF_INET, SOCK_DGRAM|SOCK_NOBLOCK, IPPROTO_UDP)) == -1) {
    fprintf(stderr,"no socket available\n");
    return(0);
    }
  c->sockets[i] = s;
  c->broadcast_in[i] = 0;
  
  

  outport = NETWORK1_START_PORT + i*NUMBER_OF_NETWORK1_PARTICIPANTS_TIMES_2 + c->participant_number;
  
  c->sockets[i] = s;
  if ((s=socket(AF_INET, SOCK_DGRAM|SOCK_NOBLOCK, IPPROTO_UDP)) == -1) {
    fprintf(stderr,"no socket available\n");
    return(0);
    }



for (int i=0;i<NUMBER_OF_NETWORK1_PARTICIPANTS;i++) {
  int o = NUMBER_OF_NETWORK1_PARTICIPANTS + i;

  c->communicator[i] = i;
  c->communicator[o] = i;
  
  
  c->buflen[i] = 0;
  c->buffers[i]=NULL;
  
  c->sent_from_ports =  NETWORK1_START_PORT + 
  

// note for windows 
//Will have to set the broadfcast to true for the receiving ports where win64_network_receiver.c:    iSockRet = setsockopt(sock, SOL_SOCKET, SO_BROADCAST, (char *) &bSockoptTrue, sizeof bSockoptTrue);
//win64_network_receiver.c:iSockRet = setsockopt(sock, SOL_SOCKET, SO_BROADCAST, (char *) &bSockoptTrue, sizeof bSockoptTrue);
//win64_network_receiver.c: iSockRet = setsockopt(sock, IPPROTO_IP, IP_RECEIVE_BROADCAST, (char *) &dwSockoptTrue, sizeof dwSockoptTrue);
//win64_network_receiver.c:iSockRet = setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (char *) &bSockoptTrue, sizeof bSockoptTrue);
//win64_network_receiver.c:iSockRet = setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (char *) &iBsockTimeout, sizeof iBsockTimeout);

// note - for receivers in windows 
/* Set the socket to nonblocking */
//    if (ioctlsocket(sock, FIONBIO, &nonblocking) != 0) {
//        logit( "socket() failed");
//        return;
//    }
  
 


    c->si_me.sin_family = AF_INET;
    si_me.sin_port = htons(inport);
    si_me.sin_addr.s_addr = htonl(INADDR_ANY);
 
    int to_port = NUMBER_OF_NETWORK1_PARTICIPANTS*participant_number + i;
    participant_number  



// used https://www.binarytides.com/programming-udp-sockets-c-linux/
       ssize_t sendto(c->sockets[o], c->buffer[o], 2048, MSG),
                             const struct sockaddr *dest_addr, socklen_t addrlen);
			     
