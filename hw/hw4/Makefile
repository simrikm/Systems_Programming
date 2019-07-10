execute:execute.c
	gcc -ggdb -std=c11 -pedantic -Wall -Wextra -Werror -Wwrite-strings execute.c -o execute
my_fork:my_fork.c
	gcc -ggdb -std=c11 -pedantic -Wall -Wextra -Werror -Wwrite-strings my_fork.c -o my_fork
my_copy:my_copy.o time.o
	gcc -ggdb -std=c11 -pedantic -Wall -Wextra -Werror -Wwrite-strings my_copy.o time.o -o my_copy
time.o:time.c time.h
	gcc -c -ggdb -std=c11 -pedantic -Wall -Wextra -Werror -Wwrite-strings time.c
my_copy.o:my_copy.c time.h
	gcc -c -ggdb -std=c11 -pedantic -Wall -Wextra -Werror -Wwrite-strings my_copy.c

clean:
	/bin/rm -f my_copy.o time.o my_copy	
