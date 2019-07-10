#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>
#include <pwd.h>
#include <time.h>
#include <grp.h>
#include <fcntl.h>
#include <unistd.h>
#include <ctype.h>
#include <limits.h>
#include "stack.h"

#define STRMAX 200

long int isNumber(char *word){
	if(*word == '-')
		if(isdigit(word[1]))
			return 1;
	return isdigit(*word);
}


void handleOther(char *word){
	char c = *word;
	long int first = 99999;
	long int second = 99999;
	long result;
	if(c == '+' || c == '-' || c == '*' || c == '/' || c == 's'){
		first = pop();
		second = pop();
	}
	switch(c){
		case 'p':
			printf("TOP of stack: %ld\n", peek());
            break;
		case 'd':
			push(peek());
			break;
		case '+': push(second + first); break;
		case '-': push(second - first); break;
		case '*': push(second * first); break;
		case '/':
			push(second / first); break;
		case 's': 
			push(first);
			push(second);
			break;
	}
}

long int getNumber(char *word){
	long int isNegative = 0;

	if(*word == '-')
		isNegative = 1;
	return isNegative ? -(atoi(word+1)) : atoi(word);
}

void handleWord(char *word){
	if(isNumber(word))
		push( getNumber(word) );
	else
		handleOther(word);
}

int main(){
	char lineStr[STRMAX];
	fgets(lineStr, STRMAX, stdin);
	lineStr[strlen(lineStr)-1] = '\0';	
	char *currentWord;
	currentWord = strtok(lineStr, " ");
	handleWord(currentWord);
	
	while( (currentWord = strtok(NULL, " ") ) )
	{
		handleWord(currentWord);
//		printAll();
	}
	return 0;
}
