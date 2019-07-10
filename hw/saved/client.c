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

/**void read_data(int sockfd){
	char buf[BUFSIZ];
	int valread;
	memset(&buf,'\0',BUFSIZ);
	while((valread=read(sockfd,buf,BUFSIZ))>0){
		printf("%s\n",buf);	
	}
}**/

int create_socket(char *hostname)
{

	int sockfd;
	struct hostent *he;
	struct sockaddr_in their_addr;
	if((he=gethostbyname(hostname))==NULL)
		perror("gethostname");
	if((sockfd=socket(AF_INET, SOCK_STREAM,0))==-1)
		perror("socket error");
	their_addr.sin_family=AF_INET;
	their_addr.sin_port=htons(PORT);
	their_addr.sin_addr=*((struct in_addr *)he->h_addr);
	memset(&(their_addr.sin_zero),'\0',8);
	if(connect(sockfd,(struct sockaddr*)&their_addr,sizeof(struct sockaddr))==-1)
		perror("Connection error");
	return sockfd;

}


int main (int argc, char *argv[])
{
	char buf[BUFSIZ];
	if (argc < 2)
		perror("less arguments");

	int sockfd=create_socket(argv[1]);	
	int i=2;
	fd_set read_fd, write_fd;
	while(1)
	{
		FD_ZERO(&read_fd);
		FD_ZERO(&write_fd);
	
		FD_SET(0,&read_fd);
		FD_SET(sockfd,&read_fd);
		FD_SET(0,&write_fd);
		FD_SET(sockfd,&write_fd);

		select(sockfd+1,&read_fd,&write_fd,NULL,NULL);
		
		if(FD_ISSET(sockfd,&read_fd))
		{
		memset(&buf,'\0',BUFSIZ);
		read(sockfd,buf,BUFSIZ);
			if(FD_ISSET(1,&write_fd))
			{
				fd_set s_stdout;
				FD_SET(1,&s_stdout);
				int s=select(1+1,NULL,&s_stdout,NULL,NULL);
				if(s==-1){
					perror("select");
					return -1;
				}	
			}	
		
		printf("%s",buf);	
		continue;
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
