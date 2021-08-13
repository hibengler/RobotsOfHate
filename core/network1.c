#include "network1.h"
#include <error.h>






static int network1_setup_and_bind(network1_complete *c, int bind_id) {
int s = c->sockets[bind_id];
if (s==-1) {
  if ((s=socket(AF_INET, SOCK_DGRAM|SOCK_NONBLOCK, IPPROTO_UDP)) == -1) {
    fprintf(stderr,"no socket available\n");
    return(0);
    }
  c->sockets[bind_id] = s;
  }
c->broadcast_permission[bind_id] = 0;

if (bind_id > NUMBER_OF_NETWORK1_PARTICIPANTS) {
  /* for ipv6 is IP_MULTICAST_TTL  and other stuff */
  if (setsockopt(s, SOL_SOCKET, SO_BROADCAST, (void *) &( c->broadcast_permission[bind_id]),
          sizeof(int)) < 0) { 
     fprintf(stderr,"cannot set permission: on poll %d port %d\n",bind_id,c->ports[bind_id]);
     return 0;
     }
  }
  
if( bind(s , (struct sockaddr*)&(c->poll_addresses[bind_id]), sizeof(struct sockaddr_in) ) == -1)
    {
//    fprintf(stderr,"error bind socket %d id %d port %d\n",s,bind_id,c->ports[bind_id]);
    fprintf(stderr,"error bind socket %d id %d\n",s,bind_id);
    return(0);
    }
c->poll_state[bind_id]=1;
}



static void network1_delay_for_milliseconds(network1_complete *c, int bind_id,unsigned int milliseconds) {
// here is a hack. c->poll_check_pollstart_timeis always called first before we do a delay, so we dont need to do gettimeofday 
struct timeval a;
a=c->local_network1_check_start_time;
a.tv_sec = a.tv_sec + (milliseconds * 1000 + a.tv_usec)/ 1000000;
a.tv_usec =           (milliseconds * 1000 + a.tv_usec)%1000000;
c->local_delay_work[bind_id] = a;
}




static int network1_unconnect(network1_complete *c,int bind_id) {
       struct sockaddr_in aaa;
       aaa.sin_family = AF_UNSPEC;
       aaa.sin_addr.s_addr= 0;
       aaa.sin_port=0; 
int e=0;       
   if (connect(c->sockets[bind_id],(struct sockaddr *)&aaa,sizeof(struct sockaddr_in)) ==-1) {
          e=errno;
          fprintf(stderr,"EINTR clear connectunlink to connect got errno %d\n",e);
	  }
return e;
}


static int generic_toone(network1_complete *c, int bind_id,int milleseconds) {
network1_unconnect(c,bind_id);
c->poll_state[bind_id]=1;
network1_delay_for_milliseconds(c,bind_id,milliseconds);
}


static int generic_tozero_screw_sockets(network1_complete *c, int bind_id,int milleseconds) {
c->sockets[bind_id] = -1;
c->pollfds[bind_id].fd = -1;
c->poll_state[bind_id]=0;
network1_delay_for_milliseconds(c,bind_id,milliseconds);
}

static int generic_tozero(network1_complete *c, int bind_id,int milliseoncs) {
if (c->sockets[bind_id] != -1) {
  close(c->sockets[bind_id]);
  c->sockets[bind_id] = -1;
  }
c->pollfds[bind_id].fd = -1;
c->poll_state[bind_id]=0;
network1_delay_for_milliseconds(c,bind_id,milliseconds);
}          

static int generic_enosupport(network1_complete *c, int bind_id) {
generic_tozero(c,bind_id,100000);
}

static int generic_eaccess(network1_complete *c, int bind_id) {
generic_tozero(c,bind_id,100000);
}

static int generic_eperm(network1_complete *c, int bind_id) {
generic_tozero(c,bind_id,100000);
}
          
static int generic_eaddrinuse(network1_complete *c, int bind_id) {
generic_tozero(c,bind_id,50000);
}
          
static int generic_eaddrnotval(network1_complete *c, int bind_id) {
generic_tozero(c,bind_id,100000);
}

	  
static int generic_wait_for_connect(network1_complete *c, int bind_id) {
c->pollfds[bind_id].fd = c->sockets[bind_id];
c->pollfds[bind_id].events = POLLOUT|POLLERR;
c->poll_state[bind_id]=2;
}


static int generic_etimeout(network1_complete *c, int bind_id) {
network1_unconnect(c,bind_id);
generic_toone(c,bind_id,2000);
}

static int generic_eintr(network1_complete *c, int bind_id) {
     network1_unconnect(c,bind_id);
     network1_delay_for_milliseconds(c,bind_id,200);
}



static int generic_badf(network1_complete *c, int bind_id) {
  generic_tozero_screw_sockets(c,bind_id,2000);
}

static int generic_econnrefused(network1_complete *c, int bind_id) {
  generic_toone(c,bind_id,1000);
}

static int generic_efault(network1_complete *c, int bind_id) {
generic_tozero(c,bind_id,300000);
}

static int generic_eisconn(network1_complete *c, int bind_id) {
network1_unconnect(c,bind_id);
generic_toone(c,bind_id,1000);
}



static int generic_enetunreach(network1_complete *c, int bind_id) {
network1_unconnect(c,bind_id);
generic_toone(c,bind_id,2000);
}

static int generic_eother(network1_complete *c, int bind_id) {
network1_unconnect(c,bind_id);
generic_tozero(c,bind_id,2000);
}

