#include <stdio.h>
#include <arpa/inet.h>
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
 
int main()
{
    /* listen on sock_fd, new connection on new_fd */
    int sockfd, proc[10], valread, i, size, proc_len, max = 0;
    /* my address information, address where I run this program */
    struct sockaddr_in my_addr;
    /* remote address information */
    struct sockaddr_in their_addr;
    int sin_size;
    char buf[MAXDATASIZE];
     
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd == -1)
    {
      perror("socket() error lol!");
      exit(1);
    }
    else
      printf("socket() is OK...\n");
    
    fd_set fd_set_client;
 
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
    sin_size = sizeof(struct sockaddr_in);
    /*
      int accept(int socket, struct sockaddr *restrict address,
            socklen_t *restrict address_len);
    */
    
    proc[0] = accept(sockfd, (struct sockaddr *)&their_addr, (socklen_t *)&sin_size);
    if( max < proc[0] ) max = proc[0];
    proc[1] = accept(sockfd, (struct sockaddr *)&their_addr, (socklen_t *)&sin_size);
    if( max < proc[1] ) max = proc[1];
    proc[2] = accept(sockfd, (struct sockaddr *)&their_addr, (socklen_t *)&sin_size);
    if( max < proc[2] ) max = proc[2];

    FD_SET(proc[0], &fd_set_client);
    FD_SET(proc[1], &fd_set_client);
    FD_SET(proc[2], &fd_set_client);

    if(proc[0] == -1)
      perror("accept() error lol!");
    else
      printf("accept() is OK...\n");
    
    int nret = 0;
    while (1)
    {
      FD_ZERO(&fd_set_client);
      FD_SET(sockfd, &fd_set_client);  
      max = sockfd;
     for ( i = 0 ; i < 3 ; i++)  
     {  
        size = proc[i];  
        if( size > 0 )  FD_SET( size , &fd_set_client);                                  
        if( size > max )  max = size;  
     }    

      /* Block until input arrives on one or more active sockets. */
      nret = select( max+1, &fd_set_client, NULL, NULL, NULL); 
        if( nret == -1 )
        {
          perror ("select");
          exit (EXIT_FAILURE);
        }
        if( nret == 0 )
        {
           continue;
        } 
        printf("check5\n");
      /* Service all the sockets with input pending. */
        for (i = 0; i < 3; ++i)
        {
           size = proc[i];
           printf("check6\n");
           
           if (FD_ISSET(size, &fd_set_client))
           { 
               printf("check7");
               memset( buf, '\0', 300 );
               nret = read( proc[i], buf, 300 );
				if(nret==0){
					printf("disconnected");
					close(proc[i]);
					proc[i]=0;
				}
               for( int index = 0; index < 3; ++index)
                   if( index == i ) continue;
                   else  send(proc[index], buf, strlen(buf), 0);
               printf("%s", buf);
               printf("check8\n");
               break;      
           }
        }
    }




    /*while(1)
    { 
       memset( &buf, '\0', 300 );
       valread = read( new_fd1, buf, 300);
       printf( "%s", buf );
       memset( &buf, '\0', 300 );
    }        

*/
     
    /*.....other codes.......*/
     
    close(sockfd);
    return 0;
}
