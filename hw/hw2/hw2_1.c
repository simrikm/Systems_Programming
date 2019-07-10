#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define require(e) if (!(e)) fprintf(stderr, "FAILED line %d        %s: %s       %s\n", __LINE__, __FILE__, __func__, #e)
#define STRMAX 1024

size_t  my_strlen(const char *string){
	size_t  length_of_given_string=0;;
	while (*string++)
		length_of_given_string++;
	return length_of_given_string;
}

char * my_strcpy(char * destination,const char *  string_to_be_copied){	
	size_t i;
	for(i=0;i<=my_strlen(string_to_be_copied);i++)
		destination[i]=string_to_be_copied[i];
	destination[i]='\0';
	return destination;
}

char * my_strncpy(char *destination, const char*string_to_be_copied, size_t n){	
	size_t i;
	for(i=0;i<n;i++)
		destination[i]=string_to_be_copied[i];
	destination[i]='\0';
	return destination;
}

char * my_strcat(char *destination, const char *string_to_append){
	size_t index=my_strlen(destination);
	size_t i;
	for(i=0;i<=my_strlen(string_to_append);i++)
		destination[index++]=string_to_append[i];
	return destination;
}

int my_strcmp(const char * first_string,const char * second_string){		
	int index=0;	
	while(first_string[index]==second_string[index] && first_string[index] && second_string[index]){
	index++;
	}
	return first_string[index]-second_string[index];
}

char * my_strchr(const char *str, int c ){	
	while(*str){
		if(*str==c)
			return (char*)str;
	str++;
	}	
	return NULL;
}

char * my_strpbrk(const char *s1, const char *s2){	
	size_t  i;
	for(i=0;i<=my_strlen(s1);i++){
		if(my_strchr(s2,s1[i]))
			return strchr(s1,s1[i]);
	}
	return NULL;

}

int  my_strncmp(const char *s1, const char * s2, size_t n){
	size_t  index=0;
	while(n && (*s1++==*s2++)){	
		if(++index==n)
			return 0;
	}		
	return s1[index]-s2[index];
}
		
char *my_strstr(const char *haystack, const char *needle){	
	char *temp=strpbrk(haystack,needle);
	if(my_strncmp(temp,needle,my_strlen(needle))==0)	
		return temp;
	return NULL;
}

size_t my_strspn(const char *str1, const char *str2){
	size_t i;
	for(i=0;i<=my_strlen(str1);i++){
		if(!my_strchr(str2,str1[i]))
			return i;
	}	
	return -1;
}

/**EXCEEDS 5 LINES BY 4**/
char * my_strtok(char *str, const char *delim){
	size_t i;
	static char *p=0;
	if(str)
		p=str;
	else
		str=p;
	for(i=0;i<my_strlen(p);i++){
		if(strchr(delim,str[i])){
			p=str+1+i;
			str[i]='\0';
		}
	}
	return str;
}

void test_strtok(){
	char a[STRMAX]="hello/world", b[STRMAX]="https://www.uci.edu";
	require(strcmp(my_strtok(a,"/"),"hello")==0);
	require(strcmp(my_strtok(NULL,"/"),"world")==0);
	require(strcmp(my_strtok(b,":"),"https")==0);
}

void test_strspn(){
	require(my_strspn("hello","hall")==1);	
	require(my_strspn("elephant","help")==5);	
	require(my_strspn("hello","hello")==5);
}

void test_strpbrk(){
	require(strcmp(my_strpbrk("hello","love"),"ello")==0);
	require(strcmp(my_strpbrk("fair","fair"),"fair")==0);
	require(strcmp(my_strpbrk("hello","elephant"),"hello")==0);
}

void test_strchr(){
	char string[STRMAX]="www.uci";	
	char string2[STRMAX]="array[100]";
	require(strcmp(my_strchr(string,'.'),".uci")==0);	
	require(strcmp(my_strchr(string2,']'),"]")==0);
	require(strcmp(my_strchr(string2,'a'),"array[100]")==0);
}

void test_strlen()
{
    require(my_strlen("") == 0);
    require(my_strlen("hello") == 5);
	require(my_strlen("This is checking")==16);
}

void test_strcpy(){
	char destination[STRMAX]="Checking", destination_small[STRMAX]="check";	
	require(strcmp(my_strcpy(destination,"Hello"),"Hello")==0);
	require(strcmp(my_strcpy(destination,"World"),"World")==0);
	require(strcmp(my_strcpy(destination_small,"This is checking"),"This is checking")==0);
}

void test_strncpy(){
	char destination[STRMAX];
	require(strcmp(my_strncpy(destination,"Hello",2),"He")==0);
	require(strcmp(my_strncpy(destination,"Hello World",7),"Hello W")==0);
	require(strcmp(my_strncpy(destination,"This is checking",13),"This is check")==0);
}

void test_strcat()
{
	char a[STRMAX]="hi", b[STRMAX]="hello", c[STRMAX]="Namaste "; 
	require(strcmp(my_strcat(a,"world"),"hiworld")==0);
	require(strcmp(my_strcat(b,"World"),"helloWorld")==0);
	require(strcmp(my_strcat(c,"This is checking"),"Namaste This is checking")==0);
}

void test_strcmp()
{
    const char *r = "hello", *s = "hello", *t = "he";
	require(my_strcmp(r,s) == 0);
    require(my_strcmp(t,r) < 0);
    require(my_strcmp(s,t) > 0);
}

void test_strncmp()
{
    const char *r = "hello", *s = "hello", *t = "hes";
	require(my_strncmp(r,s,3) == 0);
    require(my_strncmp(t,r,3) > 0);
    require(my_strncmp(s,t,4) < 0);
}

void test_strstr()
{
	const char *a="air";
	require(strcmp(my_strstr("fair",a),"air")==0);
	require(strcmp(my_strstr("gamenight","game"),"gamenight")==0);
	require(strcmp(my_strstr("fair","ir"),"ir")==0);
}


int main()
{
    test_strlen();
	test_strcmp();
	test_strncmp();
	test_strcpy();
	test_strcat();
	test_strncpy();
	test_strchr();
	test_strpbrk();
	test_strstr();
	test_strspn();
	test_strtok();
    return 0;

}
