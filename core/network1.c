




// here we gp<

#include "glue.h"
#include "network1.h"
#include <error.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <pthread.h>
              
//
//
/*
player	receivfrom	recvport	sendport	
0	0		7000		7036
0	1		7001		7042
0	2		7002		7048
0	3		7003		7054
0	4		7004		7060
0	5		7005		7066
1	0		7006		7037
1	1		7007		7043
1	2		7008		7049
1	3		7009		7055
1	4		7010		7061
1	5		7011		7067
2	0		7012		7038
2	1		7013		7044
2	2		7014		7050
2	3		7015		7056
2	4		7016		7062
2	5		7017		7068
3	0		7018		7039
3	1		7019		7045
3	2		7020		7051
3	3		7021		7057
3	4		7022		7063
3	5		7023		7069
4	0		7024		7040
4	1		7025		7046
4	2		7026		7052
4	3		7027		7058
4	4		7028		7064
4	5		7029		7070
5	0		7030		7041
5	1		7031		7047
5	2		7032		7053
5	3		7033		7059
5	4		7034		7065
5	5		7035		7071
		
			^		   ^
			7000+player*6+rf  |
					   7036+rf*6+player
					   

This is stored in 6 and above
id	player	sendto		sendport	recvport
6	0	0		7036		7000
7	0	1		7042		7001
8	0	2		5048		7002
9	0	3		5054		7003
10	0	4		5060		7004
11	0	5		5066		7005

^				^		^
|				|		|
|				|		|
|				|		|
|				|		|
|				|
6+sendto 			7036+6*(player)+sendto
						|
						7000+6*sendto+player
*/


static void nsleep() {
struct timespec req;
req.tv_sec=0;
req.tv_nsec=5000000;
            
nanosleep(&req,NULL);
}     
	      
	      
	      


