#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "time.h"
#include <unistd.h>
#include <fcntl.h>


void close_files(FILE *input_filename,FILE  *output_filename){
	fclose(input_filename);
	fclose(output_filename);	
}

void close_filedescriptor(int open_input_file, int open_output_file){
	if(close(open_input_file)==-1 || close(open_output_file) ==-1)
		perror("close failed");
}

int check_for_error(int open_input_file, int open_output_file){
	if(open_input_file==-1||open_output_file==-1){
		perror("ERROR");
		return -1;
	}
	return 0;
}


void method_2(char *input_filename,char *output_filename){
	char buffer[1];	
	int open_input_file=open(input_filename,O_RDONLY);
	int open_output_file=open(output_filename,O_RDWR|O_CREAT,0666);
	if(check_for_error(open_input_file,open_output_file)!=-1){
		while(read(open_input_file,buffer,1)!=0)
			write(open_output_file,buffer,1);
	}
	close_filedescriptor(open_input_file,open_output_file);
}

/** EXCEEDS 5 LINES BY 2**/
void method_3(char *input_filename, char *output_filename){
	char buffer[BUFSIZ+1]={'\0'};
	size_t size_of_buffer=0;
	int open_input_file=open(input_filename,O_RDONLY);
	int open_output_file=open(output_filename,O_RDWR|O_CREAT,0666);
	if(check_for_error(open_input_file,open_output_file)!=-1){
		while(read(open_input_file,buffer,BUFSIZ)!=0){
			size_of_buffer=strlen(buffer);
			write(open_output_file,buffer,size_of_buffer);
			memset(buffer,'\0',BUFSIZ);
		}	
	}
	close_filedescriptor(open_input_file,open_output_file);
	
}

void method_1(char *input_filename,char *output_filename){
	int c;
	FILE *file_input=fopen(input_filename,"r"),*file_output=fopen(output_filename,"w");
	while((c=fgetc(file_input))!=EOF){	
		fputc(c,file_output);
	}
	close_files(file_input,file_output);
}

void switch_on_the_methods(char *choose_method,char *input_filename, char *output_filename){
	switch(choose_method[0]){
		case '1':
			method_1(input_filename,output_filename);
			break;
		case '2':
			method_2(input_filename,output_filename);
			break;
		case '3':
			method_3(input_filename,output_filename);
			break;
		default:
			printf("No method specified\n");
			break;
		}
} 

void timestop(){
	double wallclock,eUTime,eSTime;
	Timer_elapsedWallclockTime(&wallclock);
	Timer_elapsedUserTime(&eUTime);
	Timer_elapsedSystemTime(&eSTime);
	printf("Wallclock: %lf User: %lf System:%lf\n",wallclock, eUTime, eSTime);
}



int main(int argc, char *argv[]){
	int number_of_times=argc>4?atoi(argv[argc-1]):1;
	Timer_start();
	for(int i=0;i<number_of_times;i++)
	switch_on_the_methods(argv[1],argv[2],argv[3]);
	timestop();
	return 0;
}
