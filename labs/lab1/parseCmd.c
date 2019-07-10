#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

void readfile(char *filename){
	FILE *file;
	file=fopen(filename,"r");
	int  c;
	while((c=fgetc(file))!=EOF){
		if(c!='<' && c!='>' && c!='&' && c!='|' && c!=' '){
		/*	printf("%c",c);*/
			putchar(c);
		}else{
			printf("\n%c\n", c);
		}
			
	}
	fclose(file);
}


int main(int argc,char *argv[]){
	int i;
	for(i = 0; i < argc; ++i) {
		printf("%s\n", argv[i]);
	}


	char *filename=argv[argc-1];
	readfile(filename);
	
return 0;
} 
