#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>


void print_letters(char letter, int number_of_times){	
	for(int i=0;i<number_of_times;i++){
		printf("%c",letter);
		fflush(stdout);		
	}
}

void error_check(pid_t pid){	
	if(pid<0){
		perror("ERROR");
		exit(1);
	}
}

void successful_creation(int letter,pid_t pid,int number_of_times){	
	if(pid==0){
		print_letters(letter,number_of_times);
		exit(0);
	}		
}


void start_child_printing_k_char(int number_of_times){
	for(int i='A';i < 'D' ;++i){
		pid_t pid=fork();
		error_check(pid);
		successful_creation(i,pid,number_of_times);
	}	
}

void wait_for_all_children(){
	for (int i=0;i<3;i++){
		if(wait(NULL)<0)
			perror("ERROR");
	}
	printf("\n");
}



int main(int argc, char * argv[]){
	char *ptr;	
	long number_of_times=argc>1?strtol(argv[1],&ptr,10):10000;
	start_child_printing_k_char(number_of_times);
	print_letters('D',number_of_times);
	wait_for_all_children();	
	return 0;
}
