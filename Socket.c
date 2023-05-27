#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>

/* Socket API headers */
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


/* Definitions */
#define DEFAULT_BUFLEN 512
#define PORT xxxx

              #define STRECOM(a,b, c)     (strncmp(a, b, c) == 0)
	          #define BUF_SIZE        4096
int main(int argc, char *argv[])
{
	int server, client;
	struct sockaddr_in local_addr;
	struct sockaddr_in remote_addr;
	int length,fd,optval;

	char *folder=NULL;
	char *password=NULL;
	int cport;
	int rcnt;


	if(argc < 3){
		fprintf(stderr, "Usage: %s <hostname> <port>\n", argv[0]);
		exit(1); 
	}

	int c;

	while ((c = getopt (argc, argv, "d:u:p:")) != -1)
    switch (c)
    {
        case 'd':
            folder = optarg;
            break;
        case 'u':
            password = optarg;
				//printf("bonsoir ");
            break;
		case 'p':
            cport =atoi(optarg);
            break;

        case '?':
            fprintf(stderr,"Unrecognized option: -%c\n", optopt);
            return 1;
        default:
            abort ();
    }


/* Open socket descriptor */
if ( (server = socket( AF_INET, SOCK_STREAM, 0 )) < 0) {
    perror("Can't create socket!");
    return(1);
}

/* Fill local and remote address structure with zero */
memset( &local_addr, 0, sizeof(local_addr) );
memset( &remote_addr, 0, sizeof(remote_addr) );

/* Set values to local_addr structure */
local_addr.sin_family = AF_INET;
local_addr.sin_addr.s_addr = htonl(INADDR_ANY);
local_addr.sin_port = htons(cport);

// set SO_REUSEADDR on a socket to true (1):
optval = 1;
setsockopt(server, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof optval);


if ( bind( server, (struct sockaddr *)&local_addr, sizeof(local_addr) ) < 0 )
{
    /* could not start server */
    perror("Bind problem!");
    return(1);
}

if ( listen( server, SOMAXCONN ) < 0 ) {
        perror("Listen");
        exit(1);
}

printf("Concurrent socket server now starting!\n");
printf("Wait for connection\n");

while(1) {  // main accept() loop
    length = sizeof remote_addr;
    if ((fd = accept(server, (struct sockaddr *)&remote_addr, \
          &length)) == -1) {
          perror("Accept Problem!");
          continue;
    }

    printf("Server: got connection from %s\n", \
            inet_ntoa(remote_addr.sin_addr));
			
			

           char buffer[BUF_SIZE], bufferout[BUF_SIZE];
           int logged = 0;
	       char recvbuf[DEFAULT_BUFLEN],bmsg[DEFAULT_BUFLEN];
	       int  recvbuflen = DEFAULT_BUFLEN;
	
     // Receive until the peer shuts down the coonection
    do {
        // Clear Receive buffer
        memset( &recvbuf, '\0', sizeof(recvbuf) );
        rcnt = recv(fd, recvbuf, recvbuflen, 0);
        if (rcnt > 0) {
            printf("Bytes received: %d\n", rcnt);

        // Echo the buffer back to the sender
        rcnt = send( fd, recvbuf, rcnt, 0 );
            if (rcnt < 0) {
                printf("Send failed:\n");
                close(fd);
                break;
            }
            printf("Bytes sent: %d\n", rcnt);
			
			if(logged==1){
				
				if(STRECOM(recvbuf, "LIST", strlen("LIST")))
				{
					sprintf(bufferout,"200 ,Hello %s , please to meet you\n", "LIST");//
					send(fd, bufferout, strlen(bufferout), 0);
				}
				else if(STRECOM(recvbuf, "GET", strlen("GET")))
				{
					sprintf(bufferout,"200 ,Hello %s , please to meet you\n", "GET");//
					send(fd, bufferout, strlen(bufferout), 0);
				}
				else if(STRECOM(recvbuf, "DEL", strlen("DEL")))
				{
					sprintf(bufferout,"200 ,Hello %s , please to meet you\n", "DEL");//
					send(fd, bufferout, strlen(bufferout), 0);
				}
				else if(STRECOM(recvbuf, "QUIT", strlen("QUIT")))
				{
					sprintf(bufferout,"200 ,Hello %s , please to meet you\n", "QUIT");//
					send(fd, bufferout, strlen(bufferout), 0);
				}
				else{  
					send(fd, "Wrong command please start your text with: LIST, GET, DEL or QUIT\n", strlen( "Wrong command please start your text with: LIST, GET, DEL or QUIT\n"), 0);
                }      
		       else{  
				if (STRECOM(recvbuf, "USER", strlen("USER"))) { // Initial greeting
					sprintf(bufferout,"200 ,Hello %s , please to meet you\n", "USER");//
					send(fd, bufferout, strlen(bufferout), 0);
					logged=1;
				}
				else{
					send(fd, "the syntax is USER follow by your name space and passwork 'USER test password' \n", 
					strlen( "the syntax is USER follow by your name space and passwork 'USER test password' \n"), 0);
				}
			}
		
		
		
		
		
			else if (rcnt == 0)
            printf("Connection closing...\n");
        else  {
            printf("Receive failed:\n");
            close(fd);
            break;
        }
    } while (rcnt > 0);



	//system_execution(fd);

}

// Final Cleanup
close(server);

}