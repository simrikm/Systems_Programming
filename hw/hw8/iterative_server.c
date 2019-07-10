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

#define PORT 9856
#define BACKLOG 50

int create_socket(){
	int sockfd;
	struct sockaddr_in my_addr;
	sockfd=socket(AF_INET,SOCK_STREAM,0);
	if(sockfd==-1)
	{
		perror("socket() error");
		exit(1);	
	}
	my_addr.sin_family=AF_INET;
	my_addr.sin_port=htons(PORT);
	my_addr.sin_addr.s_addr=INADDR_ANY;
	memset(&(my_addr.sin_zero),0,8);
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
	return sockfd;
}

void call_ls(int newfd)
{
	char buf[BUFSIZ];
	memset(buf,'\0',BUFSIZ);
	while((read(newfd,buf,BUFSIZ))>0)
	{
		printf("%s\n",buf);
		
		pid_t pid;
		char *args[]={(char*)"/bin/ls",(char*)"-l",(char*)buf,NULL};
		char* envp[]={NULL};
		switch(pid=fork()){
			case 0:
			execve("/bin/ls",args,envp);
			break;
			default:
			wait(NULL);
		}
		printf("\n\n");
		memset(buf,'\0',BUFSIZ);
	}
	
}

int main(){
	int sockfd=create_socket();
	int newfd;
	struct sockaddr_in client;
	int sin_size=sizeof(struct sockaddr_in);	
	newfd=accept(sockfd,(struct sockaddr*)&client,(socklen_t*)&sin_size);
	dup2(newfd,1);
	call_ls(newfd);
	close(newfd);
	close(sockfd);
	return 0;
}