int socket_it(network1_complete *c,int bind_id) {
  volatile network1_complete *cv = c;


  int s = cv->sockets[bind_id];
  if (s==-1) {
    fprintf(stderr,"new socket id %d\n",bind_id);
         int e = errno;
    if ((s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) {
      e = errno;
      fprintf(stderr,"no socket available error %d\n",e);
      return(0);
      }
    else {
      cv->sockets[bind_id] = s;
      e = errno;
      if (e) {
        fprintf(stderr,"something is wrong new socket %d id %d  error %d\n",s,bind_id,e);
        cv->sockets[bind_id]=-1;
        return(0);
        }
      }
    }
  cv->poll_state[bind_id]=1;
  return (1);  
}

/* bind where we receive from */
static int bind_it(network1_complete *c,int bind_id) {

volatile network1_complete *cv = (volatile network1_complete *)c;
if (bind_id>NUMBER_OF_NETWORK1_PARTICIPANTS) {

  fprintf(stderr,"why on sending bind id? %d \n",bind_id);
  return(0);
  }
  // no need to bind sender
  int communicator = cv->communicator[bind_id];
  
  struct sockaddr_in  * sa = &(c->poll_addresses[bind_id]);
        memset(sa, 0, sizeof(struct sockaddr_in));	
        sa->sin_family = AF_INET; // IPv4
        sa->sin_addr.s_addr = inet_addr(c->ip_addresses_string[communicator]);
	sa->sin_port = htons(c->ports[communicator]);
  int s=cv->sockets[bind_id];
if( bind(s , (struct sockaddr*)&(c->poll_addresses[bind_id]), sizeof(struct sockaddr_in) ) == -1)
    {
    fprintf(stderr,"error bind1 socket %d id %d port %d\n",s,bind_id,c->ports[bind_id]);
    int e=errno;
    if (e!=11) {
      fprintf(stderr,"error bind socket %d id %d err %d\n",s,bind_id,e);
      return(0);
      }
    }
   else 
   {
    fprintf(stderr,"error2 bind socket %d id %d port %d\n",s,bind_id,c->ports[bind_id]);
    int e=errno;
    if (e) {
      if (e!=11) {
        fprintf(stderr,"error b2ind socket %d id %d err %d\n",s,bind_id,e);
        return(0);
	}
      }
    }
 fprintf(stderr,"we are good binding socket %d %d\n",c->sockets[bind_id],bind_id);
   
cv->poll_state[bind_id]=1;
return 1;
}






static void network1_delay_for_milliseconds(network1_complete *c, int bind_id,unsigned int millisecondsa) {
struct timeval a,b;
b=a;
unsigned long milliseconds = millisecondsa;
unsigned long usec = a.tv_usec;
unsigned long asec = milliseconds / 1000l;
unsigned long usecm1 = milliseconds%1000l; 
unsigned long usecm2 = milliseconds *1000l;
unsigned long tusec = usec + usecm2;
if (tusec > 1000000l) {
  asec = asec + 1;
  tusec = tusec - 1000000l;
  }
a.tv_sec += asec;
a.tv_usec = tusec;
fprintf(stderr,"	delay add %d milliseconds to %ld %ld -> %ld %ld\n",millisecondsa,b.tv_sec,b.tv_usec,a.tv_sec,a.tv_usec);
struct timespec ts;
ts.tv_sec = a.tv_sec;
ts.tv_nsec = a.tv_usec*1000l;
nanosleep(&ts,NULL);
}











			

            
	    


  


																      
																																      
																																																      



     
      

  

  
  








#ifdef reader
  for (int i=0;i<c->current_number_of_polls;i++) {
    if ((c->type[i]==2)) {
      if (c->pollfds[i].revents&(POLLIN|POLLPRI)) {
          int result=read(c->sockets[i],&(c->buffers[i][0]),2047);
//	  fprintf(stderr,"read2 %lx got %d\n",(long)&(c->buffers[i]),result);
	  if (result>0) {
	    c->buflen[i]=result;
	    c->poll_state[i]=5;
	    }
	  else if (result<0) {
	    int e=nerrno(c,i);
	    if ((e==EAGAIN)||(e==EWOULDBLOCK)) {
	      c->poll_state[i]=4;
              c->pollfds[i].fd=c->sockets[i];
	      }
	    }
          }
      
      continue;
      }
    else if ((c->type[i]==3)) {
      continue;
      }





{ // Block to process events round 1
  
  if (number_of_events &&(c->network1_action_start_round1)) {
    (*c->network1_action_start_round1)(c,number_of_events,number_to_round);
    }
    
  
  if ((number_of_events) &&(c->network1_action_finish_round1)) {
    (*c->network1_action_finish_round1)(c,number_of_events,number_to_round);
    }


  } // block to process events round 1


// recompute number_to_round after round 1



  


{ // Block to process events round 2
  
  if (number_of_events &&(c->network1_action_start_round2)) {
    (*c->network1_action_start_round2)(c,number_of_events,number_to_round);
    }
    
    } /* while loopint through all events */
  
  if ((number_of_events) &&(c->network1_action_finish_round2)) {
    (*c->network1_action_finish_round2)(c,number_of_events,number_to_round);
    }


  } // block to process events round 2
  
  


  
  


{ // Block to process events round 3
  
  if (number_of_events &&(c->network1_action_start_round3)) {
    (*c->network1_action_start_round3)(c,number_of_events,number_to_round);
    }
    

  if ((number_of_events) &&(c->network1_action_finish_round3)) {
    (*c->network1_action_finish_round3)(c,number_of_events,number_to_round);
    }


  } // block to process events round 3



return number_to_round;
}

#endif








