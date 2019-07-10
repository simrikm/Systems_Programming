#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>



void check_the_code(char c,char *filename){
	switch(c){
	case 'A':
	case 'a':
		printf( "%s",". -");
		  break;
		
	case 'B':
	case 'b':
		printf( "%s","- . . .");
		  break;
	
	case 'C':
	case 'c':
		printf( "%s","- . - .");
		  break;
	
	case 'D':
	case 'd':
		printf( "%s","- . .");
		  break;

	case 'E':
	case 'e':
		printf( "%s",".");
  
		break;

	case 'F':
	case 'f':
		printf( "%s",". . - .");
	  	break;

	case 'G':
	case 'g':
		printf( "%s","- - .");
	  	break;

	case 'H':
	case 'h':
		printf( "%s",". . . .");
	  	break;

	case 'I':
	case 'i':
		printf( "%s",". .");
	  	break;
	case 'J':
	case 'j':
		printf( "%s",". - - -");
	  	break;
	
	case 'K':
	case 'k':
		printf( "%s",". - .");
	  	break;
	case 'L':
	case 'l':
		printf( "%s",". - . .");
	  	break;
	case 'M':
	case 'm':
		printf( "%s","- -");
	  	break;
	case 'N':
	case 'n':
		printf( "%s","- .");
	  	break;
	case 'O':
	case 'o':
		printf( "%s","- - -");
	  	break;
	case 'P':
	case 'p':
		printf( "%s",". - - .");
	  	break;	
	case 'Q':
	case 'q':
		printf( "%s","- - . -");
	  	break;	
	case 'S':
	case 's':
		printf( "%s",". . .");
	  	break;
	case 'T':
	case 't':
		printf( "%s","-");
	  	break;
	case 'R':
	case 'r':
		printf( "%s",". - .");
	  	break;
	case 'U':
	case 'u':
		printf( "%s",". . -");
	  	break;
	case 'V':
	case 'v':
		printf( "%s",". . . -");
	  	break;
	case 'W':
	case 'w':
		printf( "%s",". - -");
	  	break;
	case 'X':
	case 'x':
		printf( "%s","- . - -");
		  break;
	case 'Y':
	case 'y':
		printf( "%s","- . - -");
	  	break;
	case 'Z':
	case 'z':
		printf( "%s","- - . .");
	  	break;	
	case '1':
		printf( "%s",". - - - -");
	  	break;
	case '2':
		printf( "%s",". . - - -");
	  	break;
	case '3':
		printf( "%s",". . . - -");
		  break;
	case '4':
		printf( "%s",". . . . -");
		  break;
	case '5':
		printf( "%s",". . . .");
	  	break;
	case '6':
		printf( "%s","- . . . .");
	  	break;
	case '7':
		printf( "%s","- - . . .");
	  	break;
	case '8':
		printf( "%s","- - - . .");
	  	break;
	case '9':
		printf( "%s","- - - - .");
	  	break;
	case '0':
		printf( "%s","- - - - -");
	break;
	default:
	printf("not alpha num");
	}
	
}

void readfile(char *filename){
    FILE *file;
    file=fopen(filename,"r");
    int  c;
    while((c=fgetc(file))!=EOF){
   	if(isalnum(c)){	
		check_the_code(c,filename);
		printf("   ");	
	} 
	if(isspace)
		printf("       "); 
  	}
    fclose(file);
}


int main(int argc,char *argv[]){
    int i;
    char *filename=argv[argc-1];
    readfile(filename);
	printf("\n");

return 0;
}
