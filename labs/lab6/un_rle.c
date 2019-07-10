#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>



int main(){
	char buffer[BUFSIZ];
	char temp[BUFSIZ];
	int num;
	int count, index=0;

	while((buffer[index]=fgetc(stdin))!=EOF)
	{	
		if (buffer[index] == '\n')
		{
			count = 0;
			puts("");	
		}
		else if(isdigit(buffer[index])) 
		{
			temp[count++] = buffer[index];
		}else{
			temp[count]='\0';	
			num=atoi(temp);
			for(int i=0; i <num; i++){
				printf("%c",buffer[index]);
			}
			count=0;
		}		
		index++;
	}
	printf("\n");
	return 0;

}
