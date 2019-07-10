#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <dirent.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pthread.h>

struct argument
{
	char* pattern;
	int file_index;
	char* file_name;
};

pthread_t thread;
pthread_mutex_t mutex_no;

//getting the filename with full path
char *filename(char *path, char *filename){
	char *temp=malloc(BUFSIZ);
	strcpy(temp,path);
	strcat(temp,"/");
	strcat(temp,filename);
	return temp;
}

void print_string(char *s)
{
	while(*s)
		putchar( *s++);
}

void print_number(int i){
	char buf[80];
	sprintf(buf,"%d",i);
	print_string(buf);

}

void print_newline()
{
	putchar('\n');
}

void print_match(char * file_name,int file_index, int line_number, char *line)
{
	
	pthread_mutex_lock(&mutex_no);
	print_number(file_index);
	print_string(file_name);
	print_number(line_number);
	print_string(line);//was print_number but the argument is of type char
	print_newline();
	pthread_mutex_unlock(&mutex_no);
}

void add_all_matching_lines(char *pattern, int file_index, char *file_name){
	int line_number=1;
	FILE *fp;
	char buffer[BUFSIZ];
	fp=fopen(file_name,"r");
	while(fgets(buffer,BUFSIZ,fp)!=NULL){
		if(strstr(buffer,pattern))
			print_match(file_name,file_index,line_number,buffer);
			line_number++;
	}
	
}

void* threadfunc(void* args){
	struct argument* param;
	param=(struct argument *)args;
	add_all_matching_lines(param->pattern,param->file_index,param->file_name);

}

void search(char *pattern,int file_index, char *file_name)
{
	struct stat buf;
	struct argument *args=malloc(sizeof(struct argument));
	if(lstat(file_name,&buf)!=-1){
		if(!(buf.st_mode & S_IREAD))//check if the file is readable	
			return;
		if(S_ISREG(buf.st_mode)){ //check if file not working for the linker file								
			args->pattern=pattern;
			args->file_index=file_index;
			args->file_name=file_name;	
			pthread_mutex_init(&mutex_no,NULL);		
			pthread_create(&thread,NULL,threadfunc,(void*)args);
		}else if(S_ISDIR(buf.st_mode)){	
			char *f;			
			DIR *d;
			struct dirent *dir;	
			d=opendir(file_name);
			if(!d)
				perror("Error directory cannot be opened");
			else	
				while((dir=readdir(d))!=NULL)
				{				
					if (strcmp(dir->d_name,".")!=0 && strcmp(dir->d_name,"..")!=0){	
						f=filename(file_name,dir->d_name);
						search(pattern,file_index,filename(file_name, dir->d_name));
						
					}
				}
	
			closedir(d);				
		}		
							
	}
}

int main (int argc, char *argv[])
{
	int file_index=1;
	struct stat buf;
	char *f;
	if (argc<3)
		perror("Usage:search pattern file");
	for(int i=2; i < argc ; i++){
		search(argv[1], file_index++, argv[i]);
	}	
	pthread_exit(NULL);							
	pthread_mutex_destroy(&mutex_no);
	return 0;
}}
