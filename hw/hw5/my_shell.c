#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/stat.h>

void call_dup2_on_stdout(char *filenames[]){	
	if(filenames[1]){
		int out=open(filenames[1],O_WRONLY | O_TRUNC | O_CREAT,0666);
		if(dup2(out,1)==-1){
			perror("ERROR");
			exit(-1);
		}
	}		
}


/**EXCEEDS 5 lines by 1**/
void call_dup2_on_stdin(char *filenames[]){
	if(filenames[0]){
		int in=open(filenames[0],O_RDONLY);	
		if(dup2(in,0)==-1){
			perror("ERROR");
			exit(-1);
		}	
	}
}

void  save_file_at_index(int i, char *command[], char* filenames[]){
	if(strcmp(command[i],">")==0)
		filenames[1]=command[i+1];
	if(strcmp(command[i],"<")==0){
		filenames[0]=command[i+1];	
	}	
}

void call_exec(char *file_path,char *command[],char *filenames[],char *envp[]){
    pid_t pid;
    switch(pid=fork()){
        case 0:	
			call_dup2_on_stdout(filenames);
			call_dup2_on_stdin(filenames);				
            execve(file_path,command,envp);
            exit(0);
        default:
            wait(NULL);
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

void  trim_the_argument(char *trimmed_command[], char *command[]){
	int i;
	for(i=0;command[i];++i){
		if(strcmp(command[i],"<")==0||strcmp(command[i],">")==0)
				break;
		trimmed_command[i]=command[i];
	}	
	
}

/**EXCEEDS 5 LINES BY 2 **/
int check_for_valid_path(char *paths,char *command[],char *filenames[],char *envp[]){
    char *given_command=command[0], *file_path, *trimmed_command[BUFSIZ]={0};
    file_path=(given_command[0]=='/')?command[0]:pathname(paths, command[0]);
    if(check_if_executable(file_path)==1){
     	trim_the_argument(trimmed_command,command);  
   		call_exec(file_path,trimmed_command,filenames,envp);
		return 1; 
  	}	
		return 0;
}

void checking_fullpath_for_executable(char *path_variable,char *command[],char *filenames[],char *envp[]){
    char *token=strtok(path_variable,"=:");
    while(strcmp(token,".")!=0){
        token=strtok(NULL,"=:");
       if( check_for_valid_path(token,command,filenames,envp)==1)
				break;
    }
}

void extracting_path(char *envp[],char *command[],char *filenames[]){
    int i=0;
    char temp_envp[BUFSIZ];
    for(i=0; envp[i]!=NULL;i++){
        if(strncmp(envp[i],"PATH=",5)==0){
            strcpy(temp_envp,envp[i]);
            checking_fullpath_for_executable(temp_envp,command,filenames,envp);
        }
    }
}

void saving_the_file_names(char *command[],char *envp[]){
    int i;
	char *filenames[BUFSIZ]={0};
	for(i=0;command[i];++i)
		save_file_at_index(i,command,filenames);
	extracting_path(envp,command,filenames);
}

void parsing_command(char *command, char *envp[]){
	int i;
    char * parsed_command[BUFSIZ]={0};
    char *token=strtok(command," \n");
    for(i=0;token!=NULL;i++){
        parsed_command[i]=token;
        token=strtok(NULL," \n");
    }
	saving_the_file_names(parsed_command,envp);
}

/**EXCEEDS 5 LINES BY 1**/
int main(int argc,char *argv[],char *envp[]){
	while(1){	
	char command[BUFSIZ];
		if(argc>0 && argv[0]!=NULL){
			printf("%% ");
			fgets(command,BUFSIZ,stdin);	
			parsing_command(command,envp);	
		}	
	}
	return 0;
}
