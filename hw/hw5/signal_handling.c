#define _XOPEN_SOURCE
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>

struct sigaction info;

int called_number, interrupt, stop, quit=0;


void print_info(){
	printf("\nInterrupt:%d\nStop:%d\nQuit:%d\n",interrupt,stop,quit);
}


void check_for_print(){
	printf("S");
	++stop;
	if(stop==3){
		print_info();
		exit(0);
	}
}


void sig_handler(int signum){	
	switch (signum){
		case SIGINT:
			printf("I");
			++interrupt;
			break;
		case SIGQUIT:
			printf("Q");
			++quit;
			break;
		case SIGTSTP:
			check_for_print();
			signal(SIGTSTP,SIG_DFL);
			raise(SIGTSTP);
			break;
		case SIGCONT:	
			signal(SIGTSTP,sig_handler);	
			break;
		default:
			printf("No handle case for this signal");
		}	

}


void init_signal_handlers(){		
	info.sa_handler=sig_handler;
	sigaction(SIGINT, &info, NULL);	
	sigaction(SIGQUIT, &info ,NULL);
	sigaction(SIGTSTP, &info, NULL);	
	sigaction(SIGCONT, &info, NULL);
	
}


int main(){
	setvbuf(stdout,NULL,_IONBF,0);	
	memset(&info, 0, sizeof(info));
	init_signal_handlers();
	while(1){
		printf("X");
		sleep(1);	
	}
	return 0;
}