static int generic_enotsock(network1_complete *c, int bind_id) {
generic_tozero(c,bind_id,1000);
}

static int connect_worked(network1_complete *c, int bind_id) {
// 2 to 3
c->pollfds[bind_id].fd = c->sockets[bind_id];
if (i<NUMBER_OF_NETWORK1_PARTICIPANTS) {
  c->pollfds[bind_id].events = POLLPRI|POLLIN|POLLERR;
  }
else {
  c->pollfds[bind_id].events = POLLOUT|POLLERR;
  }
c->poll_state[bind_id]=3;
}






// in mode 2	  	  
static int network1_check_complete_connect_to_outside_poll(network1_complete *c, int bind_id,int e) {
int i=bind_id;
if (c->poll_state[i]==2) { // yes it is 2 here - different than the connect that is 1
    c->pollfds[bind_id].fd = -1;
    c->poll_state[i]==1;   
    }
if (e) {
  if (e==EAFNOSUPPORT)  {
    fprintf(stderr,"EAFNOSUPPORT  %d id %d port %d\n",c->sockets[bind_id],bind_id,c->ports[bind_id]);
    generic_enosupport(c,bind_id);
    }
  else if (e==EACCES) {
    fprintf(stderr,"EACCES  %d id %d port %d\n",c->sockets[bind_id],bind_id,c->ports[bind_id]);
    generic_eaccess(c,bind_id);
    }
  else if (e== EPERM) {
       fprintf(stderr,"EPERM  %d id %d port %d\n",c->sockets[bind_id],bind_id,c->ports[bind_id]);
       generic_eperm(c,bind_id);
    }
   else if (e==EADDRINUSE) {
       fprintf(stderr,"	EADDRINUSE  %d id %d port %d\n",c->sockets[bind_id],bind_id,c->ports[bind_id]);
       generic_eaddrinuse(c,bind_id);      
	}  // address in use 
   else if (e==EADDRNOTAVAIL) {
     fprintf(stderr,"	EADDRNOTAVAIL  %d id %d port %d\n",c->sockets[bind_id],bind_id,c->ports[bind_id]);
     generic_eaddrnotval(c,bind_id);
     }  // address in use 
   //??? note - EAGAIN is only used here for unix. others have insufficient routing cache and need to retry
  else if ((e==EAGAIN)    ||(e==EALREADY) || (e== EINPROGRESS)) {
    if (e==EAGAIN) {
        fprintf(stderr,"EAGAIN working \n");
	}
    else if (e==EALREADY) {
        fprintf(stderr,"EALREADY working \n");
	}
    else {
      fprintf(stderr,"EINPROGRESS  working \n");
      }
    generic_wait_for_connect(c,bind_id);
    e=0;
    return(0);    
    }
  else if (e==EINTR) {
    fprintf(stderr,"	warning EINTR  %d id %d port %d\n",c->sockets[bind_id],bind_id,c->ports[bind_id]);
    generic_eintr(c,bind_id);
    }  
  else if (e==EBADF) {
     fprintf(stderr,"   EBADF  %d id %d port %d\n",c->sockets[bind_id],bind_id,c->ports[bind_id]);
     generic_ebadf(c,bind_id);
     }
  else if (e==ECONNREFUSED) {
     fprintf(stderr,"   warning ECONNREFUSED  %d id %d port %d\n",c->sockets[bind_id],bind_id,c->ports[bind_id]);
     gneric_econnrefused(c,bind_id);
     }
  else if (e==EFAULT) {
    fprintf(stderr,"   EFAULT  %d id %d port %d\n",c->sockets[bind_id],bind_id,c->ports[bind_id]);
    generic_efault(c,bind_id);
    }
  else if (e==EISCONN) {
    fprintf(stderr,"	warning EISCONN  %d id %d port %d\n",c->sockets[bind_id],bind_id,c->ports[bind_id]);
    generic_eisconn(c,bind_id);
    }   
  else if (e==ENETUNREACH) {
    // try again in 2 seconds
    fprintf(stderr,"warning ENETUNREACH clear connectunlink to connect got errno %d\n",e);
    generic_enetunreach(c,bind_id);
    e=0;
    }
  else if (e==ENOTSOCK) {
    generic_enotsock(c,bind_id);
    e=0;
    }
  else if (e==EPROTOTYPE) { 
    fprintf(stderr,"   EPROTOTYPE  %d id %d port %d\n",c->sockets[bind_id],bind_id,c->ports[bind_id]);
    generic_eprototype(c,bind_id);
    }
  else if (e==ETIMEDOUT) {
    fprintf(stderr,"warning ETIMEOUT clear connectunlink to connect got errno %d\n",e);
    connect_etimeout(c,bind_id);
    }
  else {
    fprintf(stderr,"warning unknown clear connectunlink to connect got errno %d\n",e);
    connect_eother(c,bind_id);
    }    
  return(0);
  }  
else {
  // we are good - we got the connect no errnos
  connect_worked(c,bind_id);
  }
return(1);
}  




