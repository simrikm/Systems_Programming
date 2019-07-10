#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>
#include "my_ls.h"
#include <ctype.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>

#define STRMAX 1024

char * compare_mode(char c){
	switch(c){
		case '0':
			return (char*)"---";
		case '1':
			return (char*)"--x";
		case '2':
			return (char*)"-w-";
		case '3':
			return (char*)"-wx";
		case '4':
			return (char*)"r--";
		case '5':
			return (char*)"r-x";
		case '6':
			return (char*)"rw-";
		case '7':
			return (char*)"rwx";
		default:
			return NULL;

	}
}

void indent(int track){
	int i;
	for(i=0;i<track;i++)	
		printf("\t");

}


/**EXCEEDS 5 LINES BY 2**/
char *  change_mode(int mode,int filetype){
	char  mode_in_string[BUFSIZ], *temp=malloc(BUFSIZ);
	int i;
	temp[0]=filetype==0?'d':'-';
	temp[1]='\0';
	sprintf(mode_in_string,"%o",mode & 07777);
	for(i=0;i<3;i++)
		strcat(temp,compare_mode(mode_in_string[i]));	
	return temp;
}

void free_memory(char *mode, char *pathname_file, char *modification_time,int require_path){	
	free(mode);
	if(require_path!=0)
		free(pathname_file);
	free(modification_time);;
}

void  print_directory_name(char * argument){
	if(argument[0]!='/')
		printf("/");
	printf("%s\n",argument);
}

/**EXCEEDS 5 LINES BY 3**/
void print_info(char *argument,char *filename,int filetype, int track,int require_path){	
	struct stat buf;
	char *modification_time=malloc(BUFSIZ), *pathname_file=require_path==0?argument:pathname(argument,filename);	
	indent(track);
	if(stat(pathname_file,&buf)!=-1){	
		char *mode=change_mode(buf.st_mode,filetype);
		strftime(modification_time,18,"%b %d %H:%M",localtime(&buf.st_mtime)); 
		printf("%5s %3lu %5s %5s %10lu %7s %10s",mode,buf.st_nlink,getUserName(buf.st_uid),getGroupName(buf.st_gid),buf.st_size,modification_time,filename);
		print_directory_name(argument);
		free_memory(mode,pathname_file,modification_time,require_path);
	}
}

int check_if_file(char *path){
	struct stat buf;
	stat(path,&buf);
	return S_ISREG(buf.st_mode);
}

char * pathname(char *path, char *filename){
	char * temp =malloc(BUFSIZ);
	strcpy(temp,path);
	strcat(temp,"/");
	strcat(temp,filename);
	return temp;
}

char *getUserName(uid_t uid){
	struct passwd *pw=getpwuid(uid);
	return pw->pw_name?pw->pw_name:(char*)"";
}

char *getGroupName(gid_t gid){
	struct group *grp=getgrgid(gid);
	return grp->gr_name?grp->gr_name:(char*)"";



}

void directory_recursion(char *temp,int track, char *directory_name){
	indent(track);
	printf("%s/\n",directory_name);
	do_ls(temp,track);

}



void check_for_recursion(char *argument, char *filename, int track){
	char *temp=pathname(argument,filename);
	print_info(argument,filename,check_if_file(temp),track,1);	
		if(check_if_file(temp)==0)
			directory_recursion(temp,++track, filename);
	free(temp);
}



void ls_directory(char *argument,DIR *dirp,int track){
	struct dirent *direntp;	
	while((direntp=readdir(dirp))){	
		if((strcmp(direntp->d_name,".")!=0)&&(strcmp(direntp->d_name,"..")!=0))
			check_for_recursion(argument,direntp->d_name,track);	
			}
	track--;
}	


void do_ls(char *argument, int track){	
	DIR *dirp;
	if((!(dirp=opendir(argument))))
		return;	
	else
		ls_directory(argument,dirp,track);	
	closedir(dirp);
}






	
			
	



