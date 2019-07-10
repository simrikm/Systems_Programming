#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void check_for_division(int number){
	int copy_number=number;
	int divisible_num[BUFSIZ];
	int i=0;
	int start=2;
	while(start <=copy_number){
		if(copy_number % start == 0){
			divisible_num[i]=start;
			copy_number=copy_number/start;
			i++;
			continue;
		}
		start++;
	}	
	printf("Prime Factorization of %d is ",number);
	char buf[BUFSIZ];
	buf[0]='\0';
	char temp[BUFSIZ];
	for(int index=0;index < i; ++index){
		sprintf(temp,"%d",divisible_num[index]);	
		strcat(buf,temp);
		strcat(buf," X ");
	}
	buf[strlen(buf)-3]='\0';
	printf("%s\n",buf);
	
}


int main(){
	int number;
	printf("Enter a number: ");
	scanf("%d",&number);
	check_for_division(number);
	return 0;



}
