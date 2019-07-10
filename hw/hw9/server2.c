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


#define BACKLOG 300
// if more than BACKLOG clients in the server accept queue, client connect will fail
#define QUE_MAX 1024
#define ELE int
#define N_THREADS 5

pthread_mutex_t mutex=PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t enQ,dQ;

ELE _que[QUE_MAX];
int _front = 0, _rear = 0;

void que_error(char *msg)
{
    fprintf(stderr, "Error: %s\n", msg);
    exit(-1);
}

int que_is_full()
{
    return (_rear + 1) % QUE_MAX == _front; /* this is why one slot is unused */
}

int que_is_empty()
{
    return _front == _rear;
}

void que_enq(ELE v)
{
    if ( que_is_full() )
        que_error("enq on full queue");
    _que[_rear++] = v;
    if ( _rear >= QUE_MAX )
        _rear = 0;
}

ELE que_deq()
{
    if ( que_is_empty() )
        que_error("deq on empty queue");
    ELE ret = _que[_front++];
    if ( _front >= QUE_MAX )
        _front = 0;
    return ret;
}


void error(char *msg)
{
    fprintf(stderr, "ERROR: %s failed\n", msg);
    exit(-1);
}


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

void time_out(int arg)
{
    fprintf(stderr,  "Server timed out with nothing to do\n");
    exit(0);
}

void set_time_out(int seconds)
{
    struct itimerval value = {0};
    value.it_value.tv_sec = seconds;
    setitimer(ITIMER_REAL, &value, NULL);
    signal(SIGALRM, time_out);
}


void * accept_client_requests(void *server_socket)
{
    int client_socket;
	int sock_fd=*(int*)server_socket;
    struct sockaddr_in client_addr;
    socklen_t sin_size = sizeof client_addr;
	pthread_mutex_lock(&mutex);
    while ( (client_socket =
            accept(sock_fd, (struct sockaddr*)&client_addr, &sin_size)) > 0 )
   	{
		if(!que_is_empty())
			pthread_cond_wait(&dQ,&mutex);
		que_enq(client_socket);
		pthread_cond_signal(&enQ);
		pthread_mutex_unlock(&mutex);
	}
}

void * threadwork()
{
	while(1)
	{
		pthread_mutex_lock(&mutex);
		while (que_is_empty())
			pthread_cond_wait(&enQ,&mutex);
		int client_socket=que_deq();
		handle_request(client_socket);	
		pthread_mutex_unlock(&mutex);	
		pthread_cond_signal(&dQ);
		set_time_out(5);
	}
}

void start_thread()
{
	pthread_t threads[N_THREADS];
	for ( int i =0; i<N_THREADS; ++i)
		pthread_create(&threads[i],NULL,threadwork,NULL);

}


int main(int argc, char *argv[])
{	
    if ( argc != 2 )
        error("usage: server port");
    short port = atoi(argv[1]);
    int server_socket = create_server_socket(port);
	pthread_t mainthread;
	pthread_create(&mainthread,NULL,accept_client_requests,&server_socket);	
	start_thread();
	pthread_exit(NULL);
    shutdown(server_socket, 2);
    return 0;
}
