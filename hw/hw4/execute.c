#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>


void call_exec(char *file_path,char *command[], char *envp[]){	
	pid_t pid;
	switch( pid=fork()){
		case 0:	
			execve(file_path,command,envp);
			break;
		default:
			wait(0);	
			return;
	}
}

int check_if_executable(char *executable_path){
	struct stat buf;
	int is_executable=0;
	char check_mode[BUFSIZ];
	if(stat(executable_path,&buf)!=-1){
		sprintf(check_mode,"%o",buf.st_mode &07777);
		is_executable=check_mode[0]=='7'?1:0;
	}
	return is_executable;
}

char * pathname(char *paths, char *command){
	char *file_path=malloc(BUFSIZ);
	strcpy(file_path,paths);
	strcat(file_path,"/");
	strcat(file_path,command);	
	return file_path;


}

/** EXCEEDS 5 LINES BY 2**/
void check_for_valid_path(char *paths,char *command[],char *envp[]){
	char *given_command=command[0];		
	char *file_path;
	file_path=(given_command[0]=='/')?command[0]:pathname(paths, command[0]);		
	if(check_if_executable(file_path)==1){
		call_exec(file_path,command,envp);
		free(file_path);	
		exit(0);
	}
	free(file_path);	
}


void checking_fullpath_for_executable(char *path_variable,char *command[],char *envp[]){	
	char *token=strtok(path_variable,"=:");
	while(strcmp(token,".")!=0){
		token=strtok(NULL,"=:");	
		check_for_valid_path(token,command,envp);
		
	}	
}


void extracting_path(char *envp[],char *command[]){
	int i=0;
	char temp_envp[BUFSIZ];	
	for(i=0; envp[i]!=NULL;i++){
		if(strncmp(envp[i],"PATH=",5)==0){
			strcpy(temp_envp,envp[i]);				
			checking_fullpath_for_executable(temp_envp,command,envp);	
		}
	}
}




void parsing_command(char *command,char *envp[]){
	int i;	
	char * parsed_command[BUFSIZ]={0};
	char *token=strtok(command," \n");
	for(i=0;token!=NULL;i++){	
		parsed_command[i]=token;
		token=strtok(NULL," \n");			
	}	
	extracting_path(envp,parsed_command);


}

int main (int argc,char *argv[],char *envp[]){
	if(argc > 0 && argv[0]!=NULL){
		char command[BUFSIZ];
		fgets(command,BUFSIZ, stdin);
		parsing_command(command,envp);	
	}
	return 0;
}
