#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define h_addr h_addr_list[0]
#define PORT 9856

int create_socket(char *hostname)
{
	int sockfd;
	struct hostent *he;
	struct sockaddr_in their_addr;

	if((he=gethostbyname(hostname))==NULL)
		perror("gethostname");

	if((sockfd=socket(AF_INET, SOCK_STREAM,0))==-1)
		perror("socket error");

	memset(&(their_addr.sin_zero),'\0',8);
	their_addr.sin_family=AF_INET;
	their_addr.sin_port=htons(PORT);
	their_addr.sin_addr=*((struct in_addr *)he->h_addr);

	if(connect(sockfd,(struct sockaddr*)&their_addr,sizeof(struct sockaddr))==-1)
		perror("Connection error");

	return sockfd;

}


int main (int argc, char *argv[])
{

	if (argc < 2)
		perror("less arguments");

	int sockfd=create_socket(argv[1]);	
	int i=2;
	int valread;
	char buf[BUFSIZ];
	fd_set read_fd;

	while(1)
	{
		FD_ZERO(&read_fd);
			
		FD_SET(0,&read_fd);
		FD_SET(sockfd,&read_fd);

		if(select(sockfd+1,&read_fd,NULL,NULL,NULL)<0)
		{
			perror("select");
			return -1;	
		}
		
		if(FD_ISSET(sockfd,&read_fd))
		{
		memset(&buf,'\0',BUFSIZ);
		valread=read(sockfd,buf,BUFSIZ);
		write(1,buf,valread);
		}

		if(i < argc)
		{	
		send(sockfd, argv[i], strlen(argv[i]),0);
		i++;
		}
	}

	close(sockfd);
	return 0;
} 
