#include <sys/types.h>
#include <signal.h>
#include <sys/uio.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <pthread.h>
#include <string.h>
#include "Timer.h"

#define Q_elem int
#define Q_max 1024
#define NUM_THREADS 5

#define BACKLOG 200
// if more than BACKLOG clients in the server accept queue, client connect will fail

void error(char *msg)
{
    fprintf(stderr, "ERROR: %s failed\n", msg);
    exit(-1);
}

// Klefstad Queue implementations
Q_elem Q[Q_max];
int Q_front = 0, Q_rear = 0;

int Q_full()
{
	return (Q_rear + 1) % Q_max == Q_front;
}

int Q_empty()
{
	return Q_front == Q_rear;
}

void enQ(Q_elem val)
{
	if (Q_full())
		error("enqueue on full queue");
	Q[Q_rear++] = val;
	if (Q_rear >= Q_max)
		Q_rear = 0;
}

Q_elem deQ()
{
	if (Q_empty())
		error("dequeue on empty queue");
	Q_elem ret = Q[Q_front++];
	if (Q_front >= Q_max)
		Q_front = 0;
	return ret;
}
// end Q

struct sockaddr_in make_server_addr(short port)
{
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof addr);
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = INADDR_ANY;
    return addr;
}

int create_server_socket(short port)
{
    int s = socket(AF_INET, SOCK_STREAM, 0);
    int optval = 1;
    setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof optval);
    struct sockaddr_in my_addr = make_server_addr(port);
    if ( s == -1 )
        error("socket()");
    bind(s, (struct sockaddr*)&my_addr, sizeof my_addr);
    listen(s, BACKLOG);
    return s;
}

void get_file_request(int socket, char *fileName)
{
    char buf[BUFSIZ];
    int n = read(socket, buf, BUFSIZ);
    if ( n < 0 )
        error("read from socket");
    buf[n] = '\0';
    strcpy(fileName, buf);
    printf("Server got file name of '%s'\n", fileName);
}

void write_file_to_client_socket(char *file, int socket)
{
    char buf[BUFSIZ];
    int n;
    int ifd = open(file, O_RDONLY);
    if ( ifd == -1 )
        error("open()");
    while ( (n = read(ifd, buf, BUFSIZ)) > 0 )
        write(socket, buf, n);
    close(ifd);
}

void handle_request(int client_socket)
{
    char fileName[BUFSIZ];
    get_file_request(client_socket, fileName);
    write_file_to_client_socket(fileName, client_socket);
    close(client_socket);
}

pthread_mutex_t request_mutex;
pthread_cond_t enQ_cv, deQ_cv;

void *thread_work(void * arg)
{
	while(1) 
	{
		pthread_mutex_lock(&request_mutex);
		while (Q_empty())
			pthread_cond_wait(&enQ_cv, &request_mutex);
		int client_socket = deQ();
		handle_request(client_socket);
		pthread_mutex_unlock(&request_mutex);
		pthread_cond_signal(&deQ_cv);
	}
}


void time_out(int arg)
{
    fprintf(stderr,  "Server timed out\n");
    exit(0);
}

void set_time_out(int seconds)
{
    struct itimerval value = {0};
    value.it_value.tv_sec = seconds;
    setitimer(ITIMER_REAL, &value, NULL);
    signal(SIGALRM, time_out);
}


void accept_client_requests(int server_socket)
{
    int client_socket;
    struct sockaddr_in client_addr;
    socklen_t sin_size = sizeof client_addr;
    set_time_out(5);
    while (1)
    {
        set_time_out(5);
		pthread_mutex_lock(&request_mutex);
		while (!Q_empty())
			pthread_cond_wait(&deQ_cv, &request_mutex);
		client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &sin_size);
		printf("hello");
		enQ(client_socket);
		pthread_cond_signal(&enQ_cv);
		pthread_mutex_unlock(&request_mutex);
    }
}

/*
   Note, I put in a 5 second time out so you don't leave servers running.
*/

int main(int argc, char *argv[])
{
    if ( argc != 2 )
        error("usage: server port");
	pthread_mutex_init(&request_mutex, NULL);
	pthread_cond_init(&enQ_cv, NULL);
	pthread_cond_init(&deQ_cv, NULL);
	pthread_t consumer_threads[NUM_THREADS];
	int i;
	for(i = 0; i < NUM_THREADS; i++)
	{
		pthread_create(&consumer_threads[i], NULL, thread_work, NULL);
	}

    short port = atoi(argv[1]);
    int server_socket = create_server_socket(port);
	accept_client_requests(server_socket);
    shutdown(server_socket, 2);
    return 0;
}
