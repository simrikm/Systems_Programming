#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char sth(){

	char answer;
	printf("type");
	scanf("%c",&answer);
	return answer;

}


int main(){
	char ans;
	ans=sth();
	printf("%c",ans);


}
