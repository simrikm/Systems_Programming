PORT = 69856
PROGRAMS = server1 client server2 server3
S = -std=c99 -ggdb

both: $(PROGRAMS)

server1: server1.c
	gcc $S server1.c -lpthread -o server1

client: client.o time.o
	gcc $S client.o time.o -lpthread -o client
time.o:time.c time.h
	gcc -c $S time.c
client.o:client.c time.h
	gcc -c $S client.c -lpthread
server2:server2.c
	gcc $S server2.c -lpthread -o server2

server3:server3.c
	gcc $S server3.c -lpthread -o server3

test1: both
	server1 $(PORT) &
	echo Starting client
	client `hostname` $(PORT)
	ls -lr Thread_*
	du

test2: both
	server2 $(PORT) &
	echo Starting client
	client `hostname` $(PORT)
	ls -lr Thread_*
	du

test3: both
	server3 $(PORT) &
	echo Starting client
	client `hostname` $(PORT)
	ls -lr Thread_*
	du


clean:
	/bin/rm -rf $(PROGRAMS) Thread_* *.o
	
