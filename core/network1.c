




// here we gp<

#include "glue.h"
#include "network1.h"
#include <error.h>
#include <sys/time.h>
       





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
     fprintf(stderr,"bd cannot set permission: on poll %d port %d\n",bind_id,c->ports[bind_id]);
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



static void network1_delay_for_milliseconds(network1_complete *c, int bind_id,unsigned int millisecondsa) {
// here is a hack. c->local_poll_predo_start_time[c->network1_check_poll_runs_in_call] always called first before we do a delay, so we dont need to do gettimeofday 
struct timeval a,b;
a =c->local_poll_predo_start_time[c->network1_check_poll_runs_in_call];
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
fprintf(stderr,"delay add %d milliseconds to %ld %ld -> %ld %ld\n",millisecondsa,b.tv_sec,b.tv_usec,a.tv_sec,a.tv_usec);
c->local_delay_work[bind_id]=a;
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


static int generic_toone(network1_complete *c, int bind_id,int milliseconds) {
network1_unconnect(c,bind_id);
c->poll_state[bind_id]=1;
network1_delay_for_milliseconds(c,bind_id,milliseconds);
return 0;
}

static int generic_tothree(network1_complete *c, int bind_id,int milliseconds) {
c->poll_state[bind_id]=3;
network1_delay_for_milliseconds(c,bind_id,milliseconds);
return 0;
}

static int generic_tozero_screw_sockets(network1_complete *c, int bind_id,int milliseconds) {
c->sockets[bind_id] = -1;
c->pollfds[bind_id].fd = -1;
c->poll_state[bind_id]=0;
network1_delay_for_milliseconds(c,bind_id,milliseconds);
return 0;
}

static int generic_tozero(network1_complete *c, int bind_id,int milliseconds) {
if (c->sockets[bind_id] != -1) {
  close(c->sockets[bind_id]);
  c->sockets[bind_id] = -1;
  }
c->pollfds[bind_id].fd = -1;
c->poll_state[bind_id]=0;
network1_delay_for_milliseconds(c,bind_id,milliseconds);
return 0;
}          

static int generic_enosupport(network1_complete *c, int bind_id) {
return generic_tozero(c,bind_id,100000);
}

static int generic_eacces(network1_complete *c, int bind_id) {
return generic_tozero(c,bind_id,100000);
}

static int send_eacces(network1_complete *c, int bind_id) {
return generic_tothree(c,bind_id,100);
}

static int generic_eperm(network1_complete *c, int bind_id) {
return generic_tozero(c,bind_id,100000);
}
          
static int generic_eaddrinuse(network1_complete *c, int bind_id) {
return generic_tozero(c,bind_id,50000);
}
          
static int generic_eaddrnotval(network1_complete *c, int bind_id) {
return generic_tozero(c,bind_id,100000);
}

	  
static int generic_wait_for_connect(network1_complete *c, int bind_id) {
c->pollfds[bind_id].fd = c->sockets[bind_id];
c->pollfds[bind_id].events = POLLOUT|POLLERR;
c->poll_state[bind_id]=2;
return 1;
}


static int connect_etimeout(network1_complete *c, int bind_id) {
network1_unconnect(c,bind_id);
return generic_toone(c,bind_id,2000);
}

static int send_econnreset(network1_complete *c, int bind_id) {
network1_unconnect(c,bind_id);
return generic_toone(c,bind_id,1000);
}

static int generic_eintr(network1_complete *c, int bind_id) {
     network1_unconnect(c,bind_id);
     network1_delay_for_milliseconds(c,bind_id,200);
 return 0;
}


static int send_eintr(network1_complete *c, int bind_id) {
return   generic_tothree(c,bind_id,100);
}

static int send_eisconn(network1_complete *c, int bind_id) {
network1_unconnect(c,bind_id);
return   generic_toone(c,bind_id,100);
}

static int send_emsgsize(network1_complete *c, int bind_id) {
return   generic_tothree(c,bind_id,400);
}

static int send_epipe(network1_complete *c, int bind_id) {
return   generic_tozero(c,bind_id,1000);
}

static int send_estandardreq(network1_complete *c, int bind_id) {
return   generic_tothree(c,bind_id,100);
}

static int generic_ebadf(network1_complete *c, int bind_id) {
return   generic_tozero_screw_sockets(c,bind_id,2000);
}

static int send_econnrefused(network1_complete *c, int bind_id) {
return   generic_tothree(c,bind_id,200);
//return   generic_tozero(c,bind_id,200);
}

static int generic_econnrefused(network1_complete *c, int bind_id) {
return   generic_toone(c,bind_id,1000);
}

static int generic_efault(network1_complete *c, int bind_id) {
return generic_tozero(c,bind_id,300000);
}

static int generic_eisconn(network1_complete *c, int bind_id) {
network1_unconnect(c,bind_id);
return generic_toone(c,bind_id,1000);
}



static int generic_enetunreach(network1_complete *c, int bind_id) {
network1_unconnect(c,bind_id);
return generic_toone(c,bind_id,2000);
}

static int generic_eother(network1_complete *c, int bind_id) {
network1_unconnect(c,bind_id);
return generic_tozero(c,bind_id,2000);
}

static int generic_enomem(network1_complete *c, int bind_id) {
network1_unconnect(c,bind_id);
return generic_tozero(c,bind_id,2000);
}


static int generic_enotconn(network1_complete *c, int bind_id) {
network1_unconnect(c,bind_id);
return generic_toone(c,bind_id,2000);
}


static int connect_eother(network1_complete *c, int bind_id) {
network1_unconnect(c,bind_id);
return generic_toone(c,bind_id,1000);
}

static int generic_enotsock(network1_complete *c, int bind_id) {
return generic_tozero(c,bind_id,1000);
}

static int connect_worked(network1_complete *c, int bind_id) {
// 2 to 3
c->pollfds[bind_id].fd = c->sockets[bind_id];
if (bind_id<NUMBER_OF_NETWORK1_PARTICIPANTS) {
  c->pollfds[bind_id].events = POLLPRI|POLLIN|POLLERR;
  }
else {
  c->pollfds[bind_id].events = POLLOUT|POLLERR;
  }
c->poll_state[bind_id]=3;
return 1;
}





static int generic_eprototype(network1_complete *c, int bind_id) {
network1_unconnect(c,bind_id);
return generic_toone(c,bind_id,5000);
}

static int generic_eopnotsupp(network1_complete *c, int bind_id) {
return generic_tothree(c,bind_id,100);
}

static int send_in_progress(network1_complete *c, int bind_id) {
      c->poll_state[bind_id]=4;
      c->pollfds[bind_id].fd=c->sockets[bind_id];
      c->pollfds[bind_id].events=POLLOUT|POLLERR;
return 1;
}


static int receive_in_progress(network1_complete *c, int bind_id) {

      c->poll_state[bind_id]=4;
      c->pollfds[bind_id].fd=c->sockets[bind_id];
      c->pollfds[bind_id].events=POLLIN|POLLPRI|POLLERR;
}


static int receive_eprototype(network1_complete *c, int bind_id) {
return generic_tothree(c,bind_id,500);
}

static int receive_einval(network1_complete *c, int bind_id) {
return generic_tothree(c,bind_id,500);
}

static int send_einval(network1_complete *c, int bind_id) {
return generic_tothree(c,bind_id,500);
}

static int receive_eof(network1_complete *c, int bind_id) {
return generic_tozero(c,bind_id,5000);
}

static int send_eof(network1_complete *c, int bind_id) {
return generic_tozero(c,bind_id,5000);
}

static int send_enobuffs(network1_complete *c, int bind_id) {
return generic_tothree(c,bind_id,50);
}

static int send_ealready(network1_complete *c, int bind_id) {
return generic_tothree(c,bind_id,50);
}

static int receive_gotit(network1_complete *c, int bind_id,int result) {
  c->buflen[bind_id]=result;
  c->call_rounds[bind_id]=1;
  c->poll_state[bind_id]=5;
return 1;
}


static int send_gotit(network1_complete *c, int bind_id) {
c->poll_state[bind_id]=5;
c->call_rounds[bind_id]=1;
return 1;
}

static int receive_eintr(network1_complete *c, int bind_id) {
      c->pollfds[bind_id].fd = -1;
      c->poll_state[bind_id]=3;
      network1_delay_for_milliseconds(c,bind_id,50);
}






// in mode 2	  	  
static int network1_handle_connect_error(network1_complete *c, int bind_id,int e) {
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
    generic_eacces(c,bind_id);
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
     generic_econnrefused(c,bind_id);
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
    generic_eacces(c,bind_id);
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
  





static int network1_handle_receive_error(network1_complete *c,int bind_id,int e) {
//int result = recv(c->sockets[bind_id],&(c->buffers[bind_id]),NETWORK1_MAX_BUFFER_SIZE,MSG_DONTWAIT);
if (e==0) {  // end of file
  receive_eof(c,bind_id);
  }
else {    
  int e = errno;
  if (e==0) { // end of file
    receive_eof(c,bind_id);
    }
  else 	{		    
    if ((e==EAGAIN)||(e==EWOULDBLOCK)||(e==EINPROGRESS)) {
      receive_in_progress(c,bind_id);
      }   
    else if (e==EBADF) {
      fprintf(stderr,"   EBADF  %d id %d port %d\n",c->sockets[bind_id],bind_id,c->ports[bind_id]);
      generic_ebadf(c,bind_id);
      }
    else if (e==ECONNREFUSED) {
      fprintf(stderr,"   ECONNREFUSED  %d id %d port %d\n",c->sockets[bind_id],bind_id,c->ports[bind_id]);
      generic_econnrefused(c,bind_id);
      }
    else if (e==EFAULT) {
      fprintf(stderr,"   EFAULT  %d id %d port %d\n",c->sockets[bind_id],bind_id,c->ports[bind_id]);
      generic_efault(c,bind_id);
      }
    else if (e==EINTR) {
      fprintf(stderr,"   EINTR  %d id %d port %d\n",c->sockets[bind_id],bind_id,c->ports[bind_id]);
//      close (c->sockets[bind_id]);
      receive_eintr(c,bind_id);
      }
    else if (e==EINVAL) {
      fprintf(stderr,"   EINVAL  %d id %d port %d\n",c->sockets[bind_id],bind_id,c->ports[bind_id]);
      receive_einval(c,bind_id);
      
 //     close (c->sockets[bind_id]);
 //     c->sockets[bind_id]=-1;
 //     c->pollfds[bind_id].fd = -1;
 //     c->poll_state[bind_id]=0;
 //     network1_delay_for_milliseconds(c,bind_id,1000);
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
    else {
      fprintf(stderr,"  recva generic errno  %d id %d port %d\n",c->sockets[bind_id],bind_id,c->ports[bind_id]);
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





static int network1_attempt_receive(network1_complete *c,int bind_id) {
c->poll_state[bind_id]=4;  // waiting for read
int result = recv(c->sockets[bind_id],&(c->buffers[bind_id]),NETWORK1_MAX_BUFFER_SIZE,MSG_DONTWAIT);
if (result==0) {  // end of file
  receive_eof(c,bind_id);
  }
else if (result >0) {
  receive_gotit(c,bind_id,result);
  }
else {    
  int e = errno;
  if (e==0) { // end of file
    receive_eof(c,bind_id);
    }
  else 	{		    
    if ((e==EAGAIN)||(e==EWOULDBLOCK)||(e==EINPROGRESS)) {
      receive_in_progress(c,bind_id);
      }   
    else if (e==EBADF) {
      fprintf(stderr,"   EBADF  %d id %d port %d\n",c->sockets[bind_id],bind_id,c->ports[bind_id]);
      generic_ebadf(c,bind_id);
      }
    else if (e==ECONNREFUSED) {
      fprintf(stderr,"   ECONNREFUSED  %d id %d port %d\n",c->sockets[bind_id],bind_id,c->ports[bind_id]);
      generic_econnrefused(c,bind_id);
      }
    else if (e==EFAULT) {
      fprintf(stderr,"   EFAULT  %d id %d port %d\n",c->sockets[bind_id],bind_id,c->ports[bind_id]);
      generic_efault(c,bind_id);
      }
    else if (e==EINTR) {
      fprintf(stderr,"   EINTR  %d id %d port %d\n",c->sockets[bind_id],bind_id,c->ports[bind_id]);
//      close (c->sockets[bind_id]);
      receive_eintr(c,bind_id);
      }
    else if (e==EINVAL) {
      fprintf(stderr,"   EINVAL  %d id %d port %d\n",c->sockets[bind_id],bind_id,c->ports[bind_id]);
      receive_einval(c,bind_id);
      
 //     close (c->sockets[bind_id]);
 //     c->sockets[bind_id]=-1;
 //     c->pollfds[bind_id].fd = -1;
 //     c->poll_state[bind_id]=0;
 //     network1_delay_for_milliseconds(c,bind_id,1000);
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
    else {
      fprintf(stderr,"  recvb generic errno  %d id %d port %d\n",c->sockets[bind_id],bind_id,c->ports[bind_id]);
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
















static int network1_handle_send_error(network1_complete *c,int bind_id,int e) {
//c->poll_state[bind_id]=4;  // waiting for read
//int result = send(c->sockets[bind_id],&(c->buffers[bind_id][0]),NETWORK1_MAX_BUFFER_SIZE,MSG_DONTWAIT);
if (e==0) {  // end of file
  send_eof(c,bind_id);
  }
else {    
  if (e==0) { // end of file
    send_eof(c,bind_id);
    }
  else 	{		    
    if ((e==EAGAIN)||(e==EWOULDBLOCK)||(e==EINPROGRESS)) {
      send_in_progress(c,bind_id);
      }   
    else if (e==EACCES) {
      fprintf(stderr,"   EBADF  %d id %d port %d\n",c->sockets[bind_id],bind_id,c->ports[bind_id]);
      send_eacces(c,bind_id);
      }
    else if (e==EALREADY) {
      fprintf(stderr,"   EALREADY  %d id %d port %d\n",c->sockets[bind_id],bind_id,c->ports[bind_id]);
      send_ealready(c,bind_id);
      }
    else if (e==EBADF) {
      fprintf(stderr,"   EBADF  %d id %d port %d\n",c->sockets[bind_id],bind_id,c->ports[bind_id]);
      generic_ebadf(c,bind_id);
      }
    else if (e==ECONNREFUSED) {
      fprintf(stderr,"   ECONNREFUSED  %d id %d port %d\n",c->sockets[bind_id],bind_id,c->ports[bind_id]);
      send_econnrefused(c,bind_id);
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
      send_eintr(c,bind_id);
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
      }
    else if (e==ENOTCONN) {
      fprintf(stderr,"warning ENOTCONN  %d id %d port %d\n",c->sockets[bind_id],bind_id,c->ports[bind_id]);
      generic_enotconn(c,bind_id);
      }
    else if (e==ENOTSOCK) {
      fprintf(stderr,"  ENOTSOCK  %d id %d port %d\n",c->sockets[bind_id],bind_id,c->ports[bind_id]);
      generic_enotsock(c,bind_id);
      }
    else if (e==EOPNOTSUPP) {
      fprintf(stderr,"  EOPNOTSUPP  %d id %d port %d\n",c->sockets[bind_id],bind_id,c->ports[bind_id]);
      generic_eopnotsupp(c,bind_id);
      }
    else if (e==EPIPE) {
      fprintf(stderr,"  epipe  %d id %d port %d\n",c->sockets[bind_id],bind_id,c->ports[bind_id]);
      send_epipe(c,bind_id);
      }
    else {
      fprintf(stderr,"  send generic errno  %d id %d port %d\n",c->sockets[bind_id],bind_id,c->ports[bind_id]);
      generic_eother(c,bind_id);
      }
    }  // if e is not 0
  } // if result is negative

return (0);
}  // handke)Send_error
		  
			

            
	    

static int network1_attempt_send(network1_complete *c,int bind_id) {
fprintf(stderr,"Attempt to send id %d %s\n",bind_id,c->buffers[bind_id]);
int e=0;
c->poll_state[bind_id]=4;  // waiting for read
int result = send(c->sockets[bind_id],&(c->buffers[bind_id][0]),NETWORK1_MAX_BUFFER_SIZE,MSG_DONTWAIT|MSG_CONFIRM);
if (result==0) {  // end of file
  send_eof(c,bind_id);
  }
else if (result >0) {
  fprintf(stderr,"gotit1\n");
  send_gotit(c,bind_id);
  }
else {    
  e = errno;
  if (e==0) { // end of file
    send_eof(c,bind_id);
    }
  else 	{
    fprintf(stderr, "got erroe %d    EAGAIN %d\n",e,EAGAIN);	    
    if ((e==EAGAIN)||(e==EWOULDBLOCK)||(e==EINPROGRESS)) {
      send_in_progress(c,bind_id);
      fprintf(stderr,"we are in progress yo\n");
      }   
    else if (e==EACCES) {
      fprintf(stderr,"   EBADF  %d id %d port %d\n",c->sockets[bind_id],bind_id,c->ports[bind_id]);
      send_eacces(c,bind_id);
      }
    else if (e==EALREADY) {
      fprintf(stderr,"   EALREADY  %d id %d port %d\n",c->sockets[bind_id],bind_id,c->ports[bind_id]);
      send_ealready(c,bind_id);
      }
    else if (e==EBADF) {
      fprintf(stderr,"   EBADF  %d id %d port %d\n",c->sockets[bind_id],bind_id,c->ports[bind_id]);
      generic_ebadf(c,bind_id);
      }      
    else if (e==ECONNREFUSED) {
      fprintf(stderr,"   ECONNREFUSED  %d id %d port %d\n",c->sockets[bind_id],bind_id,c->ports[bind_id]);
      send_econnrefused(c,bind_id);
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
      send_eintr(c,bind_id);
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
      }
    else if (e==ENOTCONN) {
      fprintf(stderr,"warning ENOTCONN  %d id %d port %d\n",c->sockets[bind_id],bind_id,c->ports[bind_id]);
      generic_enotconn(c,bind_id);
      }
    else if (e==ENOTSOCK) {
      fprintf(stderr,"  ENOTSOCK  %d id %d port %d\n",c->sockets[bind_id],bind_id,c->ports[bind_id]);
      generic_enotsock(c,bind_id);
      }
    else if (e==EOPNOTSUPP) {
      fprintf(stderr,"  EOPNOTSUPP  %d id %d port %d\n",c->sockets[bind_id],bind_id,c->ports[bind_id]);
      generic_eopnotsupp(c,bind_id);
      }
    else if (e==EPIPE) {
      fprintf(stderr,"  epipe  %d id %d port %d\n",c->sockets[bind_id],bind_id,c->ports[bind_id]);
      send_epipe(c,bind_id);
      }
    else {
      fprintf(stderr,"  sendb generic errno  %d socket %d id %d port %d\n",e,c->sockets[bind_id],bind_id,c->ports[bind_id]);
      generic_eother(c,bind_id);
      }
    }  // if e is not 0
  } // if result is negative
fprintf(stderr," e is %d state is %d buffer is %lx bufken is %d\n",  e,c->poll_state[bind_id],(unsigned long)(&c->buffers[bind_id][0]),c->buflen[bind_id]);
return (0);
}  // attempt_Send     
		  
			
			            
	
/* this might do nothing if we dont have buffers to receive */	    
static int network1_set_buffer_if_necessary_and_receive_from_poll(network1_complete *c,int bind_id) {
// assume state is 3 or 5
// assume i is not player number
// assume there is no timer
if ((c->poll_state[bind_id])==5) {
  fprintf(stderr,"5 wipeout  thtrwrwrw \n");
  c->poll_state[bind_id] = 3;
  c->buflen[bind_id]=0;
  c->buffers[bind_id]=NULL;
  }
  
if (c->buffers[bind_id]) {
  if (c->buflen[bind_id]) {
    if (c->network1_get_new_receive_buffer) {
      (*c->network1_get_new_receive_buffer)(c,bind_id,0);
      }
    }
  }
else {
  if (c->network1_get_new_receive_buffer) {
    (*c->network1_get_new_receive_buffer)(c,bind_id,0);
      }
  }
  
  
  
if (c->buffers[bind_id]) {
  if (!(c->buflen[bind_id])) {
    return (network1_attempt_receive(c,bind_id));
    }
  }  	
return 0;
}



// Both:	      
//  if the state is 5 - you can do it, but the state will turn to 3.
// if the state is 4 - you cannot do it
// if the state is 3, 2, or 1, you can do it , unless it is NULL
// normally you do not need to call this because the buffers are asked for
int network1_set_buffer(network1_complete *c,int bind_id,char *buffer, int buflen,int force_flag) 
{
if (c->poll_state[bind_id] == 4) {
  return 4;
  }
if (!force_flag) {
  if ((c->poll_state[bind_id] == 3)&&(c->buflen[bind_id]=!0)&&(c->buffers[bind_id]==NULL)) { 
    return 3;
    }
  if ((c->poll_state[bind_id] == 2)&&(c->buflen[bind_id]=!0)&&(c->buffers[bind_id]==NULL)) {
    return 2;
    }
  if ((c->poll_state[bind_id] == 1)&&(c->buflen[bind_id]=!0)&&(c->buffers[bind_id]==NULL)) {
    return 1;
    }
  if ((c->poll_state[bind_id] == 0)&&(c->buflen[bind_id]=!0)&&(c->buffers[bind_id]==NULL)) {
    return 1;
    }
  }
c->buflen[bind_id]=0;
c->buffers[bind_id]=buffer;
c->buflen[bind_id] = buflen;
return 0;
}  
  


				      
static int network1_send_if_can(network1_complete *c,int bind_id) {
// assume poll state is 3, or 5.  If 5, well, we reset the buffer
//assume i is not player number
// assume thers is no timer


if (c->buffers[bind_id]) {
  if (c->buflen[bind_id]) {
    return network1_attempt_send(c,bind_id);
    }
  else {
    if (c->network1_pull_next_send_buffer_from_queue[bind_id]) {
      (*c->network1_pull_next_send_buffer_from_queue[bind_id])(c,bind_id,0);
      }
    }
  }
else {
  if (c->network1_pull_next_send_buffer_from_queue[bind_id]) {
    (*c->network1_pull_next_send_buffer_from_queue[bind_id])(c,bind_id,0);
    }
  }
  
if ((c->buffers[bind_id]==NULL)&&(c->buflen[bind_id])) {
    return network1_attempt_send(c,bind_id);
    }
  else {
    if (c->buffers[bind_id]==NULL) {
    if (c->network1_get_new_send_buffer[bind_id]) {
      (*c->network1_get_new_send_buffer[bind_id])(c,bind_id,0);
      }
    }
  if (c->buffers[bind_id]) {
    if (c->buflen[bind_id]) {
      return network1_attempt_send(c,bind_id);
      }
    }
  
  }


return 0;
}
  


																      
																																      
																																																      
																																																																      
																																																																																      
																																																																																																      
																																																																																																																																      
																																																																																																																																																      
																																																																																																																																																																      
																																																																																																																																																																																      
static int handle_three(network1_complete *c,int i,struct timeval thedate,int first_time) {

if (c->communicator[i]==c->participant_number) {  
  return 1;
  }
int communicator = c->communicator[i];


if (c->poll_state[i]==3) {
  {  // block
        struct timeval a,b;
        b = thedate;
        a = c->local_delay_work[i];
        if ((a.tv_sec > b.tv_sec) || (
          (a.tv_sec == b.tv_sec) && (a.tv_usec  >= b.tv_usec))) { // we are delayed. leave as is
// maybe     c->pollfd.fd = -1;
          fprintf(stderr,"%ld %ld   to %ld %6ld rcontinue\n",a.tv_sec,a.tv_usec,b.tv_sec,b.tv_usec);
          if (i<NUMBER_OF_NETWORK1_PARTICIPANTS) {
            c->sendable[communicator] = 0;
	    }
          else {
            c->recieveable[communicator] = 0;
            }
	  return 1;
	  }
    } // the 3 block 
  if (!first_time) {    
    if (c->call_rounds[i])    {return 1;} // dont do it
    }   
  if (i<NUMBER_OF_NETWORK1_PARTICIPANTS) {
    if (c->network1_get_new_receive_buffer[i]) {
      network1_set_buffer_if_necessary_and_receive_from_poll(c,i);
      }
    }
  else if (i<NUMBER_OF_NETWORK1_PARTICIPANTS_TIMES_2) {
    if ((c->network1_get_new_send_buffer[i])||(c->network1_pull_next_send_buffer_from_queue[i])) {
      network1_send_if_can(c,i);
      }
    }
  // now we ran, if there is no buffer, but I thinkg its still sendable
      
  if (c->poll_state[i]==5) { // we are good.
    c->call_rounds[i]=1;    
    }
  else if (c->poll_state[i]>=6) {  /* oh no- 6 needs to go to 7, etc */
    c->call_rounds[i]=1;    // to see if it is good or bad - we might default it to 3
    }
    
  if ((c->poll_state[i]==4)) {
    c->pollfds[i].fd=c->sockets[i];
    }
      
  if((c->poll_state[i]<3)||(c->poll_state[i]>4)) {
    if (c->poll_state[i] > 4) {
      c->call_rounds[i]=1; // call without even setting up the poll
      }  
    }	 // if state 3 moves - make sure we call the triggers  
      	
  {
        struct timeval a,b;
        b = thedate;
        a = c->local_delay_work[i];
        if ((a.tv_sec > b.tv_sec) || (
          (a.tv_sec == b.tv_sec) && (a.tv_usec  >= b.tv_usec))) { // we are delayed. leave as is
// maybe     c->pollfd.fd = -1;
          fprintf(stderr,"%ld %ld   to %ld %6ld rcontinue\n",a.tv_sec,a.tv_usec,b.tv_sec,b.tv_usec);
          if (i<NUMBER_OF_NETWORK1_PARTICIPANTS) {
            c->sendable[communicator] = 0;
	    }
          else {
            c->recieveable[communicator] = 0;
            }
	  return(1); // 1 means continue
          }
//        fprintf(stderr,"%ld %ld   to %ld %6ld clear\n",a.tv_sec,a.tv_usec,b.tv_sec,b.tv_usec);
    }	    	    
  if ((c->poll_state[i]==2)||(c->poll_state[i]==4)) {
    c->pollfds[i].fd=c->sockets[i];
    }
  else {
    c->pollfds[i].fd=-1; // dont poll 
    }   
  }
return(0);     
}

														
																																																																																																																																																																																																													      
																																																																																																																																																																																																																																																																																																																																																																																																												      
																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																											      
																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																										      
																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																									      
																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																								      
																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																							      
static void format_difference_date(char *s,struct timeval a,struct timeval b) {
long au = a.tv_usec - b.tv_usec;
long as = a.tv_sec - b.tv_sec;
long am = as *1000 + (au/1000);
sprintf(s,"%4ld",am);
}
																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																						      

static int compute_sendable_recieveable(network1_complete *c,int cm,int nothing) {
int i=cm;
  if ((c->poll_state[c->sending_poll[i]]) >=6) {
    c->sendable[i]=0;
    } 
  if ((c->poll_state[c->receiving_poll[i]]) >=6) {
    c->recieveable[i]=0;
    } 
  c->responsive[i] = c->sendable[i]  & c->recieveable[i];
  if ((c->poll_state[c->sending_poll[i]]) >=7) {
    c->countdown[i]=0;
    } 
  if ((c->poll_state[c->receiving_poll[i]]) >=7) {
    c->countdown[i]=0;
    } 
  if( (c->poll_state[c->sending_poll[i]]) >=8) {
    c->alive[i]=0;
    } 
  if ((c->poll_state[c->receiving_poll[i]]) >=8) {
    c->alive[i]=0;
    } 
  
return 0;  
}





static int compute_sendable_recieveables(network1_complete *c,int nothing) {
for (int cm=0;cm<6;cm++) {
  compute_sendable_recieveable(c,cm,0);
  }
return 0;
}






static int go_around_timeouts(network1_complete *c,int cm, struct timeval nowtime)  {
int ii= c->receiving_poll[cm];
int oo= c->sending_poll[cm];
//if( (c->poll_state[oo]==5)||(c->poll_state[oo]==2)|| (c->poll_state[oo]==4)) {
if (c->poll_state[oo]==5) {
  c->local_delay_work[oo] =  nowtime;
  c->local_delay_work[ii] =  nowtime;
  }
else if(c->poll_state[ii]==5) {
  c->local_delay_work[oo] =  nowtime;
  c->local_delay_work[ii] =  nowtime;
  }
}    
     
      



static int process_poll_buffer_status(network1_complete *c,int cm,int compute_statistics_at_end) {
  int communicator =cm;
  int ii=c->receiving_poll[communicator];

      
  int oo=c->sending_poll[communicator];
  if (c->buffers[ii]) {
    if (c->buflen[ii]) {
      c->recv_buffer_full[communicator] = 1;
      c->recv_buffer_missing[communicator] = 0;
      c->recv_buffer_ready[communicator] = 0;
      }
    else {
      c->recv_buffer_full[communicator] = 0;
      c->recv_buffer_missing[communicator] = 0;
      c->recv_buffer_ready[communicator] = 1;
      }
    }
  else {
    c->recv_buffer_full[communicator] = 0;
    c->recv_buffer_missing[communicator] = 1;
    c->recv_buffer_ready[communicator] = 0;
    }
  if (compute_statistics_at_end) {
    if (c->recv_buffer_full[communicator]) {
      c->recieveable[communicator]=0;
      }
    if (c->poll_state[oo]==4) { // if we are state 4
      c->recieveable[communicator]=1;
      }
    else {
      if (!c->recv_buffer_missing[communicator]) {
        c->recieveable[communicator]=0;
        }
      }
    } 
    
    

  if (c->buffers[oo]) {
    if (c->buflen[oo]) {
      c->send_buffer_full[communicator]=1;
      c->send_buffer_ready[communicator]=0;
      c->send_buffer_missing[communicator]=0;
      }
    else {
      c->send_buffer_full[communicator]=0;
      c->send_buffer_ready[communicator]=1;
      c->send_buffer_missing[communicator]=0;
      }
    }
  else {
    c->send_buffer_full[communicator]=0;
    c->send_buffer_ready[communicator]=0;
    c->send_buffer_missing[communicator]=1;
    }
  
  if (compute_statistics_at_end) {
    if (c->poll_state[oo]==4)  {
      if (!c->send_buffer_full[communicator]) {
        c->sendable[communicator]=0;
	}
      }
    else if (c->poll_state[oo]==3)  {
      if (c->send_buffer_missing[communicator]) {
        c->sendable[communicator]=0;
	}
      }
    }

return 0;
}


static int process_poll_buffer_statuses(network1_complete *c,int compute_statistics_at_end) {
for (int i=0;i<6;i++) {
   process_poll_buffer_status(c,i,compute_statistics_at_end);
  }
return 0;
}

static int network1_reset_sendable_and_stuff(network1_complete *c,int i) {
  c->sendable[i]=1;
  c->recieveable[i]=1;
  c->responsive[i]=1;
  c->countdown[i]=1;
  c->alive[i]=1;
  return 0;
}

/*jj*/
static int network1_reset_sendables_and_stuff(network1_complete *c) {
for (int i=0;i<NUMBER_OF_NETWORK1_PARTICIPANTS;i++) {
  c->sendable[i]=1;
  c->recieveable[i]=1;
  c->responsive[i]=1;
  c->countdown[i]=1;
  c->alive[i]=1;
  }
  return 0;
}



int network1_poll_check(network1_complete *c) {

gettimeofday(&(c->local_check_start_time),NULL);  // used for the offset

c->network1_check_poll_runs_in_call = 0;



sigset_t set;
int s;
struct timeval ts;
ts.tv_sec=0;
ts.tv_usec=0;
/* from man pthread_sigmask abd poll */
sigemptyset(&set);
sigaddset(&set, SIGQUIT);
sigaddset(&set, SIGUSR1);

gettimeofday(&(c->local_poll_predo_start_time[c->network1_check_poll_runs_in_call]),NULL);

// call_rounds is used to decide wheteher to call the round1-round3 to externally process messages
for (int i=0;i<c->current_number_of_polls;i++) {
  c->call_rounds[i]=0;
  }
  


network1_reset_sendables_and_stuff(c);

/* clear out revents, and turn off write polls if not necessary. read polls are necessary */
for (int i=0;i<c->current_number_of_polls;i++) {
  int communicator =c-> communicator[i];
  c->pollfds[i].fd=-1; // dont poll as default 
  if (c->communicator[i]==c->participant_number) {
    continue;
    }
    
  go_around_timeouts(c,communicator,c->local_poll_predo_start_time[c->network1_check_poll_runs_in_call]);
      
    
  {
    struct timeval a,b;
    b = c->local_poll_predo_start_time[c->network1_check_poll_runs_in_call];
    a = c->local_delay_work[i];
    if ((a.tv_sec > b.tv_sec) || (
      (a.tv_sec == b.tv_sec) && (a.tv_usec  >= b.tv_usec))) { // we are delayed. leave as is
// maybe       c->pollfd.fd = -1;
      fprintf(stderr,"%ld %ld   to %ld %6ld continue\n",a.tv_sec,a.tv_usec,b.tv_sec,b.tv_usec);
      if (i<NUMBER_OF_NETWORK1_PARTICIPANTS) {
        c->recieveable[communicator] = 0;
	}
      else {
        c->sendable[communicator] = 0;
        }
       continue;
       }
//      fprintf(stderr,"%ld %ld   to %ld %6ld clear\n",a.tv_sec,a.tv_usec,b.tv_sec,b.tv_usec);
     }
  
  if (c->poll_state[i]==0) {
    network1_setup_and_bind(c,i);
    }
  if (c->poll_state[i]==1) {
    network1_connect_to_outside_poll(c,i);
    }
  if (c->poll_state[i]==2) {
    c->pollfds[i].fd=c->sockets[i]; // we will poll the connection -- if we are done with waiting . might go to 3
    }
    
    
  if (c->poll_state[i]==5) {  // this was actioned earlier, and we got past the wait time
    if (i<NUMBER_OF_NETWORK1_PARTICIPANTS) {  
      if (c->network1_get_new_receive_buffer[i]) {  // if receivers are dynamic
        c->buffers[i]=NULL; // we are done. wipe it out
	c->buflen[i]=0;
        }
      else { // receivers are static 
	c->buflen[i]=0;  // we are done. just reuse the buffer
	}
      } // receiving
    else { // sending
      if ((c->network1_get_new_send_buffer[i])
          || (c->network1_pull_next_send_buffer_from_queue[i])) {  // if senders are dynamic
        c->buffers[i]=NULL; // we are done
	c->buflen[i]=0;
        }    
      else {
	c->buflen[i]=0; // keep the buffer around so we can reuse it
	} // static
      } // sending
    c->poll_state[i]=3;
    }
  if (c->poll_state[i]>=6) {  // we are keeping the buffer as is - still need to send it
    c->poll_state[i]=3;
    }    

  if ((c->poll_state[i]==4)) {
    c->pollfds[i].fd=c->sockets[i];
    }

  if (handle_three(c,i, c->local_poll_predo_start_time[c->network1_check_poll_runs_in_call],1)) {
    continue;
    }     

  /* now clean revents */
  c->pollfds[i].revents=0;
  
  }  // our clean up and set  for polling


compute_sendable_recieveables(c,1); 
process_poll_buffer_statuses(c,0);  
  

  
  
  
  gettimeofday(&(c->local_poll_predo_end_time[c->network1_check_poll_runs_in_call]),NULL); 





int number_of_events = poll(&(c->pollfds[0]), c->current_number_of_polls,0);		  



gettimeofday(&(c->local_poll_end_time[c->network1_check_poll_runs_in_call]),NULL); 



int number_to_round=0;
for (int i=0;i<c->current_number_of_polls;i++) {
  if (c->call_rounds[i]) number_to_round++;
  }
compute_sendable_recieveables(c,0);  
process_poll_buffer_statuses(c,1);  
  

if (!(number_of_events+number_to_round)) {
  c->local_handle_end_time[c->network1_check_poll_runs_in_call]=		c->local_poll_end_time[c->network1_check_poll_runs_in_call];
  c->local_round1_end_time[c->network1_check_poll_runs_in_call]=		c->local_poll_end_time[c->network1_check_poll_runs_in_call];
  c->local_round2_end_time[c->network1_check_poll_runs_in_call]=		c->local_poll_end_time[c->network1_check_poll_runs_in_call];
  c->local_round3_end_time[c->network1_check_poll_runs_in_call]=		c->local_poll_end_time[c->network1_check_poll_runs_in_call];
  c->local_postdo_end_time[c->network1_check_poll_runs_in_call]=		c->local_poll_end_time[c->network1_check_poll_runs_in_call];
  c->local_network1_check_end_time=					        c->local_poll_end_time[c->network1_check_poll_runs_in_call];
  
  c->network1_check_poll_runs_in_call++;
  goto full_check_poll_loop;
  }


// OK - so we are handling the polls
// I am not going to speed up the delays here

// and we can use the sendable and recieveable stuff to do less.
{ // Block to handle the polls 
  for (int i=0;i<c->current_number_of_polls;i++) {
   
  
    if (c->poll_state[i]==2) { // doing a connect and waaiting for POLLOUT
      int e=0;
      if (c->pollfds[i].revents&(POLLOUT|POLLERR)) {
        e = 0;
	socklen_t lenn=sizeof(int);
        getsockopt(c->sockets[i],SOL_SOCKET,SO_ERROR,(void *)&e,&lenn);
        c->pollfds[i].fd=-1;       
        network1_handle_connect_error(c,i,e);
      

        if (handle_three(c,i, c->local_poll_predo_start_time[c->network1_check_poll_runs_in_call],1)) {
           continue;
          }     
        } /* if we are ca connect and got a return value  on state 2 */ 
      }  /* if state=2 , connection possibly made */
    else if (c->poll_state[i]==4) { // doing a send or receive 
      if (i<NUMBER_OF_NETWORK1_PARTICIPANTS) {
        int e=0;
        if (c->pollfds[i].revents&(POLLERR)) {
          e = 0;
	  socklen_t lenn=4;
          getsockopt(c->sockets[i],SOL_SOCKET,SO_ERROR,(void *)&e,&lenn);
          c->pollfds[i].fd=-1;       
          network1_handle_receive_error(c,i,e);
          }
        else if (c->pollfds[i].revents&(POLLIN|POLLPRI)) {
          network1_attempt_receive(c,i);
	  }
	} // if input
      else { //  (i>=NUMBER_OF_NETWORK1_PARTICIPANTS)  , output
        int e=0;
        if (c->pollfds[i].revents&(POLLERR)) {
          e = 0;
	  socklen_t lenn=4;
          getsockopt(c->sockets[i],SOL_SOCKET,SO_ERROR,(void *)&e,&lenn);
          c->pollfds[i].fd=-1;       
          network1_handle_send_error(c,i,e);
          }
        else if (c->pollfds[i].revents&POLLOUT) {
//	  network1_attempt_send(c,i);
fprintf(stderr,"githtotr5r\n");
          send_gotit(c,i);
//        c->call_rounds[bind_id]=1;
//	  c->poll_state[bind_id]=5;
	  }
	} // if output
	
      } // if state is 4
    i = i + 1;
    } // while going through all events
  }     /* special block to handle connect polling */
  

// note - this above block could reduce the nuber of events that passed, but will never increase it 
// so we can call start_round/stop/round anyways - sometimes witouth any other events

// recompute number_to_round for round 1
number_to_round=0;
for (int i=0;i<c->current_number_of_polls;i++) {
  if (c->call_rounds[i]) number_to_round++;
  }

gettimeofday(&(c->local_handle_end_time[c->network1_check_poll_runs_in_call]),NULL);

process_poll_buffer_statuses(c,0);  

if (!number_to_round) {
  c->local_round1_end_time[c->network1_check_poll_runs_in_call]=		c->local_handle_end_time[c->network1_check_poll_runs_in_call];
  c->local_round2_end_time[c->network1_check_poll_runs_in_call]=		c->local_handle_end_time[c->network1_check_poll_runs_in_call];
  c->local_round3_end_time[c->network1_check_poll_runs_in_call]=		c->local_handle_end_time[c->network1_check_poll_runs_in_call];
  c->local_postdo_end_time[c->network1_check_poll_runs_in_call]=		c->local_handle_end_time[c->network1_check_poll_runs_in_call];
  c->local_network1_check_end_time					=        c->local_handle_end_time[c->network1_check_poll_runs_in_call];
  
  goto full_check_poll_loop;
  return 0;
  }
  



{ // Block to process events round 1
  
  if (number_of_events &&(c->network1_action_start_round1)) {
    (*c->network1_action_start_round1)(c,number_of_events,number_to_round);
    }
    
  for (int i=0;i<c->current_number_of_polls;i++) {
    if (c->call_rounds[i]) {    
      if  (c->network1_handle_action_round1[i]) { (*c->network1_handle_action_round1[i])(c,i,number_of_events);  }
      }
    } /* while loopint through all events */
  
  if ((number_of_events) &&(c->network1_action_finish_round1)) {
    (*c->network1_action_finish_round1)(c,number_of_events,number_to_round);
    }


  } // block to process events round 1


// recompute number_to_round after round 1
number_to_round=0;
for (int i=0;i<c->current_number_of_polls;i++) {
  if (c->call_rounds[i]) number_to_round++;
  }

process_poll_buffer_statuses(c,0);  

gettimeofday(&(c->local_round1_end_time[c->network1_check_poll_runs_in_call]),NULL);

if (!number_to_round) {
  c->local_round2_end_time[c->network1_check_poll_runs_in_call]=		c->local_round1_end_time[c->network1_check_poll_runs_in_call];
  c->local_round3_end_time[c->network1_check_poll_runs_in_call]=		c->local_round1_end_time[c->network1_check_poll_runs_in_call];
  c->local_postdo_end_time[c->network1_check_poll_runs_in_call]=		c->local_round1_end_time[c->network1_check_poll_runs_in_call];
  c->local_network1_check_end_time			=                       c->local_round1_end_time[c->network1_check_poll_runs_in_call];
  
  c->network1_check_poll_runs_in_call++;
  goto full_check_poll_loop;
  }
  


{ // Block to process events round 2
  
  if (number_of_events &&(c->network1_action_start_round2)) {
    (*c->network1_action_start_round2)(c,number_of_events,number_to_round);
    }
    
  for (int i=0;i<c->current_number_of_polls;i++) {
    if (c->call_rounds[i]) {    
      if  (c->network1_handle_action_round2[i]) { (*c->network1_handle_action_round2[i])(c,i,number_of_events);  }
      }
    } /* while loopint through all events */
  
  if ((number_of_events) &&(c->network1_action_finish_round2)) {
    (*c->network1_action_finish_round2)(c,number_of_events,number_to_round);
    }


  } // block to process events round 2
  
  

// recompute number_to_round after round 2
number_to_round=0;
for (int i=0;i<c->current_number_of_polls;i++) {
  if (c->call_rounds[i]) number_to_round++;
  }
process_poll_buffer_statuses(c,0);  

gettimeofday(&(c->local_round2_end_time[c->network1_check_poll_runs_in_call]),NULL);

if (!number_to_round) {
  c->local_round3_end_time[c->network1_check_poll_runs_in_call]=		c->local_round2_end_time[c->network1_check_poll_runs_in_call];
  c->local_postdo_end_time[c->network1_check_poll_runs_in_call]=		c->local_round2_end_time[c->network1_check_poll_runs_in_call];
  c->local_network1_check_end_time			=                       c->local_round2_end_time[c->network1_check_poll_runs_in_call];
  
  c->network1_check_poll_runs_in_call++;
  goto full_check_poll_loop;
  }
  
  


{ // Block to process events round 3
  
  if (number_of_events &&(c->network1_action_start_round3)) {
    (*c->network1_action_start_round3)(c,number_of_events,number_to_round);
    }
    
  for (int i=0;i<c->current_number_of_polls;i++) {
    if (c->call_rounds[i]) {    
      if  (c->network1_handle_action_round3[i]) { (*c->network1_handle_action_round3[i])(c,i,number_of_events);  }
      }
    } /* while loopint through all events */
  
  if ((number_of_events) &&(c->network1_action_finish_round3)) {
    (*c->network1_action_finish_round3)(c,number_of_events,number_to_round);
    }


  } // block to process events round 3


// recompute number_to_round after round 3 - to use later
number_to_round=0;
for (int i=0;i<c->current_number_of_polls;i++) {
  if (c->call_rounds[i]) number_to_round++;
  }
process_poll_buffer_statuses(c,0);  

gettimeofday(&(c->local_round3_end_time[c->network1_check_poll_runs_in_call]),NULL);


if (!number_to_round) {
  c->local_postdo_end_time[c->network1_check_poll_runs_in_call]=		c->local_round3_end_time[c->network1_check_poll_runs_in_call];
  c->local_network1_check_end_time			=                       c->local_round3_end_time[c->network1_check_poll_runs_in_call];
  
  c->network1_check_poll_runs_in_call++;
  goto full_check_poll_loop;
  }
  

network1_reset_sendables_and_stuff(c);
  
/* Extra read all about it -- postdo  */
for (int i=0;i<c->current_number_of_polls;i++) {
  int communicator = c->communicator[i];
  
  /* since we are not looping right now, start a send or receive for any type 3's */
  if (c->communicator[i]==c->participant_number) {
    continue;
    }
     

  
  {  // maybe check the time
    struct timeval a,b;
    b = c->local_round3_end_time[c->network1_check_poll_runs_in_call];
    a = c->local_delay_work[i];
    if ((a.tv_sec > b.tv_sec) || (
      (a.tv_sec == b.tv_sec) && (a.tv_usec  >= b.tv_usec))) { // we are delayed. leave as is
// maybe       c->pollfd.fd = -1;
      fprintf(stderr,"%ld %ld   to %ld %6ld continuex\n",a.tv_sec,a.tv_usec,b.tv_sec,b.tv_usec);
          if (i<NUMBER_OF_NETWORK1_PARTICIPANTS) {
            c->sendable[communicator] = 0;
	    }
          else {
            c->recieveable[communicator] = 0;
            }
       continue;
       }
//      fprintf(stderr,"%ld %ld   to %ld %6ld clearx\n",a.tv_sec,a.tv_usec,b.tv_sec,b.tv_usec);
    }
  }   

compute_sendable_recieveables(c,1); 
process_poll_buffer_statuses(c,0);  

gettimeofday(&(c->local_postdo_end_time[c->network1_check_poll_runs_in_call]),NULL);




c->network1_check_poll_runs_in_call++;

full_check_poll_loop:
// later on we can loop around a few times to make it more efficient.
gettimeofday(&(c->local_network1_check_end_time),NULL);

{
  char ds[30];
  char dpr[30];
  char dpl[30];
  char dh[30];
  char d1[30];
  char d2[30];
  char d3[30];
  char dps[30];
  
  char dse[30];
  
  format_difference_date(dse,c->local_check_start_time,c->local_network1_check_end_time);
  format_difference_date(ds,c->local_check_start_time,c->local_poll_predo_start_time[0]);
  format_difference_date(dpr,c->local_poll_predo_start_time[0],c->local_poll_predo_end_time[0]);
  format_difference_date(dpl,c->local_poll_predo_end_time[0],c->local_poll_end_time[0]);
  format_difference_date(dh,c->local_poll_end_time[0],c->local_handle_end_time[0]);
  format_difference_date(d1,c->local_handle_end_time[0],c->local_round1_end_time[0]);
  format_difference_date(d2,c->local_round1_end_time[0],c->local_round2_end_time[0]);
  format_difference_date(d3,c->local_round2_end_time[0],c->local_round3_end_time[0]);
  format_difference_date(dps,c->local_round3_end_time[0],c->local_postdo_end_time[0]);
//  fprintf(stderr,"\n\n	Date %s 	= pre %s	pl %s	ph %s		r1 %s r2 %s	r3 %s ps %s\n",dse,dpr,dpl,dh,d1,d2,d3,dps);
  }

return number_to_round;
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

for (int i=0;i<NUMBER_OF_NETWORK1_PARTICIPANTS;i++) {
  int communicator = i;
  c->poll_state[i] = 0;
  c->communicator[i] = i;
  c->direction[i] = 1;  // will have to do that later
  c->ports[i] = NETWORK1_START_PORT + c->participant_number * NUMBER_OF_NETWORK1_PARTICIPANTS_TIMES_2  + communicator;
  c->sockets[i]=-1;
  strncpy(&(c->ip_addresses_string[i][0]),ips[i],19);
  c->ip_addresses_string[i][19]='\0';
  
  c->local_delay_work[i] = nowtime;
  
  {
    struct sockaddr_in inAddr;
    memset ((char *)(& inAddr),0, sizeof(struct sockaddr_in));
    inAddr.sin_family = AF_INET;                 /* Internet address family */
    inAddr.sin_addr.s_addr = inet_addr(ips[participant_number]); /* all inqddr go to me. address is the output address*/
//  inAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    inAddr.sin_port = htons(c->ports[i]);         /* listen to our ip address on the given port */
    c->poll_addresses[i] = inAddr;
    }

  c->sent_to_ports[communicator] =  NETWORK1_START_PORT + communicator * NUMBER_OF_NETWORK1_PARTICIPANTS_TIMES_2  +  NUMBER_OF_NETWORK1_PARTICIPANTS +  c->participant_number; // this is the port we send to from the sender to us receiver

  {
    struct sockaddr_in sendToAddr;
    memset ((char *)(& sendToAddr),0, sizeof(struct sockaddr_in));
    sendToAddr.sin_family = AF_INET;                 /* Internet address family */
    sendToAddr.sin_addr.s_addr = inet_addr(ips[communicator]); /* all inqddr go to me. address is the output address*/
//  sendToAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    sendToAddr.sin_port = htons(c->sent_to_ports[communicator]);         /* listen to our ip address on the given port */
    c->sending_to[i] = sendToAddr;  /* we filter to see if it is from the same thing */
    }

  c->receiving_poll[communicator] = i;

  c->pollfds[i] = (struct pollfd){fd:-1,events:POLLIN|POLLPRI|POLLERR,revents:0};

  c->buflen[i]= 0;
  c->buffers[i]= NULL;
    
  c->broadcast_permission[i] = 0;
  c->network1_handle_action_round1[i] = network1_handle_action_round1_in;
  c->network1_handle_action_round2[i] = network1_handle_action_round2_in;
  c->network1_handle_action_round3[i] = network1_handle_action_round3_in;
  c->network1_get_new_receive_buffer[i] = network1_get_new_receive_buffer;
  }




for (int o=NUMBER_OF_NETWORK1_PARTICIPANTS;o<NUMBER_OF_NETWORK1_PARTICIPANTS_TIMES_2;o++) {
  int communicator = o-NUMBER_OF_NETWORK1_PARTICIPANTS;
  c->poll_state[o] = 0;
  c->communicator[o] = communicator;
  c->direction[o] = 1;  // will have to do that later
  c->ports[o] = NETWORK1_START_PORT + NUMBER_OF_NETWORK1_PARTICIPANTS + c->participant_number * NUMBER_OF_NETWORK1_PARTICIPANTS_TIMES_2  + communicator;
  c->sockets[o]=-1;
//  strncpy[&(c->ip_addresses[o][0]),ips[communicator],19);
//  c->ip_addresses[o][19]='\0';

  c->sending_poll[communicator] = o;
  {
    struct sockaddr_in inAddr;
    memset ((char *)(& inAddr),0, sizeof(struct sockaddr_in));
    inAddr.sin_family = AF_INET;                 /* Internet address family */
    inAddr.sin_addr.s_addr = inet_addr(ips[participant_number]); /* all inqddr go to me. address is the output address*/
  //inAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    inAddr.sin_port = htons(c->ports[o]);         /* listen to our ip address on the given port */
    c->poll_addresses[o] = inAddr;
    }
  
  c->local_delay_work[o] = nowtime;

  c->sent_to_ports[o] =   NETWORK1_START_PORT + communicator * NUMBER_OF_NETWORK1_PARTICIPANTS_TIMES_2  +  c->participant_number;  /* this is what we semd tp from us to a receiver*/ 
  {
    struct sockaddr_in sendToAddr;
    memset ((char *)(& sendToAddr),0, sizeof(struct sockaddr_in));
    sendToAddr.sin_family = AF_INET;                 /* Internet address family */
    sendToAddr.sin_addr.s_addr = inet_addr(ips[communicator]); /* all inqddr go to me. address is the output address*/
//  sendToAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    sendToAddr.sin_port = htons(c->sent_to_ports[o]);         /* listen to our ip address on the given port */
    c->sending_to[o] = sendToAddr;                      
    }   
  
  c->pollfds[o] = (struct pollfd){fd:-1,events:POLLOUT|POLLERR,revents:0};

  c->buflen[o]= 0;
  c->buffers[o]= NULL;
    
  c->broadcast_permission[o] = 0;
  
  c->network1_handle_action_round1[o] = network1_handle_action_round1_out;
  c->network1_handle_action_round2[o] = network1_handle_action_round2_out;
  c->network1_handle_action_round3[o] = network1_handle_action_round3_out;
  c->network1_get_new_send_buffer[o] = network1_get_new_send_buffer;
  c->network1_pull_next_send_buffer_from_queue[o] = network1_pull_next_send_buffer_from_queue;
  
  
  
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
  
  }
return 1;
}


/* end of network1,c */

																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																														      																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																									
