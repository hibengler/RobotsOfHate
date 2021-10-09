// Server side implementation of UDP client-server model
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
       #include <pthread.h>
       
       #include <sys/socket.h>
              #include <netinet/in.h>
	             #include <arpa/inet.h>

#define PORT	 8080
#define MAXLINE 1024
void nsleep() {
struct timespec req;
req.tv_sec=0;
req.tv_nsec=5000000;

  nanosleep(&req,NULL);
}


volatile char server_string[2000];
volatile char client_string[2000];
char *ss=(char *)server_string;
char *cs = (char *)client_string;
int serversockfd;
char *serverip;
struct sockaddr_in  servaddr;

int clientsockfd;
char *clientip;
struct sockaddr_in  clientaddr;

void setup_serv(char *servip) {
	// Creating socket file descriptor
	if ( (serversockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
		perror("server socket creation failed");
		exit(EXIT_FAILURE);
	}
	memset(&servaddr, 0, sizeof(servaddr));
        // Filling server information
        servaddr.sin_family = AF_INET; // IPv4
        servaddr.sin_addr.s_addr = inet_addr(servip);
	servaddr.sin_port = htons(PORT);

	
	// Bind the socket with the server address
	if ( bind(serversockfd, (const struct sockaddr *)&servaddr,
			sizeof(servaddr)) < 0 )
	{
		perror("bind failed");
		exit(EXIT_FAILURE);
	}
	
}


void *serv(void * a) {
fprintf(stderr,"serv\n");
while (1) {	long n;
	n = recv(serversockfd, (char *)ss, 1999,
				MSG_WAITALL);
       if (n) {
         fprintf(stderr,"%s\n",ss);
         server_string[0]='\0';
         }
fprintf(stderr,"wait for mote\n");

  while (server_string[0]!=0) {
   nsleep(); 
   }

  }
}


void setup_client(char *clip) {

	// Creating socket file descriptor
	if ( (clientsockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
		perror("socket creation failed");
		exit(EXIT_FAILURE);
	}

	memset(&clientaddr, 0, sizeof(servaddr));
	
	// Filling server information
	clientaddr.sin_family = AF_INET;
	clientaddr.sin_addr.s_addr = inet_addr(clip);
	clientaddr.sin_port = htons(PORT);
	

}

void *clnt(void *arg) {
fprintf(stderr,"clnt\n");
   while (1) {
	int n, len;
	if (client_string[0]) {
  	  sendto(clientsockfd, (const char *)cs, strlen(cs),
		MSG_CONFIRM, (const struct sockaddr *) &clientaddr,
			sizeof(servaddr));
		
           client_string[0]=0;
           }
fprintf(stderr,"client sleep\n");
      while (client_string[0]==0) {
        nsleep();
        }
fprintf(stderr,"clientup\n");
  }
}
				
	
// Driver code
int main(int argc,char **argv) { // me other
setup_client(argv[1]);
setup_serv("0.0.0.0");
char buffer[2000];
buffer[0]=0;
pthread_t t1,t2;

pthread_create(&t1,NULL,serv,NULL);
pthread_create(&t2,NULL,clnt,NULL);
long h;
while (h=fread(buffer,1,1999,stdin)) {
  if (h<0) continue;
 buffer[h]=0;
  fprintf(stderr,"semdomg...\n");
  strcpy(cs,buffer);
  client_string[0]=0;
  client_string[0] = buffer[0];

  while (client_string[0]) {
   nsleep();
   }
  buffer[0]='\0';
 fprintf(stderr,"reading again\n");
 }
	return 0;
}