void *network_send_thread(void *arg) {
int bind_id = ((network1_pthread_parameters *) arg)->bind_id;
volatile struct network1_complete *cv =  ((network1_pthread_parameters *) arg)->c;
network1_complete *c= (network1_complete *)cv;

while (1) {
  if (cv->poll_state[bind_id]==0) {
    if (!socket_it(c,bind_id)) {
      continue;
      }
    cv->poll_state[bind_id]=3;
    }  
// if (cv->poll_state[bind_id]==1) {
//    if (!bind_it(c,bind_id)) {
//      continue;
//      }
  int flag;
  if ((cv->poll_state[bind_id]==3)||(cv->poll_state[bind_id]==5)) {
    // assume poll state is 3, or 5.  If 5, well, we reset the buffer
    //assume i is not player number
    // assume thers is no timer

    if (cv->buffers[bind_id]) {
      if (cv->buflen[bind_id]) {
        flag=1;
        }   
      else { 
        if (cv->network1_pull_next_send_buffer_from_queue[bind_id]) {
          (*cv->network1_pull_next_send_buffer_from_queue[bind_id])(c,bind_id,0);
          }
       }
      }       
    else {
      if (cv->network1_pull_next_send_buffer_from_queue[bind_id]) {
        (*cv->network1_pull_next_send_buffer_from_queue[bind_id])(c,bind_id,0);
        }     
      }

    if ((cv->buffers[bind_id]!=NULL)&&(c->buflen[bind_id])) {
        flag=1;
        }
      else {
        if (cv->buffers[bind_id]==NULL) {
        if (cv->network1_get_new_send_buffer[bind_id]) {
          (*cv->network1_get_new_send_buffer[bind_id])(c,bind_id,0);
          }
        }
      if (cv->buffers[bind_id]) {
        if (cv->buflen[bind_id]) {
          flag=1;
          } 
        }
      }     
    if (flag) {
      int e=0;
      struct sockaddr_in  clientaddr;

	memset(&clientaddr, 0, sizeof(struct sockaddr_in));
        // Filling server information 
  int communicator = cv->communicator[bind_id];

        clientaddr.sin_family = AF_INET;
        clientaddr.sin_addr.s_addr = inet_addr(c->ip_addresses_string[bind_id]);
        clientaddr.sin_port = htons(c->sent_to_ports[bind_id]);

      cv->call_rounds[bind_id]=0;  // waiting for read
      int result = sendto(c->sockets[bind_id],c->buffers[bind_id],c->buflen[bind_id],MSG_CONFIRM,(const struct sockaddr *) &clientaddr,
                        sizeof(clientaddr));
      if(result<=0) {
        continue;
        }
      cv->poll_state[bind_id]=5;
      cv->call_rounds[bind_id]=1;

      fprintf(stderr," sd %d got out %d of %d from %d to %d\n",bind_id,result,c->buflen[bind_id],c->ports[bind_id],c->sent_to_ports[bind_id]);
      continue;
      }
    else {
      }
    }
  else {
    fprintf(stderr,"%d weird state %d\n",bind_id,c->poll_state[bind_id]);
    }
  nsleep();
  
  }
return 0;
}

   
      

  
void *network_recv_thread(void *arg) {
int bind_id = ((network1_pthread_parameters *) arg)->bind_id;
volatile struct network1_complete *cv =  ((network1_pthread_parameters *) arg)->c;
network1_complete *c= (network1_complete *)cv;



while (1) {
  if (cv->poll_state[bind_id]==0) {
    if (!socket_it(c,bind_id)) {
      continue;
      }
    cv->poll_state[bind_id]=2;
    }  
  if (cv->poll_state[bind_id]==2) {
    if (!bind_it(c,bind_id)) {
      continue;
      }
    cv->poll_state[bind_id]=3;
    }  
  if ((cv->poll_state[bind_id]==3)||(cv->poll_state[bind_id]==5)) {
    int flag=0;
    // assume state is 3 or 5
    // assume i is not player number
    // assume there is no timer
    //if ((c->poll_state[bind_id])==5) {
    //  fprintf(stderr,"5 wipeout  thtrwrwrw \n");
    //  c->poll_state[bind_id] = 3;
    //  c->buflen[bind_id]=0;
    //  c->buffers[bind_id]=NULL;
    //  }
    if (cv->buffers[bind_id]) {
      if (cv->buflen[bind_id]) {
        if (cv->network1_get_new_receive_buffer) {
          (*cv->network1_get_new_receive_buffer)(c,bind_id,0);
          }
        }  
      }
    else {
      if (cv->network1_get_new_receive_buffer) {
        (*cv->network1_get_new_receive_buffer)(c,bind_id,0);
        }
      }


    if (cv->buffers[bind_id]) {
      if (!(cv->buflen[bind_id])) {
        flag=1;
        }
      }
    if (flag) {
      cv->poll_state[bind_id]=4;  // waiting for read
      cv->call_rounds[bind_id]=0;  // waiting for read
      int result = recv(c->sockets[bind_id],c->buffers[bind_id],NETWORK1_MAX_BUFFER_SIZE,MSG_WAITALL);
      fprintf(stderr," rd %d got out %d of %d from %d to %d\n",bind_id,result,c->buflen[bind_id],c->sent_to_ports[bind_id],c->ports[bind_id]);
      if (result==0) {  // end of file
        }
      else if (result >0) {
        cv->buflen[bind_id]=result;
	cv->call_rounds[bind_id]=1;
	cv->poll_state[bind_id]=5;   
        }
      else {     
        continue;
        }
      continue;
      }
    }
  else {
    fprintf(stderr,"%d weird state %d\n",bind_id,c->poll_state[bind_id]);
    }
  nsleep();
  }
return 0;
}







  
void *file_read_thread(void *arg) {
int bind_id = ((network1_pthread_parameters *) arg)->bind_id;
volatile struct network1_complete *cv =  ((network1_pthread_parameters *) arg)->c;
network1_complete *c= (network1_complete *)cv;

cv->call_rounds[bind_id]=0;
  

while (1) {

     cv->poll_state[bind_id]=3;
  if ((cv->poll_state[bind_id]==3)||(cv->poll_state[bind_id]==5)) {
    int flag=0;
    // assume state is 3 or 5
    // assume i is not player number
    // assume there is no timer
    //if ((c->poll_state[bind_id])==5) {
    //  fprintf(stderr,"5 wipeout  thtrwrwrw \n");
    //  c->poll_state[bind_id] = 3;
    //  c->buflen[bind_id]=0;
    //  c->buffers[bind_id]=NULL;
    //  }
    if (cv->buffers[bind_id]) {
      if (cv->buflen[bind_id]) {
        if (cv->network1_get_new_receive_buffer) {
          (*cv->network1_get_new_receive_buffer)(c,bind_id,0);
          }
        }  
      }
    else {
      if (cv->network1_get_new_receive_buffer) {
        (*cv->network1_get_new_receive_buffer)(c,bind_id,0);
        }
      }


    if (cv->buffers[bind_id]) {
      if (!(cv->buflen[bind_id])) {
        flag=1;
        }
      }
    if (flag) {
      cv->poll_state[bind_id]=4;  // waiting for read
      cv->call_rounds[bind_id]=0;
      int result = read(c->sockets[bind_id],c->buffers[bind_id],NETWORK1_MAX_BUFFER_SIZE);
      fprintf(stderr," rd %d got out %d of %d from %d to %d\n",bind_id,result,c->buflen[bind_id],c->sent_to_ports[bind_id],c->ports[bind_id]);
      if (result==0) {  // end of file
        }
       else if (result >0) { 
        cv->call_rounds[bind_id]=1;
        cv->poll_state[bind_id]=5;  // waiting for read
        }
      else {     
        continue;
        }
      continue;
      }
    }
  else {
    fprintf(stderr,"%d weird state %d\n",bind_id,c->poll_state[bind_id]);
    }
  nsleep();
  }
return 0;
}



