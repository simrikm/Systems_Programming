CC=gcc
CFLAGS= -std=c11 -pedantic -Wall -Wextra -Werror -Wwrite-strings
DEBUG_LABEL=-ggdb
OBJECTS=product_array pipes

all:$(OBJECTS)
product_array: main.c
	$(CC) $(DEBUG_LABEL) $(CFLAGS) main.c -o product_array
pipes:pipes.c
	$(CC) $(DEBUG_LABEL) $(CFLAGS) pipes.c -o pipes

clean:
	/bin/rm -f $(OBJECTS)

strip:main.c pipes.c
	$(CC) $(CFLAGS) main.c -o product_array
	$(CC) $(CFLAGS) pipes.c -o pipes





