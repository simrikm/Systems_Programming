#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

long my_atoi(char *string_num)
{
	long num=0;
	long multiply=strlen(string_num)-2;
	for(long i=0;string_num[i]!='\0';++i){
		num=num*10+string_num[i]-'0';	
	}	
	return num;
}

void reverse(char *str,int length)
{
	char temp[BUFSIZ];
	int start,stop;
	stop=strlen(str)-1;
	for(int i=0; i < strlen(str);i++){
		temp[i]=str[stop];	
		stop--;
	}
		fputs(temp,stdout);
		fputs("\n",stdout);
}


void my_itoa(long num,long  base)
{
	char str[BUFSIZ];
    long i = 0;
    if (num == 0)
    {
        str[i++] = '0';
        str[i] = '\0';
		
 		fputs(str,stdout);
		fputs("\n",stdout);
		return;
    }
 
    while (num != 0)
    {
        long rem = num % base;
        str[i++] = (rem > 9)? (rem-10) + 'A' : rem + '0';
        num = num/base;
    }

    str[i] = '\0'; 
    reverse(str, i);

}



int  main(int argc, char * argv[])
{
	char *base=argv[1];
	char parsed_base[BUFSIZ];
	int track=0;
	long changed_num=0;
	for(int i =2; base[i]!='\0';++i){
			parsed_base[track]=base[i];
			track++;
	}
	long the_base=my_atoi(parsed_base);

	char buf[BUFSIZ];
	while(fgets(buf,BUFSIZ,stdin)!=NULL){
		buf[strlen(buf)-1]='\0';
		changed_num=my_atoi(buf);
		my_itoa(changed_num,the_base);
	}	
	return 0;
}
