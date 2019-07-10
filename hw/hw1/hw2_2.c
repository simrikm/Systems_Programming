#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define STRMAX 1024

size_t parse_the_operators(char *word, size_t index){	
	if(index!=strlen(word)-1){
		if(((strchr("+=-<>|&!",word[index+1])) && (isalpha(word[index-1]))) || ((isdigit(word[index+1])) && (!isalpha(word[index-1])) && (strchr("-",word[index])))){	 
			printf("%c",word[index]);
			index++;
		}
	}
	return index;
}


void print_out_newline(char * word,size_t index){
	if(index==0) 
		printf("%c\n",word[index]);
	else if((!strchr(",()+-*=&|.^%!?><~#:;[]{}",word[index-1])))
		printf("\n%c\n",word[index]);
	else
		printf("%c\n",word[index]);	
}


void print_other_characters(char *word, size_t index){
	if(index==strlen(word)-1 || (strchr("+-",word[index+1])))
		printf("%c\n",word[index]);
	else
		printf("%c",word[index]);
}


/**EXCEEDS 5 LINES by 2**/
void parse_in_token(char *word){
	size_t index;
	for(index=0;index<strlen(word);index++){
		if(!isspace(word[index])){	
			if(strchr("+-=<>|&!",word[index]))
				index=parse_the_operators(word,index);	
			if(strchr(",()+-*=&|.^%!?><~#:;{}[]",word[index])){		
				print_out_newline(word,index);		
			}else{
			print_other_characters(word, index);
			}
		}	
	}
}


void parse_space(char *line){
	char *word=strtok(line," ");						
	while(word!=NULL){	
		parse_in_token(word);
		word=strtok(NULL," ");		
	}
	
}

void parse_print_statement(char * line){
	size_t index;
	for(index=0;index<strlen(line);index++){
		if(strchr("(,);",line[index])){			
			print_out_newline(line, index);
		}else{
			printf("%c",line[index]);
		}	
	}

}


void check_which_function_to_call(char *line){
	if(strstr(line,"printf"))
		parse_print_statement(line);
	else
		parse_space(line);
}


void remove_the_tabs(char *line){
	while(isspace(*line))
			line++;
	check_which_function_to_call(line);	
	
}

int main(){
	char line[STRMAX], lines[STRMAX];	
	while((fgets(line, STRMAX, stdin))!=NULL){
		sscanf(line,"%[^/\n]",lines);
		remove_the_tabs(lines);
		memset(lines,0,sizeof(lines));;			
	}
	return 0;
}