/* call after network1_init to add a file or non udp socket port */
int network1_add_standard_input_fd(network1_complete *c,int fd,  
     network1_complete_round_call network1_handle_action_round1_in,  
     network1_complete_round_call network1_handle_action_round2_in,
     network1_complete_round_call network1_handle_action_round3_in,   
     network1_complete_round_call network1_get_new_receive_buffer) {
int i=  c->current_number_of_polls;
c->buflen[i]=0;
c->change_buffer_flag[i] = 0;
c->type[i] = 2;                         // read file


c->poll_state[i] = 3;
c->communicator[i]=-1;
c->ports[i]=0;
memset(&(c->sending_to[i]),0,sizeof(struct sockaddr_in));
c->sent_to_ports[i]=0;
memset(&(c->poll_addresses[i]),0,sizeof(struct sockaddr_in));
c->call_rounds[i]=0;
  
c->sockets[i]=fd;
c->pollfds[i]=(struct pollfd){fd:-1,events:POLLIN|POLLPRI|POLLERR,revents:0};
c->network1_handle_action_round1[i] = network1_handle_action_round1_in;
c->network1_handle_action_round2[i] = network1_handle_action_round2_in;
c->network1_handle_action_round3[i] = network1_handle_action_round3_in;
c->network1_get_new_receive_buffer[i] = network1_get_new_receive_buffer;
c->network1_get_new_send_buffer[i] = NULL;
c->network1_pull_next_send_buffer_from_queue[i] = NULL;

c->current_number_of_polls++;

{
int rf = c->current_number_of_polls-1;
    int ptstatus;
    fprintf(stderr,"starting char thread %d\n",rf);
    c->pthread_parameters[rf].bind_id=rf;
    c->pthread_parameters[rf].c=  (volatile network1_complete *)c;
    fprintf(stderr,"starting readchar thread %d %lx\n",i,(long unsigned int)  (&(c->pthread_parameters[rf]) ));

    ptstatus = pthread_create(&c->network_thread[rf],NULL,file_read_thread,(void *)(&(c->pthread_parameters[rf])));
    if (ptstatus) {
      c->network_thread[rf]=-1;
      fprintf(stderr,"cant make thread number %d\n",rf);
      }
      
  }
return i;
}





