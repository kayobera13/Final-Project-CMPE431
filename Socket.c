#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
/* Socket API headers */
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


/* Definitions */
#define DEFAULT_BUFLEN 512
#define PORT xxxx



void system_execution(int fd, char* folder){
              #define STRECOM(a,b, c)     (strncmp(a, b, c) == 0)
	          #define BUF_SIZE        4096
			  
void removeSubstr (char *string, char *sub) {
          char *match;
          int len = strlen(sub);
          while ((match = strstr(string, sub))) {
           *match = '\0';
           strcat(string, match+len);
    }
}
	char buffer[BUF_SIZE], bufferout[BUF_SIZE];
	int logged = 0;
	char recvbuf[DEFAULT_BUFLEN],bmsg[DEFAULT_BUFLEN];
	int  recvbuflen = DEFAULT_BUFLEN;
	int rcnt;
	char filename[100], username[100] ;
	char folderfilename[300],  thetext[BUF_SIZE] ;
	 FILE *ofp;
	 int infile=0;

    // Receive until the peer shuts down the connection
    do {
        // Clear Receive buffer
        memset( &recvbuf, '\0', sizeof(recvbuf) );
        rcnt = recv(fd, recvbuf, recvbuflen, 0);
        if (rcnt > 0) {
                           if(STRECOM(recvbuf, "QUIT", strlen("QUIT")))
			{
				send(fd, "Goodbye!\n", strlen("Goodbye!\n"), 0);
                 close(fd);	
			}
			if(logged==1){
								if(infile == 0){
					if(STRECOM(recvbuf, "LIST", strlen("LIST")))
					{
						//send(fd, folder, strlen(folder), 0);
						 DIR *d;
						  struct dirent *dir;
						  d = opendir(folder);
						  if (d) {
							while ((dir = readdir(d)) != NULL) {
							  sprintf(filename, "%s\n", dir->d_name);
							  send(fd, filename, strlen(filename), 0);
							}
							closedir(d);
						  }
						//sprintf(bufferout,"200 ,Hello %s , please to meet you\n", "LIST");//
						send(fd, bufferout, strlen(bufferout), 0);
					}
					else if(STRECOM(recvbuf, "GET", strlen("GET")))
					{

						sprintf(filename,"%s",recvbuf);
						removeSubstr(filename, "GET");//remove GET
						removeSubstr(filename, " ");
						removeSubstr(filename, "\n");//remove space

						sprintf(folderfilename,"%s/%s", folder, filename);
						//sprintf(bufferout,"200 ,Hello %s , please to meet you\n", "GET");//
						//send(fd, bufferout, strlen(bufferout), 0);
						char ch;
						char chr[3];
						ofp = fopen(folderfilename, "r");

						if (NULL == ofp) {
							printf("file can't be opened\n");
							sprintf(bufferout,"404 File %s not found\n", filename);//
							send(fd, bufferout, strlen(bufferout), 0);

						}else{
							printf("file caN be opened \n");

							do {
								ch = fgetc(ofp);
								sprintf(chr,"%c", ch);//
								send(fd, chr, strlen(chr), 0);

								printf("%c", ch);
							} while (ch != EOF);

							sprintf(chr,"\n");//

							send(fd, chr, strlen(chr), 0);
							fclose(ofp);
						}

						// Closing the file

					}
					else if(STRECOM(recvbuf, "PUT", strlen("PUT")))
					{

						sprintf(filename,"%s",recvbuf);
						removeSubstr(filename, "PUT");//remove PUT
						removeSubstr(filename, " ");
						removeSubstr(filename, "\n");//remove space

						sprintf(folderfilename,"%s/%s", folder, filename);
						infile=1;

						sprintf(bufferout,"200  %S file retrieved by server and was saved.\n", filename);//
						send(fd, bufferout, strlen(bufferout), 0);
					}
					else if(STRECOM(recvbuf, "DEL", strlen("DEL")))
					{

					 	sprintf(filename,"%s",recvbuf);
						removeSubstr(filename, "DEL");//remove DEL
						removeSubstr(filename, " ");
						removeSubstr(filename, "\n");//remove space

						sprintf(folderfilename,"%s/%s", folder, filename);

					    if (remove(folderfilename) == 0) {
							printf("The file is deleted successfully.");
							sprintf(bufferout,"200 %s , deleted\n", filename);//
							send(fd, bufferout, strlen(bufferout), 0);
						} else {
							printf("The file is not deleted.");
							sprintf(bufferout,"404 File %s is not on the server\n", filename);//
							send(fd, bufferout, strlen(bufferout), 0);
						}
					
					
					}
					else
					{  
						send(fd, "Wrong command please start your text with: LIST, GET, DEL or QUIT\n", strlen( "Wrong command please start your text with: LIST, GET, DEL or QUIT\n"), 0);
					}
				}else{
					//printf("C%d: %s\n", fd, buffer);
					strcat(thetext, recvbuf);
					if (STRECOM(recvbuf, ".",1)) { // A single "." signifies the end
                          ofp=fopen(folderfilename, "w");
						  fprintf(ofp, "\r\n%s",thetext);
						  fclose(ofp);
                          infile = 0;
                    }
				}

				}
			}else{
				if (STRECOM(recvbuf, "USER", strlen("USER"))) { // Initial greeting
										sprintf(username,"%s",recvbuf);
					removeSubstr(username, "USER");//remove GET
					//removeSubstr(username, " ");
					removeSubstr(username, "\n");//remove space
					char *result = username+1;
					printf("%s\n",result);

					//USER testtest123
					char *delim = " ";
					char *user = strtok(result,delim);  //username Provide by client
					char *password = strtok(NULL,delim);  //password Provide by client
					printf("%s\n", user);
					printf("%s\n", password);

					FILE* fileLogin;
					int bufferLength = 255;
					char bufferLogin[bufferLength]; /* not ISO 90 compatible */

					fileLogin = fopen("password.txt", "r");

					while(fgets(bufferLogin, bufferLength, fileLogin)) {
						printf("%s\n", bufferLogin);
						delim = ":";
						char *userFile =strtok(bufferLogin,delim);  //username got from password file
						char *passwordFile =strtok(NULL,delim);		 //password got from password file

						printf("User: %s   --- Password=%s",userFile, passwordFile);

						if (STRECOM(user, userFile, strlen(userFile)) && STRECOM(password, passwordFile, strlen(passwordFile)-2)) {
							logged=1;
							sprintf(bufferout,"200 User %s granted to access\n", user);//
							send(fd, bufferout, strlen(bufferout), 0);
						}
					}

					fclose(fileLogin);

					if (logged ==0){
						sprintf(bufferout,"400 User not found. Please try with another user\n");//
						send(fd, bufferout, strlen(bufferout), 0);
					}
				}
				else{
					send(fd, "the syntax is USER follow by your name space and passwork 'USER test password' \n", 
					strlen( "the syntax is USER follow by your name space and passwork 'USER test password' \n"), 0);

				}
			}


        } else if (rcnt == 0)
            printf("Connection closing...\n");
        else  {
            printf("Receive failed:\n");
            close(fd);
            break;
        }
    } while (rcnt > 0);


}


			  
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
	pid_t pid;


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
		if ((pid=fork()) == 0) {
			close(server);
			system_execution(fd, folder);
			
			printf("Child finished their job!\n");
			close(fd);
			exit(0);
	}

	



	//system_execution(fd);

}

// Final Cleanup
close(server);

}