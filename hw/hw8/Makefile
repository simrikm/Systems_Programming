C=gcc
CFLAGS= -std=gnu99 -pedantic -Wall -Wextra -Werror -Wwrite-strings
DEBUG_LABEL=-ggdb
OBJECTS=client iterative_server concurrent_server c_client

all:$(OBJECTS)
client:client.c
	$(CC) $(DEBUG_LABEL) $(CFLAGS) client.c -o client
iterative_server:iterative_server.c
	$(CC) $(DEBUG_LABEL) $(CFLAGS) iterative_server.c -o iterative_server
concurrent_server:concurrent_server.c
	$(CC) $(DEBUG_LABEL) $(CFLAGS) concurrent_server.c -o concurrent_server
c_client:c_client.c
	$(CC) $(DEBUG_LABEL) $(CFLAGS) c_client.c -o c_client

clean:
	/bin/rm -f $(OBJECTS) 

strip:client.c iterative_server.c
	$(CC) $(CFLAGS) client.c -o client
	$(CC) $(CFLAGS) iterative_server.c -o iterative_server
	$(CC) $(CFLAGS) c_client.c -o c_client
	$(CC) $(CFLAGS) concurrent_server.c -o concurrent_server
	