int network1_init (network1_complete *c,int participant_number,char *broadcast_ip,char *ips[],
     network1_complete_round_call network1_action_start_round1,
     network1_complete_round_call network1_handle_action_round1_in, network1_complete_round_call network1_handle_action_round1_out,
     network1_complete_round_call network1_action_finish_round1,
     network1_complete_round_call network1_action_start_round2,
     network1_complete_round_call network1_handle_action_round2_in, network1_complete_round_call network1_handle_action_round2_out,
     network1_complete_round_call network1_action_finish_round2,
     network1_complete_round_call network1_action_start_round3,
     network1_complete_round_call network1_handle_action_round3_in, network1_complete_round_call network1_handle_action_round3_out,
     network1_complete_round_call network1_action_finish_round3,
     network1_complete_round_call network1_get_new_receive_buffer, /* get a new receive buffer for this poll - could be broadcast in */                                       
     network1_complete_round_call network1_get_new_send_buffer, /* get a new send buffer - 0 length  - just to fill up  - why? */                                 
     network1_complete_round_call network1_pull_next_send_buffer_from_queue /* get a semd buffer off the queue for this polling address - returns queue length */          
     )  
{


memset(c,0,sizeof(network1_complete));

c->current_number_of_polls=NUMBER_OF_NETWORK1_PARTICIPANTS_TIMES_2;

c->network1_action_start_round1 = network1_action_start_round1;
c->network1_action_finish_round1 = network1_action_finish_round1;
c->network1_action_start_round2 = network1_action_start_round2;
c->network1_action_finish_round2 = network1_action_finish_round2;
c->network1_action_start_round3 = network1_action_start_round3;
c->network1_action_finish_round3 = network1_action_finish_round3;


c->participant_number = participant_number;
struct timeval nowtime;
gettimeofday(&nowtime,NULL);

/* set up input ports */
for (int i=0;i<NUMBER_OF_NETWORK1_PARTICIPANTS;i++) {
  int communicator = i;
  c->call_rounds[i]=0;
  c->change_buffer_flag[i] = 0;
  c->type[i] = 0; // read from sender
  c->poll_state[i] = 0;
  c->communicator[i] = i;
  c->ports[i] = NETWORK1_START_PORT + c->participant_number * NUMBER_OF_NETWORK1_PARTICIPANTS  + communicator;
  c->sockets[i]=-1;
  strncpy(&(c->ip_addresses_string[i][0]),ips[i],19);
  c->ip_addresses_string[i][19]='\0';
  
  
  {
    struct sockaddr_in inAddr;
    memset ((char *)(& inAddr),0, sizeof(struct sockaddr_in));
    inAddr.sin_family = AF_INET;                 /* Internet address family */
//    fprintf(stderr,"listen %s\n",ips[participant_number]);
    inAddr.sin_addr.s_addr = inet_addr(ips[participant_number]); /* all inqddr go to me. address is the output address*/
//    fprintf(stderr,"got %d port %d\n",inAddr.sin_addr.s_addr,c->ports[i]);
//  inAddr.sin_addr.s_addr = htonl(INADDR_ANY);  -- more indiscriminate
    inAddr.sin_port = htons(c->ports[i]);         /* listen to our ip address on the given port */
    c->poll_addresses[i] = inAddr;
    }

  /* also set the ports we are going to send to */


  int send_id=communicator+NUMBER_OF_NETWORK1_PARTICIPANTS;
    
  c->sent_to_ports[communicator] = NETWORK1_START_PORT + 36 + communicator * NUMBER_OF_NETWORK1_PARTICIPANTS +  c->participant_number; // this is the port we send to from the sender to us receiver
  // this is the port it should come from
  
  
  {
    struct sockaddr_in sendToAddr;
    memset ((char *)(& sendToAddr),0, sizeof(struct sockaddr_in));
    sendToAddr.sin_family = AF_INET;                 /* Internet address family */
    sendToAddr.sin_addr.s_addr = inet_addr(ips[communicator]); /* we set to the device */
    sendToAddr.sin_addr.s_addr = htonl(INADDR_ANY); // linux does not listen on ip, but it will listen on a device - aaah 
    sendToAddr.sin_port = htons(c->sent_to_ports[communicator]);         /* listen to our ip address on the given port */
    c->sending_to[i] = sendToAddr;  /* we filter to see if it is from the same thing */
    }


  c->pollfds[i] = (struct pollfd){fd:-1,events:POLLIN|POLLPRI|POLLERR,revents:0};

  c->buflen[i]= 0;
  c->buffers[i]= NULL;
    
  c->network1_handle_action_round1[i] = network1_handle_action_round1_in;
  c->network1_handle_action_round2[i] = network1_handle_action_round2_in;
  c->network1_handle_action_round3[i] = network1_handle_action_round3_in;
  c->network1_get_new_receive_buffer[i] = network1_get_new_receive_buffer;
  }


for (int o=NUMBER_OF_NETWORK1_PARTICIPANTS;o<NUMBER_OF_NETWORK1_PARTICIPANTS_TIMES_2;o++) {
  int communicator = o-NUMBER_OF_NETWORK1_PARTICIPANTS;
  c->call_rounds[o]=0;
  c->change_buffer_flag[o] = 0;
  c->type[o] = 1; // read from sender
  c->poll_state[o] = 0;
  c->communicator[o] = communicator;  // where we send from
  c->ports[o] = NETWORK1_START_PORT + 36 + c->participant_number * NUMBER_OF_NETWORK1_PARTICIPANTS  + communicator;
  c->sockets[o]=-1;
//  strncpy[&(c->ip_addresses[o][0]),ips[communicator],19);
//  c->ip_addresses[o][19]='\0';

  {
    struct sockaddr_in inAddr;
    memset ((char *)(& inAddr),0, sizeof(struct sockaddr_in));
    inAddr.sin_family = AF_INET;                 /* Internet address family */
    inAddr.sin_addr.s_addr = inet_addr(ips[participant_number]); /* all inqddr go to me. address is the output address*/
    inAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    inAddr.sin_port = htons(c->ports[o]);         /* listen to our ip address on the given port */
    c->poll_addresses[o] = inAddr;
    }
  

  c->sent_to_ports[o] =   NETWORK1_START_PORT + communicator * NUMBER_OF_NETWORK1_PARTICIPANTS  +  c->participant_number;  /* this is what we semd tp from us to a receiver*/ 
  {//
    struct sockaddr_in sendToAddr;
    memset ((char *)(& sendToAddr),0, sizeof(struct sockaddr_in));
    sendToAddr.sin_family = AF_INET;                 /* Internet address family */
    sendToAddr.sin_addr.s_addr = inet_addr(ips[communicator]); /* all inqddr go to me. address is the output address*/
//    fprintf(stderr,"to for %d is %s : %d\n",o,ips[communicator],sendToAddr.sin_addr.s_addr);
//    sendToAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    sendToAddr.sin_port = htons(c->sent_to_ports[o]);         /* listen to our ip address on the given port */
    c->sending_to[o] = sendToAddr;                      
    }   
  
  c->pollfds[o] = (struct pollfd){fd:-1,events:POLLOUT|POLLERR,revents:0};

  c->buflen[o]= 0;
  c->buffers[o]= NULL;
    
  
  c->network1_handle_action_round1[o] = network1_handle_action_round1_out;
  c->network1_handle_action_round2[o] = network1_handle_action_round2_out;
  c->network1_handle_action_round3[o] = network1_handle_action_round3_out;
  c->network1_get_new_send_buffer[o] = network1_get_new_send_buffer;
  c->network1_pull_next_send_buffer_from_queue[o] = network1_pull_next_send_buffer_from_queue;
  
  }
  
 /*
// note for windows 
//Will have to set the broadfcast to true for the receiving ports where win64_network_receiver.c:    iSockRet = setsockopt(sock, SOL_SOCKET, SO_BROADCAST, (char *) &bSockoptTrue, sizeof bSockoptTrue);
//win64_network_receiver.c:iSockRet = setsockopt(sock, SOL_SOCKET, SO_BROADCAST, (char *) &bSockoptTrue, sizeof bSockoptTrue);
//win64_network_receiver.c: iSockRet = setsockopt(sock, IPPROTO_IP, IP_RECEIVE_BROADCAST, (char *) &dwSockoptTrue, sizeof dwSockoptTrue);
//win64_network_receiver.c:iSockRet = setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (char *) &bSockoptTrue, sizeof bSockoptTrue);
//win64_network_receiver.c:iSockRet = setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (char *) &iBsockTimeout, sizeof iBsockTimeout);

// note - for receivers in windows 
Set the socket to nonblocking 
//    if (ioctlsocket(sock, FIONBIO, &nonblocking) != 0) 
//        logit( "socket() failed");
//        return;
//    }

*/
  



{
  for (int i=0;i<NUMBER_OF_NETWORK1_PARTICIPANTS;i++) {
    int o = i+NUMBER_OF_NETWORK1_PARTICIPANTS;
    int ptstatus;
    c->pthread_parameters[i].bind_id=i;
    c->pthread_parameters[i].c=  (volatile network1_complete *)c;
    fprintf(stderr,"starting recv thread %d %lx\n",i,(long unsigned int)  (&(c->pthread_parameters[i]) ));

    ptstatus = pthread_create(&c->network_thread[i],NULL,network_recv_thread,(void *)(&(c->pthread_parameters[i])));
    if (ptstatus) {
      c->network_thread[i]=-1;
      fprintf(stderr,"cant make thread number %d\n",i);
      }
      
      
    c->pthread_parameters[o].bind_id=o;
    c->pthread_parameters[o].c=  (volatile network1_complete *)c;
    fprintf(stderr,"starting send thread %d %lx\n",o,(long unsigned int)  (&(c->pthread_parameters[o]) ));
    ptstatus = pthread_create(&c->network_thread[o],NULL,network_send_thread,(void *) (&(c->pthread_parameters[o]) ));
    if (ptstatus) {
      fprintf(stderr,"cant make receive thread number %d\n",i);
      c->network_thread[o]=-1;
      }
    break;
    }    
  }  

return 1;
}










