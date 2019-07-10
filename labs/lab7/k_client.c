#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>

// the port client will be connecting to
#define PORT 5677
// max number of bytes we can get at once
#define MAXDATASIZE 300
 
int main(int argc, char *argv[])
{
    int sockfd, numbytes, valread, nret;
    char buf[MAXDATASIZE];
    struct hostent *he;
    // connector’s address information
    struct sockaddr_in their_addr;
     int user_name=0;
    // if no command line argument supplied
    if(argc != 3)
    {
        fprintf(stderr, "Client-Usage: %s the_client_hostname\n", argv[0]);
        // just exit
        exit(1);
    }
     
    // get the host info
    if((he=gethostbyname(argv[2])) == NULL)
    {
        perror("gethostbyname()");
        exit(1);
    }
    else
        printf("Client-The remote host is: %s\n", argv[2]);
     
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("socket()");
        exit(1);
    }
    else
        printf("Client-The socket() sockfd is OK...\n");
     
    // host byte order
    their_addr.sin_family = AF_INET;
    // short, network byte order
    printf("Server-Using %s and port %d...\n", argv[2], PORT);
    their_addr.sin_port = htons(PORT);
    their_addr.sin_addr = *((struct in_addr *)he->h_addr);
    // zero the rest of the struct
    memset(&(their_addr.sin_zero), '\0', 8);
    
    if(connect(sockfd, (struct sockaddr *)&their_addr, sizeof(struct sockaddr)) == -1)
    {
        perror("connect");
        exit(1);
    }
    else
        printf("Client-The connect() is OK...\n");

    fd_set read_fd, write_fd;	
		valread=send(sockfd,argv[1],sizeof(argv[1]),0);
		valread=read(sockfd,buf,300);	
		printf("%s\n", buf);
    while(1)
    {
		
       FD_ZERO(&read_fd);
       FD_ZERO(&write_fd);
       
       FD_SET(0, &read_fd);
       FD_SET(sockfd, &read_fd);
       FD_SET(0, &write_fd);
       FD_SET(sockfd, &write_fd);

       nret = select( sockfd + 1, &read_fd, &write_fd, NULL, NULL );
    
       if(FD_ISSET(0, &read_fd))
       {
          memset( &buf, '\0', 300 );
		
          valread = read(1, buf, 300);
          //valread = write( sockfd, buf, 300);
          /* ensure we can write to sock */
             if (!FD_ISSET(sockfd, &write_fd)) {
                /* select on sock for writing */
                fd_set s_sock;
                FD_SET(sockfd, &s_sock);
                int s = select(sockfd + 1, NULL, &s_sock, NULL, NULL);
                /* always going to be sock or error */
                if (s == -1) {
                   perror("select");
                   return -1;
                 }
              }

              /* write to socket */
           valread = send(sockfd , buf , strlen(buf) , 0 );
       }
       if(FD_ISSET(sockfd, &read_fd))
       {
          memset( &buf, 0, 300 );
          valread = read( sockfd, buf, 300);
          if (FD_ISSET(1, &write_fd)) {
             /* select on stdout for writing */
             fd_set s_stdout;
             FD_SET(1, &s_stdout);
             int s = select(1 + 1, NULL, &s_stdout, NULL, NULL);
                  /* always going to be stdout or error */
             if (s == -1) {
                perror("select");
                return -1;
             }
           }
     		printf("%s",buf);
        }
    }
    if((numbytes = recv(sockfd, buf, MAXDATASIZE-1, 0)) == -1)
    {
        perror("recv()");
        exit(1);
    }
    else
        printf("Client-The recv() is OK...\n");
     
    buf[numbytes] = '\0';
    printf("Client-Received: %s", buf);
    printf("Client-Closing sockfd\n");
    close(sockfd);
    return 0;
}

