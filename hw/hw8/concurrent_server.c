#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <fcntl.h>

#define BACKLOG 50


void copy_file(int from,int to)
{
	char buf[BUFSIZ];
	int valread;

	while((valread=read(from,buf,BUFSIZ))>0)
		if(write(to,buf,valread)==-1)
			perror("Write failed");

	memset(buf,'\0',BUFSIZ);
}


int make_server_socket(int port)
{
	int sockfd;
	struct sockaddr_in my_addr;
	sockfd=socket(AF_INET, SOCK_STREAM,0);

	if(sockfd==-1)
	{
		perror("socket() error\n");
		exit(1);
	}
	else
	printf("socket() is OK...\n");

	my_addr.sin_family=AF_INET;
	my_addr.sin_port=htons(port);
	my_addr.sin_addr.s_addr=INADDR_ANY;
	memset(&(my_addr.sin_zero),0,8);

	if(bind(sockfd,(struct sockaddr*)&my_addr,sizeof(struct sockaddr))==-1)
	{
		perror("bind() error\n");
		exit(1);
	}
	else 
	printf("bind() is OK...\n");
	
	if(listen(sockfd,BACKLOG)==-1)
	{
		perror("listen() error");
		exit(1);
	}
	else
		printf("listen is OK....\n");
	return sockfd;
}

void fork_child_copying_file_to_socket(int client_socket){
	char filepath[BUFSIZ];
	memset(filepath,'\0',BUFSIZ);
	pid_t pid;
	read(client_socket,filepath,BUFSIZ);
	printf("%s\n",filepath);	
	int file=open(filepath,O_RDONLY);
	switch(pid=fork()){
		case 0:
			copy_file(file,client_socket);
			exit(0);
		default:
			return;
		}
	close(file);
}


int main(int argc,char *argv[])
{
	if(argc < 2)
		perror("enter the port numer");

	struct sockaddr_in client;
	int sin_size=sizeof(struct sockaddr_in);
	int port=atoi(argv[1]);

	int server_socket=make_server_socket(port);

	while(1){
		int client_socket=accept(server_socket,(struct sockaddr*)&client,(socklen_t*)&sin_size);
		fork_child_copying_file_to_socket(client_socket);
	}

	close(server_socket);
	return 0;
}	
