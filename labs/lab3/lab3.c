#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>

void not_needed(char * filename,char * pattern){
  	
	FILE *file;
	char lines[BUFSIZ];
	file=fopen(filename,"r");
	if(file == NULL) 
		return;
	
	while(fgets(lines,sizeof lines, file)!=NULL)
		if(strstr(lines, pattern)){
			printf("\n %s", filename);   
        	printf("\n%s", lines); 
		}
	fclose(file);
}




int count_spaces(char *line, int len) {
	int space_count = 0; 
	for (long i = 0; i < len; i++) {
		if (line[i] == ' ' || line[i] == '\n' || line[i] == '\t')
			++space_count; 
	}

	return space_count; 
}

void read_file(char *file_path) {
	FILE *f; 
	if ((f = fopen(file_path, "r")) == NULL) {
		printf("file is null"); 
		return; 
	} 
	
	char line[BUFSIZ]; 
	int line_count = 0;
	int len = 0; 
	int words = 0;
	int character_count=0;

	int c; 
	while ((c = getc(f)) != EOF) {
		++len; 
		if (c == '\n') 
			++line_count; 
		else if (c == ' ' || c == '\n' || c == '\t') {
			++words; 
		}

	}

	len = len - words; 

	printf("\n%s: %d %d %d\n", file_path, line_count, len, words); 

	
}


void open_dir(char *file_path) {
	DIR *d; 
	struct dirent *d_entry; 
	if (!(d = opendir(file_path))) {
		printf("Cannot open directory"); 
		read_file(file_path); 
	}		


	while (d_entry = readdir(d)) {
							

	}

}


void check_the_type(char * filename){
	DIR *mydir;
	struct dirent *myfile;
	mydir=opendir(filename);
	char *pathname=malloc(BUFSIZ);
	if(mydir==NULL){
			read_file(filename);
		
 		
	}else{	
    
	while((myfile=readdir(mydir))) {
       strcpy(pathname,filename);
		strcat(pathname,(char*)"/");
		strcat(pathname,myfile->d_name);	
		
		if((strcmp(myfile->d_name,".")!=0)&&(strcmp(myfile->d_name,"..")!=0)){	
			if(opendir(myfile->d_name)==NULL)	
				read_file(filename);
			check_the_type(pathname);	
		
		}	
		
    }	
	closedir(mydir);
}

}

int main(int argc,char * argv[]){
	char * filename= argv[1];
	check_the_type(filename);
	return 0;






}
