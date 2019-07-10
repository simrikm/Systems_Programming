OBJECTS=my_ls.o main.o
my_ls:$(OBJECTS)
	gcc -ggdb -std=c11 -pedantic -Wall -Wextra -Werror  -Wwrite-strings $(OBJECTS) -o my_ls
my_ls.o:my_ls.c my_ls.h 
	gcc -c -ggdb -std=c11 -pedantic -Wall -Wextra -Werror -Wwrite-strings my_ls.c
main.o:main.c my_ls.h	 
	gcc -c -ggdb -std=c11 -pedantic -Wall -Wextra -Werror  -Wwrite-strings main.c
clean:
	/bin/rm -f $(OBJECTS) my_ls
