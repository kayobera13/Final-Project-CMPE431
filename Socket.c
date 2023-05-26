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
#define PORT xxxc


int main(int argc, char *argv[])
{
	int server, client;
	struct sockaddr_in local_addr;
	struct sockaddr_in remote_addr;
	int length,fd,optval;

	char *folder=NULL;
	char *password=NULL;
	int cport;


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