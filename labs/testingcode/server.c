#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#define PORT 7533
#define BACKLOG 5

int read_from_client (int fd, char* buf)
{
	int nbytes;

	nbytes = read(fd, buf, BUFSIZ);
	if(nbytes < 0) 
	{
		return 1;
		//error
	}
	else if (nbytes == 0)
	{
		//EOF
		return -1;
	}
	else
	{
		printf("%s\n", buf);
		return 0;
	}
}

int write_to_client (int fd, char* buf, int nbytes)
{
	write(fd, buf, nbytes);
	return 0;
}


int make_socket (uint16_t port)
{
	int sock;
	struct sockaddr_in name;

	sock = socket (PF_INET, SOCK_STREAM, 0);
	if(sock < 0)
	{
		//error
	}
	name.sin_family = AF_INET;
	name.sin_port = htons(port);
	name.sin_addr.s_addr = htonl (INADDR_ANY);
	if (bind (sock, (struct sockaddr *) &name, sizeof (name)) < 0)
	{
		//error
	}
	return sock;
}


int main()
{
	//extern int make_socket (uint16_t port);
	int sock;
	fd_set active_set, read_set;//, write_set;
	char buf[1024];
	int i;
	struct sockaddr_in client_name;
	socklen_t size;

	sock = make_socket (PORT);

	if (listen(sock, 1) < 0)
	{
		perror ("listen");
		exit(EXIT_FAILURE);
	}

	FD_ZERO(&active_set);
	FD_SET(sock, &active_set);

	while (1)
	{
		
		printf("connected");
		read_set = active_set;
		if (select (FD_SETSIZE, &read_set, NULL, NULL, NULL) < 0) 
		{
			perror("select");
			exit(EXIT_FAILURE);
		}

		for (i = 0; i < FD_SETSIZE; ++i )
		{
			if (FD_ISSET(i, &read_set))
			{
				if (i == sock)
				{
					int new;
					size = sizeof (client_name);
					new = accept (sock, (struct sockaddr *) &client_name, &size);

					if (new < 0)
					{
						perror("accept");
						exit(EXIT_FAILURE);
					}
					fprintf(stderr, "Server: connect from host");
					FD_SET(new, &active_set);
				}
				else 
				{
					close(i);
					FD_CLR (i , &active_set);
				}
			}
		}

		for (i = 0; i < FD_SETSIZE;i++)
		{
			if (FD_ISSET(i, &read_set))
			{
				write(i, buf, BUFSIZ);
			}
		}
		strcpy(buf, "");
	}
}
