#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

void morse_to_alph(char *s)
{
	if(strcmp(s,". -" ) == 0){printf("a");}
	if(strcmp(s,"- . . ." ) == 0){printf("b");}
	if(strcmp(s,"- . - ." ) == 0){printf("c");}
	if(strcmp(s,"- . ." ) == 0){printf("d");}
	if(strcmp(s,"." ) == 0){printf("e");}
	if(strcmp(s,". . - ." ) == 0){printf("f");}
	if(strcmp(s,"- - ." ) == 0){printf("g");}
	if(strcmp(s,". . . ." ) == 0){printf("h");}
	if(strcmp(s,". ." ) == 0){printf("i");}
	if(strcmp(s,". - - -" ) == 0){printf("j");}
	if(strcmp(s,"- . -" ) == 0){printf("k");}
	if(strcmp(s,". - . ." ) == 0){printf("l");}
	if(strcmp(s,"- -" ) == 0){printf("m");}
	if(strcmp(s,"- ." ) == 0){printf("n");}
	if(strcmp(s,"- - -" ) == 0){printf("o");}
	if(strcmp(s,". - - ." ) == 0){printf("p");}
	if(strcmp(s,"- - . -" ) == 0){printf("q");}
	if(strcmp(s,". - ." ) == 0){printf("r");}
	if(strcmp(s,". . ." ) == 0){printf("s");}
	if(strcmp(s,"-" ) == 0){printf("t");}
	if(strcmp(s,". . -" ) == 0){printf("u");}
	if(strcmp(s,". . -" ) == 0){printf("v");}
	if(strcmp(s,". - -" ) == 0){printf("w");}
	if(strcmp(s,"- . . -" ) == 0){printf("x");}
	if(strcmp(s,"- . - -" ) == 0){printf("y");}
	if(strcmp(s,"- - . ." ) == 0){printf("z");}
	if(strcmp(s,". - - - -" ) == 0){printf("1");}
	if(strcmp(s,". . - - -" ) == 0){printf("2");}
	if(strcmp(s,". . . - -" ) == 0){printf("3");}
	if(strcmp(s,". . . . -" ) == 0){printf("4");}
	if(strcmp(s,". . . . ." ) == 0){printf("5");}
	if(strcmp(s,"- . . . ." ) == 0){printf("6");}
	if(strcmp(s,"- - . . ." ) == 0){printf("7");}
	if(strcmp(s,"- - - . ." ) == 0){printf("8");}
	if(strcmp(s,"- - - - ." ) == 0){printf("9");}
	if(strcmp(s,"- - - - -" ) == 0){printf("0");}
}
char *return_string(int s, int e, char *a)
{
	char *string_return = malloc(BUFSIZ);
	int j = 0;
	for(int i = s; i <= e; ++i, ++j)
	{
		string_return[j] = a[i];
	}
	string_return[j] = '\0';
	return string_return;
}
void print_morse_code(char *array)
{
	char temp_string[BUFSIZ];
	int start = 0, end;

	for(int i = 0; i < BUFSIZ; ++i)
	{
		
		/*if( (array[i] == ' ') && (array[i+1] == ' ') && (array[i+2] == ' ')&& (array[i+3] == ' ') && (array[i+4] == ' ') && (array[i+5] == ' ') && (array[i+6] == ' '))
		{
			start = i + 7;
			i = i + 6;
			printf(" ");
		}*/
		if( (array[i] == ' ') && (array[i+1] == ' ') && (array[i+2] == ' ') )
		{
			end = i - 1;
			morse_to_alph(return_string(start, end, array));
			start = i+3;
		}
		if( (array[i] == ' ') && (array[i+1] == ' ') && (array[i+2] == ' ')&& (array[i+3] == ' ') && (array[i+4] == ' ') && (array[i+5] == ' ') && (array[i+6] == ' '))
		{
			start = i + 7;
			i = i + 6;
			printf(" ");
		}
		
		
	}
	printf("\n");
}

int main(int argc,char *argv[]){	

/* morse to alpha*/
	FILE *file = fopen(argv[1], "r");
	char this_array[BUFSIZ];
	fgets(this_array, BUFSIZ, file);
	print_morse_code(this_array);
	fclose(file);

return 0;
}