// instant connect from type 1 - no polling
static int network1_connect_to_outside_poll(network1_complete *c, int bind_id) {
if (connect(c->sockets[bind_id],(struct sockaddr *)&(c->sending_to[bind_id]),sizeof(struct sockaddr_in)) ==-1) {
  int e = errno;
  if (e==EAFNOSUPPORT)  {
    fprintf(stderr,"EAFNOSUPPORT  %d id %d port %d\n",c->sockets[bind_id],bind_id,c->ports[bind_id]);
    generic_enosupport(c,bind_id);
    }
  else if (e==EACCES) {
    fprintf(stderr,"EACCES  %d id %d port %d\n",c->sockets[bind_id],bind_id,c->ports[bind_id]);
    generic_eaccess(c,bind_id);
    exit(-1);
    }
  else if (e== EPERM) {
       fprintf(stderr,"EPERM  %d id %d port %d\n",c->sockets[bind_id],bind_id,c->ports[bind_id]);
       generic_eperm(c,bind_id);
    }
   else if (e==EADDRINUSE) {
       fprintf(stderr,"	EADDRINUSE  %d id %d port %d\n",c->sockets[bind_id],bind_id,c->ports[bind_id]);
       generic_eaddrinuse(c,bind_id);      
	}  // address in use 
   else if (e==EADDRNOTAVAIL) {
     fprintf(stderr,"	EADDRNOTAVAIL  %d id %d port %d\n",c->sockets[bind_id],bind_id,c->ports[bind_id]);
     generic_eaddrnotval(c,bind_id);
     }  // not avail
   //??? note - EAGAIN is only used here for unix. others have insufficient routing cache and need to retry
   else if ((e==EAGAIN)    ||(e==EALREADY) || (e== EINPROGRESS)) {
     if (e==EAGAIN) {
        fprintf(stderr,"EAGAIN working \n");
	}
     else if (e==EALREADY) {
        fprintf(stderr,"EALREADY working \n");
	}
     else {
        fprintf(stderr,"EINPROGRESS  working \n");
        }
    generic_wait_for_connect(c,bind_id);
    return(0);
    }
   else if (e==EINTR) {
     fprintf(stderr,"	warning EINTR  %d id %d port %d\n",c->sockets[bind_id],bind_id,c->ports[bind_id]);
     generic_eintr(c,bind_id);
     }   
  else if (e==ENETUNREACH) {
    // try again in 2 seconds
    fprintf(stderr,"warning ENETUNREACH clear connectunlink to connect got errno %d\n",e);
    generic_enetunreach(c,bind_id);
    }
  else if (e==ENOTSOCK) {
    generic_enotsock(c,bind_id);
    }
  else if (e==EPROTOTYPE) { 
    fprintf(stderr,"   EPROTOTYPE  %d id %d port %d\n",c->sockets[bind_id],bind_id,c->ports[bind_id]);
    generic_eprototype(c,bind_id);
    }
  else if (e==ETIMEDOUT) {
    fprintf(stderr,"warning ETIMEOUT clear connectunlink to connect got errno %d\n",e);
    connect_etimeout(c,bind_id);
    }
  else {
    fprintf(stderr,"erro rother  clear connectunlink to connect got errno %d\n",e);
    connect_eother(c,bind_id);  
    }
  }  
else {
  // we are good - we got the connect no errnos
  connect_worked(c,bind_id);
  }
return 1;
} 
  



