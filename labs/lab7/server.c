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
#define MYPORT PORT
#define MAXDATASIZE 300

/* how many pending connections queue will hold */
#define BACKLOG 10
 
int read_from_client (int filedes)
{
  char buffer[BUFSIZ];
  int nbytes;

  nbytes = read (filedes, buffer, BUFSIZ);
  if (nbytes < 0)
    {
      /* Read error. */
      perror ("read");
      exit (EXIT_FAILURE);
    }
  else if (nbytes == 0)
    /* End-of-file. */
    return -1;
  else
    {
      /* Data read. */
      fprintf (stderr, "Server: got message: `%s'\n", buffer);
      return 0;
    }
}

int main()
{
    /* listen on sock_fd, new connection on new_fd */
    int sockfd, new_fd,val;
	char buffer[BUFSIZ]={0};
    /* my address information, address where I run this program */
    struct sockaddr_in my_addr;
    /* remote address information */
    struct sockaddr_in their_addr;
    int sin_size;
	fd_set active_fd_set, read_fd_set;
	int i; 
     
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd == -1)
    {
      perror("socket() error lol!");
      exit(1);
    }
    else
      printf("socket() is OK...\n");
     
    /* host byte order */
    my_addr.sin_family = AF_INET;
    /* short, network byte order */
    my_addr.sin_port = htons(MYPORT);
    /* auto-fill with my IP */
    my_addr.sin_addr.s_addr = INADDR_ANY;
     
    /* zero the rest of the struct */
    memset(&(my_addr.sin_zero), 0, 8);
     
    if(bind(sockfd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr)) == -1)
    {
      perror("bind() error lol!");
      exit(1);
    }
    else
      printf("bind() is OK...\n");
     
    if(listen(sockfd, BACKLOG) == -1)
    {
      perror("listen() error lol!");
      exit(1);
    }
    else
      printf("listen() is OK...\n");
     
    /* ...other codes to read the received data... */
    
	FD_ZERO(&active_fd_set);
	FD_SET(sockfd,&active_fd_set);
	
	while(1){
 		read_fd_set=active_fd_set;
		if(select(FD_SETSIZE, &read_fd_set, NULL,NULL, NULL)<0)
		{
			perror("select");
			exit(EXIT_FAILURE);
		}
		for(i=0;i<FD_SETSIZE;++i)
			if(FD_ISSET(i,&read_fd_set)){
			if(i==sockfd){
				sin_size=sizeof(struct sockaddr_in);
			 	new_fd = accept(sockfd, (struct sockaddr *)&their_addr, (socklen_t *)&sin_size);			     
    			if(new_fd == -1){
      				perror("accept() error lol!"); 
					exit(EXIT_FAILURE);
				}	
		char username[BUFSIZ]={0};
		read(new_fd,buffer,BUFSIZ);	
		strcpy(username,buffer);
		username[strlen(username)-1]='\0';
		strcat(username," has joined the chat room"); 	
		send(new_fd,username,strlen(username),0);	
		memset(&username,0,BUFSIZ);
		memset(&buffer,0,BUFSIZ);
     		FD_SET(new_fd, &active_fd_set);
		}
     else{
		if(read_from_client(i)<0)
		{
		close(i);
		FD_CLR(i,&active_fd_set);
		}
	}
	}	
}
    /*.....other codes.......*/

    return 0;
}