int network1_poll_check(network1_complete *c) {
int i;
volatile network1_complete *cv;
int number_to_round=0;

for (i=0;i<cv->current_number_of_polls;i++) {
  if (cv->call_rounds[i]) number_to_round++;
  }  
      
if (!number_to_round) return 0;

{ // Block to process events round 1
  
  if (number_to_round &&(cv->network1_action_start_round1)) {
    (*cv->network1_action_start_round1)(c,number_to_round,number_to_round);
    }
    
  for (int i=0;i<cv->current_number_of_polls;i++) {
    if (cv->call_rounds[i]) {    
      if  (cv->network1_handle_action_round1[i]) { (*cv->network1_handle_action_round1[i])(c,i,number_to_round);  }
      }
    } /* while loopint through all events */
  
  if ((number_to_round) &&(cv->network1_action_finish_round1)) {
    (*cv->network1_action_finish_round1)(c,number_to_round,number_to_round);
    }


  } // block to process events round 1


// recompute number_to_round after round 1
number_to_round=0;
for (int i=0;i<cv->current_number_of_polls;i++) {
  if (cv->call_rounds[i]) number_to_round++;
  }

//process_poll_buffer_statuses(c,0);  


if (!number_to_round) { 
  goto full_check_poll_loop;
  }
  


{ // Block to process events round 2
  
  if (number_to_round &&(cv->network1_action_start_round2)) {
    (*cv->network1_action_start_round2)(c,number_to_round,number_to_round);
    }
    
  for (int i=0;i<cv->current_number_of_polls;i++) {
    if (cv->call_rounds[i]) {    
      if  (cv->network1_handle_action_round2[i]) { (*cv->network1_handle_action_round2[i])(c,i,number_to_round);  }
      }
    } /* while loopint through all events */
  
  if ((number_to_round) &&(cv->network1_action_finish_round2)) {
    (*cv->network1_action_finish_round2)(c,number_to_round,number_to_round);
    }


  } // block to process events round 2
  
  

// recompute number_to_round after round 2
number_to_round=0;
for (int i=0;i<cv->current_number_of_polls;i++) {
  if (cv->call_rounds[i]) number_to_round++;
  }
//process_poll_buffer_statuses(c,0);  


if (!number_to_round) {
  goto full_check_poll_loop;
  }
  
  


{ // Block to process events round 3
  
  if (number_to_round &&(cv->network1_action_start_round3)) {
    (*cv->network1_action_start_round3)(c,number_to_round,number_to_round);
    }
    
  for (int i=0;i<cv->current_number_of_polls;i++) {
    if (cv->call_rounds[i]) {    
      if  (cv->network1_handle_action_round3[i]) { (*cv->network1_handle_action_round3[i])(c,i,number_to_round);  }
      }
    } /* while loopint through all events */
  
  if ((number_to_round) &&(cv->network1_action_finish_round3)) {
    (*cv->network1_action_finish_round3)(c,number_to_round,number_to_round);
    }


  } // block to process events round 3


// recompute number_to_round after round 3 - to use later
number_to_round=0;
for (int i=0;i<cv->current_number_of_polls;i++) {
  if (cv->call_rounds[i]) number_to_round++;
  }
//process_poll_buffer_statuses(c,0);  


if (!number_to_round) {
  goto full_check_poll_loop;
  }
  
full_check_poll_loop:

//network1_reset_sendables_and_stuff(c);
//compute_sendable_recieveables(c,1);
//process_poll_buffer_statuses(c,0);
   
   
return 0;
}


/* end of network1.c */

																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																														      																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																														