static int network1_attempt_receive(network1_complete *c,int bind_id) {
c->poll_state[bind_id]=4;  // waiting for read
int result = recv(c->sockets[bind_id],&(c->buffers[bind_id]),NETWORK1_MAX_BUFFER_SIZE,MSG_DONTWAIT);
if (result==0) {  // end of file
  receive_eof(c,bind_id);
//  close(c->sockets[bind_id]); // close and try to reopen the socket later
//  c->sockets[bind_id] = -1;
//  c->pollfds[bind_id].fd = -1;
//  c->poll_state[bind_id]=0;
//  network1_delay_for_milliseconds(c,bind_id,1000);
  }
else if (result >0) {
  receive_gotit(c,bind_id);
//  c->buflen[bind_id]=result;
//  c->call_rounds[bind_id]=1;
//  c->poll_state[bind_id]=5;
//  c->call_rounds[bind_id]=1
  }
else {    
  int e = errno;
  if (e==0) { // end of file
    receive_eof(c,bind_id);
//    close(c->sockets[bind_id]); // close and try to reopen the socket later
//    c->sockets[bind_id] = -1;
//    c->pollfds[bind_id].fd = -1;
//    c->poll_state[bind_id]=0;
//    network1_delay_for_milliseconds(c,bind_id,1000);
    }
  else 	{		    
    if ((e==EAGAIN)||(e==EWOULDBLOCK)||(e==EINPROGRESS)) {
      reveive_in_progress(c,bind_id);
//      c->poll_state[bind_id]=4;
//      c->pollfds[bind_id].fd=c->sockets[bind_id];
//      c->pollfds[bind_id].events=POLLIN|POLLPRI|POLLERR;
      }   
    else if (e==EBADF) {
      fprintf(stderr,"   EBADF  %d id %d port %d\n",c->sockets[bind_id],bind_id,c->ports[bind_id]);
      generic_ebadf(c,bind_id);
      }
    else if (e==ECONNREFUSED) {
      fprintf(stderr,"   ECONNREFUSED  %d id %d port %d\n",c->sockets[bind_id],bind_id,c->ports[bind_id]);
      gneric_econnrefused(c,bind_id);
      }
    else if (e==EFAULT) {
      fprintf(stderr,"   EFAULT  %d id %d port %d\n",c->sockets[bind_id],bind_id,c->ports[bind_id]);
      generic_efault(c,bind_id);
      }
    else if (e==EINTR) {
      fprintf(stderr,"   EINTR  %d id %d port %d\n",c->sockets[bind_id],bind_id,c->ports[bind_id]);
//      close (c->sockets[bind_id]);
      reveive_eintr(c,bind_id);
//      c->pollfds[bind_id].fd = -1;
//      c->poll_state[bind_id]=3;
//      network1_delay_for_milliseconds(c,bind_id,50);
      }
    else if (e==EINVAL) {
      fprintf(stderr,"   EINVAL  %d id %d port %d\n",c->sockets[bind_id],bind_id,c->ports[bind_id]);
      generic_einval(c,bind_id);
      
 //     close (c->sockets[bind_id]);
 //     c->sockets[bind_id]=-1;
 //     c->pollfds[bind_id].fd = -1;
 //     c->poll_state[bind_id]=0;
 //     network1_delay_for_milliseconds(c,bind_id,1000);
 //     }
    else if (e==ENOMEM) {
      fprintf(stderr,"   NOMEM  %d id %d port %d\n",c->sockets[bind_id],bind_id,c->ports[bind_id]);
      generic_enomem(c,bind_id);
//      close (c->sockets[bind_id]);
//      c->sockets[bind_id]=-1;
//      c->pollfds[bind_id].fd = -1;
//      c->poll_state[bind_id]=0;
//      network1_delay_for_milliseconds(c,bind_id,1000);
      }
    else if (e==ENOTCONN) {
      fprintf(stderr,"warning ENOTCONN  %d id %d port %d\n",c->sockets[bind_id],bind_id,c->ports[bind_id]);
      generic_enotconn(c,bind_id);
//      close (c->sockets[bind_id]);
//      c->pollfds[bind_id].fd = -1;
//      c->poll_state[bind_id]=1;
//      network1_delay_for_milliseconds(c,bind_id,100);
      }
    else if (e==ENOTSOCK) {
      fprintf(stderr,"  ENOTSOCK  %d id %d port %d\n",c->sockets[bind_id],bind_id,c->ports[bind_id]);
      generic_enotsock(c,bind_id);
//      c->sockets[bind_id]=-1;
//      c->pollfds[bind_id].fd = -1;
//      c->poll_state[bind_id]=0;
//      network1_delay_for_milliseconds(c,bind_id,1000);
      }
    else {
      fprintf(stderr,"  recv generic errno  %d id %d port %d\n",c->sockets[bind_id],bind_id,c->ports[bind_id]);
      generic_eother(c,bind_id);
//      c->sockets[bind_id]=-1;
//      c->pollfds[bind_id].fd = -1;
//      c->poll_state[bind_id]=0;
//      network1_delay_for_milliseconds(c,bind_id,1000);
      }
    }  // if e is not 0
  } // if result is negative

return (0);
}     

      
	    

static int network1_attempt_send(network1_complete *c,int bind_id) {
c->poll_state[bind_id]=4;  // waiting for read
int result = send(c->sockets[bind_id],&(c->buffers[bind_id]),NETWORK1_MAX_BUFFER_SIZE,MSG_DONTWAIT);
if (result==0) {  // end of file
  send_eof(c,bind_id);
//  close(c->sockets[bind_id]); // close and try to reopen the socket later
//  c->sockets[bind_id] = -1;
//  c->pollfds[bind_id].fd = -1;
//  c->poll_state[bind_id]=0;
//  network1_delay_for_milliseconds(c,bind_id,1000);
  }
else if (result >0) {
  send_gotit(c,bind_id);
//  c->buflen[bind_id]=result;
//  c->call_rounds[bind_id]=1;
//  c->poll_state[bind_id]=5;
//  c->call_rounds[bind_id]=1
  }
else {    
  int e = errno;
  if (e==0) { // end of file
    send_eof(c,bind_id);
//    close(c->sockets[bind_id]); // close and try to reopen the socket later
//    c->sockets[bind_id] = -1;
//    c->pollfds[bind_id].fd = -1;
//    c->poll_state[bind_id]=0;
//    network1_delay_for_milliseconds(c,bind_id,1000);
    }
  else 	{		    
    if ((e==EAGAIN)||(e==EWOULDBLOCK)||(e==EINPROGRESS)) {
      send_in_progress(c,bind_id);
//      c->poll_state[bind_id]=4;
//      c->pollfds[bind_id].fd=c->sockets[bind_id];
//      c->pollfds[bind_id].events=POLLOUT|POLLERR;
      }   
    else if (e==EACCESS) {
      fprintf(stderr,"   EBADF  %d id %d port %d\n",c->sockets[bind_id],bind_id,c->ports[bind_id]);
      send_eaccess(c,bind_id);
      }
    else if (e==EALREADY) {
      fprintf(stderr,"   EALREADY  %d id %d port %d\n",c->sockets[bind_id],bind_id,c->ports[bind_id]);
      send_ealready(c,bind_id);
      }
    else if (e==EBADF) {
      fprintf(stderr,"   EBADF  %d id %d port %d\n",c->sockets[bind_id],bind_id,c->ports[bind_id]);
      generic_ebadf(c,bind_id);
      }
      
    else if (e==ECONNRESET) {
      fprintf(stderr,"   econnreset  %d id %d port %d\n",c->sockets[bind_id],bind_id,c->ports[bind_id]);
      send_econnreset(c,bind_id);
      }
    else if (e==EDESTADDRREQ) {
      send_estandardreq(c,bind_id);
      }
    else if (e==EFAULT) {
      fprintf(stderr,"   EFAULT  %d id %d port %d\n",c->sockets[bind_id],bind_id,c->ports[bind_id]);
      generic_efault(c,bind_id);
      }
    else if (e==EINTR) {
      fprintf(stderr,"   EINTR  %d id %d port %d\n",c->sockets[bind_id],bind_id,c->ports[bind_id]);
//      close (c->sockets[bind_id]);
      send_eintr(c,bind_id);
//      c->pollfds[bind_id].fd = -1;
//      c->poll_state[bind_id]=3;
//      network1_delay_for_milliseconds(c,bind_id,50);
      }
    else if (e==EINVAL) {
      fprintf(stderr,"   EIONVAL  %d id %d port %d\n",c->sockets[bind_id],bind_id,c->ports[bind_id]);
      send_einval(c,bind_id);
      }
      
    else if (e==EISCONN) {
      fprintf(stderr,"   EISCONN  %d id %d port %d\n",c->sockets[bind_id],bind_id,c->ports[bind_id]);
      send_eisconn(c,bind_id);
      }
    else if (e==EMSGSIZE) {
      fprintf(stderr,"   EMSGSIZE  %d id %d port %d\n",c->sockets[bind_id],bind_id,c->ports[bind_id]);
      send_emsgsize(c,bind_id);
      }
    else if (e==ENOBUFS) {
      fprintf(stderr,"   ENOBUFS  %d id %d port %d\n",c->sockets[bind_id],bind_id,c->ports[bind_id]);
      send_enobuffs(c,bind_id);
      }
    else if (e==ENOMEM) {
      fprintf(stderr,"   NOMEM  %d id %d port %d\n",c->sockets[bind_id],bind_id,c->ports[bind_id]);
      generic_enomem(c,bind_id);
//      close (c->sockets[bind_id]);
//      c->sockets[bind_id]=-1;
//      c->pollfds[bind_id].fd = -1;
//      c->poll_state[bind_id]=0;
//      network1_delay_for_milliseconds(c,bind_id,1000);
      }
    else if (e==ENOTCONN) {
      fprintf(stderr,"warning ENOTCONN  %d id %d port %d\n",c->sockets[bind_id],bind_id,c->ports[bind_id]);
      generic_enotconn(c,bind_id);
//      close (c->sockets[bind_id]);
//      c->pollfds[bind_id].fd = -1;
//      c->poll_state[bind_id]=1;
//      network1_delay_for_milliseconds(c,bind_id,100);
      }
    else if (e==ENOTSOCK) {
      fprintf(stderr,"  ENOTSOCK  %d id %d port %d\n",c->sockets[bind_id],bind_id,c->ports[bind_id]);
      generic_enotsock(c,bind_id);
//      c->sockets[bind_id]=-1;
//      c->pollfds[bind_id].fd = -1;
//      c->poll_state[bind_id]=0;
//      network1_delay_for_milliseconds(c,bind_id,1000);
      }
    else if (e==ENOTSUPP) {
      fprintf(stderr,"  ENOTSOCK  %d id %d port %d\n",c->sockets[bind_id],bind_id,c->ports[bind_id]);
      generic_enotsupp(c,bind_id);
//      c->sockets[bind_id]=-1;
//      c->pollfds[bind_id].fd = -1;
//      c->poll_state[bind_id]=0;
//      network1_delay_for_milliseconds(c,bind_id,1000);
      }
    else if (e==EPIPE) {
      fprintf(stderr,"  epipe  %d id %d port %d\n",c->sockets[bind_id],bind_id,c->ports[bind_id]);
      send_epipe(c,bind_id);
      }
    else {
      fprintf(stderr,"  recv generic errno  %d id %d port %d\n",c->sockets[bind_id],bind_id,c->ports[bind_id]);
      generic_eother(c,bind_id);
      }
    }  // if e is not 0
  } // if result is negative

return (0);
}  // attempt_Send     
		  
			
			            
	
/* this might do nothing if we dont have buffers to receive */	    
static int network1_set_buffer_if_necessary_and_receive_from_poll(network1_complete *c,int bind_id) {
// assume state is 3 or 5
// assume i is not player number
// assume there is no timer
if ((c->poll_state[bind_id])==5) {
  c->poll_state[bind_id] = 3;
  c->buflen[bind_id]=0;
  c->buffer[bind_id]=NULL;
  }
if (c->buffer[bind_id]) {
  if (c->buflen[bind_id]) {
    if (c->network1_get_new_receive_buffer[bind_id]) {
      *(c->network1_get_new_receive_buffer[bind_id]);
      }
    }
  }
else {
  if (c->network1_get_new_receive_buffer[bind_id]) {
    *(c->network1_get_new_receive_buffer[bind_id]);
      }
  }
  
  
  
if (c->buffer[bind_id]) {
  if (!(c->buflen[bind_id])) {
    return (network1_attempt_receive(c,bind_id);
    }  	
return 0;
}



// Both:	      
//  if the state is 5 - you can do it, but the state will turn to 3.
// if the state is 4 - you cannot do it
// if the state is 3, 2, or 1, you can do it , unless it is NULL
// normally you do not need to call this because the buffers are asked for
int network1_set_buffer(network1_complete *c,int bind_id,char *buffer, int buflen) 
{
if (c->poll_state[bind_id] == 4) {
  return 4;
  }
if ((c->poll_state[bind_id] == 3)&&(c->buflen[bind_id]=!0)&&(c->buffer[bind_id]==NULL)) {
  return 3;
  }
if ((c->poll_state[bind_id] == 2)&&(c->buflen[bind_id]=!0)&&(c->buffer[bind_id]==NULL)) {
  return 2;
  }
if ((c->poll_state[bind_id] == 1)&&(c->buflen[bind_id]=!0)&&(c->buffer[bind_id]==NULL)) {
  return 1;
  }
if ((c->poll_state[bind_id] == 0)&&(c->buflen[bind_id]=!0)&&(c->buffer[bind_id]==NULL)) {
  return 1;
  }
if (c->poll_state[bind_id] == 5) {
  c->poll_state[bind_id] = 3;
}
c->buflen[bind_id]=0;
c->buffer[bind_id]=buffer;
c->buflen[bind_id] = buflen;
return 0;
}  
  

				      
static int network1_send_if_can(network1_complete *c,int bind_id) {
// assume poll state is 3, or 5.  If 5, well, we reset the buffer
//assume i is not player number
// assume thers is no timer
if (c->poll_state)==5 {
  c->buflen[bind_id]=0;
  c->buffer[bind_id]=NULL;
  }
if (c->buffer[bind_id]) {
  if (c->buflen[bind_id]) {
    return network1_attempt_send(c,bind_id);
    }
  else {
    if (c->network1_pull_next_send_buffer_from_queue[bind_id]);
    *(c->network1_pull_next_send_buffer_from_queue[bind_id])(c,bind_id));
    }
  }
else {
  if (c->network1_pull_next_send_buffer_from_queue[bind_id]);
  *(c->network1_pull_next_send_buffer_from_queue[bind_id])(c,bind_id));
  }
if (c->buffer[bind_id]) {
  if (c->buflen[bind_id]) {
    return network1_attempt_send(c,bind_id);
    }
  }
return 0;
}
  


																      
																																      
																																																      
																																																																      
																																																																																      
																																																																																																      
																																																																																																																																      
																																																																																																																																																      
																																																																																																																																																																      
																																																																																																																																																																																      
																																																																																																																																																																																																      												      			      	      
int network1_poll_check(network1_complete *c) {
c->network1_check_poll_runs_in_call = 0;

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

sr_gettimeofday(&(c->poll_check_start_time[c->network1_check_poll_runs_in_call]));

// call_rounds is used to decide wheteher to call the round1-round3 to externally process messages
for (int i=0;i<c->current_number_of_polls;i++) {
  c->call_rounds[i]=0;
  }

/* clear out revents, and turn off write polls if not necessary. read polls are necessary */
for (int i=0;i<c->current_number_of_polls;i++) {
 
  if (i==c->participant_number) {
    continue;
    }
  {
    struct timespec a,b;
    b = c->poll_check_start_time;
    a = c->local_delay_work[i];
    if (a->tv_sec > b_tv_sec) || (
      (a->tv_sec == b.tv_sec) && (a.tv_usec  >= b.tv_usec)) { // we are delayed. leave as is
// maybe       c->pollfd.fd = -1;
     continue;
     }
   }
  if (c->poll_state[i]==0) {
    network1_setup_and_bind(c,i);
    }
  if (c->poll_state[i]==1) {
    network1_connect_to_poll(c,i);
    }
  if (c->poll_state[i]==5) {
    c->memlen[i]=0; // reset this. We can reuse the buffers
    c->poll_state[i]=3; // At this level, we ran the rounds
    }
  if (c->poll_state[i]==3) {
    if (i<NUMBER_OF_NETWORK1_PARTICIPANTS) {
      network1_set_buffer_if_necessary_and_receive_from_poll(c,i);
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
  
  }  // our clean up and set  for polling
  



sigset_t origmask;
int timeout;
struce timevspec two_p;
two_p.tv_sec=0;
two_p.tv_nsec=0;
sigget_t sigmask = 0;


// The actual poll!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
sr_gettimeofday(&c->local_poll_check_poll_start_time[c->network1_check_poll_runs_in_call]);
int number_of_events = ppoll(&(c->pollfds[0]), c->current_number_of_polls,&two_p,&sigmask);		  
sr_gettimeofday(&(c->local_poll_end_time[c->network1_check_poll_runs_in_call]); 
;



int number_to_round=0;
for (int i=0;i<c->current_number_of_polls;i++) {
  if (c->call_rounds[i] number_to_round++;;
  }
  

if (!(number_of_events+number_to_round)) {
  c->local_handle_poll_end_time[c->network1_check_poll_runs_in_call])=		c->local_poll_end_time[c->network1_check_poll_runs_in_call];
  c->local_round1_end_time[c->network1_check_poll_runs_in_call]=		c->local_poll_end_time[c->network1_check_poll_runs_in_call];
  c->local_round2_end_time[c->network1_check_poll_runs_in_call]=		c->local_poll_end_time[c->network1_check_poll_runs_in_call];
  c->local_round3_end_time[c->network1_check_poll_runs_in_call]=		c->local_poll_end_time[c->network1_check_poll_runs_in_call];
  c->local_network1_check_end_time					        c->local_poll_end_time[c->network1_check_poll_runs_in_call];
  
  c->network1_check_poll_runs_in_call++;
  goto full_check_poll_loop;
  }
  


{ // Block to process polls
  for (int i=0;i<c->current_number_of_polls;i++) {
    if (c->poll_state[i]==2)) { // doing a connect and waaiting for POLLOUT
      int e=0;
      if (c->revents&(POLLOUT|POLERR)) {
        e = 0;
        getsockopt(c->socket[i],SOL_SOCKET,SO_ERROR,(void *)&e,sizeof(int));
        c->pollfds[i].fd=-1;       
        network1_handle_connect_error(c,i,e);
      

        if (c->poll_state[i]==3)) { //  we might have finished connecting
          if (i<NUMBER_OF_NETWORK1_PARTICIPANTS) {
            network1_set_buffer_if_necessary_and_receivefrom_poll(c,i);
            }
          else if (i<NUMBER_OF_NETWORK1_PARTICIPANTS_TIMES_2) {
            network1_send_if_can(c,i);
            }
	  } /* if we got to state 3 after the error check on state 2 */
        } /* if we are ca connect and got a return value  on state 2 */ 
      }  /* if state=2 , connection possibly made */
    else if (c->poll_state[i]==4)) { // doing a send or receive 
      if (i<NUMBER_OF_NETWORK1_PARTICIPANTS) {
        int e=0;
        if (c->revents&(POLERR)) {
          e = 0;
          getsockopt(c->socket[i],SOL_SOCKET,SO_ERROR,(void *)&e,sizeof(int));
          c->pollfds[i].fd=-1;       
          network1_handle_receive_error(c,i,e);
          }
        else if (revents&(POLLIN|POLLPRI) {
          network_attempt_receive(c,i);
	  }
	} // if input
      else { //  (i>=NUMBER_OF_NETWORK1_PARTICIPANTS)  , output
        int e=0;
        if (c->revents&(POLERR)) {
          e = 0;
          getsockopt(c->socket[i],SOL_SOCKET,SO_ERROR,(void *)&e,sizeof(int));
          c->pollfds[i].fd=-1;       
          network1_handle_send_error(c,i,e);
          }
        else if (revents&POLLOUT) {
          c->call_rounds[bind_id]=1;
	  c->poll_state[bind_id]=5;
	  }
	} // if input
	
      } // if state is 4
    i = i + 1;
    } // while going through all events
  }     /* special block to handle connect polling */
  

// note - this above block could reduce the nuber of events that passed, but will never increase it 
// so we can call start_round/stop/round anyways - sometimes witouth any other events

// recompute number_to_round
number_to_round=0;
for (int i=0;i<c->current_number_of_polls;i++) {
  if (c->call_rounds[i] number_to_round++;;
  }

sr_gettimeofday(&(c->local_handle_poll_end_time][c->network1_check_poll_runs_in_call]));


if (!number_to_round) {
  c->local_round1_end_time[c->network1_check_poll_runs_in_call]=		c->local_handle_poll_end_time[c->network1_check_poll_runs_in_call];
  c->local_round2_end_time[c->network1_check_poll_runs_in_call]=		c->local_handle_poll_end_time[c->network1_check_poll_runs_in_call];
  c->local_round3_end_time[c->network1_check_poll_runs_in_call]=		c->local_handle_poll_end_time[c->network1_check_poll_runs_in_call];
  c->local_network1_check_end_time					=        c->local_handle_poll_end_time[c->network1_check_poll_runs_in_call];
  
  goto full_check_poll_loop;
  return 0;
  }
  



{ // Block to process events round 1
  
  if (number_of_events &&(c->network1_action_start_round1)) {
    (*c->network1_action_start_round1)(c,0,number_of_events);
    }
    
  for (int i=0;i<c->current_number_of_polls;i++) {
    if (c->call_rounds[bind_id]) {    
      if  (c->network1_handle_action_round1[i]) { (*c->network1_handle_action_round1[i])(c,i,number_of_events);  }
    } /* while loopint through all events */
  
  if ((number_of_events) &&(c->network1_action_finish_round1)) {
    (*c->network1_action_finish_round1)(c,number_of_events);
    }


  } // block to process events round 1


// recompute number_to_round after round 1
number_to_round=0;
for (int i=0;i<c->current_number_of_polls;i++) {
  if (c->call_rounds[i] number_to_round++;;
  }

sr_gettimeofday(&(c->local_round1_end_time][c->network1_check_poll_runs_in_call]));

if (!number_to_round) {
  c->local_round2_end_time[c->network1_check_poll_runs_in_call]=		c->local_round1_end_time[c->network1_check_poll_runs_in_call];
  c->local_round3_end_time[c->network1_check_poll_runs_in_call]=		c->local_round1_end_time[c->network1_check_poll_runs_in_call];
  c->local_network1_check_end_time			=                       c->local_round1_end_time[c->network1_check_poll_runs_in_call];
  
  c->network1_check_poll_runs_in_call++;
  goto full_check_poll_loop;
  }
  


{ // Block to process events round 2
  
  if (number_of_events &&(c->network1_action_start_round2)) {
    (*c->network1_action_start_round2)(c,0,number_of_events);
    }
    
  for (int i=0;i<c->current_number_of_polls;i++) {
    if (c->call_rounds[bind_id]) {    
      if  (c->network1_handle_action_round2[i]) { (*c->network1_handle_action_round2[i])(c,i,number_of_events);  }
    } /* while loopint through all events */
  
  if ((number_of_events) &&(c->network1_action_finish_round2)) {
    (*c->network1_action_finish_round2)(c,number_of_events);
    }


  } // block to process events round 2
  
  

// recompute number_to_round after round 2
number_to_round=0;
for (int i=0;i<c->current_number_of_polls;i++) {
  if (c->call_rounds[i] number_to_round++;
  }

sr_gettimeofday(&(c->local_round2_end_time][c->network1_check_poll_runs_in_call]));

if (!number_to_round) {
  c->local_round3_end_time[c->network1_check_poll_runs_in_call]=		c->local_round2_end_time[c->network1_check_poll_runs_in_call];
  c->local_network1_check_end_time			=                       c->local_round2_end_time[c->network1_check_poll_runs_in_call];
  
  c->network1_check_poll_runs_in_call++;
  goto full_check_poll_loop;
  }
  
  


{ // Block to process events round 3
  
  if (number_of_events &&(c->network1_action_start_round3)) {
    (*c->network1_action_start_round3)(c,0,number_of_events);
    }
    
  for (int i=0;i<c->current_number_of_polls;i++) {
    if (c->call_rounds[bind_id]) {    
      if  (c->network1_handle_action_round3[i]) { (*c->network1_handle_action_round3[i])(c,i,number_of_events);  }
    } /* while loopint through all events */
  
  if ((number_of_events) &&(c->network1_action_finish_round3)) {
    (*c->network1_action_finish_round3)(c,number_of_events);
    }


  } // block to process events round 3


// recompute number_to_round after round 3 - to use later
number_to_round=0;
for (int i=0;i<c->current_number_of_polls;i++) {
  if (c->call_rounds[i] number_to_round++;
  }

sr_gettimeofday(&(c->local_round3_end_time][c->network1_check_poll_runs_in_call]));

if (!number_to_round) {
  c->local_network1_check_end_time			=                       c->local_round3_end_time[c->network1_check_poll_runs_in_call];
  
  c->network1_check_poll_runs_in_call++;
  goto full_check_poll_loop;
  }
  



  
/* Extra read all about it */
for (int i=0;i<c->current_number_of_polls;i++) {
/* since we are not looping right now, start a send or receive for any type 3's */
  if (i==c->participant_number) {
    continue;
    }
  {  // maybe check the time
    struct timespec a,b;
    b = c->poll_check_start_time;
    a = c->local_delay_work[i];
    if (a->tv_sec > b_tv_sec) || (
      (a->tv_sec == b.tv_sec) && (a.tv_usec  >= b.tv_usec)) { // we are delayed. leave as is
// maybe       c->pollfd.fd = -1;
     continue;
     }
   }
  if (c->poll_state[i]==5) {
    c->memlen[i]=0; // reset this. We can reuse the buffers
    c->poll_state[i]=3; // At this level, we ran the rounds
    }
  if ((c->poll_state[i]==3) {
    if (i<NUMBER_OF_NETWORK1_PARTICIPANTS) {
      network1_set_buffer_if_necessary_and_receivefrom_poll(c,i);
      }
    else if (i<NUMBER_OF_NETWORK1_PARTICIPANTS_TIMES_2) {
      network1_send_if_can(c,i);
      }
    }
  }   




c->network1_check_poll_runs_in_call++;

full_check_poll_loop:
// later on we can loop around a few times to make it more efficient.
sr_gettimeofday(&(c->network1_check_end_time));


return number_of_events;
}





int network1_init (network1_complete *c,int participant_number,char *broadcast_ip,char *ips[],
     network1_complete_round_call network1_action_start_round1,
     network1_complete_round_call network1_handle_action_round1_in, network1_complete_round_call network1_handle_action_round1_out,
     network1_complete_round_call network1_action_end_round1,
     network1_complete_round_call network1_action_start_round2,
     network1_complete_round_call network1_handle_action_round2_in, network1_complete_round_call network1_handle_action_round2_out,
     network1_complete_round_call network1_action_end_round2,
     network1_complete_round_call network1_action_start_round3,
     network1_complete_round_call network1_handle_action_round3_in, network1_complete_round_call network1_handle_action_round3_out,
     network1_complete_round_call network1_action_end_round3,
     network1_complete_round_call network1_get_new_receive_buffer, /* get a new receive buffer for this poll - could be broadcast in */                                       
     network1_complete_round_call network1_get_new_send_buffer, /* get a new send buffer - 0 length  - just to fill up  - why? */                                 
     network1_complete_round_call network1_pull_next_send_buffer_from_queue /* get a semd buffer off the queue for this polling address - returns queue length */          
     )  {


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
struct timespec nowtime;
sr_gettimeofday(&nowtime);

for (i=0;i<NUMBER_OF_NETWORK1_PARTICIPANTS;i++) {
  int communicator = i;
  c->poll_state[i] = 0;
  c->communicator[i] = i;
  c->direction[i] = 1;  // will have to do that later
  c->ports[i] = NETWORK1_START_PORT + c->participant_number * NUMBER_OF_NETWORK1_PARTICIPANTS_TIMES_2  + communicator;
  c->sockets[i]=-1;
  strncpy[&(c->ip_addresses[i][0]),ips[i],19);
  c->ip_addresses[i][19]='\0';
  c->local_delay_work[i] = nowtime;
  
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
  c->local_delay_work[o] = nowtime;

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



for (int i=0;i<NUMBER_OF_NETWORK1_PARTICIPANTS;i++) {
  int o = NUMBER_OF_NETWORK1_PARTICIPANTS + i;

  c->communicator[i] = i;
  c->communicator[o] = i;
  
  
  c->buflen[i] = 0;
  c->buffers[i]=NULL;
  c->buflen[o] = 0;
  c->buffers[o]=NULL;
  }  

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
  
 



return 1;
}
