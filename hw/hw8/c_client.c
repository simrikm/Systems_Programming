#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h> 
#include <stdio.h>
#include <sys/wait.h>
#include <fcntl.h>

//#define h_addr h_addr_list[0]
#define PORT 9856

int make_client_socket(char *host,int port)
{
	int sockfd;
	struct hostent *he;
	struct sockaddr_in their_addr;
	
	if((he=gethostbyname(host))==NULL)
		perror("gethistname");

	if((sockfd=socket(AF_INET, SOCK_STREAM, 0))==-1)
		perror("socket error");

	memset(&(their_addr.sin_zero),'\0',8);
	their_addr.sin_family=AF_INET;
	their_addr.sin_port=htons(port);
	their_addr.sin_addr=*((struct in_addr*)he->h_addr);
	if(connect(sockfd,(struct sockaddr*)&their_addr,sizeof(struct sockaddr))==-1)
		perror("Connection error");
	return sockfd;
}

void copy_file(int from, int to)
{
	char buf[BUFSIZ];
	int n;
	while((n=read(from,buf,BUFSIZ))>0)
		if(write(to,buf,n)==-1)perror("Write failed");
		memset(buf,'\0',BUFSIZ);
}

void fork_child_copying_file_from_socket(int transfer_socket,char *f,char *base_name)
{
	printf("%s\n",f);
	int new_file=open(base_name,O_RDWR|O_CREAT,0666);
	if(new_file < 0)
		return;
	pid_t pid;
	switch(pid=fork())
	{
		case 0:
			copy_file(transfer_socket,new_file);
			exit(0);
		default:
			return;
	}
}

int main(int argc, char *argv[])
{
	if(argc <2)
		perror("less argument");

	for(int i=3; i< argc; ++i)
	{
		char *base_name=strrchr(argv[i],'/');
		int port=atoi(argv[2]);
		int client_socket=make_client_socket(argv[1],port);
		send(client_socket,argv[i],strlen(argv[i]),0);
		fork_child_copying_file_from_socket(client_socket,argv[i],++base_name);
	}
	return 0;
}
