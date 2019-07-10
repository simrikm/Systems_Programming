#include <stdio.h>
#include <stdlib.h>

int main(int argc,char *argv[]){
	FILE *ifp;
	int total=0;
	int score=0;
	char *inputFilename =argv[1];
	ifp=fopen(inputFilename, "r");
	while(fscanf(ifp,"%d", &score)!=EOF){
		score=score+score;
		total++;
	}
	printf("%d", score);
	printf("%d", total);
	fclose(ifp);
	score=score/total;
	printf("the score is %d",score);
	return 0;
}
