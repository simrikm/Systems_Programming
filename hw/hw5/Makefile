all:signal_handling my_shell
signal_handling:signal_handling.c
	gcc -ggdb -std=c11 -pedantic -Wall -Wextra -Werror -Wwrite-strings signal_handling.c -o signal_handling
my_shell:my_shell.c
	gcc -ggdb -std=c99 -pedantic -Wall -Wextra -Werror -Wwrite-strings my_shell.c -o my_shell
